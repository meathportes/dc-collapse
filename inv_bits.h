#ifndef INV_BITS_H
#define INV_BITS_H

#include "config.h"

/*
	Trivially passes
   correctness tests
	by brute force,
	but any values not
	1 <= d < 16 or not
	having the correct
	value for b is UB.
*/
inline static void
        inv_lt_4bits(
const UINT_TYPE   d ,
const UINT_TYPE   b ,
      UINT_TYPE * q ,
      UINT_TYPE * r){
	const UINT_TYPE
	 one =         1U,
	 bpb = b   <<   1;
	const UINT_TYPE
	 two = one <<   1,
	 bmo = b    - one,
	 bas = one << b  ,
	 nnn = one << bpb;
	const UINT_TYPE
	 hhh = one << bmo,
	 bx2 = b    ^ two;
	const UINT_TYPE
	 s2_ = bx2  ^ one;

	UINT_TYPE
	 ddd = d    - hhh;

	UINT_TYPE
	 brd = bas >> ddd,
	 dn_ = ddd  & s2_,
	 d0_ = ddd  & one,
	 d1h = ddd >>   1;

	UINT_TYPE
	 br_ = brd  - dn_,
	 d1_ = d1h  & one,
	 d0s = d0_ <<   4;

	UINT_TYPE
	 md0 = d0s  - d0_,
	 d1s = d1_ <<   5;

	UINT_TYPE
	 md1 = d1s  - d1_;

	UINT_TYPE
	 qh_,  qn_,  qx2 ,
	 qx_,  qd_,  qp_ ;

	*q   = bas  + br_;

	 /* independent */
	 qh_ = *q  << bmo;
	 qn_ = *q   & md0;
	 qx2 = *q  <<   1;

	 /*  dependent  */
	 qx_ = qx2  & md1;
	 qd_ = qn_  + qx_;
	 qp_ = qh_  + qd_;
	 *r  = nnn  - qp_;

	*q -= bas;
	--*q;
}

inline static void inv_nib(
 const UINT_TYPE * d,
   UINT_TYPE * q, UINT_TYPE * r) {

	/* Works with every builtin
	   integer type, but is
	   dependent on two's compl. */

	const UINT_TYPE c255 = 255U,
	                c129 = 129U,
	                 c67 =  67U,
	                 c45 =  45U,
	                  c8 =   8U;

	const UINT_TYPE t = *d  -   c8;
	const UINT_TYPE a = c129>> t  ,
	                f =  c67>> t  ;
	const UINT_TYPE b =  a  & UONE,
	                g =  f  & UONE;
	const UINT_TYPE c =  b  - *d  ;
	const UINT_TYPE h =  c  <<   1;
	const UINT_TYPE m =  h  +  c45;
	               *q =  m  + g   ;
	const UINT_TYPE p = *q  * *d  ;
	               *r = c255 - p  ;
	 /*Don't overflow 8-bit type.*/
	             ++*r             ;

	*q -= (UINT_TYPE) 17U;
}

#endif
