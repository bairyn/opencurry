/*
 * opencurry: type.h
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
 * type.h
 * ------
 */


#ifndef TYPE_H
#define TYPE_H
#include "base.h"

/*
 * tval:
 *
 * Label for typed values.
 *
 * In the code:
 *
 * > tval *foo
 *
 * The type "tval" is intended to denote that the type used for "foo" is, or is
 * equivalent to, a struct whose first field has type "type_t *".
 *
 * This allows users to obtain the type of "foo", by dereferencing the first
 * field of "foo":
 *
 * > type_t *type_of_foo;
 * >
 * > type_of_foo = *((type_t **) foo);
 *
 * "type_t *" can also indirectly be the first field: a valid alternative is a
 * concrete struct, but not a pointer to a concrete struct, whose first field
 * has type "type_t *".
 */
typedef void tval;

/* A structure that describes a "type", a structure with associated procedures
 * for initialization and such.
 *
 * A "type_t" describes a struct.  Such structs should have the first field set
 * to "type_t *".
 */
#define USER_STATIC_SIZE 256
enum type_heap_info_e
{
  type_heap_none    = 0,

  type_heap_self    = (0 << 1) | (1 << 0),
  type_heap_dynamic = (1 << 1) | (0 << 0),

  type_heap_dynamic_self
                    = (1 << 1) | (1 << 1)
};
typedef enum type_heap_info_e type_heap_info_t;

#define TYPE_VALID_TVAL        0
#define TYPE_NOVALID_TVAL_GEN_1 1
typedef struct type_s type_t;
struct type_s
{
  /* Make "type_t" itself a type by setting the first field as "type_t *".  */
  /* All initialized "type_t" values should have "type" set to "type_type". */
  type_t         *type;

  /* ---------------------------------------------------------------- */

  /*
   * Wherever "tval" occurs in "type_t" values, "tval" must be equal to the
   * associated type that "type_t" describes.
   */

  /* The type of "tval *" parameters.  Should return "self". */
  type_t             *(*id)              (type_t *self);

  /* The type of "initialization" parameters. */
  type_t             *(*init_type)       (type_t *self);

  /* Initialization a value.  Some types support NULL "initialization"
   * arguments.
   */
  void                (*init)            (type_t *self, tval *val,                  const tval *initialization);
  void                (*free)            (type_t *self, tval *val);

  /*
   * Verify the initialization argument, which can be NULL, is valid.
   *
   * Should return "0" when valid, and anything else when not.
   *
   * NULL output arguments must be supported.
   */
  int                 (*verify_init)     (type_t *self, const tval *initialization, char *out_err_msg,          size_t err_msg_size);

  /*
   * It is recommended that invalid initialization arguments are cleanly
   * handled.  "NULL" might (or might not) be returned for invalid
   * initialization arguments.
   */
  tval               *(*new)             (type_t *self, const tval *initialization);

  /*
   * 3 Variants for copying data:
   *
   * These descriptions informally describe an intuition that depends on the
   * contexts in which they are used.
   *
   * In particular, what constitutes a level of indirection is unspecified
   * here, and what constitutes a level of indirection according to one type is
   * not necessarily what constitutes a level of indirection according to
   * another (but, particularly when that alternate type is a
   * "type_t"-associated type, it usually is).
   *
   * For example, "int **" could plausible be treated as each of the following:
   *   1) A 4-4 matrix.
   *
   *   2) A reference to an "int *"-represented file handle.
   *   3) A reference to a vector represented by 4 integers.
   *
   *   4) A reference to a reference of an integer.
   *
   * Importantly, each pointer "reference" could be considered *incidental* to
   * a structure, e.g. as part of an efficient way to link two structures, but
   * they could also be considered to be proper "references" that conceptually
   * add a level of indirection.
   *
   * Depending on what the type considers a level of indirection for a field of
   * this type, the 3 copying variants could do different things:
   *   1): "direct":    dest =   src;
   *       "shallow":  *dest =  *src;
   *       "deep":    **dest = **src;
   *
   *   2),
   *   3): "direct":    dest =   src;
   *       "shallow":  *dest =  *src;
   *       "deep":     *dest =  *src;
   *
   *   4): "direct":    dest =   src;
   *       "shallow":   dest =   src;
   *       "deep":      dest =   src;
   *
   * deep:
   *   Mutating the state of either "src" or "src"'s dependents should not
   *   affect the state of "dest" and its referents.
   *
   *   TODO
   *
   *   Referents, e.g. file handles or pointers considered as "references", can
   *   still be copied as such.
   *
   * shallow:
   *   Mutating the fields of "src" should not affect the fields of "dest".
   *
   *   The most flexibility is given to implementations of "shallow" cloning.
   *
   *   TODO
   *
   * direct:
   *   All references and values should be TODO
   * If "src" is at first independent from "dest"
   */
  void                (*deep_dup)        (type_t *self, tval *dest,                 const tval *src);
  void                (*shallow_dup)     (type_t *self, tval *dest,                 const tval *src);
  void                (*direct_dup)      (type_t *self, tval *dest,                 const tval *src);

