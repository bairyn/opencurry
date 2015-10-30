/*
 * opencurry: bits.h
 *
 * Copyright (c) 2015, Byron James Johnson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * bits.h
 * ------
 */

#ifndef BITS_H
#define BITS_H
#include "base.h"

/*
 * ONE_BIT_REPEAT(num): generate a value whose last "num" bits are 1.
 *
 * Example:
 *   ONE_BIT_REPEAT(0): 0000 0000
 *   ONE_BIT_REPEAT(2): 0000 0011
 *   ONE_BIT_REPEAT(7): 0111 1111
 *   ONE_BIT_REPEAT(8): 1111 1111
 *
 * FIXME: 1st note
 *
 * Implementation note:
 *   "(1 << num) - 1" works except when generating fixed-width values with all
 *   1 bits, because what "(1 << num)" should be in this case is too large.
 *
 *   "(1 << num) - 1 | (1 << (num-1))" works except when num is 0, due to
 *   negative shift width.
 */
#define ONE_BIT_REPEAT(num) ((1 << (num)) - 1)
unsigned int one_bit_repeat_uint(unsigned int num);

#endif /* ifndef BITS_H */
