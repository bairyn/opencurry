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
/* stdlib.h:
 *   - calloc
 *   - free
 *   - malloc
 *   - realloc
 */
#include <stdlib.h>

#include "base.h"

/* TODO: NULL "type" to indicate initialization once referenced? */

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/*
 * tval:
 *
 * Label for so-called "typed values", values associated with a "type_t".
 *
 * "tvals" are generally values of a struct whose first field is "type_t *".
 *
 * Example:
 *
 * > extern type_t intpair_type;
 * >
 * > typedef struct intpair_s intpair_t;
 * > struct intpair_s
 * > {
 * >   type_t *type;
 * >
 * >   int     a;
 * >   int     b;
 * > };
 * >
 * > void debug_print(const char *fmt, ...);
 * >
 * > void debug_tval(const tval *val);
 * >
 * > int intpair_sum(intpair_t *pair, int is_debug)
 * > {
 * >   int sum;
 * >
 * >   sum = pair->a + pair->b;
 * >
 * >   if (is_debug)
 * >   {
 * >     debug_print
 * >       ( "intpair_sum:\n"
 * >         "  %d + %d => %d\n"
 * >       , pair->a
 * >       , pair->b
 * >       , sum
 * >       );
 * >
 * >     debug_tval(pair);
 * >   }
 * >
 * >   return sum;
 * > }
 * >
 * > void debug_tval(const tval *val)
 * > {
 * >   const type_t *type = (const type_t *) val;
 * >
 * >   size_t      type_size;
 * >   const char *type_name;
 * >
 * >   type_size = type->size(type);
 * >   type_name = type->name(type);
 * >
 * >   debug_print
 * >     ( "debug_tval:\n"
 * >       "  type name: %s\n"
 * >       "  type size: %d\n"
 * >     , (const char *) type_name
 * >     , (size_t)       type_size
 * >     );
 * > }
 * >
 * > static intpair_t my_intpair =
 * >   { intpair_type
 * >
 * >   , 7
 * >   , 8
 * >   };
 * >
 * > static tval_t *my_intpair_val  = &my_intpair;
 * > static type_t *my_intpair_type = (type_t *) my_intpair_val;
 *
 * ----------------------------------------------------------------
 *
 * A "tval" is a value of a struct whose first field is "type_t *", a reference
 * to the type .
 *
 * --
 *
 * Given:
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

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

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

/*
 * Information about a type, with methods to construct and allocate memory to
 * new values.
 */
#define TYPE_VALID_TVAL        0
#define TYPE_NOVALID_TVAL_GEN_1 1
typedef struct type_s type_t;
struct type_s
{
  /* Make "type_t" itself a type by setting the first field as "type_t *".  */
  /* All initialized "type_t" values should have "type" set to "type_type". */
  type_t *type;

  /* ---------------------------------------------------------------- */

  struct memory_tracker_s *memory;

  /* ---------------------------------------------------------------- */
  /* Basic struct info.                                               */
  /* ---------------------------------------------------------------- */

  /* The type of "tval *" parameters.  Should return "self".          */
  type_t     *(*id)          (type_t *self);


  /* Name of the type.  (Not necessarily unique.)                     */
  const char *(*name)        (type_t *self);

  /* General information about the type.                              */
  const char *(*info)        (type_t *self);

  /* Size of a value of this type.                                    */
  size_t      (*size)        (type_t *self, const tval_t *val);

  /* Size of any value of this type.                                  */
  /*                                                                  */
  /* This must be NULL if values can have a variable size.            */
  /* Otherwise, this must return the size of a value of this type.    */
  size_t      (*uniform_size)(type_t *self);


  /* ---------------------------------------------------------------- */
  /* Initializers.                                                    */
  /* ---------------------------------------------------------------- */

