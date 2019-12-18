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

int bn_compare(const BIG_INT *num1, const BIG_INT *num2)
{
	int ret = 0;
	int i = 0;

	// 比较空指针
	if (!num1 && !num2) {
		return 0;
	}

	if (!num1 || !num2) {
		return (num1 == NULL) ? -1 : 1;
	}

	// 如果异号，正数为大
	if (num1->neg != num2->neg) {
		return (num1->neg == BIGNUM_POSITIVE) ? 1 : -1;
	}

	// 同号比较
	// ---------------------------------------------

	// 比较长度
	if (num1->len > num2->len) {
		ret = 1;
	} else if (num1->len < num2->len) {
		ret = -1;
	} else {
		// 小端法
		// 长度相同，逐位比较
		for (i = num1->len - 1; i >= 0; i--) {
			if (num1->data[i] > num2->data[i]) {
				ret = 1;
				break;
			} else if (num1->data[i] < num2->data[i]) {
				ret = -1;
				break;
			}
		}
	}

	return (num1->neg == BIGNUM_POSITIVE) ? ret : -ret;
}

BOOL bn_add(BIG_INT *orign, BIG_INT *addend, BIG_INT *ret)
{
	// 循环计数器
	int i = 0;
	int j = 0, k = 0, w = 0;

	// 计算中间值，用于进位判断
	short tmp = 0;
	unsigned char flag = 0;

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
			tmp = pa[i] + pb[i] + flag;
		} else if (i >= j && i < k) {
			tmp = pb[i] + flag;
		} else if (i < j && i >= k) {
			tmp = pa[i] + flag;
		} else {
			ret->len = ((0xFF00 & tmp) && (i < (w - 1))) ? (i + 1) : i;
			break;
		}

		flag = (0xFF00 & tmp) && i < (w - 1) ? 1 : 0;
		pr[i] += tmp & 0x00FF;
	}

	return TRUE;
}

BOOL bn_sub(BIG_INT *orign, BIG_INT *subend, BIG_INT *ret)
{
	// 比较结果
	int comp_ret = 0;
	
	// 循环计数器
	int i = 0;
	int j, k, w = 0;

	short tmp = 0;
	short lend = 0;

	// 被减数、减数、差
	BIG_INT *na = NULL;
	BIG_INT *nb = NULL;

	unsigned char *pa = NULL;
	unsigned char *pb = NULL;
	unsigned char *pc = NULL;

	if (!orign || !subend || !ret) {
		return FALSE;
	}

	if (!orign->data || !subend->data || !ret->data) {
		return FALSE;
	}

	// 差的缓冲区大小必须大于被减数以及减数
	if (ret->buf_len <= orign->len || ret->buf_len <= subend->len) {
		return FALSE;
	}

	// 比较两数大小，定符号和减法顺序（同号）
	if ((comp_ret = bn_compare(orign, subend)) == 0) {
		ret->len = 0;
		ret->neg = BIGNUM_POSITIVE;
		memset(ret->data, 0, ret->buf_len);
		return TRUE;
	} else if (comp_ret > 0) {
		ret->neg = BIGNUM_POSITIVE;
		na = orign;
		nb = subend;
	} else {
		ret->neg = BIGNUM_NEGATIVE;
		na = subend;
		nb = orign;
	}

	// 这里需要判断绝对值 TODO

	pa = na->data;
	pb = nb->data;
	pc = ret->data;

	/**
	 * 基本的减法操作(绝对值大小为pa > pb)
	 * 如下所示：
	 *   10 00 11 00 01 10 (j = 6)
	 * -    10 00 10 10 01 (k = 5, w > 6)
	 * 逐步借位运算
	 */
	for (j = na->len, k = nb->len, w = ret->buf_len; i < w; i++) {
		if (i < j && i < k) {
			tmp = pa[i] - pb[i] - lend;
		} else if (i >= j && i < k) {
			tmp = pa[i] - lend;
		} else {
			ret->len = i;
			break;
		}

		lend = (tmp & 0xFF00) ? 1 : 0;
		pc[i] = (tmp & 0x00FF);
	}

	return TRUE;
}

