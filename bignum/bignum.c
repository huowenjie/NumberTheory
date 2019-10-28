#include "basic.h"
#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bn_print(unsigned char *data, int len)
{
	int i = 0;

	if (!data) return;

	for (; i < len; i++) {
		printf("0x%02X ", data[i]);
		if (!((i + 1) % 8)) {
			printf("\n");
		}
	}
	printf("\n");
}

BOOL bn_malloc(BIG_INT *num, int len)
{
	if (!num || len <= 0) {
		return FALSE;
	}

	num->buf_len = len;
	num->len = 0;
	num->neg = 0;
	num->bto = is_little_endian() ? BIGNUM_LITTLE_ENDIAN : BIGNUM_BIG_ENDIAN;
	num->data = malloc(len * sizeof(unsigned char));
	if (!num->data) {
		return FALSE;
	}
	memset(num->data, 0, len * sizeof(unsigned char));

	return TRUE;
}

void bn_free(BIG_INT *num)
{
	if (num && num->data) {
		free(num->data);

		num->len = 0;
		num->neg = 0;
		num->bto = 0;
	}
}

BOOL bn_add(BIG_INT *orign, BIG_INT *addend, BIG_INT *ret)
{
	// 循环计数器
	int i = 0;
	int j = 0, k = 0, w = 0;

	// 计算中间值，用于进位判断
	short tmp = 0;

	// 加数、被加数、和
	unsigned char *pa = NULL;
	unsigned char *pb = NULL;
	unsigned char *pr = NULL;

	// 判断错误情况
	if (!orign || !addend || !ret) {
		return FALSE;
	}

	if (!orign->data || !addend->data || !ret->data) {
		return FALSE;
	}

	// 和的缓冲区大小必须大于被加数以及加数
	if (ret->buf_len <= orign->len || ret->buf_len <= addend->len) {
		return FALSE;
	}

	pa = orign->data;
	pb = addend->data;
	pr = ret->data;

	for (j = orign->len, k = addend->len, w = ret->buf_len; i < w; i++) {
		if (i < j && i < k) {
			tmp = pa[i] + pb[i];
		} else if (i >= j && i < k) {
			tmp = pb[i];
		} else if (i < j && i >= k) {
			tmp = pa[i];
		} else {
			ret->len = ((0xFF00 & tmp) && (i < (w - 1))) ? (i + 1) : i;
			break;
		}
		
		if ((0xFF00 & tmp) && i < (w - 1)) {
			// 进位
			pr[i + 1] += 0x01;
		}
		
		pr[i] += tmp & 0x00FF;
	}

	return TRUE;
}

BOOL bn_sub(BIG_INT *orign, BIG_INT *subend, BIG_INT *ret)
{
	if (!orign || !subend || !ret) {
		return FALSE;
	}

	if (!orign->data || !subend->data || !ret->data) {
		return FALSE;
	}

	// 差的缓冲区不小于被减数以及减数
	if (ret->buf_len < orign->len || ret->buf_len < subend->len) {
		return FALSE;
	}

	// 被减数、减数、差
	unsigned char *pa = NULL;
	unsigned char *pb = NULL;
	unsigned char *pc = NULL;

	return TRUE;
}

