#include "basic.h"

BOOL is_little_endian()
{
	int tmp = 0x12345678;
	char *ch = (char *)&tmp;
	return (ch[0] == 0x78 && ch[3] == 0x12);
}

BOOL is_big_endian()
{
	return !is_little_endian();
}

