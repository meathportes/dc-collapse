#include "inv_bits.h"

#include <stdio.h>

void inv_lt_4bits_test() {

	size_t bits, num_errors = 0,
	 bits_by_d[]={1,2,2,3,3,3,3};

	UINT_TYPE q, r, qq, rr, D, N,
	 N_by_d[]={4,16,16,64,64,64,64};

	for (D = 1U; D < 8U; ++D,
	      num_errors+=((q!=qq)|(r!=rr))){

		bits = bits_by_d[D-1U];/* d>0 */

		/* our oracle */
		inv_lt_4bits(D, bits, &q, &r);

		/* get values to test against */
		N=N_by_d[D-1U];
		qq=N/D - (UONE << bits) - UONE;
		rr=N%D;

		printf(UFMT " / "  UFMT " == "
		  "[ " UFMT   "r"  UFMT " " UFMT
		   "r" UFMT " ] (" "%d" " bits)"
		 "\n",N,D,q,r,qq,rr,(int) bits);
	}

	printf("\nError count: %u\n\n",
	 (unsigned int) num_errors);
}

void inv_nib_test() {

	UINT_TYPE q, r, qq, rr, d = 8U;

	unsigned short int n = 256;

	for (; d < (UINT_TYPE) 16U; ++d) {

		inv_nib(&d, &q, &r);

		qq = n / (unsigned short int) d - 17U;
		rr = n % (unsigned short int) d;

		printf("%d / %d == "
		            "[ %dr%d %dr%d ]\n",
		 (int) n, (int) d , (int) q ,
		 (int) r, (int) qq, (int) rr  );

		if (q != qq || r != rr)
			fprintf(stderr,"\nError!\n");
	}
}

int main() {

	printf("Testing inv_lt_4bits...\n");

	inv_lt_4bits_test();

	printf("Testing inv_nib...\n");

	inv_nib_test();

	return 0;
}
