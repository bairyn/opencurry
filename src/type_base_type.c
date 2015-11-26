/*
 * opencurry: type_base_type.c
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

/* string.h:
 *   - memcmp
 */
#include <string.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_type.h"

#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_memory_manager.h"
#include "type_base_memory_tracker.h"
#ifndef TODO /* TODO */
#include "type_base.h"
#endif /* #ifndef TODO /-* TODO *-/ */

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/* type type. */

const type_t *type_type(void)
  { return &type_type_def; }

static const char          *type_type_name       (const type_t *self);
static size_t               type_type_size       (const type_t *self, const tval *val);
static const struct_info_t *type_type_is_struct  (const type_t *self);
static const tval          *type_type_has_default(const type_t *self);
static void                *type_type_user       (const type_t *self, tval *val);
static const void          *type_type_cuser      (const type_t *self, const tval *val);

const type_t type_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ type_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_typed

  , /* @name                  */ type_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_type_size
  , /* @is_struct             */ type_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ type_type_has_default
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ type_type_user
  , /* cuser                  */ type_type_cuser
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char          *type_type_name       (const type_t *self)
  { return "type_t"; }

static size_t               type_type_size       (const type_t *self, const tval *val)
  { return sizeof(type_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(type)
static const struct_info_t *type_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(type);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* memory_tracker_t memory; */
    STRUCT_INFO_RADD(memory_tracker_type(), memory);

    /* type_t *self_is_mutable; */
    STRUCT_INFO_RADD(objp_type(), self_is_mutable);

    /* typed_t indirect; */
    STRUCT_INFO_RADD(typed_type(), indirect);

    /* const type_t        *(*self)       (const type_t *self); */
    /* typed_t              (*container)  (const type_t *self); */
    STRUCT_INFO_RADD(funp_type(), self);
    STRUCT_INFO_RADD(funp_type(), container);

    /* const type_t        *(*typed)      (const type_t *self); */
    STRUCT_INFO_RADD(funp_type(), typed);

    /* const char          *(*name)       (const type_t *self);                           */
    /* const char          *(*info)       ( const type_t *self                            */
    /*                                    , char         *out_info_buf                    */
    /*                                    , size_t        info_buf_size                   */
    /*                                    );                                              */
    /* size_t               (*size)       (const type_t *self, const tval *val);          */
    /* const struct_info_t *(*is_struct)  (const type_t *self);                           */
    /* tval                *(*is_mutable) (const type_t *self, const tval *val);          */
    /* const type_t        *(*is_subtype) ( const type_t *self                            */
    /*                                    , const type_t *is_subtype                      */
    /*                                    );                                              */
    /* const type_t        *(*is_supertype)                                               */
    /*                                    ( const type_t *self                            */
    /*                                    , const type_t *is_supertype                    */
    /*                                    );                                              */
    STRUCT_INFO_RADD(funp_type(), name);
    STRUCT_INFO_RADD(funp_type(), info);
    STRUCT_INFO_RADD(funp_type(), size);
    STRUCT_INFO_RADD(funp_type(), is_struct);
    STRUCT_INFO_RADD(funp_type(), is_mutable);
    STRUCT_INFO_RADD(funp_type(), is_subtype);
    STRUCT_INFO_RADD(funp_type(), is_supertype);

    /* typed_t              (*cons_type)  (const type_t *self);                             */
    /* tval                *(*init)       (const type_t *self, tval *cons);                 */
    /* void                 (*free)       (const type_t *self, tval *val);                  */
    /* const tval          *(*has_default)(const type_t *self);                             */
    /* memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw);              */
    /* void                *(*mem_init)   ( const type_t *self                              */
    /*                                    , tval         *val_raw                           */
    /*                                    , int           is_dynamically_allocated          */
    /*                                    );                                                */
    /* int                  (*mem_is_dyn) ( const type_t *self                              */
    /*                                    , tval         *val                               */
    /*                                    );                                                */
    /* int                  (*mem_free)   ( const type_t *self                              */
    /*                                    , tval         *val                               */
    /*                                    );                                                */
    /* const memory_manager_t                                                               */
    /*                     *(*default_memory_manager)                                       */
    /*                                    ( const type_t *self                              */
    /*                                    , tval *val                                       */
    /*                                    );                                                */
    STRUCT_INFO_RADD(funp_type(), cons_type);
    STRUCT_INFO_RADD(funp_type(), init);
    STRUCT_INFO_RADD(funp_type(), free);
    STRUCT_INFO_RADD(funp_type(), has_default);
    STRUCT_INFO_RADD(funp_type(), mem);
    STRUCT_INFO_RADD(funp_type(), mem_init);
    STRUCT_INFO_RADD(funp_type(), mem_is_dyn);
    STRUCT_INFO_RADD(funp_type(), mem_free);
    STRUCT_INFO_RADD(funp_type(), default_memory_manager);

    /* tval                *(*dup)        ( const type_t *self             */
    /*                                    , tval *dest                     */
    /*                                    , const tval *src                */
    /*                                    , int defaults_src_unused        */
    /*                                    , int rec_copy                   */
    /*                                    , int dup_metadata               */
    /*                                    , ref_traversal_t *vals          */
    /*                                    );                               */
    STRUCT_INFO_RADD(funp_type(), dup);

    /* void                *(*user)       (const type_t *self, tval *val);       */
    /* const void          *(*cuser)      (const type_t *self, const tval *val); */
    /* int                  (*cmp)        ( const type_t *self                   */
    /*                                    , const tval *check                    */
    /*                                    , const tval *baseline                 */
    /*                                    , int deep                             */
    /*                                    , ref_traversal_t *vals                */
    /*                                    );                                     */
    STRUCT_INFO_RADD(funp_type(), user);
    STRUCT_INFO_RADD(funp_type(), cuser);
    STRUCT_INFO_RADD(funp_type(), cmp);

    /* const char *parity; */
    STRUCT_INFO_RADD(objp_type(), parity);

    STRUCT_INFO_DONE();
  }

static const tval          *type_type_has_default(const type_t *self)
  { return type_has_default_value(self, &type_defaults); }

static void                *type_type_user       (const type_t *self, tval *val)
{
  if (!self || !val)
    return NULL;

  return type_user(val, NULL);
}

static const void          *type_type_cuser      (const type_t *self, const tval *val)
{
  if (!self || !val)
    return NULL;

  return type_cuser(val, NULL);
}

/* ---------------------------------------------------------------- */
/* type_t: Various methods and method helpers.                      */
/* ---------------------------------------------------------------- */

/* self */
const type_t *type_has_self(const type_t *self)
{
  return self;
}

/* container */
typed_t type_has_indirect(const type_t *self)
{
  return self->indirect;
}

typed_t type_has_container(const type_t *self, typed_t container)
{
  return container;
}

/* typed */

/*
 * type_is_typed_from_struct:
 *
 * Values of this type are "typed" if and only if the type is a struct -
 * "is_struct" returns non-NULL - and its first field has type "typed".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > /-* "mytype" in this case is "typed", because it assigns *-/
 * > /-* "type_is_typed_from_struct" and "mytype" is a struct *-/
 * > /-* whose first field has type "typed" ("typed_t type"). *-/
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_typed_from_struct
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_typed_from_struct(const type_t *self)
{
  const struct_info_t *struct_info;

  struct_info = type_is_struct(self);

  if      (!struct_info)
  {
    return type_is_untyped(self);
  }
  else if (!struct_info_has_typed_field(struct_info))
  {
    return type_is_untyped(self);
  }
  else
  {
    return type_is_typed(self);
  }
}

/*
 * Values of this type are "typed", meaning references can be treated as
 * "tval *", indicating that they can be treated as "type_t *" to obtain the
 * type of the value.
 *
 * If the type is a struct, this means that the struct's first field is "typed
 * <name_of_field (conventionally "type">".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_typed
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_typed(const type_t *self)
{
  return self;
}

/*
 * Values of this type are not "typed", meaning references cannot necessarily
 * be "tval *".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_untyped
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_untyped(const type_t *self)
{
  return NULL;
}

/* name */
const char *type_has_name(const type_t *self, char *out_info_name, size_t name_buf_size, const char *name)
{
  return name;
}

const char *type_has_static_name(const type_t *self, const char *name)
{
  return name;
}

/* info */
const char *type_has_no_info(const type_t *self, char *out_info_buf, size_t info_buf_size)
{
  return NULL;
}

/* size */
size_t type_has_unknown_size(const type_t *self, const tval *val)
{
  if (!val)
    return ((size_t) (TYPE_SIZE_VARIABLE_WIDTH_VALUE));
  else
    return ((size_t) (TYPE_SIZE_UNKNOWN_VALUE));
}

/* is_struct */
const struct_info_t *type_is_not_struct(const type_t *self)
{
  return NULL;
}

#ifndef TODO
size_t field_default_value_from_type      (const field_info_t *self, void *dest_field_mem, const type_t *type)
{ /* TODO */ return 0; }
size_t field_default_value_from_field_type(const field_info_t *self, void *dest_field_mem)
{ /* TODO */ return 0; }
size_t field_default_value_zero           (const field_info_t *self, void *dest_field_mem)
{ /* TODO */ return 0; }
size_t field_template_unused_value_zero(const field_info_t *self, void *dest_field_mem)
{ /* TODO */ return 0; }
#endif /* #ifndef TODO */

/* is_mutable */

/* If the type has a struct, check the following fields for an objp whose
 * value is equal to "val", returning it if so, otherwise NULL:
 *
 * - The field immediately following any designated "memory_tracker" field.
 * - The field immediately following any designated "is_typed" field.
 * - The first field.
 */
tval *type_mutable_from_struct(const type_t *self, const tval *val)
{
  tval *mutable;

  const struct_info_t *struct_info;
  const field_info_t  *field_info;

  /* See whether we have a struct_info. */

  if (!self || val)
    return NULL;

  struct_info = type_is_struct(self);
  if (!struct_info)
    return NULL;

  /* Check each field. */

  /* + 1: The field terminator requirement makes this safe. */
  if ((field_info = struct_info_has_memory_tracker(struct_info)))
    if ((mutable = type_mutable_field(self, val, field_info + 1)))
      return mutable;

  /* + 1: The field terminator requirement makes this safe. */
  if ((field_info = struct_info_has_typed_field(struct_info)))
    if ((mutable = type_mutable_field(self, val, field_info + 1)))
      return mutable;

  if (struct_info_num_fields(struct_info) >= 1)
    if ((field_info = struct_info_index_field(struct_info, 0)))
      if ((mutable = type_mutable_field(self, val, field_info)))
        return mutable;

  return NULL;
}

tval *type_mutable_field(const type_t *self, const tval *val, const field_info_t *self_reference)
{
  tval *mutable;

  if (!self || !val || !self_reference)
    return NULL;

  if (!is_subtype(objp_type(), self_reference->field_type))
    return NULL;

  mutable = (void *) field_info_cref(self_reference, val);

  if ((const tval *) mutable != val)
    return NULL;

  return mutable;
}

tval *type_immutable    (const type_t *self, const tval *val)
{
  return NULL;
}

/* is_subtype */
const type_t *type_has_no_nonextensible_subtypes(const type_t *self, const type_t *is_subtype)
{
  if (!self || !is_subtype)
    return NULL;

  if (self == is_subtype)
    return is_subtype;

  return NULL;
}

/* is_supertype */
const type_t *type_has_no_extensible_supertypes(const type_t *self, const type_t *is_supertype)
{
  if (!self || !is_supertype)
    return NULL;

  if (self == is_supertype)
    return self;

  return NULL;
}

/* cons_type */

/*
 * The type's initializer uses "template_cons_t" references.
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...       *-/ ...
 * >     , /-* cons_type *-/ type_has_template_cons_type
 * >     , /-* ...       *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
typed_t type_has_template_cons_type(const type_t *self)
{
  return template_cons_type;
}

/* init */

/*
 * type_has_template_cons_basic_initializer:
 *
 * The type uses conventional "template_cons_t"-based initialization.
 *
 * This function invokes "template_cons_basic_initializer" and nothing else.
 *
 * "template_cons_basic_initializer" uses data, that we obtain here from the
 * type, and the data provided in "cons" to handle dynamic memory allocation
 * when requested and initialization of an existing value from a source value
 * by copying struct fields, if the type is a struct.
 */
tval *type_has_template_cons_basic_initializer(const type_t *type, tval *cons)
{
  static const int allow_alternate_memory_manager =
    1;
  return template_cons_basic_initializer(type, (template_cons_t *) cons, allow_alternate_memory_manager);
}

/*
 * type_has_template_cons_basic_initializer_force_memory_manager:
 *
 * Like "type_has_template_cons_basic_initializer", but prevent a memory
 * manager different from the type's "default_memory_manager" from being used.
 *
 * Normally, in other cases, a memory manager can be provided in a
 * "template_cons_t".
 */
tval *type_has_template_cons_basic_initializer_force_memory_manager(const type_t *type, tval *cons)
{
  static const int allow_alternate_memory_manager =
    0;
  return template_cons_basic_initializer(type, (template_cons_t *) cons, allow_alternate_memory_manager);
}

/*
 * template_cons_basic_initializer:
 *
 * Perform standard, minimum initialization from a "template_cons_t"
 * constructor.
 *
 * A type may want to perform more additionalization once this is finished.
 *
 * This invokes "template_cons_dup_struct" with initialization data obtained
 * from the type.
 *
 * Note: this is not an alternative to a type's initializer, even if it it uses
 * "template_cons_t", because the type may need to perform additional
 * initialization.  Erroneously using this to initialize a value *externally*
 * may result in a corrupt, possibly only partially initialized value.
 */
tval *template_cons_basic_initializer(const type_t *type, template_cons_t *cons, int allow_alternate_memory_manager)
{
  size_t                     size;
  const tval                *default_initials;
  const struct_info_t       *struct_info;
  void                    *(*mem_init)( const tval *self
                                      , tval       *val_raw
                                      , int         is_dynamically_allocated
                                      );
  const tval                *mem_init_object;
  const memory_manager_t    *def_memory_manager;

  if (!type)
  {
    if (cons && cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_basic_initializer: \"type\" argument is NULL!\n"
          "  Failed to initialize a value without a provided type.\n"
        );

    return NULL;
  }

  size               = type_size(type, NULL);
  default_initials   = type_has_default(type);
  struct_info        = type_is_struct(type);
  mem_init           = template_cons_dup_struct_meminit_type;
  mem_init_object    = (const tval *) type;
  def_memory_manager = type_default_memory_manager(type, NULL);

  return template_cons_dup_struct(cons, size, default_initials, struct_info, mem_init, mem_init_object, def_memory_manager, allow_alternate_memory_manager);
}

/* free */

/*
 * type_has_template_cons_basic_freer:
 *
 * The type uses conventional "template_cons_t"-based initialization,
 * optionally with additional initialization that does not allocate additional
 * resources that require being freed.
 *
 * This function invokes "template_cons_basic_freer" and nothing else.
 */
int type_has_template_cons_basic_freer(const type_t *type, tval *cons)
{
  return template_cons_basic_freer(type, cons);
}

/*
 * template_cons_basic_freer:
 *
 * Free resources allocated by "template_cons_basic_initializer".
 *
 * This can be used by types that use conventional "template_cons_t"-based
 * initialization with "template_cons_basic_initializer".
 *
 * If a type's initializer can allocate additional resources beyond those
 * allocated by "template_cons_basic_initializer", the type's "free" method
 * should also free those that are allocated before returning.
 *
 * This function invokes "template_cons_basic_free" and nothing further.
 *
 * Note: this is not an alternative to a type's freer, even if it it uses
 * "template_cons_t", because the type may have performed additional
 * initialization.  Erroneously using this to free a value *externally*
 * may result in memory leaks or corruption.
 */
int template_cons_basic_freer(const type_t *type, tval *val)
{
  int                      (*mem_free)( const tval *self
                                      , tval       *val
                                      );
  const tval                *mem_free_object;

  if (!type)
  {
    return -6;
  }
  mem_free           = template_cons_free_struct_memfree_type;
  mem_free_object    = (const tval *) type;

  return template_cons_free_struct(val, mem_free, mem_free_object);
}

/* has_default */

/*
 * type_has_no_default_value:
 *
 * This can be used when the type has no default value.
 *
 * Simply returns NULL.
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...         *-/ ...
 * >     , /-* has_default *-/ type_has_no_default_value
 * >     , /-* ...         *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const tval *type_has_no_default_value(const type_t *self)
{
  return NULL;
}

/*
 * type_has_default_value:
 *
 * This can be used when the type has a default value.
 *
 * Simply returns "val".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > static const tval *mytype_has_default(const type_t *self);
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...         *-/ ...
 * >     , /-* has_default *-/ mytype_has_default
 * >     , /-* ...         *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static const tval *mytype_has_default(const type_t *self)
 * >   { return type_has_default_value(self, mytype_defaults); }
 * >
 * > ...
 * >
 * > const mytype_t mytype_defaults =
 * >   { mytype_type
 * >
 * >   , /-* a *-/ 0
 * >   , /-* b *-/ 0
 * >   };
 */
const tval *type_has_default_value(const type_t *self, const tval *val)
{
  return val;
}

/* mem */

/*
 * type_mem_struct_or_global_dyn:
 *
 * Default memory tracker lookup method.
 *
 * Use the type's "struct_info" to obtain a value's "memory_tracker" field,
 * defaulting to the global memory tracker used valuelessly when absent.
 *
 * The type has no valueless memory unless it lacks a "struct_info" with a
 * designated "memory_tracker" field.
 *
 * This can be used with a default "mem_init" when dynamically allocated values
 * should be tracked inside the value itself.
 *
 * If the type isn't a struct, or if its "struct_info" doesn't reference a
 * memory tracker, then the type has no memory tracker, and we'll return NULL.
 *
 * Otherwise return a reference to the value's memory tracker field.
 */
memory_tracker_t *type_mem_struct_or_global_dyn(const type_t *self, tval *val_raw)
{
  const struct_info_t *struct_info;
  const field_info_t  *memory_tracker_field;

  /* Does the type have a struct_info with a designated memory tracker field?
   */

  struct_info = type_is_struct(self);
  if (!struct_info)
  {
    /* The type uses the global-dynamic-typed-allocations memory tracker valuelessly. */
    return &global_typed_dyn_memory_tracker;
  }

  memory_tracker_field = struct_info_has_memory_tracker(struct_info);
  if (!memory_tracker_field)
  {
    /* The type uses the global-dynamic-typed-allocations memory tracker valuelessly. */
    return &global_typed_dyn_memory_tracker;
  }

  /* Yes, so track memory inside values. */

  if (!val_raw)
  {
    /* The type has no valueless memory tracker. */
    return NULL;
  }

  /* The type has a memory tracker.  */
  /*                                 */
  /* Return a reference to the       */
  /* value's memory tracker field.   */
  return
    (memory_tracker_t *)
      field_info_ref
        ( memory_tracker_field
        , val_raw
        );
}

/*
 * type_mem_valueless:
 *
 * The type has a valueless memory tracker.
 *
 * (If the memory tracker passed is NULL, this behaves like
 * "type_mem_struct_or_global_dyn".)
 */
memory_tracker_t *type_mem_valueless(const type_t *self, tval *val_raw, memory_tracker_t *valueless_memory_tracker)
{
  if (!valueless_memory_tracker)
    return type_mem_struct_or_global_dyn(self, val_raw);

  return valueless_memory_tracker;
}

/* mem_init */

/*
 * type_supports_dynamical_allocation:
 *
 * Default allocation tracking method.
 *
 * This is an alias for "type_mem_init_valueless_or_inside_value".
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for any value of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_init".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...      *-/ ...
 * >     , /-* mem_init *-/ mytype_mem_init
 * >     , /-* ...      *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static memory_tracker_t *mytype_mem_init
 * >   ( const type_t *self
 * >   , tval *val_raw
 * >   , int is_dynamically_allocated
 * >   )
 * > {
 * >   return type_supports_dynamical_allocation
 * >     ( self
 * >     , val_raw
 * >     , is_dynamically_allocated
 * >     );
 * > }
 */
void *type_supports_dynamic_allocation
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  )
{
  /* "type_supports_dynamic_allocation" is an alias of */
  /* "type_mem_init_valueless_or_inside_value".        */
  return type_mem_init_valueless_or_inside_value(self, val_raw, is_dynamically_allocated);
}

