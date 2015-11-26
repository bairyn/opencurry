/*
 * opencurry: type_base_type.h
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
 * type_base_type.h
 * ------
 */

#ifndef TYPE_BASE_TYPE_H
#define TYPE_BASE_TYPE_H
#include "base.h"

#include "type_base_prim.h"
#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_memory_manager.h"
#include "type_base_memory_tracker.h"
#ifndef TODO /* TODO */
#include "type_base.h"
#endif /* #ifndef TODO /-* TODO *-/ */

/* TODO: macros to abstract decls and defs, optionally static, for name, size,
 * info, etc.
 */

/* ---------------------------------------------------------------- */

/* Infinite loop cycles are handled. */
#define DEEP_RECURSE()                (1)
#define DEEP_NORECURSE()              (0)
#define DEEP_RECURSE_LIMIT(max_depth) (-(max_depth))

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

  type_t *self_is_mutable;

  /* ---------------------------------------------------------------- */

  typed_t indirect;

  /* ---------------------------------------------------------------- */
  /* Basic object info.                                               */
  /* ---------------------------------------------------------------- */

  /* This should return "self".                                       */
  const type_t        *(*self)       (const type_t *self);

  /* This should return a "typed_t" that returns the same type.       */
  typed_t              (*container)  (const type_t *self);

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
  const type_t        *(*typed)      (const type_t *self);

  /* Name of the type.  (Not necessarily unique.)                     */
  const char          *(*name)       (const type_t *self);

  /* TODO: update field! */
  /* General information about the type.                              */
  /*                                                                  */
  /* Both the result of "info" and "info" itself may be NULL.         */
  /*                                                                  */
  /* This procedure can depend upon "out_info", returning NULL when   */
  /* it isn't provided.                                               */
  const char          *(*info)       ( const type_t *self
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     );

  /* Size of a value of this type.                                    */
  /*                                                                  */
  /* Types can be variable-width or constant-width:                   */
  /*                                                                  */
  /*   Variable-width types should return "0" when "val" is NULL.     */
  /*                                                                  */
  /*   Constant-width types should return the size of any value when  */
  /*   "val" is NULL.                                                 */
