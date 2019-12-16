#include <limits.h>

#define UNALIGNED(X) ((long)X & (sizeof(long) - 1))

/* How many bytes are loaded each iteration of the word copy loop.  */
#define LBLOCKSIZE (sizeof(long))

#if LONG_MAX == 2147483647L
#define DETECTNULL(X) (((X)-0x01010101) & ~(X)&0x80808080)
#else
#if LONG_MAX == 9223372036854775807L
#define DETECTNULL(X) (((X)-0x0101010101010101) & ~(X)&0x8080808080808080)
#else
#error long int is not a 32bit or 64bit type.
#endif
#endif

#ifndef DETECTNULL
#error long int is not a 32bit or 64bit byte
#endif

/* DETECTCHAR returns nonzero if (long)X contains the byte used
   to fill (long)MASK. */
#define DETECTCHAR(X, MASK) (DETECTNULL(X ^ MASK))

void *rawmemchr(const void *src_void, int c) {
  const unsigned char *src = (const unsigned char *)src_void;
  unsigned char d = c;
  while (UNALIGNED(src)) {
    if (*src == d)
      return (void *)src;
    src++;
  }

  /* If we get this far, src is word-aligned. */
  unsigned long *asrc = (unsigned long *)src;
  unsigned long mask = d << 8 | d;
  mask = mask << 16 | mask;
  for (unsigned long i = 32; i < LBLOCKSIZE * 8; i <<= 1)
    mask = (mask << i) | mask;
  while (1) {
    if (DETECTCHAR(*asrc, mask))
      break;
    asrc++;
  }

  /* resort to a bytewise loop. */
  for (src = (unsigned char *)asrc;; src++) {
    if (*src == d)
      return (void *)src;
  }
}
