#ifndef X86_H
#define X86_H

inline void stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
	       "=D" (addr), "=c" (cnt) :
	       "0" (addr), "1" (cnt), "a" (data) :
	       "memory", "cc");
}

inline void stosl(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosl" :
	       "=D" (addr), "=c" (cnt) :
	       "0" (addr), "1" (cnt), "a" (data) :
	       "memory", "cc");
}

#endif