#define TYPE_SIZE_UNKNOWN_VALUE        0
#define TYPE_SIZE_VARIABLE_WIDTH_VALUE 0
  size_t               (*size)       (const type_t *self, const tval *val);

  /* If this type describes a struct, return information about its    */
  /* fields; otherwise return NULL.                                   */
  const struct_info_t *(*is_struct)  (const type_t *self);

  /* Can a "const tval *val" be made mutable?                         */
  tval                *(*is_mutable) (const type_t *self, const tval *val);

  /* One-way subtyping: "is_subtype" should not call any "is_supertype" method
   * and visa versa.
   *
   * B is a subtype of A if at least one of "is_subtype" or "is_supertype"
   * returns non-NULL.
   *
   * "is_subtype" overrides "is_supertype" when both return non-NULL, but given
   * the current constraints, this is redundant.
   *
   * Both relational methods should return a most specific type (the subtype)
   * when the relation holds.
   */

  /* One-way non-extensible subtyping.          */
  /* Should return either NULL or "is_subtype". */
  /* Should not call "is_supertype".            */
  /*                                            */
  /* A subtype value reference can be cast to a */
  /* supertype value reference.                 */
  const type_t        *(*is_subtype) ( const type_t *self
                                     , const type_t *is_subtype
                                     );

  /* One-way extensible subtyping.              */
  /* Should return either NULL or "self".       */
  /* Should not call "is_subtype".              */
  const type_t        *(*is_supertype)
                                     ( const type_t *self
                                     , const type_t *is_supertype
                                     );


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
  typed_t              (*cons_type)  (const type_t *self);

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
  tval                *(*init)       (const type_t *self, tval *cons);

  /* Deinitialization a value, freeing resources allocated by "init". */
  /*                                                                  */
  /* Support for idempotence (multiple calls to "free") is            */
  /* recommended but not required.                                    */
  size_t               (*free)       (const type_t *self, tval *val);

  /* A type can be associated with a designated default value,        */
  /* returned by this method.                                         */
  /*                                                                  */
  /* A type that doesn't should return NULL.                          */
  const tval          *(*has_default)(const type_t *self);

  /* ---------------------------------------------------------------- */
  /* Dynamic memory allocation.                                       */
  /* ---------------------------------------------------------------- */

  /* How to track memory allocation for values of this type.          */
  /*                                                                  */
  /* "mem" should return a reference to the memory tracker associated */
  /* with "val_raw" if it exists, otherwise NULL.                     */
  /*                                                                  */
  /* "val" might not be initialized, in which case the memory tracker */
  /* returned probably isn't either.                                  */
  /*                                                                  */
  /* "mem" should properly handle uninitialized "val_raw" values, but */
  /* it isn't required to return one for a "val" that isn't           */
  /* initialized (perhaps, for instance, a type keeps track of        */
  /* which values of the type are initialized externally from any     */
  /* individual value, which allows the type to check whether         */
  /* a reference to any given value is one present in some container  */
  /* of initialized values of this type.                              */
  /*                                                                  */
  /* In the simplest case, the type has a "memory_tracker_t"          */
  /* reference that this method initializes and returns.              */
  /*                                                                  */
  /* Alternatively, some types might want to reference a memory       */
  /* tracker outside values.                                          */
  /*                                                                  */
  /* This can be NULL, or can always return NULL, if this type        */
  /* isn't associated with a memory tracker.                          */
  /*                                                                  */
  /* A type itself can be associated with its own unique memory       */
  /* tracker, which it returns it when "val" is NULL; but usually     */
  /* this is not done (and just NULL is returned in this case.)       */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* One of the primary uses of this is to relocate information about */
  /* whether value of a given type was dynamically allocated.         */
  /*                                                                  */
  /* "free" determines whether it should free a dynamically allocated */
  /* value.  In a simple case, the value itself contains a field      */
  /* indicating whether the value was dynamically allocated, e.g.     */
  /* with "malloc".                                                   */
  /*                                                                  */
  /* But "mem" allows such tracking to have data that resides outside */
  /* values.  In this case, "mem" returns the appropriate memory      */
  /* tracker.                                                         */
  /*                                                                  */
  /* A simple way to do this is to set "mem" to return a top-level    */
  /* memory-tracker associated to the type itself.                    */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* If this returns a memory tracker with a NULL "val_raw", then     */
  /* conventional initializers, including                             */
  /* "template_cons_basic_initializer", will not initialize a new     */
  /* memory tracker.                                                  */
  memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw);

  /* How to track dynamic memory allocations.                         */
  /*                                                                  */
  /* This is called by predefined initializers and initializer        */
  /* helpers when a value is dynamically allocated.                   */
  /*                                                                  */
  /* (The default "mem_init" method                                   */
  /* "type_supports_dynamic_allocation"                               */
  /* is in most cases appropriate: where if the type has its own      */
  /* "valueless" memory tracker, use it, else use "is_struct"'s       */
  /* memory tracker field.)                                           */
  /*                                                                  */
  /* (The default "mem" method type_mem_struct_or_global_dyn"         */
  /* method only assigns a valueless memory tracker when the type     */
  /* lacks a struct_info with a designated memory tracker field;      */
  /* in this case it is assigned "global_typed_dyn_memory_tracker".)  */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* This should return NULL when dynamical memory allocation         */
  /* is not supported, for all values in general when "val_raw" is    */
  /* NULL, and for particular values otherwise.                       */
  /*                                                                  */
  /* Otherwise its return value is type-specific.  In simple cases,   */
  /* this return value can simply be the associated memory tracker,   */
  /* but it can also be any other non-NULL reference.                 */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Thus mainly this is used by standard initializers for 2          */
  /* purposes:                                                        */
  /*   1) To track newly dynamically allocated values.                */
  /*   2) To determine whether such dynamic allocation is supported   */
  /*      for all values of this type in general,                     */
  /*      where "val_raw" is NULL.                                    */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* This can be NULL.                                                */
  void                *(*mem_init)   ( const type_t *self
                                     , tval         *val_raw
                                     , int           is_dynamically_allocated
                                     );

  /* TODO: update field_info! */
  /* Check whether a value is dynamically allocated.                  */
  /*                                                                  */
  /* Returns 1 if so, 0 otherwise, and <= -1 on error.                */
  int                  (*mem_is_dyn) ( const type_t *self
                                     , tval         *val
                                     );

  /* TODO: update field_info! */
  /* Check whether a value is dynamically allocated and, if so, free  */
  /* it, otherwise do nothing.                                        */
  /*                                                                  */
  /* Returns 1 if freed, 0 if not dynamically allocated,              */
  /* and <= -1 on error.                                              */
  int                  (*mem_free)   ( const type_t *self
                                     , tval         *val
                                     );

  /* TODO: update field_info! */
  /* Default memory manager for this type.                            */
  /*                                                                  */
  /* 1) Which memory manager default should be chosen for new values  */
  /*    when no memory manager is explicitly requested; "val_raw" is  */
  /*    null in this case                                             */
  /*                                                                  */
  /* 2) For a given initialized value, which memory manager should be */
  /*    used to access it by default, e.g. when "mem" doesn't         */
  /*    associate the value with a memory_tracker, in which case it   */
  /*    is used instead, by obtaining this memory tracker's memory    */
  /*    manager.                                                      */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* "val_raw" might be NULL, and it might be uninitialized.          */
  /*                                                                  */
  /* If NULL, "mem_manager" can return a default memory manager,      */
  /* or alternatively a memory manager used for all values of this    */
  /* type if this type forces all values to use a specific            */
  /* memory manager.                                                  */
  const memory_manager_t
                      *(*default_memory_manager)
                                     ( const type_t *self
                                     , tval *val
                                     );

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

  /* Copy one value into another.                                     */
  /*                                                                  */
  /* Some implementations support NULL "dest" pointers, indicating    */
  /* a new one should be dynamically allocated, using management      */
  /* associated with "src" in case of differences.                    */
  /*                                                                  */
  /* defaults_src_unused:                                             */
  /*   Choose a default value in "dest" for each field in "src" that  */
  /*   is 0 or NULL, unless the type specifies a value other than 0   */
  /*   or NULL as its "special unused value" in which case check      */
  /*   against that, or none at all, in which case don't supply a     */
  /*   default value.                                                 */
  /*                                                                  */
  /*   For each field in "src", if the type's field has a             */
  /*   special "unused value", check whether it is equal to the field */
  /*   value in "src", and, if so, choose a default one instead.      */
  /*                                                                  */
  /*   Usually, the special "unused value" is zero / NULL, so this    */
  /*   flag determines whether to choose a default value when a field */
  /*   is 0 or NULL.                                                  */
  /*                                                                  */
  /*   When this flag is disabled, just copy from "source", without   */
  /*   choosing default values.                                       */
  /*                                                                  */
  /* rec_copy:                                                        */
  /*   0 to skip recursively calling "dup" on subcomponents that are  */
  /*   references designated as copyable.                             */
  /*                                                                  */
  /*   For subcomponents that are references and designated as        */
  /*   copyable, "rec_copy" determines whether to recursively copy    */
  /*   these.                                                         */
  /*                                                                  */
  /*   If this type is a struct, then the type's struct_info returned */
  /*   by "is_struct" associates each field in the struct with a flag */
  /*   that indicates whether it is a copyable reference.             */
  /*                                                                  */
  /*   Each field is also associated with a "type_t *" reference,     */
  /*   whose "dup" procedure is used to recursively copy the field.   */
  /*   (If the type lacks a "dup" procedure, behaviour will be as if  */
  /*   "rec_copy" were 0.                                             */
  /*                                                                  */
  /*   Set this to a negative value to limit traversal depth equal    */
  /*   to the inverse of the limit.                                   */
  /*                                                                  */
  /* dup_metadata:                                                    */
  /*   Whether to assign all subcomponents, and not skip those        */
  /*   designated as "metadata", data which is specific to each       */
  /*   value, such as fields like memory trackers that are usually    */
  /*   unique to each value.                                          */
  /*                                                                  */
  /*   It is usually an error to set this flag without a "dest"       */
  /*   argument, because of a lack of a direct appropriate memory     */
  /*   tracker.                                                       */
  /*                                                                  */
  /* ref_traversal:                                                   */
  /*   History of values duplicated, used to avoid loops during       */
  /*   recursive copying.                                             */
  /*                                                                  */
  /*   When "dup" is called at the root level, this can be NULL.      */
  /*                                                                  */
  /*   Users can also preinitialize a ref_traversal_t before calling. */
  /*   One possible useful purpose for this is to set a callback      */
  /*   for when a loop is detected.                                   */
  /*                                                                  */
  /*   (At non-root-level recursive calls, passing a "ref_traversal"  */
  /*   is optional but highly recommended.)                           */
  /*                                                                  */
  /* Returns NULL on failure.                                         */
  tval                *(*dup)        ( const type_t *self
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *vals
                                     );

  /* ---------------------------------------------------------------- */
  /* Value accessors.                                                 */
  /* ---------------------------------------------------------------- */

  /* A type can associate an arbitrary value to a value, and to       */
  /* the type itself when "val" is NULL.                              */
  /*                                                                  */
  /* This can be NULL.                                                */
  void                *(*user)       (const type_t *self, tval *val);
  const void          *(*cuser)      (const type_t *self, const tval *val);

  /* Returns 0 when the two values are equal, non-zero otherwise and  */
  /* on error.                                                        */
  int                  (*cmp)        ( const type_t *self
                                     , const tval *check
                                     , const tval *baseline
                                     , int deep
                                     , ref_traversal_t *vals
                                     );

  /* ---------------------------------------------------------------- */

  const char *parity;
};

