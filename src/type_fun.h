/*
 * opencurry: type_fun.h
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
 * type_fun.h
 * ----------
 */

#ifndef TYPE_FUN_H
#define TYPE_FUN_H
#include "base.h"

#include "type_base.h"

#ifdef TODO

/* ---------------------------------------------------------------- */
/* Function pointer wrappers.                                       */
/* ---------------------------------------------------------------- */

/*
 * Function pointer wrappers:
 *
 * These are trivial struct wrappers around function pointers.
 * These allow object pointers ("void *"-type) to be treated as function
 * pointers ("void (*)(void)"-type).
 */

/*
 * Type "primfun":
 *
 * Values of this type are untyped!
 *
 * Used to convert object pointers to function pointers and visa versa.
 */
const type_t *primfun_type(void);
extern const type_t primfun_type_def;
typedef struct primfun_s primfun_t;
struct primfun_s
{
  void *(*fun)(void *, ...);
};

/* Cast an object pointer to a function pointer. */
void *(*
  cast_objp_to_funp(void *objp)
  )(void *, ...);

/* Cast a function pointer to an object pointer. */
void *cast_funp_to_objp
  ( void *(*funp)(void *, ...)
  );

/* ---------------------------------------------------------------- */

const type_t *paramlist_type(void);
extern const type_t paramlist_type_def;
typedef struct paramlist_s paramlist_t;
struct paramlist_s
{
  typed_t type;

  memory_tracker_t memory;

  /* If this paramlist supports variable-length arguments, these determine the
   * first several arguments and their types.  This pair is called the "prefix"
   * in this case.
   */
  size_t         num_params;
  const type_t **param_types;

  /* Whether this list of parameters supports variable-length parameters.
   *
   * Fixed-width paramlist_t's can set this to NULL.
   *
   * "is_vararg" when called with arguments according to this paramlist's
   * prefix (num_params and param_types) (and no further arguments) should
   * return the argument-dependent "paramlist_t" describing the parameters
   * after the prefix, called the "tail".
   *
   * The "tail" may also be variable-width.
   */
  const paramlist_t *(*is_vararg)(const paramlist_t *self, ...);
};

/* ---------------------------------------------------------------- */

/*
 * Type "fun":
 *
 * A struct containing a function pointer, that might be partially applied.
 */
const type_t *fun_type(void);
extern const type_t fun_type_def;
typedef struct fun_s fun_t;
struct fun_s
{
  typed_t type;

  memory_tracker memory;

  /* ---------------------------------------------------------------- */

  void *(*fun)(void *, ...);

  /* NULL for "void" return type. */
  const type_t      *return_type;
  /* NULL is equivalent to an empty parameter list, e.g. "void foo(void)". */
  const paramlist_t *paramlist;

  /* ---------------------------------------------------------------- */

  /* "partially_applied_args" should contain space for at least
   * "num_partially_applied".
   */
  size_t       num_partially_applied;
  void       **partially_applied_args;
}

/* The arguments must exclude parameters already partially applied! */
void *call_fun(const fun_t *fun, ...);

/* "dest" and "fun" can overlap.
 *
 * If "dest" is NULL, allocate a new one.  Otherwise, "dest" should be
 * initialized.
 *
 * Returns a pointer to partially applied function on success, which is "dest"
 * when non-NULL.
 */
void partially_apply_fun(fun_t *dest_initialized, const fun_t *fun, ...);

#endif /* #ifdef TODO */

#endif /* ifndef TYPE_FUN_H */
