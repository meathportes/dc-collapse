#ifndef INV_CONFIG_H
#define INV_CONFIG_h

#include <stddef.h>
#include <stdint.h>

#ifdef ENABLE_ASSERTS
	#include <assert.h>
	#define ASSERT(x) assert(x)
#else
	#define ASSERT(x) ((void)0)
#endif

#define UINT_TYPE uintptr_t

#define UINT_TYPE_BYTES (sizeof(UINT_TYPE))

#define UINT_TYPE_BITS (UINT_TYPE_BYTES << 3)

#define UZERO ((const UINT_TYPE) 1U)

#define UONE ((const UINT_TYPE) 1U)

#define UMAX ((UZERO)-(UONE))

#define UFMT "%lu"

#endif