/*
 * type_supports_dynamical_allocation_only_some_vals:
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for only some values of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_init".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 */
void *type_supports_dynamic_allocation_only_some_vals
  ( const type_t *self
  , tval *val_raw
  , int   is_dynamically_allocated
  )
{
  static const int are_all_vals_supported =
    0;

  const memory_manager_t *val_memory_manager =
    type_default_memory_manager(self, NULL);

  return
    mem_init_type_valueless_or_inside_value
      ( self
      , val_raw
      , is_dynamically_allocated
      , are_all_vals_supported
      , val_memory_manager

      , NULL
      , 0
      );
}

/*
 * type_mem_init_valueless_or_inside_value:
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for any value of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_init".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...      *-/ ...
 * >     , /-* mem_init *-/ mytype_mem_init
 * >     , /-* ...      *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static memory_tracker_t *mytype_mem_init
 * >   ( const type_t *self
 * >   , tval *val_raw
 * >   , int is_dynamically_allocated
 * >   )
 * > {
 * >   return type_supports_dynamical_allocation
 * >     ( self
 * >     , val_raw
 * >     , is_dynamically_allocated
 * >     );
 * > }
 */
void *type_mem_init_valueless_or_inside_value
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  )
{
  static const int are_all_vals_supported =
    1;

  const memory_manager_t *val_memory_manager =
    type_default_memory_manager(self, NULL);

  return
    mem_init_type_valueless_or_inside_value
      ( self
      , val_raw
      , is_dynamically_allocated
      , are_all_vals_supported
      , val_memory_manager

      , NULL
      , 0
      );
}