/* ---------------------------------------------------------------- */

/*
 * Various "type_t" methods and method helpers.
 */

/* self */
const type_t *type_has_self(const type_t *self);

/* container */
typed_t type_has_indirect(const type_t *self);
typed_t type_has_container(const type_t *self, typed_t container);

/* typed */
const type_t *type_is_typed_from_struct(const type_t *self);

const type_t *type_is_typed(const type_t *self);
const type_t *type_is_untyped(const type_t *self);

/* name */
const char *type_has_name(const type_t *self, char *out_info_name, size_t name_buf_size, const char *name);
const char *type_has_static_name(const type_t *self, const char *name);

/* info */
const char *type_has_no_info(const type_t *self, char *out_info_buf, size_t info_buf_size);

/* size */
size_t type_has_unknown_size(const type_t *self, const tval *val);

/* is_struct */
#define STRUCT_INFO_CACHE(struct_info)          \
  static struct_info_t *struct_info = NULL;     \
  if (struct_info)                              \
  {                                             \
    return (const struct_info_t *) struct_info; \
  }                                             \
  else                                          \
  {                                             \
    static struct_info_t struct_info_def;       \
                                                \
    struct_info = &struct_info_def;             \
  }

#define STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value) \
  do                                                                                            \
  {                                                                                             \
    /* Initialize struct_info. */                                                               \
    struct_info = struct_info_init                                                              \
      ( struct_info                                                                             \
      , field_default_value                                                                     \
      , field_template_unused_value                                                             \
      );                                                                                        \
  } while(0)

