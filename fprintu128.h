#ifndef PRINTU128_H
#define PRINTU128_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#define uint128_t __uint128_t

inline static uint8_t quorem100(const uint16_t a, uint8_t * r) {

	uint32_t q  = a     ;
	uint16_t r_ = 10000U;

	++q;

	q  *= 233U;  q >>=  6;
	q  *=   5U;  q >>=  2;
	q  *=   9U;  q >>= 12;
	r_ -=  q*a; *r   = r_;

	return q;
}

inline static size_t printu10(FILE * fp, uint8_t a, size_t * any) {

	if (*any | a) {

		fprintf(fp, "%u", (unsigned int) a);
		*any = 1;
	}

	return 1;
}

inline static size_t printu100(FILE * fp, uint8_t a, size_t * any) {

	if (! printu10(fp, a / 10, any)) return 0;
	return printu10(fp, a % 10, any);
}

inline static size_t printu10000(FILE * fp, uint16_t a, size_t * any) {

	if (! printu100(fp, a / 100, any)) return 0;
	return printu100(fp, a % 100, any);
}

inline static size_t printu100000000(FILE * fp, uint32_t a, size_t * any) {

	if (! printu10000(fp, a / 10000, any)) return 0;
	return printu10000(fp, a % 10000, any);
}

size_t fprintu128(FILE * fp, const uint128_t a, const size_t pad) {

	size_t any = pad != 0;

	uint128_t a_ = a;

	const uint32_t  lo_lo = a_ % 100000000UL; a_ /= 100000000UL;
	const uint32_t  lo_hi = a_ % 100000000UL; a_ /= 100000000UL;
	const uint32_t mid_lo = a_ % 100000000UL; a_ /= 100000000UL;
	const uint32_t mid_hi = a_ % 100000000UL; a_ /= 100000000UL;

	const uint16_t hi_lo  = a_ % 10000U; a_ /= 10000U;

	const uint8_t  hi_mid = a_ %   100U; a_ /=   100U;
	const uint8_t  hi_hi  = a_;

	if (! printu10       (fp,  hi_hi , &any)) return 0;
	if (! printu100      (fp,  hi_mid, &any)) return 0;
	if (! printu10000    (fp,  hi_lo , &any)) return 0;
	if (! printu100000000(fp, mid_hi , &any)) return 0;
	if (! printu100000000(fp, mid_lo , &any)) return 0;
	if (! printu100000000(fp,  lo_hi , &any)) return 0;
	if (! printu100000000(fp,  lo_lo , &any)) return 0;

	if (! any)
		fprintf(fp, "0");

	return 1;
}

size_t printu128(const uint128_t a, const size_t pad) { return fprintu128(stdout, a, pad); }

#endif
