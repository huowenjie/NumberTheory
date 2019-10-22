#ifndef __BASIC_H__
#define __BASIC_H__

typedef unsigned char BOOL;

#define TRUE 1
#define FALSE 0

// 确定本机字节序
BOOL is_little_endian();
BOOL is_big_endian();

#endif // __BASIC_H__