#define STRUCT_INFO_DONE_COMPLEX(struct_info)                                   \
  do                                                                            \
  {                                                                             \
    /* Done adding fields. */                                                   \
    struct_info = struct_info_add_field_terminator(struct_info);                \
                                                                                \
    if (verify_struct_info(struct_info, NULL, 0) != verify_struct_info_success) \
      struct_info = NULL;                                                       \
                                                                                \
    return ((const struct_info_t *) struct_info);                               \
  } while(0)

/* -- */

#define STRUCT_INFO_BEGIN_COMPLEX(struct_type, struct_info, field_default_value, field_template_unused_value) \
  /* typedef mystruct_t struct_type; */                                                                       \
  typedef struct_type struct_##type;                                                                          \
                                                                                                              \
  /* struct_info_t *struct_info; */                                                                           \
  STRUCT_INFO_CACHE(struct_info)                                                                              \
                                                                                                              \
  do                                                                                                          \
  {                                                                                                           \
    /* Initialize struct_info. */                                                                             \
    STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value);                  \
  } while(0)

#define STRUCT_INFO_ADD_COMPLEX(struct_info, struct_type, field_name, field_type_rep) \
  do                                                                                  \
  {                                                                                   \
    struct_info = struct_info_add_field                                               \
      ( struct_info                                                                   \
      , (size_t) (OFFSET_FIELD(struct_type, field_name))                              \
      , (size_t) (SIZEOF_FIELD(struct_type, field_name))                              \
      , field_type_rep                                                                \
      );                                                                              \
  } while(0)

#define STRUCT_INFO_LAST_COMPLEX(struct_info) \
  struct_info_get_last_field_elem_ref(struct_info)

#define STRUCT_INFO_REINIT_WITH_COMPLEX(field_default_value)                                           \
  STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value_zero)