/*
 * Standard memory initialization behaviour.
 *
 * This is used for "mem_init", which is used for 2 purposes:
 *   1) To track newly dynamically allocated values.
 *   2) To determine whether such dynamic allocation is supported for all
 *      values in general, where "val_raw" is NULL.
 *
 * If the type has its own memory tracker, it is used to track each dynamic
 * value allocation.  A type has its own memory tracker if
 * "type_mem(type, NULL)" returns a "memory_tracker_t".
 *
 * Otherwise, call "type_mem(type, val_raw)" to get a reference to the value's
 * memory tracker, and initialize it with
 * "memory_tracker_init", with "val_memory_manager",
 * or with the type's "default_memory_manager" if none is provided, otherwise
 * with the global "default_memory_manager".
 *
 * This is inappropriate for types that lack a valueless tracker and that
 * associate via "mem" independent memory trackers for each value trackers that
 * exist *outside* the value.
 *
 * val_raw:
 * + NULL:
 *     Check whether we support dynamic memory allocation for all values in
 *     general.
 *
 *     Returns:
 *       NULL:     no such support (or an error occurred).
 *       non-NULL: we can dynamically allocate a value.
 *
 * + otherwise:
 *     Track a new value that might be dynamically allocated.
 *
 *     Returns:
 *       NULL:     either an error occured, or dynamic memory allocation for
 *                 this type is not supported.
 *       non-NULL: success, and dynamic memory allocation for this type is
 *                 supported.
 */
