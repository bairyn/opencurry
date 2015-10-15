/*
 * opencurry: fun.c
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

/* stddef.h:
 *   - NULL
 */
#include <stddef.h>

#include "base.h"
#include "util.h"

#include "fun_prim.h"
#include "fun_pair.h"

/* Pairs of primitive values. */

/* ---------------------------------------------------------------- */
/* Constructors. */

/* Create a pair of two primitive values. */
primpair_t primpair_pair(prim_t a, prim_t b)
{
  primpair_t pair;

  pair.a = a;
  pair.b = b;

  return pair;
}

/* Create a copy of a pair. */
primpair_t primpair_copy(primpair_t pair)
{
  primpair_t new_pair;

  new_pair.a = pair.a;
  new_pair.b = pair.b;

  return new_pair;
}

/* ---------------------------------------------------------------- */
/* Accessors. */

/* Return the first value from a pair. */
prim_t primpair_fst(primpair_t pair)
{
  return pair.a;
}

/* Return the second value from a pair. */
prim_t primpair_snd(primpair_t pair)
{
  return pair.b;
}

/* Create a copy of a pair with a different first value. */
primpair_t primpair_setFst(primpair_t pair, prim_t a)
{
  primpair_t new_pair;

  new_pair.a = a;
  new_pair.b = pair.b;

  return new_pair;
}

/* Create a copy of a pair with a different second value. */
primpair_t primpair_setSnd(primpair_t pair, prim_t b)
{
  primpair_t new_pair;

  new_pair.a = pair.a;
  new_pair.b = b;

  return new_pair;
}

/* ---------------------------------------------------------------- */
/* Mutators. */

/* Mutate the first value of a pair in-place, setting it to the provided value. */
void primpair_impureMutateFst(primpair_t pair, prim_t a)
{
  pair.a = a;
}

/* Mutate the second value of a pair in-place, setting it to the provided value. */
void primpair_impureMutateSnd(primpair_t pair, prim_t b)
{
  pair.b = b;
}