#define STRUCT_INFO_REINIT_WITH_COMPLEX_BOTH(field_default_value, field_template_unused_value)         \
  STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value)

/* -- */

/*
 * An "is_struct" method can use these 4 macros to describe a struct's fields.
 *
 * Start with BEGIN, ADD each field, and then call DONE.
 *
 * To supply a custom field_default_value or field_template_unused_value, call
 * "REINIT_WITH" after BEGIN.
 *
 * Example:
 */

#define STRUCT_INFO_BEGIN(type_name) \
  STRUCT_INFO_BEGIN_COMPLEX(CAT(type_name, _t), struct_info, CAT(type_name, _default_field_value), field_template_unused_value_zero)

#define STRUCT_INFO_ADD(field_name, field_type_rep)                             \
  STRUCT_INFO_ADD_COMPLEX(struct_info, struct_type, field_name, field_type_rep)

#define STRUCT_INFO_RADD(field_type_rep, field_name) \
  STRUCT_INFO_ADD(field_name, field_type_rep)

#define STRUCT_INFO_LAST() \
  STRUCT_INFO_LAST_COMPLEX(struct_info)

#define STRUCT_INFO_DONE() \
  STRUCT_INFO_DONE_COMPLEX(struct_info)

/* -- */

#define DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(type_name)                                                 \
  static size_t CAT(type_name, _default_field_value)(const field_info_t *self, void *dest_field_mem) \
  {                                                                                                  \
    return field_default_value_from_type(self, dest_field_mem, CAT(type_name, _type)());             \
  }

/* -- */

const struct_info_t *type_is_not_struct(const type_t *self);

/* TODO: from_type: use the type's has_default, otherwise from_field_type. */
size_t field_default_value_from_type      (const field_info_t *self, void *dest_field_mem, const type_t *type);
size_t field_default_value_from_field_type(const field_info_t *self, void *dest_field_mem);
size_t field_default_value_zero           (const field_info_t *self, void *dest_field_mem);

size_t field_template_unused_value_zero(const field_info_t *self, void *dest_field_mem);

/* is_mutable */
tval *type_mutable_from_struct(const type_t *self, const tval *val);

tval *type_mutable_field(const type_t *self, const tval *val, const field_info_t *self_reference);

tval *type_immutable    (const type_t *self, const tval *val);

/* is_subtype */
const type_t *type_has_no_nonextensible_subtypes(const type_t *self, const type_t *is_subtype);

/* is_supertype */
const type_t *type_has_no_extensible_supertypes(const type_t *self, const type_t *is_supertype);

/* cons_type" */
typed_t type_has_template_cons_type(const type_t *self);

/* init */
tval *type_has_template_cons_basic_initializer                     (const type_t *type, tval *cons);
tval *type_has_template_cons_basic_initializer_force_memory_manager(const type_t *type, tval *cons);

tval *template_cons_basic_initializer(const type_t *type, template_cons_t *cons, int allow_alternate_memory_manager);

/* free */
size_t type_has_template_cons_basic_freer(const type_t *type, tval *val);

size_t template_cons_basic_freer(const type_t *type, tval *cons);

/* has_default */
const tval   *type_has_no_default_value(const type_t *self);
const tval   *type_has_default_value   (const type_t *self, const tval *val);

/* mem */
memory_tracker_t *type_mem_struct_or_global_dyn(const type_t *self, tval *val_raw);
memory_tracker_t *type_mem_valueless(const type_t *self, tval *val_raw, memory_tracker_t *valueless_memory_tracker);

/* mem_init */

/* Standard optional-struct_info-based "mem_init" methods. */
void *type_supports_dynamic_allocation
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );
void *type_supports_dynamic_allocation_only_some_vals
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );

void *type_mem_init_valueless_or_inside_value
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );

