#include <stdio.h>
#include <stdlib.h>

#include "inv_bits.h"
#include "mul_trunc_dimq.h"

void inv_machine_word(
 UINT_TYPE * d, const size_t b,
 UINT_TYPE * q, UINT_TYPE  * r) {

	size_t h, i, k, bmo;

	UINT_TYPE d_hi, d_lo, lm, p, q0_1, p_hi, r_hi, d_los, m, hf, lsb;

	if (b == 4)
		return inv_nib(d, q, r);

	if (b < 4)
		return inv_lt_4bits(*d,
		       (UINT_TYPE) b, q, r);

	if (b & 1) {

		*d <<= 1;
		inv_machine_word(d, b+1, q, r);
		*d >>= 1;

		*r >>= 1;

		if (! (*q & UONE)) {

			*r += *d;

			--*q;
		}

		*q >>= 1;
		*r >>= 1;

		ASSERT((*r < *d) == 1);

		return;
	}


	// dependencies for the recursive call
	h    =  b >> 1;
	d_hi = *d >> h;


	// recursive call (FIXME: call naive function)
	inv_machine_word(&d_hi,h,q,r);


	// create mask for d_lo
	bmo  =  b  -    1;
	hf   =       UONE;
	hf <<=  h        ;
	m=lm =  hf - UONE;
	m  <<=  h        ;
	m   |=  lm       ;
	d_lo = *d  & lm  ;


	ASSERT(*r < hf);
	/* scale *r */
	*r <<= h;


	/* MUL1 */
	p = *q * d_lo;


	/* first correction */
	r_hi   =
	         (UINT_TYPE)0;
	p     += d_lo        ;
	d_los  = d_lo << h   ;
	p     += d_los       ;
	p     &= m           ;
	p_hi   = p    < d_los;
	for (i = 0; i < 4; ++i) {

		if (r_hi < p_hi || (r_hi == p_hi && *r < p)) {

			*r += *d; *r &= m; if (*r < *d) ++r_hi;
			--*q;
		}
	}


	if (r_hi == 2 && p_hi == 1) {

		--r_hi;
		--p_hi;
	}


	ASSERT((r_hi  | p_hi) <= 1);
	ASSERT( r_hi >= p_hi      );


	/* lift */
	*r -= p;

	 r_hi  = r_hi != p_hi;
	*r    += r_hi << bmo ;
	*r    += r_hi << bmo ;


	/* mul2/3 -- q0_1 = (*q * *r) >> b */
	mul_dimq_trunc(&q0_1, q, r, b);
	ASSERT(q0_1 < hf);


	/* Mul 4 */
	p = q0_1 * d_hi;


	/* lift */
	*r -= p;


	/* penultimate correction */
	for (k = 0; k < 2; ++k) {

		if (! (*r < d_hi)) {

			*r -= d_hi;
			++q0_1;
		}
	}


	ASSERT(*r < d_hi);
	lsb    = q0_1 & UONE;
	q0_1 >>= 1;
	/* Mul 5 */
	p      = q0_1 * d_lo;
	q0_1 <<= 1;
	q0_1  |= lsb;
	p    <<= 1;

	ASSERT(p <= (p + d_lo));
	p += lsb ? d_lo : 0;


	/* final correction */
	ASSERT(*r < hf);
	*r <<= h;
	ASSERT(*r < *d);
	for (r_hi = 0, i = 0; i < 2; ++i) {

		if (*r < p && !r_hi) {

			*r += *d;
			r_hi += *r < *d;
			--q0_1;
		}
	}
	ASSERT(r_hi <= 1);
	*r -= p;
	*r += r_hi << bmo;
	*r += r_hi << bmo;


	ASSERT(*r < *d);


	/* reassemble */
	*q <<= h   ;
	*q  += q0_1;
	*q  += lm  ;
}
