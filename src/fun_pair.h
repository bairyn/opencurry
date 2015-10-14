/*
 * opencurry: fun_prim.h
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
 * fun_prim.h
 * ------
 *
 * Primitive, unwrapped values.
 */


#ifndef FUN_PAIR_H
#define FUN_PAIR_H
#include "base.h"

#include "fun_prim.h"

struct primpair_s
{
  union prim_u a;
  union prim_u b;
};
typedef struct primpair_s primpair_t;

/* Constructors. */
primpair_t primpair_pair(prim_t a, prim_t b);

/* Accessors. */
prim_t primpair_fst(primpair_t pair);
prim_t primpair_snd(primpair_t pair);
primpair_t primpair_setFst(primpair_t pair, prim_t a);
primpair_t primpair_setSnd(primpair_t pair, prim_t b);

/* Impure mutators. */
void primpair_impureMutateFst(primpair_t pair, prim_t a);
void primpair_impureMutateSnd(primpair_t pair, prim_t b);

#endif /* ifndef FUN_PAIR_H */
