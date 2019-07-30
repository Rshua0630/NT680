#ifndef _NVT_LIBC_SETJMP_H
#define _NVT_LIBC_SETJMP_H

typedef int jmp_buf[14];

extern int  setjmp(jmp_buf env);
extern void longjmp(jmp_buf env, int val);

#endif