void *mem_init_type_valueless_or_inside_value
  ( const type_t           *type
  , tval                   *val_raw
  , int                     is_dynamically_allocated
  , int                     are_all_vals_supported
  , const memory_manager_t *val_memory_manager

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  int               check_dynamic_allocation_support_only;
  memory_tracker_t *memory_tracker;

  check_dynamic_allocation_support_only = 0;
  if (!val_raw)
    check_dynamic_allocation_support_only = 1;

  if (!type)
  {
    /* Error: we need a type! */

    if (check_dynamic_allocation_support_only)
    {
      /* We'll write to err_buf if available, and then just return NULL,
       * indicating no dynamic memory allocation is support.
       */

      /* An error occured. */
      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_init_type_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
            "  Failed to determine whether dynamic allocation is supported\n"
            "  without a type.\n"
            "  Without a type, returning NULL to indicate\n"
            "  no dynamic memory allocation is supported, without a type.\n"
          );

      /* Error occurred: return NULL. */
      return NULL;
    }
    else
    {
      /* We'll return NULL, indicating failure.
       */

      /* An error occured. */
      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_init_type_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
            "  Failed to initialize a value's associated memory tracker without a \"type\".\n"
          );

      /* Error occurred: return NULL. */
      return NULL;
    }
  }

  /* Does the type have its own associated memory tracker for tracking values?
   */
  memory_tracker = type_mem(type, NULL);
  if (memory_tracker)
  {
    /* Yes, it has an associated memory tracker. */

    if (check_dynamic_allocation_support_only)
    {
      /* We're checking whether we support dynamic memory allocation.
       *
       * Yes: return non-NULL:
       *
       * "mem_init_type_valueless_or_inside_value" considers the type to
       * support dynamic memory allocation, because the type has an associated
       * memory tracker and "mem_init_type_valueless_or_inside_value" uses it
       * to track newly initialize values.
       */
      return memory_tracker;
    }
    else
    {
      /* We need to handle memory tracking. */
      if (is_dynamically_allocated)
      {
        int track_result;

        /* The value was dynamically allocated, so track it. */

        /* TODO: once snprintf_prepend is written, replace with the following: */
        /* memory_tracker = memory_tracker_track_allocation(memory_tracker, val_raw, out_err_buf, err_buf_size); */
        track_result = track_byte_allocation(memory_tracker, val_raw);

        if (track_result < 0)
        {
          /* Failed to track allocation; we'll return NULL. */
          if (out_err_buf)
          {
            /* TODO: snprintf_prepend */
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
                "Error: mem_init_type_valueless_or_inside_value: failed to track dynamic allocation!\n"
                "  Failed to initialize a value's associated memory tracker without successful tracking.\n"
                "  memory_tracker_track_allocation returned < 0, indicating failure: %d.\n"

              , (int) track_result
              );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
          }

          /* Error occurred: return NULL. */
          return NULL;
        }
        else
        {
          /* Tracked allocation, and we have nothing further to check.  */
          /* Return success (non-NULL).                                 */
          return memory_tracker;
        }
      }
      else
      {
        /* We have nothing further to check; return success (non-NULL). */
        return memory_tracker;
      }
    }
  }
  else
  {
    /*
     * The type has no associated memory tracker, but it might associate one to
     * a value.
     */

    if (check_dynamic_allocation_support_only)
    {
      /* val_raw might be NULL.  (In this particular context, it is always
       * NULL.)
       */
      memory_tracker = type_mem(type, val_raw);

      if      (memory_tracker)
      {
        /* Yes, this type supports dynamic memory allocation for "val_raw": */
        /* return non-NULL.                                                 */
        return memory_tracker;
      }
      else if (are_all_vals_supported)
      {
        /* Yes, this type supports dynamic memory allocation,             */
        /* according to the caller via "are_all_vals_supported",          */
        /* so this procedure can be called with a non-NULL "val_raw".     */
        /*                                                                */
        /* Yes: return non-NULL.                                          */
        return (void *) type;
      }
      else
      {
        /* No, this type does not support dynamic memory allocation       */
        /* according to the caller via "are_all_vals_supported".          */
        /*                                                                */
        /* No: return NULL.                                               */
        return NULL;
      }
    }
    else
    {
      /* val_raw might be NULL.  (In this particular context, it is never
       * NULL.)
       */
      memory_tracker = type_mem(type, val_raw);

      if (memory_tracker)
      {
        /* Track the value. */
        memory_tracker_t *tracker_init;

        if (!val_memory_manager)
          val_memory_manager = type_default_memory_manager(type, NULL);
        if (!val_memory_manager)
          val_memory_manager = default_memory_manager;

        if (is_dynamically_allocated)
          tracker_init = memory_tracker_init(memory_tracker, val_memory_manager, val_raw);
        else
          tracker_init = memory_tracker_init(memory_tracker, val_memory_manager, NULL);

        if (!tracker_init)
        {
          /* An error occured. */
          if (out_err_buf)
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
                "Error: mem_init_type_valueless_or_inside_value: initializing the memory tracker with \"memory_tracker_init\" failed!\n"
                "  Failed to initialize a value's associated memory tracker.\n"
                "  \"memory_tracker_init\" return NULL.\n"
              );

          /* Error occurred: return NULL. */
          return NULL;
        }
        else
        {
          /* We've succesfully tracked the value. */
          return tracker_init;
        }
      }
      else
      {
        /* There is no memory tracker associated with the value. */
        if (are_all_vals_supported)
        {
          /* Error: are_all_vals_supported is incorrect! */
          if (out_err_buf)
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
                "Error: mem_init_type_valueless_or_inside_value: the type doesn't associated a memory_tracker via \"mem\" for this value, contrary to the caller!\n"
                "  The caller invoked \"mem_init_type_valueless_or_inside_value\" with the \"are_all_vals_supported\" incorrect *enabled*.\n"
                "  But the type doesn't support a memory tracker for this value (\"val_raw\").\n"
                "  \n"
                "  are_all_vals_supported argument: %d\n"
              , (int) are_all_vals_supported
              );

          return NULL;
        }
        else
        {
          /* Success, but dynamic memory allocation is not supported
           * for this particular value.
           */
          return NULL;
        }
      }
    }
  }
}

/* mem_is_dyn */


/*
 * type_is_dyn_valueless_or_inside_value:
 *
 * According to whether the type has a valueless memory tracker, check whether
 * a value was dynamically allocated, with no further computations.
 *
 * This only calls "mem_is_dyn_valueless_or_inside_value" and does no further processing.
 */
int type_is_dyn_valueless_or_inside_value
  ( const type_t *self
  , tval         *val
  )
{
  return mem_is_dyn_valueless_or_inside_value(self, val, NULL, 0);
}

/*
 * mem_is_dyn_valueless_or_inside_value:
 *
 * According to whether the type has a valueless memory tracker, check whether
 * a value was dynamically allocated.
 */
