#include <stdio.h>
#include <string.h>
#include "basic.h"
#include "bignum.h"

int main(int argc, char *argv[])
{
	printf("a and b:");

	unsigned int ia = 0;
	unsigned int ib = 0;

	scanf("%d", &ia);
	scanf("%d", &ib);

	BIG_INT a = { 0 };
	BIG_INT b = { 0 };
	BIG_INT c = { 0 };

	bn_malloc(&a, 100);
	bn_malloc(&b, 100);
	bn_malloc(&c, 100);

	memcpy(a.data, &ia, sizeof(unsigned int));
	memcpy(b.data, &ib, sizeof(unsigned int));

	a.len = sizeof(unsigned int);
	b.len = sizeof(unsigned int);

	bn_print((unsigned char *)(a.data), sizeof(unsigned int));	
	bn_print((unsigned char *)(b.data), sizeof(unsigned int));

	BOOL r = bn_add(&a, &b, &c);
	printf("result = %d, c.len = %d\n", r, c.len);
	bn_print(c.data, c.len);
	printf("c = %d\n", *((int *)c.data));

	bn_free(&a);
	bn_free(&b);
	bn_free(&c);

	return 0;
}