  /*
   * Initializers:
   * -------------
   *
   * A type contains methods that initialize memory allocated for a value of
   * the represented type.
   *
   * How "init" initializes a value depends on the so-called "constructor",
   * passed as the "cons" parameter.
   *
   *
   * Constructors:
   * -------------
   *
   * In the most common case, "cons" contains:
   *   1) Whether to "malloc" a new value, or initialize an already-allocated
   *     "tval *" (e.g. it's on the stack, or it was "malloc" externally.)
   *
   *      If "init" calls "malloc" on the new value, then "free" should
   *      likewise free this memory.
   *
   *   2) Initial values for some fields.
   *
   * Example:
   *
   * > extern type_t intpair_type;
   * >
   * > typedef struct intpair_s intpair_t;
   * > struct intpair_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Whether this pair exists on the heap. *-/
   * >   /-* intpair_type's "free" checks this.    *-/
   * >   int is_malloc : 1;
   * >
   * >   int a;
   * >   int b;
   * > };
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one. *-/
   * >   intpair_t *val;
   * >
   * >   /-* Initial values for "a" and "b".    *-/
   * >   int     a;
   * >   int     b;
   * > };
   *
   * Constructor convention #1: ad-hoc initial values
   * ------------------------------------------------
   *
   * Here we establish a convention for the contents of "constructor"
   * structures:
   *   1) > type_t *type
   *
   *      for the associated type.
   *
   *   2) > <type *>val
   *
   *      Or
   *
   *      > <type *>val_non_null
   *
   *      If this is non-NULL, initializers should assume the memory is already
   *      being managed indepentently and should initialize "val".
   *
   *      Sometimes developers don't want to track whether values have been
   *      dynamically allocated.  If initializers don't support dynamic
   *      allocation, this value should be non-NULL.
   *
   *      If dynamic memory allocation is supported, this value may be NULL.
   *
   *   3) If the initializer supports dynamic memory allocation:
   *
   *      Whether and how to "malloc" and "free" a new value.  In the simplest
   *      case, this can be a pointer used as described in the above example.
   *
   *      However, we recommend for non-trivial cases also including a more
   *      versatile pair of function pointers:
   *
   *      > <type *> (*malloc)(size_t  size);
   *      > void     (*free)  (type   *val);
   *
   *      More complicated constructers might instead include 2 function
   *      pointers for allocating
   *
   *   3) Fields to calculate essential initial values.
   *
   *      Values of 0 or NULL should indicate that a reasonable default should
   *      be chosen.
   *
   *      If it's important to allow convenient non-default initial 0 or NULL
   *      values, additional fields can be added for this; for example:
   *
   *      > typedef struct foo_cons_s foo_cons_t
   *      > struct foo_cons_s
   *      > {
   *      >   type_t *type;
   *      >
   *      >   /-* Set to NULL to "malloc" a new one.             *-/
   *      >   intpair_t *val;
   *      >
   *      >   /-* Set to NULL to default to "malloc" and "free". *-/
   *      >   intpair_t (*malloc)(size_t     size);
   *      >   void      (*free)  (intpair_t *val);
   *      >
   *      >   double     size;
   *      >   double    *pos;  int force_pos;
   *      >   double    *vel;  int force_vel;
   *      > };
   *
   *      (Here we assume a "size" of "0.0" represents a default "size" value,
   *      perhaps because a "size" of "0.0" doesn't make sense.)
   *
   *      Note: it is often unnecessary to include a copy of *every* field,
   *      because in relevant use cases, values could simply be directly
   *      constructed by hand, or copied from another value.
   *
   *   4) Any ad-hoc extra fields.
   *
   * Example:
   *
   * > extern type_t intpair_type;
   * >
   * > typedef struct intpair_s intpair_t;
   * > struct intpair_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Can be NULL (or a no-op) if this pair was not dynamically *-/
   * >   /-* allocated.                                                *-/
   * >   void (*free)(intpair_t *val);
   * >
   * >   int a;
   * >   int b;
   * > };
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one.             *-/
   * >   intpair_t *val;
   * >
   * >   /-* Set to NULL to default to "malloc" and "free". *-/
   * >   intpair_t (*malloc)(size_t     size);
   * >   void      (*free)  (intpair_t *val);
   * >
   * >   /-* Initial values for "a" and "b".                *-/
   * >   int a;
   * >   int b;
   * > };
   *
   * #2: Same-type constructors:
   * ---------------------------
   *
   * For some types, it is simpler to set the constructor type as the same type
   * as the type itself.
   *
   * In this case, the conventional behaviour is this:
   *
   *   The constructor itself is initialized.
   *
   *   No dynamic memory allocation is done, e.g. with "malloc".
   *
   *   All "0" and "NULL" fields are interpreted to represent the default
   *   values.
   *
   * The constructor type of constructor types themselves are usually the same.
   *
   * #3: Template constructurors:
   * ----------------------------
   *
   * This convention for constructor types affords the simplicity of the 2nd
   * type described above along with the functionality of the 1st.
   *
   * Rather than providing ad-hoc initial values, a pointer to the struct is
   * provided instead, where each "0" and "NULL" field indicates that an
   * uninitialized field, and that a default or newly initialized one should be
   * assigned.
   *
   * Example:
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one.             *-/
   * >   intpair_t *val;
   * >
   * >   /-* Set to NULL to default to "malloc" and "free". *-/
   * >   intpair_t (*malloc)(size_t     size);
   * >   void      (*free)  (intpair_t *val);
   * >
   * >   intpair_t *initials;
   * >   intpair_t *force_non_defaults;
   * > };
   *
   * A second reference, "force_non_defaults" is also included.  This can be
   * NULL.
   *
   * A non-default, i.e. non-zero/non-NULL field, value is unconditionally
   * respected.
   *
   * For all default values in "initials", if force_non_defaults is not NULL,
   * its value is checked.  If the value is any non-zero/non-NULL value, the
   * the value in "initials" forced to be its 0/NULL value.
   *
   * Several general template constructor types are provided below, but some
   * types might want to define their own variant.
   */


