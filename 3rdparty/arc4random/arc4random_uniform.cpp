/*	$OpenBSD: arc4random_uniform.c,v 1.2 2015/09/13 08:31:47 guenther Exp $	*/

/*
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// This is arc4random_uniform.c from OpenBSD's source tree at
// $OPENBSD_ROOT/lib/libc/crypt/arc4random_uniform.c.
//
// Mumble-specific changes:
//
// - Change file extension to .cpp and rename to mumble_arc4random_uniform.cpp.
// - Add CryptographicRandom.h include.
// - Remove <sys/types.h> and <stdlib.h> includes, include <stdint.h> for uint32_t.
// - Added an implementation of arc4random called mumble_arc4random.
// - Rename arc4random_uniform to mumble_arc4random_uniform.
// - Change call to arc4random in mumble_arc4random_uniform to call
//   mumble_arc4random instead of arc4random.
// - Remove DEF_WEAK for arc4random_uniform.
// - Disable MSVC warning 4146.

#include <stdint.h>
#include "crypto/CryptographicRandom.h"

#if defined(_MSC_VER)
// Disable "unary minus operator applied to unsigned type, result still unsigned"
// warning. The unary minus operator used in arc4random_uniform is perfectly valid.
# pragma warning(disable: 4146)
#endif

// Call through to CryptographicRandom::uint32()
uint32_t
mumble_arc4random(void)
{
	return CryptographicRandom::uint32();
}

/*
 * Calculate a uniformly distributed random number less than upper_bound
 * avoiding "modulo bias".
 *
 * Uniformity is achieved by generating new random numbers until the one
 * returned is outside the range [0, 2**32 % upper_bound).  This
 * guarantees the selected random number will be inside
 * [2**32 % upper_bound, 2**32) which maps back to [0, upper_bound)
 * after reduction modulo upper_bound.
 */
uint32_t
mumble_arc4random_uniform(uint32_t upper_bound)
{
	uint32_t r, min;

	if (upper_bound < 2)
		return 0;

	/* 2**32 % x == (2**32 - x) % x */
	min = -upper_bound % upper_bound;

	/*
	 * This could theoretically loop forever but each retry has
	 * p > 0.5 (worst case, usually far better) of selecting a
	 * number inside the range we need, so it should rarely need
	 * to re-roll.
	 */
	for (;;) {
		r = mumble_arc4random();
		if (r >= min)
			break;
	}

	return r % upper_bound;
}