/* Procedure for standard "mem_init" initialization. */
/* TODO: rename */
void *mem_init_type_valueless_or_inside_value
  ( const type_t           *type
  , tval                   *val_raw
  , int                     is_dynamically_allocated
  , int                     are_all_vals_supported
  , const memory_manager_t *val_memory_manager

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* mem_is_dyn */
int type_is_dyn_valueless_or_inside_value
  ( const type_t *self
  , tval         *val
  );

/* Standard optional-struct_info-based "mem_is_dyn" methods. */
int mem_is_dyn_valueless_or_inside_value
  ( const type_t *type
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* mem_free */
int type_mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval         *val
  );

/* Standard optional-struct_info-based "mem_free" methods. */
int mem_free_valueless_or_inside_value_allocation
  ( const type_t *self
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* default_memory_manager */

const memory_manager_t *type_has_no_default_memory_manager(const type_t *self, tval *val);

const memory_manager_t *type_prefers_given_memory_manager(const type_t *self, tval *val, const memory_manager_t *memory_manager);
const memory_manager_t *type_prefers_default_memory_manager(const type_t *self, tval *val);
const memory_manager_t *type_prefers_malloc_memory_manager(const type_t *self, tval *val);

/* If the type has no global memory tracker, just prefer default_memory_manager, after
 * usual "struct_info" checking, making it equivalent to
 * "type_prefers_default_memory_manager" in this case.
 */
const memory_manager_t *type_prefers_global_memory_manager(const type_t *type, tval *val_raw);

/* dup */
tval *type_has_struct_dup_allow_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      );
tval *type_has_struct_dup_never_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      );

/* user */
void *type_has_no_user_data(const type_t *self, tval *val);

/* cuser */
const void *type_has_no_cuser_data(const type_t *self, const tval *val);

/* cmp */
int type_has_standard_cmp(const type_t *self, const tval *check, const tval *baseline, int deep, ref_traversal_t *vals);

/* ---------------------------------------------------------------- */

/*
 * "type_t" defaults.
 */

const type_t        *default_type_self       (const type_t *self);
typed_t              default_type_container  (const type_t *self);
const type_t        *default_type_typed      (const type_t *self);
/* const char          *default_type_name       (const type_t *self); */
const char          *default_type_info       ( const type_t *type
                                             , char         *out_info_buf
                                             , size_t        info_buf_size
                                             );
/* size_t               default_type_size       (const type_t *self, const tval *val); */
/* const struct_info_t *default_type_is_struct  (const type_t *self); */
tval                *default_type_is_mutable (const type_t *self, const tval *val);
const type_t        *default_type_is_subtype ( const type_t *self
                                             , const type_t *is_subtype
                                             );
const type_t        *default_type_is_supertype
                                             ( const type_t *self
                                             , const type_t *is_supertype
                                             );
typed_t              default_type_cons_type  (const type_t *self);
tval                *default_type_init       (const type_t *self, tval *cons);
size_t               default_type_free       (const type_t *self, tval *val);
const tval          *default_type_has_default(const type_t *self);
memory_tracker_t    *default_type_mem        (const type_t *self, tval *val_raw);
void                *default_type_mem_init   ( const type_t *self
                                             , tval *val_raw
                                             , int is_dynamically_allocated
                                             );
int                  default_type_mem_is_dyn ( const type_t *self
                                             , tval         *val
                                             );
int                  default_type_mem_free   ( const type_t *self
                                             , tval         *val
                                             );
const memory_manager_t
             *default_type_default_memory_manager
                                             ( const type_t *self
                                             , tval *val
                                             );
tval                *default_type_dup        ( const type_t *self
                                             , tval *dest
                                             , const tval *src
                                             , int defaults_src_unused
                                             , int rec_copy
                                             , int dup_metadata
                                             , ref_traversal_t *ref_traversal
                                             );
void                *default_type_user       (const type_t *self, tval *val);
const void          *default_type_cuser      (const type_t *self, const tval *val);
int                  default_type_cmp        ( const type_t *self
                                             , const tval *check
                                             , const tval *baseline
                                             , int deep
                                             , ref_traversal_t *vals
                                             );