int mem_is_dyn_valueless_or_inside_value
  ( const type_t *type
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  memory_tracker_t *valueless_memory_tracker;

  if (!type)
  {
    /* Error: we need a type! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_is_dyn_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
          "  Failed to determine whether a value was dynamically allocated, with no provided type.\n"
        );

    /* Error occurred: return <= -1. */
    return -1;
  }

  if (!val)
  {
    /* Error: we need a val! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_is_dyn_valueless_or_inside_value: called with a NULL \"val\" argument!\n"
          "  Failed to determine whether a value was dynamically allocated, with no provided value reference.\n"
        );

    /* Error occurred: return <= -1. */
    return -2;
  }

  /* Does the type have its own associated memory tracker for tracking
   * values?
   */
  valueless_memory_tracker = type_mem(type, NULL);
  if (valueless_memory_tracker)
  {
    int is_value_tracked;

    is_value_tracked = tracked_byte_allocation(valueless_memory_tracker, val);

    if (is_value_tracked < 0)
    {
      /* Error: memory_tracker_is_allocation_tracked failed! */

      int error_code;

      if (out_err_buf)
      {
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
            "Error: mem_is_dyn_valueless_or_inside_value: calling \"memory_tracker_is_allocation_tracked\" failed!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  without a successful invocation to \"memory_tracker_is_allocation_tracked\"\n"
            "  for the valuess type-associated memory tracker.\n"
            "\n"
            "  \"memory_tracker_is_allocation_tracked\" returned this error code: %d\n"

          , (int) is_value_tracked
          );

          ensure_ascii_ends_in_char
            ( (char *) out_err_buf
            , (size_t) err_buf_size
            , '\n'
            );
      }

      /* Error occurred: return <= -1. */
      error_code = is_value_tracked - 16;
      if (!(error_code <= 0))
        error_code = -16;
      return (int) error_code;
    }

    if (!is_value_tracked)
    {
      /* No, the value is not dynamically allocated. */

      return 0;
    }
    else
    {
      /* Yes, the value is dynamically allocated. */

      int success_code_yes;

      success_code_yes = is_value_tracked + 8 + 1;
      if (!(success_code_yes >= 1))
        success_code_yes = 8;
      return success_code_yes;
    }
  }
  else
  {
    memory_tracker_t *memory_tracker;
    void             *memory_tracker_container;

    memory_tracker = type_mem(type, val);

    if (!memory_tracker)
    {
      /* Error: the value should have a memory tracker! */

      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_is_dyn_valueless_or_inside_value: the type doesn't associate the value with a memory tracker!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  without a valueless type-associated memory tracker or a memory tracker inside the value.\n"
          );

      /* Error occurred: return <= -1. */
      return -3;
    }

    memory_tracker_container = memory_tracker->dynamic_container;

    if      (!memory_tracker_container)
    {
      /* This value was not dynamically allocated. */
      return 0;
    }
    else if (memory_tracker_container == (void *) val)
    {
      /* This value was dynamically allocated. */
      return 1;
    }
    else
    {
      /* Error: "dynamically_allocated_container" should */
      /* reference the value, but it doesn't!            */

      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_is_dyn_valueless_or_inside_value: the type's memory tracker has a container that differs from the value!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  with an incorrect \"dynamically_allocated_container\" value.\n"
            "\n"
            "  If the container is non-NULL, these references should be equal, but aren't.\n"
            "\n"
            "  memory_tracker->dynamically_allocated_container: %p\n"
            "  val (the value)  - - - - - - - - - - - - - - - : %p\n"

          , (void *) memory_tracker_container
          , (void *) val
          );

      /* Error occurred: return <= -1. */
      return -4;
    }
  }
}

/* mem_free */

/*
 * type_mem_free_valueless_or_inside_value_allocation:
 *
 * The type uses standard dynamic allocation, only tracking the allocation
 * buffer for dynamically allocated values.
 *
 * If a type lacks a valueless memory tracker, then values are assumed to
 * contain the memory tracker, and the value itself is freed to free the memory
 * tracker, with "memory_tracker_free_container".
 *
 * This will free tracked memory allocated by the
 * "type_supports_dynamic_allocation*" methods.
 *
 * Don't assign a type's "mem_free" method to this if the type lacks a
 * valueless memory tracker and values themselves also lack a memory tracker;
 * in this case dynamic memory allocation is usually not supported for this
 * type (not recommended).
 */
int type_mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval         *val
  )
{
  return mem_free_valueless_or_inside_value_allocation(type, val, NULL, 0);
}

/* TODO TODO TODO TODO FIXME FIXME FIXME FIXME: if "container" is not equal to
 * the value, e.g. if "container" points to the memory tracker itself, then
 * what?
 * Oh, oh, add a new field to memory_tracker_t!
 *
 * AFFECTS: at *least* mem_free and is_dyn methods!!
 */
/*
 * mem_free_valueless_or_inside_value_allocation:
 *
 * Free memory allocated by "mem_init_type_valueless_or_inside_value".
 *
 * If a type lacks a valueless memory tracker, then values are assumed to
 * contain the memory tracker, and the value itself is freed to free the memory
 * tracker, with "memory_tracker_free_container".
 *
 * If a type performs additional memory tracking for dynamic allocation (as
 * opposed to other memory tracking, e.g. at runtime for dynamically allocated
 * fields; this is something that "free" should handle), it should free these
 * too after calling "mem_free_valueless_or_inside_value_allocation" before
 * returning, but usually this isn't the case.
 *
 * Like "mem_init_type_valueless_or_inside_value", the behaviour of this
 * procedure depends on whether the type is associated with its own valueless
 * memory tracker ("type_mem(type, NULL) != NULL").  If so, remove the
 * reference from the type's valueless memory tracker when freeing after
 * freeing a dynamically allocated value and then return (>= 1).  Otherwise,
 * just free the value after freeing its trakced memory.
 */
int mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval          *val

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  int is_dynamically_allocated;

  if (!type)
  {
    /* Error: we need a type! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: called with a NULL \"type\" argument!\n"
          "  Failed to free a value's dynamically allocated memory with no provided type.\n"
        );

    /* Error occurred: return <= -1. */
    return -2;
  }

  if (!val)
  {
    /* Error: we need a val! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: called with a NULL \"val\" argument!\n"
          "  Failed to free a value's dynamically allocated memory with no provided value reference.\n"
        );

    /* Error occurred: return <= -1. */
    return -3;
  }

  is_dynamically_allocated = type_mem_is_dyn(type, val);
  if (is_dynamically_allocated < 0)
  {
    /* Error: the type needs mem_is_dyn! */

    int error_code;

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: calling the type's \"mem_is_dyn\" method failed!\n"
          "  Failed to free a value's dynamically allocated memory without succesfully\n"
          "  determining whether this value was dynamically allocated.\n"
          "\n"
          "  The type's \"mem_is_dyn\" method returned this error code: %d\n"

        , (int) is_dynamically_allocated
        );

    /* Error occurred: return <= -1. */
    error_code = is_dynamically_allocated - 16;
    if (!(error_code <= 0))
      error_code = -16;
    return (int) error_code;
  }

  if (!is_dynamically_allocated)
  {
    return 0;
  }
  else
  {
    /* Does the type have its own associated memory tracker for tracking
     * values?
     */

    memory_tracker_t *valueless_memory_tracker;
    
    valueless_memory_tracker = type_mem(type, NULL);
    if (valueless_memory_tracker)
    {
      int    success_code;
      size_t num_freed;

      /* TODO use "out_err_buf" once snprintf_prepend is implemented. */
      num_freed = track_mfree(valueless_memory_tracker, val, NULL);

      if (num_freed <= 0)
      {
        /* "memory_tracker_free_allocation" failed. */

        int error_code;

        if (out_err_buf)
        {
          /* TODO use "snprintf_prepend" once snprintf_prepend is implemented. */
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
              "Error: mem_free_valueless_or_inside_value_allocation: the call to \"track_free\" failed to free allocation!\n"
              "  Failed to free a value's valueless-type-memory-tracker allocation\n"
              "  of dynamically allocated memory without a successful invocation\n"
              "  of \"memory_tracker_free_allocation\".\n"
              "\n"
              "  memory_tracker_free_allocation returned error code: %d\n"
              "\n"
              "TODO memory_tracker_free_allocation's error message: "

            , (int) num_freed
            );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
        }

        /* Return failure. */
        error_code = ((int) num_freed) - 64;
        if (!(error_code <= -1))
          error_code = -64;
        return error_code;
      }

      /* Done freeing. */
      success_code = ((int) num_freed) + 16 + 1;
      if (!(success_code >= 1))
        success_code = 16;
      return success_code;
    }
    else
    {
      int success_code;
      int free_container_result;

      memory_tracker_t *memory_tracker;

      memory_tracker = type_mem(type, val);
      if (!memory_tracker)
      {
        /* Uh oh: we need an associated memory tracker! */

        if (out_err_buf)
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
            , "Error: mem_free_valueless_or_inside_value_allocation: the type lacks a memory tracker association for the value!\n"
              "  Failed to free a value's dynamically allocated memory with no associated memory tracker.\n"
            );

        /* Error occurred: return <= -1. */
        return -4;
      }

      /* ---------------------------------------------------------------- */

      free_container_result = memory_tracker_free(memory_tracker);

      if (free_container_result <= -1)
      {
        /* "memory_tracker_free_container" failed. */

        int error_code;

        if (out_err_buf)
        {
          /* TODO use "snprintf_prepend" once snprintf_prepend is implemented. */
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size), ""
            , "Error: mem_free_valueless_or_inside_value_allocation: the call to \"memory_tracker_free\" failed!\n"
              "  Failed to free a no-valueless-memory-tracker-typed value's\n"
              "  dynamically allocated memory without a successful invocation\n"
              "  of \"memory_tracker_free\".\n"
              "\n"
              "  memory_tracker_free error code: %d\n"

            , (int) free_container_result
            );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
        }

        /* Return failure. */
        error_code = free_container_result - 32;
        if (!(error_code <= -1))
          error_code = -32;
        return error_code;
      }

      /* Done freeing. */
      success_code = free_container_result + 32 + 1;
      if (!(success_code >= 1))
        success_code = 32;
      return success_code;
    }
  }
}

