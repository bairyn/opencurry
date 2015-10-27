/*
 * opencurry: type_base.h
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
 * type_base.h
 * -----------
 */

#ifndef TYPE_BASE_H
#define TYPE_BASE_H
/* stddef.h:
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

/* ---------------------------------------------------------------- */
/* Out-of-order forward declarations.                               */
/* ---------------------------------------------------------------- */

/* Declarations for types used before their definitions. */

typedef struct type_s type_t;

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/* TODO: update documentation to reflect change from "type_t *" to "typed" for
 * procedurally generated types! */
const type_t *typed_type(void);
typedef const type_t *(*typed_t)(void);

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
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

const type_t *memory_manager_type(void);
extern const type_t memory_manager_type_def;
typedef struct memory_manager_s memory_manager_t;
struct memory_manager_s
{
  typed_t type;

  void *(*malloc) (const memory_manager_t *self, size_t  size);
  void  (*free)   (const memory_manager_t *self, void   *ptr);
  void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size);
  void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size);

  void   *state;
  size_t  state_size;
};

/* A "memory_manager_t" with NULLS. */
extern const memory_manager_t default_manager;

extern const memory_manager_t malloc_manager;

/* Both the reference and individual function pointers may be NULL. */
/*                                                                  */
/* Behaviour in case of NULL "calloc" and "realloc" pointers will   */
/* be based on the behaviour of the "malloc" and "free" fields.     */
/*                                                                  */
/* If only one of "malloc" and "free" is NULL, behaviour is         */
/* undefined.                                                       */
void *memory_manager_malloc (const memory_manager_t *memory_manager, size_t  size);
void  memory_manager_free   (const memory_manager_t *memory_manager, void   *ptr);
void *memory_manager_calloc (const memory_manager_t *memory_manager, size_t  nmemb, size_t size);
void *memory_manager_realloc(const memory_manager_t *memory_manager, void   *ptr,   size_t size);


const type_t *memory_tracker_type(void);
extern const type_t memory_tracker_type_def;
typedef struct memory_tracker_s memory_tracker_t;
struct memory_tracker_s
{
  typed_t type;

  memory_manager_t memory_manager;


  /* Was the associated object allocated on the heap? */
  int      is_heap_allocated;

  /* Array of "void *".  NULL elements are unused slots. */
  /*                                                     */
  /* This itself may be NULL.                            */
  /*                                                     */
  /* The first element must be freed last:               */
  /*   This allows the first element to refer to this    */
  /*   buffer itself.                                    */
  void   **dynamically_allocated_buffers;
  size_t   dynamically_allocated_buffers_num;
  size_t   dynamically_allocated_buffers_size;
};

/* A default memory tracker appropriate for top-level declarations. */
/* Uses "default_manager" with a NULL buffer-array pointer.         */
/* 0 dynamically allocated buffer slots.                            */
extern const memory_tracker_t memory_tracker_default;

/* ---------------------------------------------------------------- */
/* struct_info_t and field_info_t                                   */
/* ---------------------------------------------------------------- */

/* TODO */

const type_t *field_info_type(void);
extern const type_t field_info_type_def;
typedef struct field_info_s field_info_t;
struct field_info_s
{
  typed_t type;

  size_t        field_pos;
  size_t        field_size;
  const type_t *field_type;

  /* Optional information. */

  /* Should "dup" recursively copy this field? */
  int           is_recurse;

  /* Type initializers can use this when initializing  */
  /* a struct from a template.                         */
  /*                                                   */
  /* Can be NULL.                                      */
  /*                                                   */
  /* In the most common case, this function will check */
  /* check whether the field in the source is 0 or     */
  /* NULL, and if so, write a default value.           */
  int (*initializer_check_default)(field_info_t *self, void *dest_mem, const void *src_mem);
};

extern const field_info_t default_field_info;

/* TODO */

#define STRUCT_INFO_NUM_FIELDS 1024
const type_t *struct_info_type(void);
extern const type_t struct_info_type_def;
typedef struct struct_info_s struct_info_t;
struct struct_info_s
{
  typed_t type;

  field_info_t fields[STRUCT_INFO_NUM_FIELDS];
  size_t       fields_len;

  struct_info_t *tail;


  /* Optional information, for this chunk (excludes tail). */

  int       has_memory_tracker;

  /* Index into fields_pos. */
  size_t    memory_tracker_field;
};

extern const struct_info_t default_struct_info;

/*
 * struct_dup:
 *
 * Assign each field in "dest" the value of the field in "src" according to
 * "struct_info".
 *
 * Behaviour for default values:
 *   If "struct_info" describes how to check whether a field in "src"
 *   indicates a default value should be chosen, and if so, what the default
 *   value is, it will be used.
 *
 *   Normally "struct_info" does this by supplying a default value for "src"
 *   field values equal to "0" or NULL.
 *
 * rec:
 *   Whether to recursively duplicate subfields marked in "struct_info" for
 *   recursive traversal.
 *
 *   Set to a negative number to limit traversal depth.
 */
void struct_dup(const struct_info_t *struct_info, tval *dest, const tval *src, int rec);

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/*
 * type_t: Type representations.
 *
 * A "type_t" describes a kind for values, possibly with information about its
 * name, that about its size, procedures to initialize values and allocate
 * memory for values of this type, and optionally, if the type is a struct,
 * information about the struct's fields.
 */
const type_t *type_type(void);
extern const type_t type_type_def;
/* Forward declaration: typedef struct type_s type_t; */
struct type_s
{
  /* Make "type_t" itself a type by setting the first field as "type_t *".  */
  /* All initialized "type_t" values should have "type" set to "type_type". */
  typed_t type;

  /* ---------------------------------------------------------------- */

