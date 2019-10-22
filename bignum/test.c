#include <stdio.h>
#include <string.h>
#include "basic.h"
#include "bignum.h"

int main(int argc, char *argv[])
{
	unsigned int ia = 0x12345678;
	unsigned int ib = 0x12345678;

	BIG_INT a = { 0 };
	BIG_INT b = { 0 };
	BIG_INT c = { 0 };

	bn_malloc(&a, 100);
	bn_malloc(&b, 100);
	bn_malloc(&c, 100)

	memcpy(a.data, &ia, sizeof(unsigned int));
	memcpy(b.data, &ib, sizeof(unsigned int));

	bn_print((unsigned char *)(&num), sizeof(int));	

	


	bn_free(&a);
	bn_free(&b);
	bn_free(&c);

	return 0;
}
