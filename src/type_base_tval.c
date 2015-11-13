/*
 * opencurry: type_base_tval.c
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

#include "base.h"
#include "type_base_prim.h"
#include "type_base_tval.h"

#include "type_base_type.h"
#include "type_base_typed.h"

#include "ptrs.h"

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/*
 * Obtain the type of a "tval *".
 *
 * Only typed "tval"s are supported!
 */
const type_t *typeof(const tval *val)
{
  return type_is_typed(typeof_indirect(val));
}

/*
 * Obtain the indirect type of a "tval *", without calling "type_is_typed" to
 * obtain a possibly more specific reference to the type.
 */
const type_t *typeof_indirect(const tval *val)
{
  return (const type_t *) (tval_get_typed(val)());
}

/*
 * "tval *"s are values that can be cast to "typed_t", generally because they
 * are structs whose first field has type "typed_t".
 *
 * Interpret the "tval *" as its type, "typed_t".
 */
typed_t tval_get_typed(const tval *val)
{
  return (typed_t) (objpc_to_funpc((void *) val));
}