  tval               *(*new_deep_copy)   (type_t *self, const tval *src);
  tval               *(*new_shallow_copy)(type_t *self, const tval *src);
  tval               *(*new_direct_copy) (type_t *self, const tval *src);


  const char        **(*name)            (type_t *self);
  const char        **(*info)            (type_t *self);
  size_t              (*size)            (type_t *self);

  /*
   * Whether values of this type are necessarily valid "tval"s.
   *
   * This should be "false" if the described type doesn't always contain
   * "type_t *" in the first field.
   */
  int                 (*valid_tval)      (type_t *self);

  void               *(*user_bufTODO)        (type_t *self);
  size_t              (*user_bufTODO_size)   (type_t *self);


  unsigned char         user_static[USER_STATIC_SIZE];
  size_t                user_static_num;
  size_t                user_static_size;

  void                 *user_buf;
  size_t                user_buf_size;

  type_heap_info_t      heap_info;
};

/* TODO */
#ifdef TODO
extern const type_t type_type;

typedef struct type_init_s type_init_t;
struct type_init_t
{
  type_t *type;

  /* ---------------------------------------------------------------- */
  /* Minimum required arguments to construct a "type_t".              */
  /* ---------------------------------------------------------------- */

  /* ---------------------------------------------------------------- */
  /* Optional arguments.                                              */
  /* ---------------------------------------------------------------- */

  int override_id;
  int override_init_type;
  int override_init;
  int override_free;
  int override_verify_init;
  int override_new;
  int override_deep_dup;
  int override_shallow_dup;
  int override_new_deep_copy;
  int override_new_shallow_copy;
  int override_new_direct_copy;
  int override_name;
  int override_info;
  int override_size;
  int override_valid_tval;
  int override_user_static;
  int override_user_static_size;
  int override_user_buf;
  int override_user_buf_size;

  type_t             *(*id)              (type_t *self);
  type_t             *(*init_type)       (type_t *self);
  void                (*init)            (type_t *self, tval *val,                  const tval *initialization);
  void                (*free)            (type_t *self, tval *val);
  int                 (*verify_init)     (type_t *self, const tval *initialization, char *out_err_msg,          size_t err_msg_size);
  tval               *(*new)             (type_t *self, const tval *initialization);
  void                (*deep_dup)        (type_t *self, tval *dest,                 const tval *src);
  void                (*shallow_dup)     (type_t *self, tval *dest,                 const tval *src);
  void                (*direct_dup)      (type_t *self, tval *dest,                 const tval *src);
  tval               *(*new_deep_copy)   (type_t *self, const tval *src);
  tval               *(*new_shallow_copy)(type_t *self, const tval *src);
  tval               *(*new_direct_copy) (type_t *self, const tval *src);
  const char        **(*name)            (type_t *self);
  const char        **(*info)            (type_t *self);
  size_t              (*size)            (type_t *self);
  int                 (*valid_tval)      (type_t *self);
  void               *(*user_buf)        (type_t *self);
  size_t              (*user_buf_size)   (type_t *self);
  unsigned char         user_static[USER_STATIC_SIZE];
  size_t                user_static_size;
  void                 *user_buf;
  size_t                user_buf_size;
};

extern const type_t type_init_type;

/* TODO */
#endif /* TODO */

/* ---------------------------------------------------------------- */

#endif /* ifndef TYPE_H */