#define TYPE_DEFAULTS                                                \
  { type_type                                                        \
                                                                     \
    /* @: Required.           */                                     \
                                                                     \
  , /* memory                 */ MEMORY_TRACKER_DEFAULTS             \
  , /* is_self_mutable        */ NULL                                \
  , /* @indirect              */ NULL                                \
                                                                     \
  , /* self                   */ default_type_self                   \
  , /* container              */ default_type_container              \
                                                                     \
  , /* typed                  */ default_type_typed                  \
                                                                     \
  , /* @name                  */ NULL /* default_type_name */        \
  , /* info                   */ default_type_info                   \
  , /* @size                  */ NULL /* default_type_size */        \
  , /* @is_struct             */ NULL /* default_type_is_struct */   \
  , /* is_mutable             */ default_type_is_mutable             \
  , /* is_subtype             */ default_type_is_subtype             \
  , /* is_supertype           */ default_type_is_supertype           \
                                                                     \
  , /* cons_type              */ default_type_cons_type              \
  , /* init                   */ default_type_init                   \
  , /* free                   */ default_type_free                   \
  , /* has_default            */ default_type_has_default            \
  , /* mem                    */ default_type_mem                    \
  , /* mem_init               */ default_type_mem_init               \
  , /* mem_is_dyn             */ default_type_mem_is_dyn             \
  , /* mem_free               */ default_type_mem_free               \
  , /* default_memory_manager */ default_type_default_memory_manager \
                                                                     \
  , /* dup                    */ default_type_dup                    \
                                                                     \
  , /* user                   */ default_type_user                   \
  , /* cuser                  */ default_type_cuser                  \
  , /* cmp                    */ default_type_cmp                    \
                                                                     \
  , /* parity                 */ ""                                  \
  }
extern const type_t type_defaults;

/* ---------------------------------------------------------------- */

/*
 * Fundamental "type_t" accessors.
 */

const memory_tracker_t *type_get_memory         (const type_t *type);
const type_t           *type_get_self_is_mutable(const type_t *type);
typed_t                 type_get_indirect       (const type_t *type);

const type_t        *type_self       (const type_t *type);
typed_t              type_container  (const type_t *type);
const type_t        *type_typed      (const type_t *type);
const char          *type_name       (const type_t *type);
const char          *type_info       ( const type_t *type
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     );
size_t               type_size       (const type_t *type, const tval *val);
const struct_info_t *type_is_struct  (const type_t *type);
tval                *type_is_mutable (const type_t *type, const tval *val);
const type_t        *type_is_subtype ( const type_t *type
                                     , const type_t *is_subtype
                                     );
const type_t        *type_is_supertype
                                     ( const type_t *type
                                     , const type_t *is_supertype
                                     );
typed_t              type_cons_type  (const type_t *type);
tval                *type_init       (const type_t *type, tval *cons);
size_t               type_free       (const type_t *type, tval *val);
const tval          *type_has_default(const type_t *type);
memory_tracker_t    *type_mem        (const type_t *type, tval *val_raw);
void                *type_mem_init   ( const type_t *type
                                     , tval *val_raw
                                     , int is_dynamically_allocated
                                     );
int                  type_mem_is_dyn ( const type_t *type
                                     , tval         *val
                                     );
int                  type_mem_free   ( const type_t *type
                                     , tval         *val
                                     );
const memory_manager_t
                    *type_default_memory_manager
                                     ( const type_t *type
                                     , tval *val
                                     );
tval                *type_dup        ( const type_t *type
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *ref_traversal
                                     );
void                *type_user       (const type_t *type, tval *val);
const void          *type_cuser      (const type_t *type, const tval *val);
int                  type_cmp        ( const type_t *type
                                     , const tval *check
                                     , const tval *baseline
                                     , int deep
                                     , ref_traversal_t *vals
                                     );

/* ---------------------------------------------------------------- */

/*
 * Compositional "type_t" accessors.
 */

const type_t *is_subtype(const type_t *sub, const type_t *super);
const type_t *is_type_equivalent(const type_t *this, const type_t *that);
const type_t *is_subtype_via(const type_t *sub, const type_t *mid, const type_t *super);
const type_t *is_proper_subtype(const type_t *sub, const type_t *super);
const type_t *is_supertype(const type_t *super, const type_t *sub);
const type_t *is_supertype_via(const type_t *super, const type_t *mid, const type_t *sub);
const type_t *is_proper_supertype(const type_t *super, const type_t *sub);

#define CMP_WITH_TYPE_DEFAULT_DEEP (DEEP_RECURSE())

int cmp_with_type_deep(const type_t *type, const tval *check, const tval *baseline, int deep);
int cmp_with_type     (const type_t *type, const tval *check, const tval *baseline);

/* ---------------------------------------------------------------- */

/*
 * "type_t" accessors for typed "tval"s.
 *
 * All provided values must start with a "typed_t" value (so-called "tval"s)!
 */

/* TODO: more */

size_t tval_free (tval *val);

#endif /* ifndef TYPE_BASE_TYPE_H */
