#ifndef __BIG_NUM_H__
#define __BIG_NUM_H__

#define BIGNUM_LITTLE_ENDIAN 0	// 小端字节序
#define BIGNUM_BIG_ENDIAN 1		// 大端字节序

#define BIGNUM_POSITIVE 0		// 正数
#define BIGNUM_NEGATIVE 1		// 负数

/*
 * 大整数结构定义
 */
typedef struct big_int_st {
	int				buf_len;	// 缓冲区长度（字节）
	int 			len;		// 大数有效长度（字节）
	int 			neg;		// 是否是负数 0 正数，1 负数
	int				bto;		// 字节序 0 大端法，1 小端法
	unsigned char 	*data;		// 大数数据
} BIG_INT;

// 打印十六进制
void bn_print(unsigned char *data, int len);

// 申请内存
BOOL bn_malloc(BIG_INT *num, int len);
void bn_free(BIG_INT *num);

// 大数相加减
BOOL bn_add(BIG_INT *orign, BIG_INT *addend, BIG_INT *ret);
BOOL bn_sub(BIG_INT *orign, BIG_INT *subend, BIG_INT *ret);

#endif // __BIG_NUM_H__

