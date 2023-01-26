#ifndef __VSPRINTF_H__
#define __VSPRINTF_H__

#include <stdarg.h>
int vsprintf(char *buf, const char *fmt, va_list args);

typedef struct Buffer{
	int char_num;
	char char_buf[200];
} Buffer;



#endif