/* default_memory_manager */

const memory_manager_t *type_has_no_default_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, NULL);
}

const memory_manager_t *type_prefers_given_memory_manager(const type_t *self, tval *val, const memory_manager_t *memory_manager)
{
  if (val)
  {
    memory_tracker_t *memory_tracker;

    memory_tracker = type_mem(self, val);

    if (memory_tracker)
    {
      /* Return the value's memory tracker's memory manager. */
      return &memory_tracker->memory_manager;
    }
    else
    {
      memory_tracker = type_mem(self, NULL);

      if (memory_tracker)
      {
        /* Return the type's memory tracker's memory manager. */
        return &memory_tracker->memory_manager;
      }
      else
      {
        return memory_manager;
      }
    }
  }
  else
  {
    return memory_manager;
  }
}

const memory_manager_t *type_prefers_default_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, default_memory_manager);
}

const memory_manager_t *type_prefers_malloc_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, &malloc_manager);
}

/* dup */

/*
 * type_has_struct_dup_allow_malloc:
 *
 * Copy "src" into "dest" with "dup_struct".
 *
 * If "src" is NULL, use the type's default value.  (If this is also NULL, fail
 * and return NULL.)
 *
 * Allows malloc: if "dest" is NULL, request allocation of a new one with
 * "type_init(self, NULL)".
 */
tval *type_has_struct_dup_allow_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      )
{
  int dyn_alloc;
  const struct_info_t *struct_info;

  const char *struct_dup_is_err;

  struct_info = type_is_struct(self);

  if (!src)
  {
    src  = type_has_default(self);
  }
  dyn_alloc = 0;
  if (!dest)
  {
    dyn_alloc = 1;

    dest = type_init(self, NULL);

    if (!dest)
      return NULL;
  }

  if (!dest || !src || !struct_info)
  {
    if (dyn_alloc)
    {
      type_free(self, dest);

      dyn_alloc = 0;
    }

    return NULL;
  }

  struct_dup_is_err =
    struct_dup( struct_info
              , dest
              , src
              , defaults_src_unused
              , rec_copy
              , dup_metadata
              , ref_traversal
              );

  if (struct_dup_is_err)
  {
    if (dyn_alloc)
    {
      type_free(self, dest);

      dyn_alloc = 0;
    }

    return NULL;
  }

  return dest;
}

/*
 * type_has_struct_dup_never_malloc:
 *
 * Copy "src" into "dest" with "dup_struct".
 *
 * If "src" is NULL, use the type's default value.  (If this is also NULL, fail
 * and return NULL.)
 *
 * If "dest" is NULL, returns NULL.
 */
tval *type_has_struct_dup_never_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      )
{
  const struct_info_t *struct_info;

  const char *struct_dup_is_err;

  struct_info = type_is_struct(self);

  if (!src)
    src  = type_has_default(self);

  if (!dest || !src || !struct_info)
    return NULL;

  struct_dup_is_err =
    struct_dup( struct_info
              , dest
              , src
              , defaults_src_unused
              , rec_copy
              , dup_metadata
              , ref_traversal
              );

  if (struct_dup_is_err)
    return NULL;

  return dest;
}

/* user */
void *type_has_no_user_data(const type_t *self, tval *val)
{
  return NULL;
}

/* cuser */
const void *type_has_no_cuser_data(const type_t *self, const tval *val)
{
  return NULL;
}

/* cmp */
/* Compare fields if struct, otherwise compare memory. */
int type_has_standard_cmp(const type_t *self, const tval *check, const tval *baseline, int deep, ref_traversal_t *vals)
{
  const struct_info_t *struct_info;

  if (!self || !check || !baseline)
    return -1;

  if ((struct_info = type_is_struct(self)))
  {
    return struct_cmp(struct_info, check, baseline, deep, vals);
  }
  else
  {
    size_t check_size;
    size_t baseline_size;

    check_size    = type_size(self, check);
    baseline_size = type_size(self, baseline);

    if (check_size && baseline_size && check_size == baseline_size)
    {
      return memcmp(check, baseline, min_size(check_size, baseline_size));
    }
    else
    {
      return -1;
    }
  }
}

/* ---------------------------------------------------------------- */
/* type_t: Defaults.                                                */
/* ---------------------------------------------------------------- */

/*
 * This section in the "type" module contains default definitions for types.
 *
 * This section is designed to also serve as a template to cargo cult
 * type_t definitions that override the defaults.  A template for 3 fields is
 * provided below, commented out.  A type definition can copy this and make the
 * appropriate changes, starting by replacing "this" with the name of the type.
 *
 * Also, the default definitions can also be copied to override default
 * behaviour, or more commonly to add to the default behaviour, e.g. by
 * performing additional initialization on new values.
 *
 * ----------------------------------------------------------------
 *
 * A "type_t" definition of a type is a struct that contains function pointers
 * to C functions that handle the type.  A type must specify a minimum of 3
 * fields.
 *
 * Every type should be defined and accessed through a function like this:
 *
 * > const type_t *foo_type(void);
 *
 * A function pointer to a function with this type is called "typed_t".
 *
 * > typed_t which_type_is_it = foo_type;
 *
 * A "typed_t" is the core, fundamental part of a type's API.  A public type
 * exports at least this; other parts are optional, e.g. a top-level "typed_t"
 * reference.  Some types are procedurally generated, because they invoke other
 * procedures to generate the type definition, and so can't use a constant
 * initializer.
 *
 * So a minimal type definition requires at least this:
 *
 * > const type_t *<name_of_type>_type(void);
 *
 * This is a function declaration that "typed_t" function pointers can be
 * assigned to.
 *
 * ----------------------------------------------------------------
 *
 * For convenience, some type_t implementations also provide an optional top-level
 * "type_t" reference, but a const "type_t" definition requires a constant
 * initializer, precluding e.g. using the API provided by "type_structs".
 *
 * For example:
 *
 * > const type_t *foo_type(void);
 * > extern const type_t foo_type_def; /-* optional *-/
 * >
 * > ...
 * >
 * > const type_t *foo_type { return &foo_type_def };
 * > const type_t foo_type_def =
 * >   { type_type
 * >
 * >   , ...
 * >   }
 *
 * ----------------------------------------------------------------
 *
 * Typically, a low-level "type_t" definition will look like this:
 *
 * > /-* intpair.h *-/
 * >
 * > ...
 * >
 * > /-* "intpair_type" *-/
 * > const type_t *intpair_type(void);
 * > /-* optional: *-/ extern const type_t intpair_type_def;
 * > typedef struct intpair_s intpair_t;
 * > struct intpair_s
 * > {
 * >   typed_t type;
 * >
 * >   memory_tracker memory;
 * >
 * >   int a;
 * >   int b;
 * > };
 *
 * > /-* intpair.c *-/
 * >
 * > ...
 * >
 * > const type_t *intpair_type(void)
 * >   { return &intpair_type_def; }
 * >
 * > static const char          *intpair_type_name     (const type_t *self);
 * > static size_t               intpair_type_size     (const type_t *self, const tval *val);
 * > static const struct_info_t *intpair_type_is_struct(const type_t *self);
 * >
 * > const type_t intpair_type_def =
 * >   { type_type
 * > 
 * >     /-* @: Required.           *-/
 * > 
 * >   , /-* memory                 *-/ MEMORY_TRACKER_DEFAULTS
 * >   , /-* is_self_mutable        *-/ NULL
 * >   , /-* @indirect              *-/ intpair_type
 * > 
 * >   , /-* self                   *-/ NULL
 * >   , /-* container              *-/ NULL
 * > 
 * >   , /-* typed                  *-/ NULL
 * > 
 * >   , /-* @name                  *-/ intpair_type_name
 * >   , /-* info                   *-/ NULL
 * >   , /-* @size                  *-/ intpair_type_size
 * >   , /-* @is_struct             *-/ intpair_type_is_struct
 * >   , /-* is_mutable             *-/ NULL
 * >   , /-* is_subtype             *-/ NULL
 * >   , /-* is_supertype           *-/ NULL
 * > 
 * >   , /-* cons_type              *-/ NULL
 * >   , /-* init                   *-/ NULL
 * >   , /-* free                   *-/ NULL
 * >   , /-* has_default            *-/ NULL
 * >   , /-* mem                    *-/ NULL
 * >   , /-* mem_init               *-/ NULL
 * >   , /-* mem_is_dyn             *-/ NULL
 * >   , /-* mem_free               *-/ NULL
 * >   , /-* default_memory_manager *-/ NULL
 * >
 * >   , /-* dup                    *-/ NULL
 * >
 * >   , /-* user                   *-/ NULL
 * >   , /-* cuser                  *-/ NULL
 * >   , /-* cmp                    *-/ NULL
 * > 
 * >   , /-* parity                 *-/ ""
 * >   };
 * >
 * > static const char          *intpair_type_name     (const type_t *self)
 * >   { return "memory_manager"; }
 * >
 * > static size_t               intpair_type_size     (const type_t *self, const tval *val)
 * >   { return sizeof(intpair_t); }
 * >
 * > static const struct_info_t *intpair_type_is_struct(const type_t *self)
 * >   {
 * >     ...
 * >
 * >     return struct_info;
 * >   }
 *
 * ----------------------------------------------------------------
 */

