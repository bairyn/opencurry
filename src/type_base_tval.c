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

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/*
 * Obtain the type of a value.
 *
 * "val" must be valid "tval" or NULL, but its polymorphically associated
 * "typed_t" type can be NULL.
 *
 * If "val" or its type is NULL, this returns NULL.
 */
#ifdef TODO /* TODO: needs "type_container" implemented first. */
typed_t typedof(const tval *val)
{
  const type_t *type_def;

  if (!val)
    return NULL;

  type_def = typeof(val);
  if (!type_def)
    return NULL;

  return type_container(type_def);
}
#endif /* #ifdef TODO */

/*
 * Obtain the type of a value.
 *
 * "val" must be valid "tval" or NULL, but its polymorphically associated
 * "typed_t" type can be NULL.
 *
 * If "val" or its type is NULL, this returns NULL.
 */
const type_t *typeof(const tval *val)
{
  const type_t *indirect_type_def;

  if (!val)
    return NULL;

  indirect_type_def = typeof_indirect(val);
  if (!indirect_type_def)
    return NULL;

  return type_is_typed(indirect_type_def);
}

/*
 * Obtain the indirect type of a "tval *", without calling "type_is_typed" to
 * obtain a possibly more specific reference to the type.
 *
 * "val" can be NULL, and it can also have a NULL "typed_t" value, but it still
 * must be a valid "tval".
 *
 * Ultimately, "(tval *) val" is interpreted as "(typed_t *) val", i.e. a
 * pointer to a function that returns a "type_t".
 */
const type_t *typeof_indirect(const tval *val)
{
  typed_t typed;

  if (!val)
    return NULL;

  typed = tval_get_typed(val);
  if (!typed)
    return NULL;

  return (const type_t *) (typed());
}

/*
 * Dereference the tval reference as a reference to "typed_t".
 *
 * Returns NULL if "val" is NULL.
 *
 * Also returns NULL if "val"'s "typed" type is NULL (i.e. when the value's
 * relevant (first) "typed_t" field is NULL).
 *
 * Passing a reference to a value that is not a "tval" results in undefined
 * behaviour.
 *
 * Generally, the type of the value is a struct whose first field has type
 * "typed_t" (i.e., the value is in fact a "tval").
 */
typed_t tval_get_typed(const tval *val)
{
  const typed_t *typed_ref;
  typed_t        typed;

  if (!val)
    return NULL;

  typed_ref = (const typed_t *) (tval_to_typed(val));
  if (!typed_ref)
    return NULL;

  typed = *typed_ref;
  if (!typed)
    return NULL;

  return (typed_t) typed;
}

/*
 * "tval *"s are values that can be cast to "typed_t", generally because they
 * are structs whose first field has type "typed_t".
 *
 * Interpret a reference to a tval as a reference to a typed_t, i.e. the "tval
 * *" as its type, "const typed_t *".
 */
const typed_t *tval_to_typed(const tval *val)
{
  if (!val)
    return NULL;

  return (const typed_t *) val;
}
