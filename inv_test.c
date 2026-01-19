#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "config.h"
//#include "inv_sub_machine_word.h"
#include "inv_machine_word.h"
#include "fprintu128.h"

UINT_TYPE rand_UINT_TYPE(const size_t b) {

	size_t i;

	UINT_TYPE r = UZERO;

	r ^= rand();

	for (i = 0; i < 9; ++i) {

		r <<= 7;
		r ^= rand();
	}

	return r >> ((sizeof(UINT_TYPE) << 3) - b);
}

int main(int argc, char ** argv) {

	unsigned int benchmode = 0;

	size_t b, num_errors, tmp_bits;
	UINT_TYPE i, d, q, r, rr, d_;

	__uint128_t n, q_, qq_, d_min, d_max;

	size_t num_tests, bit_sizes_to_test[65];

	for (i = 0; i < 65; ++i)
		bit_sizes_to_test[i] = 0;

	for (num_tests = 0, i = 1; i < (UINT_TYPE) argc; ++i) {

		tmp_bits = (size_t) atoi(argv[i]);

		if (tmp_bits < 1 || tmp_bits > UINT_TYPE_BITS) {

			fprintf(stderr, "\nIgnoring invalid size: %u\n", (unsigned int) tmp_bits);

		} else if (bit_sizes_to_test[tmp_bits]) {

			fprintf(stderr, "\nIgnoring duplicate bit size: %u\n", (unsigned int) tmp_bits);

		} else {

			bit_sizes_to_test[tmp_bits] = 1;
			++num_tests;
		}
	}

	if (argc < 2 || num_tests == 0) {

		fprintf(stderr, "\n\nUsage: %s [0 < TEST BIT SIZE <= %u]...\n\n\n", argv[0], (unsigned int) UINT_TYPE_BITS);
		return 1;
	}

	printf("\n\nTesting bit size(s):");

	for (i = 0; i < 65; ++i) {

		if (bit_sizes_to_test[i])
			printf(" %u", (unsigned int) i);
	}

	printf("\n\n");

	srand(time(0));

	num_errors = 0;

	for (i = 0; i < 65; ++i) {

		if (bit_sizes_to_test[i] == 0)
			continue;

		b = i;

		if (b < 33) {

			printf("Testing all denominators for %u bits...\n\n", (unsigned int) b);

		} else {

			printf("Testing random %u bit denominators...\n\n", (unsigned int) b);
		}

		d_max = ((__uint128_t) UONE)  << b;

		d_min = d_max >> 1;

		for (d = d_min; d < d_max; ++d) {

			if (b < 33) {

				d_ = d;

			} else {

				d_ = rand_UINT_TYPE(b-1) + (UONE << (b-1));
			}

			inv_machine_word(&d_, b, &q, &r);

			/* Canonicalize. */
			q_ = q;
			q_ += ((__uint128_t) UONE) << b;
			++q_;


			/* Use diminished N to make max word width work. */

if (! benchmode) {

			n  = UONE;
			n <<= (b << 1) - 1;
			--n;
			n <<= 1;
			n |= UONE;
			qq_ = n / d_;

			if (d_ == d_min) {

				qq_ = d_ << 2;
				rr  = 0;
			}

			rr = n - ((__uint128_t) qq_)*d_ + 1;

} else {

	qq_ = q_;
	rr = r;
}

			if ((d & 0xFFFFFF) == 0 || b < 24 || 0) {

				printf("2^%lu", b << 1);
				printf("/" UFMT " == [ ", d_);
				printu128(q_, 0);
				printf("r" UFMT " ", r);
				printu128(qq_,0);
				printf("r" UFMT " ] (%lu errors)\n", rr, num_errors);
			}

			if (q_ != qq_  || r != rr) {

				++num_errors;
				fprintf(stderr, "Error!\n\n");
				/* exit(1); */
			}
		}
	}

	if (num_errors)
		fprintf(stderr, "\nNumber of errors: %lu", num_errors);
	else
		printf("\nOk!\n\n");

	return 0;
}
