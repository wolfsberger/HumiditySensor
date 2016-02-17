#ifndef PORTABILITY_H_INCLUDED
#define PORTABILITY_H_INCLUDED

#ifdef MBED
#define ALIGN_MBED(n) (__align(n))
#define ALIGN_EMBLOCKS(n)
#else
#define ALIGN_MBED(n)
#define ALIGN_EMBLOCKS(n) (__attribute__ ((aligned (n))))
#endif

#endif /* PORTABILITY_H_INCLUDED */