/*
 * Default fields for types.
 *
 * A minimum of 4 fields is required for each type:
 *  - Either "indirect" or "container"
 *      A "typed_t" that returns this type.
 *
 *      "indirect" is a direct "typed_t" value, and "container" returns
 *      typed_t.
 *
 *  - name      (returns const char *):
 *      The name of the type.
 *
 *      Example: "memory_manager".
 *
 *  - size      (returns size_t):
 *      The size of the type.
 *
 *      Example: "sizeof(memory_manager_t)".
 *
 *  - is_struct (returns const struct_info_t *):
 *      The fields of the type's struct, or NULL if the type isn't a struct.
 *
 * The remaining default values, used when a "type_t" method is NULL for each
 * "type_t" method, specify a type that has the following characteristics:
 *
 *   - has_default:
 *       "type_has_no_default":
 *
 *       By default, a type lacks a default value.  A type can specify a
 *       default value by assigning a method that returns it.
 *
 *   - "template_cons_t" constructors:
 *       Initialization uses "template_cons_t" and is based on standard
 *       "template_cons_t"-based methods.
 *
 *   - standard memory tracking:
 *       If the type is a struct and its "struct_info" (i.e. that returned by
 *       "is_struct") references a memory tracker field, it is used when the
 *       type doesn't assign itself a valueless memory tracker (which would
 *       allow the struct to optionall omit a "memory_tracker" field, relocating tracking
 *       of dynamically allocated values to this valueless memory tracker).
 *
 *       (A type can choose a valueless memory tracker by assigning its "mem"
 *       method to a function that wraps "type_mem_valueless", which it calls
 *       with the additional argument of the memory tracker to use as its
 *       valueless memory tracker.)
 *
 *       If a type is not a struct and lacks a valueless memory tracker, the
 *       global memory tracker "global_typed_dyn_memory_tracker" is used to
 *       track dynamic memory allocations.
 *
 *   - whether the type is typed (values are "tval *"s):
 *       "type_is_typed_from_struct":
 *
 *       By default, the type is considered to be "typed" if and only if the
 *       type is a struct whose first field has type "typed", for example
 *       "typed_t type".
 *
 *   - type info:
 *       "type_has_no_info":
 *
 *       By default, a type has no information string.
 */

const type_t type_defaults =
  TYPE_DEFAULTS;

const type_t        *default_type_self       (const type_t *self)
{
  return type_has_self(self);
}

typed_t              default_type_container  (const type_t *self)
{
  return type_has_indirect(self);
}

const type_t        *default_type_typed      (const type_t *self)
  { return type_is_typed_from_struct(self); }

/*
const char          *default_type_name       (const type_t *self)
  { return "this"; }
*/

const char          *default_type_info       ( const type_t *type
                                                    , char         *out_info_buf
                                                    , size_t        info_buf_size
                                                    )
  { return type_has_no_info(type, out_info_buf, info_buf_size); }

/*
size_t               default_type_size       (const type_t *self, const tval *val)
  { return sizeof(this_t); }
*/

/*
DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(this)
const struct_info_t *default_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(this);

    /-* typed_t type *-/
    STRUCT_INFO_RADD(typed_type(), type);

    /-* int foo *-/
    STRUCT_INFO_RADD(int_type(),   foo);

    /-* int bar *-/
    STRUCT_INFO_RADD(int_type(),   bar);

    STRUCT_INFO_DONE();
  }
*/

tval                *default_type_is_mutable (const type_t *self, const tval *val)
  { return type_mutable_from_struct(self, val); }

const type_t        *default_type_is_subtype ( const type_t *self
                                             , const type_t *is_subtype
                                             )
  { return type_has_no_nonextensible_subtypes(self, is_subtype); }

const type_t        *default_type_is_supertype
                                             ( const type_t *self
                                             , const type_t *is_supertype
                                             )
  { return type_has_no_extensible_supertypes(self, is_supertype); }

typed_t              default_type_cons_type  (const type_t *self)
  { return type_has_template_cons_type(self); }

tval                *default_type_init       (const type_t *self, tval *cons)
  { return type_has_template_cons_basic_initializer(self, cons); }

void                 default_type_free       (const type_t *self, tval *val)
  {        type_has_template_cons_basic_freer(self, val); }

const tval          *default_type_has_default(const type_t *self)
  { return type_has_no_default_value(self); }

memory_tracker_t    *default_type_mem        (const type_t *self, tval *val_raw)
  { return type_mem_struct_or_global_dyn(self, val_raw); }

void                *default_type_mem_init   ( const type_t *self
                                             , tval *val_raw
                                             , int is_dynamically_allocated
                                             )
  { return type_supports_dynamic_allocation(self, val_raw, is_dynamically_allocated); }

int                  default_type_mem_is_dyn ( const type_t *self
                                             , tval         *val
                                             )
  { return type_is_dyn_valueless_or_inside_value(self, val); }

int                  default_type_mem_free   ( const type_t *self
                                             , tval         *val
                                             )
  { return type_mem_free_valueless_or_inside_value_allocation(self, val); }

const memory_manager_t
                    *default_type_default_memory_manager
                                             ( const type_t *self
                                             , tval *val
                                             )
  { return type_has_no_default_memory_manager(self, val); }

tval                *default_type_dup        ( const type_t *self
                                             , tval *dest
                                             , const tval *src
                                             , int defaults_src_unused
                                             , int rec_copy
                                             , int dup_metadata
                                             , ref_traversal_t *ref_traversal
                                             )
  {
    return
      type_has_struct_dup_allow_malloc
        ( self
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , ref_traversal
        );
  }

void                *default_type_user       (const type_t *self, tval *val)
  { return type_has_no_user_data(self, val); }

const void          *default_type_cuser      (const type_t *self, const tval *val)
  { return type_has_no_cuser_data(self, val); }

