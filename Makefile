CC 		:= gcc
INC		:= include
SRC_DIR := . addition bignum subtraction
DEBUG	:= -g
TARGET	:= test
TAR_DIR	:= $(foreach file,$(TARGET),$(patsubst %/test,%,$(file)))

COMP_FLG:= -O0 -Wall

C_FILES	:= $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
C_OBJS	:= $(foreach obj,$(C_FILES),$(patsubst %.c,%.o,$(obj)))
C_DEFS	:= $(foreach def,$(C_DEFS),$(patsubst %.c,%.d,$(def)))

vpath %.h $(INC)
vpath %.c $(SRC_DIR)

$(TARGET):$(C_OBJS)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) -c $< $(DEBUG) -o $@ -I $(INC)

include $(C_DEFS)

%.d:%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(COMP_FLG) -I $(INC) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY:clean
clean:
	rm -f $(C_OBJS) $(C_DEFS) $(TARGET)