  memory_tracker_t memory;

  /* ---------------------------------------------------------------- */
  /* Basic type info.                                                 */
  /* ---------------------------------------------------------------- */

  /* Whether values of this type are "tval"s.                         */
  /*                                                                  */
  /* "self":                                                          */
  /*   Normally they are; in this case "id" should return "self".     */
  /*                                                                  */
  /* "NULL":                                                          */
  /*   Sometimes, however, e.g. in the case of "type_t" for C's       */
  /*   primitive data types, value pointers lack polymorphism with    */
  /*   "type_t *".  In this case, "id" should return NULL.            */
  const type_t        *(*typed)    (const type_t *self);

  /* Name of the type.  (Not necessarily unique.)                     */
  const char          *(*name)     (const type_t *self);

  /* General information about the type.                              */
  /*                                                                  */
  /* Both the result of "info" and "info" itself may be NULL.         */
  const char          *(*info)     (const type_t *self);

  /* Size of a value of this type.                                    */
  /*                                                                  */
  /* Types can be variable-width or constant-width:                   */
  /*                                                                  */
  /*   Variable-width types should return "0" when "val" is NULL.     */
  /*                                                                  */
  /*   Constant-width types should return the size of any value when  */
  /*   "val" is NULL.                                                 */
  size_t               (*size)     (const type_t *self, const tval *val);

  /* If this type describes a struct, return information about its    */
  /* fields; otherwise return NULL.                                   */
  const struct_info_t *(*is_struct)(const type_t *self);


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

  /* The type of the "cons" constructor parameter for "init".         */
  /*                                                                  */
  /* "init" must be called with a "cons" value of this type.          */
  typed_t              (*cons_type)(const type_t *self);

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
  /* Note: the type of "cons" is not necessarily the same as this     */
  /* type.                                                            */
  /*                                                                  */
  /* Each type defines its own constructor ("cons") type, which       */
  /* may be different.                                                */
  /*                                                                  */
  /* A common constructor type is "template_cons_t".                  */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Returns NULL on failure to initialize.                           */
  tval                *(*init)     (const type_t *self, tval *cons);

  /* Deinitialization a value, freeing resources allocated by "init". */
  /*                                                                  */
  /* Support for idempotence (multiple calls to "free") is            */
  /* recommended but not required.                                    */
  void                 (*free)     (const type_t *self, tval *val);

  /* ---------------------------------------------------------------- */
  /* Copying.                                                         */
  /* ---------------------------------------------------------------- */

  /* Pass non-zero argument to recursively make copies of subfields.  */
  /*                                                                  */
  /* Some implementations support NULL "dest" pointers, indicating    */
  /* a new one should be dynamically allocated, using management      */
  /* associated with "src" in case of differences.                    */
  /*                                                                  */
  /* Returns NULL on failure.                                         */
  tval                *(*dup)      (const type_t *self, tval *dest, const tval *src, int rec);

  /* ---------------------------------------------------------------- */

  const char *parity;
};

const struct_info_t *type_is_not_struct(const type_t *self);

const type_t *type_typed(const type_t *self);
const type_t *type_untyped(const type_t *self);

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

const type_t *template_cons_type(void);
extern const type_t template_cons_type_def;
typedef struct template_cons_s template_cons_t;
struct template_cons_s
{
  type_t *type;

  /*
   * The value to initialize.
   *
   * Set to NULL to tell the initializer to "malloc" a new one.
   */
  tval *dest;

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
   * Optional: can be NULL.
   *
   * This allows initializers to write informative error messages in case
   * initialization fails.
   */
  char   *out_init_error_msg;
  size_t  init_error_msg_size;
};

/* TODO. */
/* This can be used by initializers. */
/* This does not necessarily perform full initialization for a given type. */
/* The type's "free" procedure can be used for this. */
tval *template_cons_dup_struct(const template_cons_t *cons, const tval *default_initials, const struct_info_t *struct_info);

/* Free memory allocated by "template_cons_dup_struct" and memory referred to
 * by "memory_tracker".
 *
 * This does not necessarily perform full cleanup for a given type.
 * The type's "free" procedure can be used for this.
 *
 * "template_cons_" can be used to implement "free".
 */
void  template_cons_free_struct(tval *val, const struct_info_t *struct_info);

/* ---------------------------------------------------------------- */
/* Primitive C data types.                                          */
/* ---------------------------------------------------------------- */

/* http://en.cppreference.com/w/c/header */

/* General type. */
const type_t *void_type(void);

/* General pointers. */
const type_t *funp_type(void);
const type_t *objp_type(void);

/* Scalar types. */
const type_t *char_type(void);
const type_t *schar_type(void);
const type_t *uchar_type(void);

const type_t *short_type(void);
const type_t *ushort_type(void);

const type_t *int_type(void);
const type_t *uint_type(void);

const type_t *long_type(void);
const type_t *ulong_type(void);

const type_t *float_type(void);
const type_t *double_type(void);
const type_t *ldouble_type(void);

/* <math.h> */
const type_t *div_type(void);
const type_t *ldiv_type(void);

/* <setjmp.h> */
const type_t *jmpbuf_type(void);

/* <stdarg.h> */
const type_t *va_list_type(void);

/* <stddef.h> */
const type_t *size_type(void);
const type_t *ptrdiff_type(void);

/* <stdlib.h> */
const type_t *sig_atomic_type(void);

/* <stdio.h> */
const type_t *file_type(void);
const type_t *fpos_type(void);

/* <time.h> */
const type_t *tm_type(void);
const type_t *time_type(void);
const type_t *clock_type(void);

/* ---------------------------------------------------------------- */

#endif /* ifndef TYPE_BASE_H */