  /* Initialize a value of this type.                                 */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Initializers might support both initializing an already          */
  /* allocated value, and also allocated new memory with dynamically  */
  /* allocated.                                                       */
  /*                                                                  */
  /* Which forms of initialization are support are reflected in the   */
  /* type of "cons", the constructor.                                 */
  /*                                                                  */
  /* Any allocation performed by "init" should be properly freed by   */
  /* "free".  If a new "tval *" is allocated with "malloc", for       */
  /* example, then "free" should free this memory.                    */
  /*                                                                  */
  /* Some initializers support NULL constructor arguments,            */
  /* generally indicating all default values.                         */
  /*                                                                  */
  /* Generally, multiple calls to "init" results in undefined         */
  /* behaviour.                                                       */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Returns NULL on failure to initialize.                           */
  tval   *(*init)     (type_t *self, tval *cons);

  /* The type of the "cons" constructor parameter for "init".         */
  /*                                                                  */
  /* "init" must be called with a "cons" value of this type.          */
  type_t *(*cons_type)(type_t *self);

  /* Deinitialization a value, freeing resources allocated by "init". */
  /*                                                                  */
  /* Support for idempotence (multiple calls to "free") is            */
  /* recommended but not required.                                    */
  void    (*free)     (type_t *self, tval *val);

  /* ---------------------------------------------------------------- */
  /* Copying.                                                         */
  /* ---------------------------------------------------------------- */

  /* TODO: -1 for unbounded recursive */
  TODO    (*dup)      (type_t *self, tval *dest, const tval *src, int rec);




































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

  void               *(*user_buf)        (type_t *self);
  size_t              (*user_buf_size)   (type_t *self);


  unsigned char         user_static[USER_STATIC_SIZE];
  size_t                user_static_num;
  size_t                user_static_size;

  void                 *user_buf;
  size_t                user_buf_size;

  type_heap_info_t      heap_info;
};

/* TODO */
#ifdef TODO
extern type_t type_type;

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

extern type_t memory_manager_type;
typedef struct memory_manager_s memory_manager_t;
struct memory_manager_s
{
  type_t *type;

  void *(*malloc) (size_t  size);
  void  (*free)   (void   *ptr);
  void *(*calloc) (size_t  nmemb, size_t size);
  void *(*realloc)(void   *ptr,   size_t size);
};

extern const memory_manager_t malloc_manager;

/* Both the reference and individual function pointers may be NULL. */
/*                                                                  */
/* Behaviour in case of NULL "calloc" and "realloc" pointers will   */
/* based on the behaviour of the "malloc" and "free" fields.        */

void *memory_manager_malloc (const memory_manager_t *memory_manager, size_t  size);
void *memory_manager_free   (const memory_manager_t *memory_manager, void   *ptr);
void *memory_manager_calloc (const memory_manager_t *memory_manager, size_t  nmemb, size_t size);
void *memory_manager_realloc(const memory_manager_t *memory_manager, void   *ptr,   size_t size);


extern type_t memory_tracker_type;
typedef struct memory_tracker_s memory_tracker_t;
struct memory_tracker_s
{
  type_t *type;

  memory_manager_t *memory_manager;

  int      is_heap_allocated;
  void   **dynamically_allocated_buffers;
  size_t   dynamically_allocated_buffers_num;
  size_t   dynamically_allocated_buffers_size;
};

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

extern type_t template_cons_type;
typedef struct template_cons_s template_cons_t;
struct template_cons_s
{
  type_t *type;

  /*
   * The value to initialize.
   *
   * Set to NULL to tell the initializer to "malloc" a new one.
   */
  tval *val;

  /*
   * Set NULL to tell the initializer to select
   * reasonable defaults, usually "malloc_manager".
   *
   * Set individual components of the "memory_manager_t" value to NULL to tell
   * the initializer to select a reasonable default for that field.
   *
   * If "calloc" and "realloc" are NULL, the initializer should default to
   * behaviour that depends on its behaviour for "malloc" and "free".
   *
   * This means that only "malloc" and "free" can be provided with a guarantee
   * that conforming initializers will not use external memory management
   * algorithms for "calloc" and "realloc", instead using the procedures
   * provided.
   */
  const memory_manager_t *memory_manager;

  /*
   * The initial values will be chosen from this value, which must be of the
   * type as the value being constructed.
   *
   * Set to "NULL" to force all default values.
   */
  const tval *initials;

  /*
   * How to determine which fields from "initials" are relevant, and for which
   * fields a default value should be chosen.
   *
   * By default, all values equal to zero
   */
  unsigned long long default_value;

  /*
   * Optional: can be NULL.
   *
   * This allows initializers to write informative error messages in case
   * initialization fails.
   */
  char   *out_init_error_msg;
  size_t  init_error_msg_size;
};


   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one.             *-/
   * >   intpair_t *val;
   * >
   * >   /-* Set to NULL to default to "malloc" and "free". *-/
   * >   intpair_t (*malloc)(size_t     size);
   * >   void      (*free)  (intpair_t *val);
   * >
   * >   intpair_t *initials;
   * >   intpair_t *force_non_defaults;
   * > };






























































typedef struct type_init_params_s type_init_params_t;
struct type_init_params_t
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

extern type_t type_init_params_type;

/* TODO */
#endif /* TODO */

/* ---------------------------------------------------------------- */

#endif /* ifndef TYPE_H */