int                  default_type_cmp        ( const type_t *self
                                             , const tval *check
                                             , const tval *baseline
                                             , int deep
                                             , ref_traversal_t *vals
                                             )
  { return type_has_standard_cmp(self, check, baseline, deep, vals); }

/* ---------------------------------------------------------------- */

/*
 * Fundamental "type_t" accessors.
 */

const memory_tracker_t *type_get_memory         (const type_t *type)
{
  if (!type)
    return NULL;

  return &type->memory;
}

const type_t           *type_get_self_is_mutable(const type_t *type)
{
  if (!type)
    return NULL;

  return type->self_is_mutable;
}

typed_t                 type_get_indirect       (const type_t *type)
{
  if (!type)
    return NULL;

  return type->indirect;
}


const type_t        *type_self       (const type_t *type)
{
  if (!type || !type->self)
    return type_defaults.self(type);
  else
    return type->self(type);
}

typed_t              type_container  (const type_t *type)
{
  if (!type || !type->container)
    return type_defaults.container(type);
  else
    return type->container(type);
}

const type_t        *type_typed      (const type_t *type)
{
  if (!type || !type->typed)
    return type_defaults.typed(type);
  else
    return type->typed(type);
}

const char          *type_name       (const type_t *type)
{
  if (!type || !type->name)
    return NULL;
  else
    return type->name(type);
}

const char          *type_info       ( const type_t *type
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     )
{
  if (!type || !type->info)
    return type_defaults.info(type, out_info_buf, info_buf_size);
  else
    return type->info(type, out_info_buf, info_buf_size);
}

size_t               type_size       (const type_t *type, const tval *val)
{
  if (!type || !type->size)
    return 0;
  else
    return type->size(type, val);
}

const struct_info_t *type_is_struct  (const type_t *type)
{
  if (!type || !type->is_struct)
    return NULL;
  else
    return type->is_struct(type);
}

tval                *type_is_mutable (const type_t *type, const tval *val)
{
  if (!type || !type->is_mutable)
    return type_defaults.is_mutable(type, val);
  else
    return type->is_mutable(type, val);
}

const type_t        *type_is_subtype ( const type_t *type
                                     , const type_t *is_subtype
                                     )
{
  if (!type || !type->is_subtype)
    return type_defaults.is_subtype(type, is_subtype);
  else
    return type->is_subtype(type, is_subtype);
}

const type_t        *type_is_supertype
                                     ( const type_t *type
                                     , const type_t *is_supertype
                                     )
{
  if (!type || !type->is_supertype)
    return type_defaults.is_supertype(type, is_supertype);
  else
    return type->is_supertype(type, is_supertype);
}

typed_t              type_cons_type  (const type_t *type)
{
  if (!type || !type->cons_type)
    return type_defaults.cons_type(type);
  else
    return type->cons_type(type);
}

tval                *type_init       (const type_t *type, tval *cons)
{
  if (!type || !type->init)
    return type_defaults.init(type, cons);
  else
    return type->init(type, cons);
}

void                 type_free       (const type_t *type, tval *val)
{
  if (!type || !type->free)
    type_defaults.free(type, val);
  else
    type->free(type, val);
}

const tval          *type_has_default(const type_t *type)
{
  if (!type || !type->has_default)
    return type_defaults.has_default(type);
  else
    return type->has_default(type);
}

memory_tracker_t    *type_mem        (const type_t *type, tval *val_raw)
{
  if (!type || !type->mem)
    return type_defaults.mem(type, val_raw);
  else
    return type->mem(type, val_raw);
}

void                *type_mem_init   ( const type_t *type
                                     , tval *val_raw
                                     , int is_dynamically_allocated
                                     )
{
  if (!type || !type->mem_init)
    return type_defaults.mem_init(type, val_raw, is_dynamically_allocated);
  else
    return type->mem_init(type, val_raw, is_dynamically_allocated);
}

int                  type_mem_is_dyn ( const type_t *type
                                     , tval         *val
                                     )
{
  if (!type || !type->mem_is_dyn)
    return type_defaults.mem_is_dyn(type, val);
  else
    return type->mem_is_dyn(type, val);
}

int                  type_mem_free   ( const type_t *type
                                     , tval         *val
                                     )
{
  if (!type || !type->mem_free)
    return type_defaults.mem_free(type, val);
  else
    return type->mem_free(type, val);
}

const memory_manager_t
                    *type_default_memory_manager
                                     ( const type_t *type
                                     , tval *val
                                     )
{
  if (!type || !type->default_memory_manager)
    return type_defaults.default_memory_manager(type, val);
  else
    return type->default_memory_manager(type, val);
}

tval                *type_dup        ( const type_t *type
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *vals
                                     )
{
  if (!type || !type->dup)
    return
      type_defaults.dup
        ( type
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , vals
        );
  else
    return
      type->dup
        ( type
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , vals
        );
}

void                *type_user       (const type_t *type, tval *val)
{
  if (!type || !type->user)
    return type_defaults.user(type, val);
  else
    return type->user(type, val);
}

const void          *type_cuser      (const type_t *type, const tval *val)
{
  if (!type || !type->cuser)
    return type_defaults.cuser(type, val);
  else
    return type->cuser(type, val);
}

int                  type_cmp        ( const type_t *type
                                     , const tval *check
                                     , const tval *baseline
                                     , int deep
                                     , ref_traversal_t *vals
                                     )
{
  if (!type || !type->cmp)
    return type_defaults.cmp(type, check, baseline, deep, vals);
  else
    return type->cmp(type, check, baseline, deep, vals);
}

/* ---------------------------------------------------------------- */

/*
 * Compositional "type_t" accessors.
 */

const type_t *is_subtype(const type_t *sub, const type_t *super)
{
  const type_t *is_sub;

  if (!sub || !super)
    return NULL;

  if ((is_sub = type_is_subtype(super, sub)))
  {
    return is_sub;
  }
  else
  {
    if ((is_sub = type_is_supertype(sub, super)))
      return is_sub;
  }

  return NULL;
}

const type_t *is_type_equivalent(const type_t *this, const type_t *that)
{
  const type_t *this_subof_that;
  const type_t *that_subof_this;

  if
    (  (this_subof_that = is_subtype(this, that))
    && (that_subof_this = is_subtype(that, this))
    )
    return is_subtype(this_subof_that, that_subof_this);
  else
    return NULL;
}

const type_t *is_subtype_via(const type_t *sub, const type_t *mid, const type_t *super)
{
  const type_t *is_sub;

  if ((is_sub = is_subtype(is_subtype(sub, mid), super)))
  {
    return is_sub;
  }
  else
  {
    if ((is_sub = is_subtype(sub, is_subtype(mid, super))))
      return is_sub;
  }

  return NULL;
}

const type_t *is_proper_subtype(const type_t *sub, const type_t *super)
{
  if (!sub || !super)
    return NULL;

  if (!is_type_equivalent(sub, super))
  {
    return NULL;
  }
  else
  {
    return is_subtype(sub, super);
  }
}

const type_t *is_supertype(const type_t *super, const type_t *sub)
{
  return is_subtype(sub, super);
}

const type_t *is_supertype_via(const type_t *super, const type_t *mid, const type_t *sub)
{
  return is_subtype_via(sub, mid, super);
}

const type_t *is_proper_supertype(const type_t *super, const type_t *sub)
{
  return is_proper_subtype(sub, super);
}

/* ---------------------------------------------------------------- */

int cmp_with_type_deep(const type_t *type, const tval *check, const tval *baseline, int deep)
{
  return type_cmp(type, check, baseline, deep, NULL);
}

int cmp_with_type     (const type_t *type, const tval *check, const tval *baseline)
{
  return cmp_with_type_deep(type, check, baseline, CMP_WITH_TYPE_DEFAULT_DEEP);
}

/* ---------------------------------------------------------------- */

/*
 * "type_t" accessors for typed "tval"s.
 *
 * All provided values must start with a "typed_t" value (so-called "tval"s)!
 */

/* TODO: more */

void tval_free(tval *val)
{
  type_free(typeof(val), val);
}
