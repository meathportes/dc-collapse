#ifndef MUL_TRUNC_DIMQ_H
#define MUL_TRUNC_DIMQ_H

#include "config.h"

inline static void mul_dimq_trunc(
 UINT_TYPE * rhi, UINT_TYPE * lhs,
  UINT_TYPE * rhs, const unsigned int b) {

	const size_t h = b >> 1;

	const UINT_TYPE mh = (UONE << h) - UONE;

	UINT_TYPE rlo;

	 rlo   = *rhs & mh;
	*rhi   = *rhs >> h;
	 rlo  *= *lhs     ; // MUL 2
	*rhi  *= *lhs     ; // MUL 3
	 rlo  += *rhs & mh;
	*rhi  +=  rlo >> h;
	*rhi  += *rhs >> h;
	*rhi  += *rhs     ;
	*rhi >>=         h;
}

#endif
