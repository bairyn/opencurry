/*
 * opencurry: type_base.c
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
 *   - size_t
 */
#include <stddef.h>

/*
 * stdio.h:
 *  - fprintf
 *  - stderr
 */
#include <stdio.h>

/* stdlib.h:
 *   , calloc
 *   - free
 *   - malloc
 *   , realloc
 */
#include <stdlib.h>

/* string.h:
 *   - memcpy
 *   - memmove
 *   - memset
 *   - strlen
 */
#include <string.h>

#include "base.h"
#include "type_base.h"

#include "util.h"

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/* TODO:
const type_t *typed_type(void);
*/

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

/* memory_manager type. */

/*
 * memory_manager_type: memory_manager_t (struct memory_manager_s)
 *
 * This type definition, like many others, is a standard representation for a
 * struct, defining how new values of this struct are initialized, allocated,
 * freed, and copied, and also fully represents each field in the struct,
 * allowing general programming based on the definition of the struct, rather
 * than ad-hoc programming handling each individual struct on an individual
 * basis.
 *
 * The "type_struct" module abstracts this pattern.  But here we hand-code
 * "memory_manager_type" at a low-level instead because it is a foundational
 * type definition in "type_base".  (As a bonus, this also illustrates how
 * types are defined directly.)
 *
 * memory_manager_type's most important subcomponent is
 * "memory_manager_is_struct", which describes "struct memory_manager_s".
 *
 * ----------------------------------------------------------------
 *
 * Normally, the "type_struct" module can be used to define "struct" type_t's.
 */

/*
 * memory_manager_type: The core part of a type definition.
 *  .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *
 * This is just a C function that returns the "const type_t *" represent the
 * "memory_manager_t" type.
 *
 * "memory_manager_type" is how this type is accessed.
 *
 * ----------------------------------------------------------------
 *
 * .   .   .   .   .   .   .   .   .   .   .   .   .   .
 * # > const type_t *memory_manager_type(void);
 * # VS
 * # > const type_t  memory_manager_type;
 * .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *
 * So why isn't this just "const type_t memory_manager_type"?  Why are
 * "type_t"'s generally obtained from functions, rather than from direct
 * references to the type?
 *
 * Because a type obtain from a function allows the type_t to be *procedurally*
 * generated, allowing it to initialize itself with code, rather than only
 * static definitions.
 *
 * "memory_manager_type" itself doesn't need this.  Like many other types, its
 * "type_t" can be defined at the top-level with a constant initializer.
 *
 * As a convention, because "memory_manager_type" is available at the top-level
 * without procedural generation, this is also available without a function, as
 * "const type_t memory_manager_type_def", which "memomry_manager_type" just
 * returns a reference to.  But not all types do.
 */
const type_t *memory_manager_type(void)
  { return &memory_manager_type_def; }

/*
 * "type_t" is a struct mostly of function pointer fields.
 *
 * Our struct is defined with function pointers to these functions.
 * These functions are "static", so they're only locally available here.
 *
 * (But they can still be accessed through "memory_manager_type"'s fields.)
 */
static const type_t        *memory_manager_type_typed      (const type_t *self);
static const char          *memory_manager_type_name       (const type_t *self);
static const char          *memory_manager_type_info       (const type_t *self);
static size_t               memory_manager_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_manager_type_is_struct  (const type_t *self);
static typed_t              memory_manager_type_cons_type  (const type_t *self);
static tval                *memory_manager_type_init       (const type_t *self, tval *cons);
static void                 memory_manager_type_free       (const type_t *self, tval *val);
static const tval          *memory_manager_type_has_default(const type_t *self);
static memory_tracker_t    *memory_manager_type_mem        (const type_t *self, tval *val_raw);
static tval                *memory_manager_type_dup        ( const type_t *self
                                                           , tval *dest
                                                           , const tval *src
                                                           , int rec_copy
                                                           , int dup_metadata
                                                           , ref_traversal_t *ref_traversal
                                                           );

/*
 * We'll use these in the definition of "memory_manager_type_is_struct".
 */
static size_t memory_manager_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t memory_manager_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

/*
 * "memory_manager_type"'s type_t definition.
 *
 * The main way to access "memory_manager_type"'s type_t is via
 * "memory_manager_type", but since we can provide a "type_t" for
 * memory_manager, as a convenience, we provide a globally-accessible
 * definition of the "type_t" value here.
 *
 * Not all types are defined with a constant struct initializer.  For example,
 * a type might want to call other functions, e.g. those from "type_struct", to
 * define itself.  A "const struct mystruct_s myvalue = { ... };" at the
 * top-level cannot call functions.
 *
 * Even those types that can be defined with a constant struct initializer
 * don't need to provide a global reference to it.  We only do it here, for
 * "memory_manager_type", for convenience.
 *
 * The convention for type definitions that decide to use and provide a
 * constant struct initializer is to name the "type_t" value
 * "<type_name>_type_def".
 *
 * A function that generates a "const type_t *" reference is called a
 * "typed_t":
 *
 * > typedef const type_t *(*typed_t)(void);
 *
 * Example:
 *
 * >       typed_t     our_type;
 * > const type_t     *our_type_struct;
 * > const type_t     *also_our_type_struct;
 * >
 * > memory_manager_t  our_manager;
 * > template_cons_t   cons = { template_cons_type, &our_manager };
 * >
 * > our_type             = memory_manager;
 * >
 * > our_type_struct      = our_type();
 * > also_our_type_struct = memory_manager();
 * >
 * > type_init(our_type_struct, cons);
 */
const type_t memory_manager_type_def =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ memory_manager_type_typed

  , /* name        */ memory_manager_type_name
  , /* info        */ memory_manager_type_info
  , /* size        */ memory_manager_type_size
  , /* is_struct   */ memory_manager_type_is_struct

  , /* cons_type   */ memory_manager_type_cons_type
  , /* init        */ memory_manager_type_init
  , /* free        */ memory_manager_type_free
  , /* has_default */ memory_manager_type_has_default
  , /* mem         */ memory_manager_type_mem
  , /* dup         */ memory_manager_type_dup

  , /* parity      */ ""
  };

/* We could also directly set the "typed" field above to "type_is_typed". */
static const type_t        *memory_manager_type_typed      (const type_t *self)
  { return type_is_typed(self); }

/*
 * Just return the name of the type.
 *
 * "memory_manager".
 */
static const char          *memory_manager_type_name       (const type_t *self)
  { return "memory_manager"; }

/*
 * Just return a string with general information about the type.
 */
static const char          *memory_manager_type_info       (const type_t *self)
  { return "typedef struct memory_manager_s memory_manager_t"; }

/*
 * Return the size of values of the represented type.
 */
static size_t               memory_manager_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_manager_t); }

/*
 * "memory_manager_t" is a struct.
 *
 * Thus this function returns a representation of each field in the struct.
 *
 * This is the most important part of the definiton of
 */
static const struct_info_t *memory_manager_type_is_struct  (const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const memory_manager_t  empty;

        field_info_t           *field;
        size_t                  fields_len;

        size_t (*default_value)        (const field_info_t *self, void *dest_field_mem);
        size_t (*template_unused_value)(const field_info_t *self, void *dest_field_mem);

        default_value         = memory_manager_type_is_struct_default_value;
        template_unused_value = memory_manager_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.malloc);
          field->field_size            = sizeof            (empty.malloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.free);
          field->field_size            = sizeof            (empty.free);

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.calloc);
          field->field_size            = sizeof            (empty.calloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.realloc);
          field->field_size            = sizeof            (empty.realloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*on_oom) (const memory_manager_t *self, size_t      size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.on_oom);
          field->field_size            = sizeof            (empty.on_oom);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*on_err) (const memory_manager_t *self, const char *msg); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.on_err);
          field->field_size            = sizeof            (empty.on_err);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void   *state; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.state);
          field->field_size            = sizeof            (empty.state);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t  state_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.state_size);
          field->field_size            = sizeof            (empty.state_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

/* memory_manager_type_is_struct methods. */

/*
 * Return the default value for a field in "memory_manager_type_t".
 *
 * This function is how we assign a default value for each field in
 * "memory_manager_type_t"
 *
 * In the simplest case, we can choose 0/NULL as the default value for all
 * fields, by just using "default_value_zero".
 *
 * "memory_manager_type_is_struct" returns a "struct_info" where each
 * "field_info"'s "default_value" is set to this function.
 *
 * This function should handle NULL "dest_field_mem"'s, in which case the
 * caller is probably only concerned with whether this field has a default
 * value, and not with what that default value is.
 *
 * (Note: in this case, we could also direct set "default_value" to
 * "default_value_zero" in "memory_manager_type_is_struct" above.)
 *
 * ----------------------------------------------------------------
 *
 * Note: here we provide a low-level implementation allowing us greater control
 * TODO
 *
 * TODO: this doesn't match with memory_manager_defaults!
 */
static size_t memory_manager_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem)
{
  /* We could choose a default value of 0 / NULL for all fields       */
  /* with just the following:                                         */
  /*                                                                  */
  /* > return default_value_zero(self, dest_field_mem);               */
  /*                                                                  */
  /* However, we'll usually want to at least assign a default value   */
  /* to the "typed_t type" field, if the type is "typed" (i.e. values */
  /* are also "tval *"'s, because their first field is "typed_t".     */

  /* ---------------------------------------------------------------- */

  /* If this field lacks a default value, we should just return "0"   */
  /* without writing to "dest_field_mem"; otherwise return the size   */
  /* of the field's type after writing the default value.             */
  /*                                                                  */
  /* (Note: even if a default value is 0 or NULL, if this is a        */
  /* default value, we'll still want to return a non-zero value.      */
  /*                                                                  */
  /* We shouldn't write a (0/NULL) default value and                  */
  /* erroneously return zero,                                         */
  /* incorrectly indicating that "dest_field_mem" was written to.     */

  static const memory_manager_t empty;

  return default_value_from_type(self, dest_field_mem, memory_manager_type, NULL);

  /* TODO */

  /* memory_manager_type_is_struct_default_value must be called with  */
  /* "self".                                                          */
  if (!self)
    return 0;


  /* Which field are we returning a default value for?                */

  /* typed_t type                                                     */
  if      (field_info_cref(self, &empty) == &empty.type)
  {
    typedef typed_t field_type;

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      memory_manager_type;

    return sizeof(field_type);
  }

  /* void *(*malloc) (const memory_manager_t *self, size_t  size);    */
  else if (field_info_cref(self, &empty) == &empty.malloc)
  {
    typedef void *(*field_type) (const memory_manager_t *self, size_t  size);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      NULL;

    return sizeof(field_type);
  }

  /* void  (*free)   (const memory_manager_t *self, void   *ptr);     */
  else if (field_info_cref(self, &empty) == &empty.free)
  {
    typedef void  (*field_type)   (const memory_manager_t *self, void   *ptr);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      NULL;

    return sizeof(field_type);
  }

  /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
  else if (field_info_cref(self, &empty) == &empty.calloc)
  {
    typedef void *(*field_type) (const memory_manager_t *self, size_t  nmemb, size_t size);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      NULL;

    return sizeof(field_type);
  }

  /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
  else if (field_info_cref(self, &empty) == &empty.realloc)
  {
    typedef void *(*field_type)(const memory_manager_t *self, void   *ptr,   size_t size);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      NULL;

    return sizeof(field_type);
  }

  /* void  (*on_oom) (const memory_manager_t *self, size_t      size); */
  else if (field_info_cref(self, &empty) == &empty.on_oom)
  {
    typedef void  (*field_type) (const memory_manager_t *self, size_t      size);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      memory_manager_default_on_oom;

    return sizeof(field_type);
  }

  /* void  (*on_err) (const memory_manager_t *self, const char *msg); */
  else if (field_info_cref(self, &empty) == &empty.on_err)
  {
    typedef void  (*field_type) (const memory_manager_t *self, const char *msg);

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      memory_manager_default_on_err;

    return sizeof(field_type);
  }

  /* void   *state;                                                   */
  else if (field_info_cref(self, &empty) == &empty.state)
  {
    typedef void   *field_type;

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      NULL;

    return sizeof(field_type);
  }

  /* size_t  state_size;                                              */
  else if (field_info_cref(self, &empty) == &empty.state_size)
  {
    typedef size_t  field_type;

    field_type *dest_field = (field_type *) dest_field_mem;

    if (dest_field) *dest_field =
      0;

    return sizeof(field_type);
  }

  /* (Note: an assignment for a field without a default value would   */
  /* look like this):                                                 */
  /*
  /-* foo_t foo *-/
  else if (field_info_cref(self, &empty) == &empty.foo)
  {
    return 0;
  }
  */

  /* General case for all other fields, and unrecognized fields. */
  else
  {
    return default_value_zero(self, dest_field_mem);
  }
}
/*
 * TODO
 */
static size_t memory_manager_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem)
{
  /* TODO */
  return template_unused_value_zero(self, dest_field_mem);
}

/*
 * "memory_manager_type"'s constructor is the standard "template_cons_t" struct.
 *
 * This means a "memory_manager" is initialized from another "memory_manager_t"
 * contained in a "template_cons_t", where the fields are copied, and some
 * fields have default values when the constructor's "memory_manager_t"'s are
 * zero/NULL (or, less commonly, possibly some special value depending on the
 * type's struct_info).
 *
 * Thus "memory_manager_t"'s "init" function must be called with a
 * "template_cons_t *".
 *
 * (Note: we could also have directly assigned "memory_manager_type_def"'s
 * "cons_type" field to "type_uses_template_cons", since we don't do any
 * further initialization.)
 *
 * (Note: "type_uses_template_cons" directly returns "template_cons_type".
 * We could also have defined that directly...
 *
 * > static typed_t memory_manager_type_cons_type(const type_t *self)
 * >   { return template_cons_type; }
 *
 * ...at the sacrifice of modularity.)
 */
static typed_t              memory_manager_type_cons_type  (const type_t *self)
  { return template_cons_type; }

/*
 * The initializer for "memory_manager_type".
 *
 * This is called to initialize an existing "memory_manager_t", and to "malloc"
 * (and also possibly initialize) a new "memory_manager_t *".
 *
 * "memory_manager_type"'s initializer uses a "template_cons_t" constructor.
 * This struct contains another "memory_manager_t" in this case, whose -
 * generally speaking - non-zero fields are copied and whose zero fields are
 * assigned a default value.
 *
 * So to initialize a new "memory_manager_t" with default values except for
 * specific fields, these fields can be assigned a non-zero/non-NULL value.
 *
 * A "template_cons_t" also contains other general information for
 * initialization.  For example, it can specify an alternate method to manage
 * memory, different from the default "malloc", whether to force all fields to
 * be copied rather than providing defaults for fields equal to zero/NULL, and
 * also whether to dynamically "malloc" a new "memory_manager_t *", etc.
 *
 * The most important parts of a "template_cons_t" constructor are these:
 *   - memory_manager_t *dest:
 *       The "memory_manager_t" to initialize.
 *
 *       If NULL, a new one is "malloc"'d.
 *   - memory_manager_t *initials;
 *
 * Example:
 *   > TODO
 */

/*
 * Just use "template_cons_basic_initializer" to initialize our value.
 *
 * (Note: we could also have directly assigned "memory_manager_type_def"'s
 * "init" field to "template_cons_basic_initializer", since we don't do any
 * further initialization.)
 */
static tval                *memory_manager_type_init       (const type_t *self, tval *cons)
  {
    /* Initialize a "memory_manager_t".     */
    memory_tracker_t *val = template_cons_basic_initializer(self, cons);

    /* Some types may want to perform more  */
    /* initialization before returning:     */
    /* if (val)
     * {
     *   /-* ... *-/
     * }
     */

    /* Return the "memory_tracker_t" value. */
    return val;
  }

/*
 * Free an initialized "memory_tracker_t *".
 *
 * Just use "template_cons_basic_freer".
 *
 * (Note: we could also have directly assigned "memory_manager_type_def"'s
 * "free" field to "template_cons_basic_free", since we don't do any
 * further initialization.)
 */
static void                 memory_manager_type_free       (const type_t *self, tval *val)
  {
    template_cons_basic_freer(self, val);
  }

/*
 * TODO
 */
static const tval          *memory_manager_type_has_default(const type_t *self)
  { return &memory_manager_defaults; }

/*
 * TODO
 */
static memory_tracker_t    *memory_manager_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; /* TODO */ }

/*
 * TODO
 */
static tval                *memory_manager_type_dup        ( const type_t *self
                                                           , tval *dest
                                                           , const tval *src
                                                           , int rec_copy
                                                           , int dup_metadata
                                                           , ref_traversal_t *ref_traversal
                                                           )
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

/* ---------------------------------------------------------------- */

const memory_manager_t * const default_memory_manager = &malloc_manager;

static void *malloc_manager_malloc (const memory_manager_t *self, size_t  size);
static void  malloc_manager_free   (const memory_manager_t *self, void   *ptr);
static void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size);
static void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size);
static void  malloc_manager_on_oom (const memory_manager_t *self, size_t      size);
static void  malloc_manager_on_err (const memory_manager_t *self, const char *msg);

const memory_manager_t malloc_manager =
  { memory_manager_type

  , malloc_manager_malloc
  , malloc_manager_free
  , malloc_manager_calloc
  , malloc_manager_realloc

  , malloc_manager_on_oom
  , malloc_manager_on_err

  , NULL
  , 0
  };

static void *malloc_manager_malloc (const memory_manager_t *self, size_t  size)
  { return malloc (size); }
static void  malloc_manager_free   (const memory_manager_t *self, void   *ptr)
  {        free   (ptr); }
static void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size)
  { return calloc (nmemb, size); }
static void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size)
  { return realloc(ptr,   size); }
static void  malloc_manager_on_oom (const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (char *) buf, (size_t) buf_size
    , "*** (EE) Error: malloc_manager_on_oom: Out of memory!  Requested '%d' bytes.\n"
    , (int) size
    );

  buf[buf_size - 1] = 0;
  malloc_manager_on_err(self, buf);
}
static void  malloc_manager_on_err (const memory_manager_t *self, const char *msg)
{
  size_t len;

  fprintf
    ( stderr
    , "malloc_manager_on_err: %s"
    , msg
    );

  /* If "msg" lacks a trailing newline character, add one. */
  len = strlen(msg);
  if (len >= 1 && msg[len - 1] != '\n')
  {
    fprintf
      ( stderr
      , "\n"
      );
  }
}

/* ---------------------------------------------------------------- */

void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (char *) buf, (size_t) buf_size
    , "*** (EE) Error: memory_manager_default_on_oom: Out of memory!  Requested '%d' bytes.\n"
    , (int) size
    );

  buf[buf_size - 1] = 0;
  malloc_manager_on_err(self, buf);
}

void memory_manager_default_on_err(const memory_manager_t *self, const char *msg)
{
  size_t len;

  fprintf
    ( stderr
    , "malloc_manager_on_err: %s"
    , msg
    );

  /* If "msg" lacks a trailing newline character, add one. */
  len = strlen(msg);
  if (len >= 1 && msg[len - 1] != '\n')
  {
    fprintf
      ( stderr
      , "\n"
      );
  }
}

void on_memory_manager_oom_do_nothing(const memory_manager_t *self, size_t      size)
{
}

void on_memory_manager_err_do_nothing(const memory_manager_t *self, const char *msg)
{
}

/* ---------------------------------------------------------------- */

const memory_manager_t memory_manager_defaults =
  { memory_manager_type

  , NULL
  , NULL
  , NULL
  , NULL

  , memory_manager_default_on_oom
  , memory_manager_default_on_err

  , NULL
  , 0
  };

/* ---------------------------------------------------------------- */

void *memory_manager_malloc(const memory_manager_t *memory_manager, size_t size)
{
  void *mem;

  if (!memory_manager || !memory_manager->malloc)
    memory_manager = &malloc_manager;

  if (!memory_manager->malloc || !memory_manager->free)
  {
    if (memory_manager->malloc)
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"malloc\" without \"free\"!"
        );
    else
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"free\" without \"malloc\"!"
        );

    return NULL;
  }

  mem = memory_manager->malloc(memory_manager, size);

  if (!mem)
    memory_manager_on_oom(memory_manager, size);

  return mem;
}

void memory_manager_free(const memory_manager_t *memory_manager, void *ptr)
{
  if (!memory_manager || !memory_manager->free)
    memory_manager = &malloc_manager;

  if (!memory_manager->malloc || !memory_manager->free)
  {
    if (memory_manager->malloc)
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"malloc\" without \"free\"!"
        );
    else
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"free\" without \"malloc\"!"
        );

    return NULL;
  }

  memory_manager->free(memory_manager, ptr);
}

void *memory_manager_calloc(const memory_manager_t *memory_manager, size_t nmemb, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->calloc)
  {
    return memory_manager->calloc(memory_manager, nmemb, size);
  }
  else
  {
    size_t total_size;

    total_size = nmemb * size;

    return memory_manager_malloc(memory_manager, total_size);
  }
}

void *memory_manager_realloc(const memory_manager_t *memory_manager, void *ptr, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->realloc)
  {
    return memory_manager->realloc(memory_manager, ptr, size);
  }
  else
  {
    if (!memory_manager->malloc || !memory_manager->free)
      memory_manager = &malloc_manager;

    memory_manager_free(memory_manager, ptr);

    return memory_manager_malloc(memory_manager, size);
  }
}

void  memory_manager_on_oom (const memory_manager_t *memory_manager, size_t      size)
{
  void (*on_oom)(const memory_manager_t *self, size_t size);

  if (!memory_manager || !memory_manager->on_oom)
    memory_manager = &malloc_manager;

  on_oom = memory_manager->on_oom;
  if (!on_oom)
    on_oom = memory_manager_default_on_oom;

  on_oom(memory_manager, size);
}

void  memory_manager_on_err (const memory_manager_t *memory_manager, const char *msg)
{
  void (*on_err)(const memory_manager_t *self, size_t size);

  if (!memory_manager || !memory_manager->on_err)
    memory_manager = &malloc_manager;

  on_err = memory_manager->on_err;
  if (!on_err)
    on_err = memory_manager_default_on_err;

  on_err(memory_manager, msg);
}

/* ---------------------------------------------------------------- */

/* memory_tracker type. */

static const type_t        *memory_tracker_type_typed      (const type_t *self);
static const char          *memory_tracker_type_name       (const type_t *self);
static const char          *memory_tracker_type_info       (const type_t *self);
static size_t               memory_tracker_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_tracker_type_is_struct  (const type_t *self);
static typed_t              memory_tracker_type_cons_type  (const type_t *self);
static tval                *memory_tracker_type_init       (const type_t *self, tval *cons);
static void                 memory_tracker_type_free       (const type_t *self, tval *val);
static const tval          *memory_tracker_type_has_default(const type_t *self);
static memory_tracker_t    *memory_tracker_type_mem        (const type_t *self, tval *val_raw);
static tval                *memory_tracker_type_dup        ( const type_t *self
                                                           , tval *dest
                                                           , const tval *src
                                                           , int rec_copy
                                                           , int dup_metadata
                                                           , ref_traversal_t *ref_traversal
                                                           );

static size_t memory_tracker_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t memory_tracker_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

const type_t memory_tracker_type_def =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ memory_tracker_type_typed

  , /* name        */ memory_tracker_type_name
  , /* info        */ memory_tracker_type_info
  , /* size        */ memory_tracker_type_size
  , /* is_struct   */ memory_tracker_type_is_struct

  , /* cons_type   */ memory_tracker_type_cons_type
  , /* init        */ memory_tracker_type_init
  , /* free        */ memory_tracker_type_free
  , /* has_default */ memory_tracker_type_has_default
  , /* mem         */ memory_tracker_type_mem
  , /* dup         */ memory_tracker_type_dup

  , /* parity      */ ""
  };

const type_t *memory_tracker_type(void)
  { return &memory_tracker_type_def; }

static const type_t        *memory_tracker_type_typed      (const type_t *self)
  { return type_is_typed(self); }

static const char          *memory_tracker_type_name       (const type_t *self)
  { return "memory_tracker"; }

static const char          *memory_tracker_type_info       (const type_t *self)
  { return "typedef struct memory_tracker_s memory_tracker_t"; }

static size_t               memory_tracker_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *memory_tracker_type_is_struct  (const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const memory_tracker_t  empty;

        field_info_t           *field;
        size_t                  fields_len;

        size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = memory_tracker_type_is_struct_default_value;
        template_unused_value = memory_tracker_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_manager_t memory_manager; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory_manager);
          field->field_size            = sizeof            (empty.memory_manager);
          field->field_type            = memory_manager_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *dynamically_allocated_container; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_container);
          field->field_size            = sizeof            (empty.dynamically_allocated_container);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void   **dynamically_allocated_buffers; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_num; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_num);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_num);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_size);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_last_even; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_last_even);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_last_even);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_last_odd; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_last_odd);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_last_odd);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

static typed_t              memory_tracker_type_cons_type  (const type_t *self)
  { return template_cons_type; }

static tval                *memory_tracker_type_init       (const type_t *self, tval *cons)
  {
    static memory_tracker_t *defaults = NULL;
    if (!defaults)
    {
      static memory_tracker_t type_defaults;

      type_defaults = memory_tracker_default;

      defaults = &type_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 memory_tracker_type_free       (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

/*
 * TODO
 */
static const tval          *memory_tracker_type_has_default(const type_t *self)
  { return &memory_manager_defaults; }

/*
 * TODO
 *
 * TODO: uhh, get it from the struct??
 *{ return (val_raw ? ((memory_manager_t *) val_raw)->memory : NULL); }
 */
static memory_tracker_t    *memory_tracker_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; }

static tval                *memory_tracker_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

/* ---------------------------------------------------------------- */

/*
 * Default memory tracker:
 *   - Default memory manager.
 *   - Associated value not allocated on heap.
 *   - No allocated buffers to track dynamically allocated memory.
 */
const memory_tracker_t memory_tracker_defaults =
  { memory_tracker_type

  /* memory_manager_defaults */
  , /* memory_manager                          */ { memory_manager_type }

  , /* dynamically_allocated_container         */ NULL
  , /* dynamically_allocated_buffers           */ NULL
  , /* dynamically_allocated_buffers_num       */ 0
  , /* dynamically_allocated_buffers_size      */ 0
  , /* dynamically_allocated_buffers_last_even */ 0
  , /* dynamically_allocated_buffers_last_odd  */ 0
  };

const char *memory_tracker_initialize_no_buffers(memory_tracker_t *memory_tracker, const memory_manager_t *memory_manager, void *dynamically_allocated_container)
{
  if (!memory_tracker)
  {
    return "error: memory_tracker_no_buffers: "memory_tracker" is NULL!\n"
  }

  if (!memory_manager)
    memory_manager = default_memory_manager;

  memory_tracker->memory_manager                          = memory_manager;
  memory_tracker->dynamically_allocated_container         = dynamically_allocated_container;
  memory_tracker->dynamically_allocated_buffers           = NULL
  memory_tracker->dynamically_allocated_buffers_num       = 0;
  memory_tracker->dynamically_allocated_buffers_size      = 0;
  memory_tracker->dynamically_allocated_buffers_last_even = 0;
  memory_tracker->dynamically_allocated_buffers_last_odd  = 0;

  return "";
}

/* ---------------------------------------------------------------- */
/* struct_info_t and field_info_t                                   */
/* ---------------------------------------------------------------- */

/* field_info type. */

static const type_t        *field_info_type_typed      (const type_t *self);
static const char          *field_info_type_name       (const type_t *self);
static const char          *field_info_type_info       (const type_t *self);
static size_t               field_info_type_size       (const type_t *self, const tval *val);
static const struct_info_t *field_info_type_is_struct  (const type_t *self);
static typed_t              field_info_type_cons_type  (const type_t *self);
static tval                *field_info_type_init       (const type_t *self, tval *cons);
static void                 field_info_type_free       (const type_t *self, tval *val);
static const tval          *field_info_type_has_default(const type_t *self);
static memory_tracker_t    *field_info_type_mem        (const type_t *self, tval *val_raw);
static tval                *field_info_type_dup        ( const type_t *self
                                                       , tval *dest
                                                       , const tval *src
                                                       , int rec_copy
                                                       , int dup_metadata
                                                       , ref_traversal_t *ref_traversal
                                                       );

static size_t field_info_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t field_info_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

const type_t field_info_type_def =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ field_info_type_typed

  , /* name        */ field_info_type_name
  , /* info        */ field_info_type_info
  , /* size        */ field_info_type_size
  , /* is_struct   */ field_info_type_is_struct

  , /* cons_type   */ field_info_type_cons_type
  , /* init        */ field_info_type_init
  , /* free        */ field_info_type_free
  , /* has_default */ field_info_type_has_default
  , /* mem         */ field_info_type_mem
  , /* dup         */ field_info_type_dup

  , /* parity      */ ""
  };

const type_t *field_info_type(void)
  { return &field_info_type_def; }

static const type_t        *field_info_type_typed      (const type_t *self)
  { return type_is_typed(self); }

static const char          *field_info_type_name       (const type_t *self)
  { return "field_info"; }

static const char          *field_info_type_info       (const type_t *self)
  { return "typedef struct field_info_s field_info_t"; }

static size_t               field_info_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *field_info_type_is_struct  (const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const field_info_t      empty;

        field_info_t           *field;
        size_t                  fields_len;

        size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = field_info_type_is_struct_default_value;
        template_unused_value = field_info_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* ptrdiff_t     field_pos; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_pos);
          field->field_size            = sizeof            (empty.field_pos);
          field->field_type            = ptrdiff_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t        field_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_size);
          field->field_size            = sizeof            (empty.field_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const type_t *field_type; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_type);
          field->field_size            = sizeof            (empty.field_type);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int           is_metadata; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_metadata);
          field->field_size            = sizeof            (empty.is_metadata);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int           is_copyable_ref; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_copyable_ref);
          field->field_size            = sizeof            (empty.is_copyable_ref);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t (*write_default)(const field_info_t *self, void *dest_mem); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.write_default);
          field->field_size            = sizeof            (empty.write_default);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t (*write_template_unused)(const field_info_t *self, void *dest_mem); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.write_template_unused);
          field->field_size            = sizeof            (empty.write_template_unused);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

static typed_t              field_info_type_cons_type  (const type_t *self)
  { return template_cons_type; }

static tval                *field_info_type_init       (const type_t *self, tval *cons)
  {
    static field_info_t *defaults = NULL;
    if (!defaults)
    {
      static field_info_t field_info_defaults;

      field_info_defaults = default_field_info;

      defaults = &field_info_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 field_info_type_free       (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static const tval          *field_info_type_has_default(const type_t *self)
  { return &field_info_defaults; }

/* TODO */
static memory_tracker_t    *field_info_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; }

static tval                *field_info_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

const field_info_t field_info_defaults =
  { field_info_type

  , /* field_pos             */ (ptrdiff_t) 0
  , /* field_size            */ (size_t)    0
  , /* field_type            */ NULL

  , /* is_metadata           */ 0
  , /* is_copyable_ref       */ 0

  , /* default_value         */ default_value_zero
  , /* template_unused_value */ template_unused_value_zero
  };

/* The field has a default value of that represented by 0 bytes. */
size_t default_value_zero(const field_info_t *self, void *dest_mem)
{
  size_t size = self->field_size;

  if (dest_mem)
    memset(dest_mem, 0, size);

  return size;
}

/* The field has an unused value of that represented by 0 bytes. */
size_t template_unused_value_zero(const field_info_t *self, void *dest_mem)
{
  size_t size = self->field_size;

  if (dest_mem)
    memset(dest_mem, 0, size);

  return size;
}

/* The field has no special unused value. */
size_t template_no_unused_value(const field_info_t *self, void *dest_mem)
{
  return 0;
}

int is_field_terminator(const field_info_t *field_info)
{
  if (!field_info || !terminating_field_info)
    return -1;

  if (*field_info == *terminating_field_info)
    return 1;
  else
    return 0;
}

void       *field_info_ref (const field_info_t *field_info,  void       *val)
{
  if (!field_info || !base)
    return NULL;

  return field_ref(field_info->field_pos, base);
}

const void *field_info_cref(const field_info_t *field_info,  const void *val)
{
  if (!field_info || !base)
    return NULL;

  return field_cref(field_info->field_pos, base);
}

int field_memcmp(const field_info_t *field_info, const void *s1, const void *s2)
{
  size_t i;
  size_t size = self->field_size;

  if (!s1)
    return FIELD_MEMCMP_ERR_NULL_S1;
  if (!s2)
    return FIELD_MEMCMP_ERR_NULL_S2;

  for (i = 0; i < size; ++i)
  {
    unsigned char b1 = ((const unsigned char *) s1)[i];
    unsigned char b2 = ((const unsigned char *) s2)[i];

    if      (b1 > b2)
    {
      int i1         = (int) b1;
      int i2         = (int) b2;

      int difference = i2 - i1;

      if (difference <= 0)
        difference = 1;

      return difference;
    }
    else if (b1 < b2)
    {
      int i1         = (int) b1;
      int i2         = (int) b2;

      int difference = i2 - i1;

      if (difference >= 0)
        difference = -1;

      return difference;
    }
  }

  return 0;
}

void *field_memcpy(const field_info_t *field_info, void *dest, const void *src)
{
  if (!dest)
    return NULL;
  if (!src)
    return NULL;

  return memcpy(field_info_ref(field_info, dest), field_info_cref(field_info, src), field_info->field_size);
}

void *field_memmove(const field_info_t *field_info, void *dest, const void *src)
{
  if (!dest)
    return NULL;
  if (!src)
    return NULL;

  return memmove(field_info_ref(field_info, dest), field_info_cref(field_info, src), field_info->field_size);
}

int is_field_template_unused(const field_info_t *field_info, const void *src_field_mem, void *field_working_mem, const memory_manager_t *memory_manager, void *out_field_default_mem)
{
  int      is_unused;

  int      need_free = 0;
  size_t   has_unused;
  size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

  if (!field_info)
    return -4;
  if (!src_field_mem)
    return -2;

  /* Allocate resources. */
  {
    need_free = 0;
    if (!field_working_mem)
    {
      need_free = 1;

      if (!memory_manager)
        memory_manager = malloc_manager;

      field_working_mem = memory_manager_malloc(memory_manager, field_info->field_size());

      if (!field_working_mem)
        return -1;
    }
  }
  /* Body: use resources. */
  {
    /* Read in the field's special unused value, to "field_working_mem". */
    template_unused_value = field_info->template_unused_value;
    if (!template_unused_value)
    {
      template_unused_value = field_info_defaults.template_unused_value;
      if (!template_unused_value)
        template_unused_value = template_unused_value_zero;
    }

    has_unused = template_unused_value(field_info, field_working_mem);

    /* Does the field have a special unused value for equality checking? */
    if (!has_unused)
    {
      /* No; we'll just return 0. */
      is_unused = 0;
    }
    else
    {
      /* Yes.  Is "src" equal to this special unused value? */
      if (field_memcmp(field_info, src_field_mem, field_working_mem) != 0)
      {
        /* No; we'll just return 0. */
        is_unused = 0;
      }
      else
      {
        /* Yes.  We'll return >= 1, and output the default value if an output
         * buffer is provided.
         */
        if (!out_field_default_mem)
          is_unused = 1;

        if (out_field_default_mem)
        {
          /* Write the field's default value to "out_field_default_mem". */

          size_t (*default_value)(const field_info_t *self, void *dest_mem);

          is_unused = 2;

          default_value = field_info->default_value;
          if (!default_value)
          {
            default_value = field_info_defaults.default_value;
            if (!default_value)
              default_value = default_value_zero;
          }

          default_value(field_info, out_field_default_mem);

          /* We've written to "out_field_default_mem", so we can't write */
          /* to "field_working_mem" anymore in case it overlaps with     */
          /* "out_field_default_mem".                                    */
          if (need_free)
          {
            need_free = 0;
            memory_manager_free(memory_manager, field_working_mem);
          }
          field_working_mem = NULL;
        }
      }
    }
  }
  /* Free resources. */
  if (need_free)
  {
    memory_manager_free(memory_manager, field_working_mem);
  }

  return is_unused;
}

verify_field_info_status_t verify_field_info(const field_info *field_info, char *out_err, size_t err_size)
{
  size_t field_type_size;

  if (!field_info)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_info\" is NULL.\n"
        );
    return verify_field_info_null_field_info;
  }

  /* Ensure a type is present. */
  if (!field_info->type)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_info\" lacks a type!\n"
        );
    return verify_field_info_no_type;
  }

  /* Ensure the type's size matches field_size. */
  field_type_size = type_size(field_type);
  if (field_info->field_size != field_type_size)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_size\" is different from the type's size!!\n"
        , "  field_info->field_size:      %d\n"
        , "  type_size(field_info->type): %d\n"
        , (int) field_info->field_size
        , (int) field_type_size
        );
    return verify_field_info_size_mismatch;
  }

  /* The conditions we checked for passed. */
  return verify_field_info_success;
}

/* NULL on success. */
/* TODO: memory_manager! */
const char *field_dup(const field_info_t *field_info, void *dest, const void *src, int force_no_defaults, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
{
  static char         err_buf[DEFAULT_ERR_BUF_SIZE];
  static const size_t err_buf_size = sizeof(err_buf);
  static const size_t err_buf_num  = sizeof(err_buf) / sizeof(err_buf[0]);

  size_t i;

  verify_field_info_status_t verify_status;

  /* TODO: We're verifying this each time a field is dup'd! */
  verify_status = verify_field_info(field_info, err_buf, err_buf_size, NULL);
  if (!verify_status)
    return err_buf;

  if (!dest)
    return "Error: field_dup: \"dest\" is NULL.\n";
  if (!src)
    return "Error: field_dup: \"src\" is NULL.\n";

  if (field_info->is_metadata)
  {
    if (!dup_metadata)
      return NULL;
  }

  if (field_info->is_copyable_ref && rec_copy != 0)
  {
    tval *type_dup_status;

    if (rec_copy < 0)
      ++rec_copy;

    type_dup_status =
      type_dup
        ( field_info->type
        , field_info_ref (field_info, dest)
        , field_info_cref(field_info, src)
        , rec_copy
        , dup_metadata
        , NULL
        );

    if (!type_dup_status)
    {
      return "Error: field_dup: recursively copying field failed!  \"type_dup\" returned NULL.\n";
    }

    return NULL;
  }
  else
  {
    void *field_memmove_status;

    if (!force_no_defaults)
    {
      int wrote_default;

      wrote_default = is_field_template_unused(field_info, src, dest, NULL, dest);

      if (wrote_default < 0)
      {
        snprintf
          ( (char *) err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: field_dup: \"is_field_template_unused\" failed!\n"
            "  \"is_field_template_unused\" return error code: %d\n"
          , (int) wrote_default
          );

        return err_buf;
      }

      if (wrote_default)
      {
        return NULL;
      }
    }

    field_memmove_status = field_memmove(field_info, dest, src);
    if (!field_memmove_status)
    {
      return "Error: field_dup: field_memcpy returned NULL, indicating failure.\n";
    }

    /* Done. */
    return NULL;
  }
}

/* ---------------------------------------------------------------- */

/* struct_info type. */

static const type_t        *struct_info_type_typed      (const type_t *self);
static const char          *struct_info_type_name       (const type_t *self);
static const char          *struct_info_type_info       (const type_t *self);
static size_t               struct_info_type_size       (const type_t *self, const tval *val);
static const struct_info_t *struct_info_type_is_struct  (const type_t *self);
static typed_t              struct_info_type_cons_type  (const type_t *self);
static tval                *struct_info_type_init       (const type_t *self, tval *cons);
static void                 struct_info_type_free       (const type_t *self, tval *val);
static const tval          *struct_info_type_has_default(const type_t *self);
static memory_tracker_t    *struct_info_type_mem        (const type_t *self, tval *val_raw);
static tval                *struct_info_type_dup        ( const type_t *self
                                                        , tval *dest
                                                        , const tval *src
                                                        , int rec_copy
                                                        , int dup_metadata
                                                        , ref_traversal_t *ref_traversal
                                                        );

static size_t struct_info_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t struct_info_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

const type_t struct_info_type_def =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ struct_info_type_typed

  , /* name        */ struct_info_type_name
  , /* info        */ struct_info_type_info
  , /* size        */ struct_info_type_size
  , /* is_struct   */ struct_info_type_is_struct

  , /* cons_type   */ struct_info_type_cons_type
  , /* init        */ struct_info_type_init
  , /* free        */ struct_info_type_free
  , /* has_default */ struct_info_type_has_default
  , /* mem         */ struct_info_type_mem
  , /* dup         */ struct_info_type_dup

  , /* parity      */ ""
  };

const type_t *struct_info_type(void)
  { return &struct_info_type_def; }

static const type_t        *struct_info_type_typed      (const type_t *self)
  { return type_is_typed(self); }

static const char          *struct_info_type_name       (const type_t *self)
  { return "struct_info"; }

static const char          *struct_info_type_info       (const type_t *self)
  { return "typedef struct struct_info_s struct_info_t"; }

static size_t               struct_info_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *struct_info_type_is_struct  (const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const struct_info_t     empty;

        field_info_t           *field;
        size_t                  fields_len;

        size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = struct_info_type_is_struct_default_value;
        template_unused_value = struct_info_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* field_info_t fields[STRUCT_INFO_NUM_FIELDS]; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.fields);
          field->field_size            = sizeof            (empty.fields);
          field->field_type            = array_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t       fields_len; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.fields_len);
          field->field_size            = sizeof            (empty.fields_len);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* struct_info_t *tail; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.tail);
          field->field_size            = sizeof            (empty.tail);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int       has_memory_tracker; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.has_memory_tracker);
          field->field_size            = sizeof            (empty.has_memory_tracker);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t    memory_tracker_field; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory_tracker_field);
          field->field_size            = sizeof            (empty.memory_tracker_field);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

static typed_t              struct_info_type_cons_type  (const type_t *self)
  { return template_cons_type; }

static tval                *struct_info_type_init       (const type_t *self, tval *cons)
  {
    static struct_info_t *defaults = NULL;
    if (!defaults)
    {
      static struct_info_t struct_info_defaults;

      struct_info_defaults = default_struct_info;

      defaults = &struct_info_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 struct_info_type_free       (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static const tval          *struct_info_type_has_default(const type_t *self)
  { return &struct_info_defaults; }

/*
 * TODO
 */
static memory_tracker_t    *struct_info_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; /* TODO */ }

static memory_tracker_t    *struct_info_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

const struct_info_t struct_info_defaults =
  { struct_info_type

  , { { field_info_type, (size_t) (-1) } }
  , 0

  , NULL

  , 0
  , 0
  };

const field_info_t * const field_terminator = &terminating_field_info;

const field_info_t terminating_field_info =
  { field_info_type

  , (size_t) (-1)
  , 0
  , NULL /* TODO: void_type? */

  , 0
  , NULL
  };

/* ---------------------------------------------------------------- */

memory_tracker_t *struct_value_has_memory_tracker(const struct_info_t *struct_info, void *src_mem)
{
  if (!struct_info || !src_mem)
    return NULL;

  if (struct_info->has_memory_tracker)
  {
    const field_info_t *memory_tracker_field = struct_info->fields[struct_info->memory_tracker_field];

    memory_tracker_t *memory_tracker;

    memory_tracker = (memory_tracker_t *) field_cref(memory_tracker_field->field_pos, src_mem);

    return memory_tracker;
  }
  else
  {
    return NULL;
  }
}

verify_struct_info_status_t verify_struct_info(const struct_info_t *struct_info, char *out_err, size_t err_size, ref_traversal_t *structs_checked)
{
  size_t i;
  size_t num_fields;

  if (!struct_info)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: \"struct_info\" is NULL.\n"
        );
    return verify_struct_info_null_struct_info;
  }

  num_fields = struct_info->fields_len;

  /* Make sure fields_len isn't too big.   */
  if (num_fields >  STRUCT_INFO_NUM_FIELDS)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: fields_len is too big!  Exceeds STRUCT_INFO_NUM_FIELDS capacity.\n"
          "  fields_len:             %d\n"
          "  STRUCT_INFO_NUM_FIELDS: %d\n"
        , struct_info->fields_len
        , STRUCT_INFO_NUM_FIELDS
        );
    return verify_struct_info_need_tail_max;
  }

  /* Make sure fields_len isn't too big.   */
  /*                                       */
  /* Ideally, struct_infos would still be  */
  /* terminated at max capacity before its */
  /* tail is filled, but so long as it has */
  /* a tail, we'll permit this.            */
  /*                                       */
  /* We'll still verify tails, so          */
  /* ultimately there still must be a      */
  /* field terminator.                     */
  if (num_fields >= STRUCT_INFO_NUM_FIELDS && !struct_info->tail)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: fields is not terminated with field_terminator!\n"
          "  fields is at maximum capacity, but there is no tail, so there is no space\n"
          "  for a field_terminator.\n"
          "\n"
          "  fields_len:             %d\n"
          "  STRUCT_INFO_NUM_FIELDS: %d\n"
          "  tail is NULL.\n"
        , struct_info->fields_len
        , STRUCT_INFO_NUM_FIELDS
        );
    return verify_struct_info_need_tail_excess;
  }

  /* Make sure fields is field-terminated. */
  if (num_fields < STRUCT_INFO_NUM_FIELDS)
  {
    int terminated;

    terminated = is_field_terminator(&struct_info->fields[num_fields]);
    if (terminated < 0)
    {
      /* Something went wrong. */
      if (out_err)
        snprintf
          ( (char *) out_err, (size_t) terminator_size(err_size)
          , "Error: verify_struct_info: verifying fields is field-terminated failed!\n"
            "  \"is_field_terminator\" failed with a general error code: %d\n"
            "\n"
            "  fields_len:             %d\n"
            "  STRUCT_INFO_NUM_FIELDS: %d\n"
          , terminated
          , (int) struct_info->fields_len
          , (int) STRUCT_INFO_NUM_FIELDS
          );
      return verify_struct_info_is_field_terminator_error;
    }

    if (!terminated)
    {
      /* A field_terminator does not occur at the end. */
      if (out_err)
        snprintf
          ( (char *) out_err, (size_t) terminator_size(err_size)
          , "Error: verify_struct_info: the field array does not end with field_terminator!\n"
            "  A struct_info's array of fields must be immediately followed by \"field_terminator\".\n"
            "  \"field_terminator\" helps ensure that a \"struct_info\" is initialized correctly, with\n"
            "  a correct length value.\n"
            "\n"
            "  Was fields_len set to the correct length?\n"
            "\n"
            "  fields_len:             %d\n"
            "  STRUCT_INFO_NUM_FIELDS: %d\n"
          , (int) struct_info->fields_len
          , (int) STRUCT_INFO_NUM_FIELDS
          );
      return verify_struct_info_is_field_terminator_error;
    }
  }

  /* Verify each field. */
  for (i = 0; i < num_fields; ++i)
  {
    static const char           prefix[]    =
      "Error: verify_struct_info: verification of a field failed!\n"
      "  \"verify_field_info\" failed.  Its error message is provided below:\n"
      "\n"
      "----------------------------------------------------------------\n"
      ;
    static const size_t         prefix_size = sizeof(prefix);
    size_t                      prefix_len;

    verify_field_info_status_t field_status;

    prefix_len = strlen(prefix);

    if (out_err)
      field_status = verify_field_info(&struct_info->fields[i], out_err + prefix_len, size_minus(err_size, prefix_len));
    else
      field_status = verify_field_info(&struct_info->fields[i], out_err, err_size);
    if (field_status != verify_field_info_success)
    {
      verify_struct_info_status_t status;

      status =
          verify_struct_info_verify_field_info_error
        | (field_status << verify_struct_info_bits)
        ;

      if (out_err)
      {
        int    width;
        size_t err_len;

        /* Write prefix.  */
        memcpy(out_err, prefix, min_size(prefix_len, terminator_size(err_size)));

        /* verify_field_info already wrote its error message. */

        /* Append suffix. */
        err_len = strlen(out_err);

        /* (Get width.) */
        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , "%d"
          , max_int((int) i, max_int((int) struct_info->fields_len, (int) STRUCT_INFO_NUM_FIELDS))
          );
        if (terminator_size(size_minus(err_size, err_len)) > 0)
          width = strlen(out_err + err_len);
        else
          width = 0;
        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , ""
          );

        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , "----------------------------------------------------------------\n"
            "\n"
            "  field number  - - - - - - - - - - - - - - - : %*d\n"
            "  field numbers total - - - - - - - - - - - - : %*d\n"
            "  STRUCT_INFO_NUM_FIELDS  - - - - - - - - - - : %*d\n"
            "  verify_field_info error code  - - - - - - - : %d\n"
            "  verify_struct_info_verify_field_info_error  : %d\n"
            "  verify_struct_info_bits - - - - - - - - - - : %d\n"
            "  verify_struct_info returning with error code: %d\n"

          , (int) width
          , (int) i

          , (int) width
          , (int) struct_info->fields_len

          , (int) width
          , (int) STRUCT_INFO_NUM_FIELDS

          , (int) field_status
          , (int) verify_struct_info_verify_field_info_error
          , (int) verify_struct_info_bits
          , (int) status
          );
      }

      return status;
    }
  }

  /* If the "struct_info" has a tail, recursively verify it. */
  /*                                                         */
  /* This is particular important when "fields" is,          */
  /* non-ideally, terminated and is it max capacity.         */
  if (struct_info->tail)
  {
    return verify_struct_info(struct_info, out_err, err_size, NULL);
  }

  /* The conditions we checked for passed. */
  return verify_struct_info_success;
}

/* NULL on success. */
const char *struct_dup(const struct_info_t *struct_info, void *dest, const void *src, int force_no_defaults, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
{
  static char         err_buf[DEFAULT_ERR_BUF_SIZE];
  static const size_t err_buf_size = sizeof(err_buf);
  static const size_t err_buf_num  = sizeof(err_buf) / sizeof(err_buf[0]);

  size_t i;

  verify_struct_info_status_t verify_status;

  /* TODO: We're verifying this each time a struct is dup'd! */
  verify_status = verify_struct_info(struct_info, err_buf, err_buf_size, NULL);
  if (!verify_status)
  {
    return err_buf;
  }

  /* Copy each field. */
  for (i = 0; i < struct_info->fields_len; ++i)
  {
    const char *field_status;

    field_error_status = field_dup(&struct_info->fields[i], dest, src, force_no_defaults, rec_copy, dup_metadata, NULL);

    if (field_error_status)
      return field_error_status;
  }

  /* Recurse. */
  if (struct_info->tail)
  {
    return struct_dup(struct_info->tail, dest, src, force_no_defaults, rec_copy, dup_metadata, NULL);
  }

  /* Done. */
  return NULL;
}

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/* type type. */

static const type_t        *type_type_typed      (const type_t *self);
static const char          *type_type_name       (const type_t *self);
static const char          *type_type_info       (const type_t *self);
static size_t               type_type_size       (const type_t *self, const tval *val);
static const struct_info_t *type_type_is_struct  (const type_t *self);
static typed_t              type_type_cons_type  (const type_t *self);
static tval                *type_type_init       (const type_t *self, tval *cons);
static void                 type_type_free       (const type_t *self, tval *val);
static const tval          *type_type_has_default(const type_t *self);
static memory_tracker_t    *type_type_mem        (const type_t *self, tval *val_raw);
static tval                *type_type_dup        ( const type_t *self
                                                 , tval *dest
                                                 , const tval *src
                                                 , int rec_copy
                                                 , int dup_metadata
                                                 , ref_traversal_t *ref_traversal
                                                 );

static size_t type_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t type_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

const type_t type_type_def =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ type_type_typed

  , /* name        */ type_type_name
  , /* info        */ type_type_info
  , /* size        */ type_type_size
  , /* is_struct   */ type_type_is_struct

  , /* cons_type   */ type_type_cons_type
  , /* init        */ type_type_init
  , /* free        */ type_type_free
  , /* has_default */ type_type_has_default
  , /* mem         */ type_type_mem
  , /* dup         */ type_type_dup

  , /* parity      */ ""
  };

const type_t *type_type(void)
  { return &type_type_def; }

static const type_t        *type_type_typed      (const type_t *self)
  { return type_is_typed(self); }
static const char          *type_type_name       (const type_t *self)
  { return "type"; }

static const char          *type_type_info       (const type_t *self)
  { return "typedef struct type_s type_t"; }

static size_t               type_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *type_type_is_struct  (const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const type_t            empty;
        size_t                  memory_tracker_field;

        field_info_t           *field;
        size_t                  fields_len;

        size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = type_type_is_struct_default_value;
        template_unused_value = type_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_tracker_t memory; */
          memory_tracker_field = fields_len;
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory);
          field->field_size            = sizeof            (empty.memory);
          field->field_type            = memory_tracker_type();

          field->is_metadata           = 1;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const type_t        *(*typed)    (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.typed);
          field->field_size            = sizeof            (empty.typed);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char          *(*name)     (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.name);
          field->field_size            = sizeof            (empty.name);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char          *(*info)     (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.info);
          field->field_size            = sizeof            (empty.info);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t               (*size)     (const type_t *self, const tval *val); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.size);
          field->field_size            = sizeof            (empty.size);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const struct_info_t *(*is_struct)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_struct);
          field->field_size            = sizeof            (empty.is_struct);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* typed_t              (*cons_type)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.cons_type);
          field->field_size            = sizeof            (empty.cons_type);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* tval                *(*init)     (const type_t *self, tval *cons); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.init);
          field->field_size            = sizeof            (empty.init);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void                 (*free)     (const type_t *self, tval *val); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.free);
          field->field_size            = sizeof            (empty.free);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const tval          *(*has_default)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.has_default);
          field->field_size            = sizeof            (empty.has_default);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.mem);
          field->field_size            = sizeof            (empty.mem);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* tval                *(*dup)        ( const type_t *self             */
          /*                                    , tval *dest                     */
          /*                                    , const tval *src                */
          /*                                    , int rec_copy                   */
          /*                                    , int dup_metadata               */
          /*                                    , ref_traversal_t *ref_traversal */
          /*                                    );                               */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dup);
          field->field_size            = sizeof            (empty.dup);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char *parity; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.parity);
          field->field_size            = sizeof            (empty.parity);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_copyable_ref       = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* Has memory tracker field. */
        struct_info_def.has_memory_tracker   = 1;
        struct_info_def.memory_tracker_field = memory_tracker_field;
      }
    }

    return struct_info;
  }

static typed_t              type_type_cons_type  (const type_t *self)
  { return template_cons_type; }

static tval                *type_type_init       (const type_t *self, tval *cons)
  {
    static type_t *defaults = NULL;
    if (!defaults)
    {
      static type_t type_defaults;

      type_defaults = default_type;

      defaults = &type_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 type_type_free       (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static const tval          *type_type_has_default(const type_t *self)
  { return &type_defaults; }

/*
 * TODO
 */
static memory_tracker_t    *type_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; /* TODO */ }

static tval                *type_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

/* ---------------------------------------------------------------- */
/* type_t: Common abstractions.                                     */
/* ---------------------------------------------------------------- */

/* ---------------------------------------------------------------- */
/* type_t: Defaults.                                                */
/* ---------------------------------------------------------------- */

/*
 * Default fields for types.
 *
 * The following fields are required for each type:
 *  - name      (returns const char *):
 *      The name of the type.
 *
 *      Example: "memory_manager".
 *
 *  - info      (returns const char *):
 *      An arbitrary string associated with the type.
 *
 *      Example: "typedef struct memory_manager_s memory_manager_t".
 *
 *  - size      (returns size_t):
 *      The size of the type.
 *
 *      Example: "sizeof(memory_manager_t)".
 *
 *  - is_struct (returns const strruct_info_t *):
 *      The fields of the type's struct, or NULL if the type isn't a struct.
 *
 * The remaining default values specify a type that has the following
 * characteristics:
 *
 *   - typed:
 *       By default, yes: value pointers of this type are assumed to be
 *       "tval *"'s.
 *
 *       Default: "type_is_typed".
 *
 *   - cons_type:
 *       By default, "template_cons_t".
 *
 *       Default: "type_"
 */

static const type_t        *default_type_typed      (const type_t *self);
/* static const char          *default_type_name       (const type_t *self);                  */
/* static const char          *default_type_info       (const type_t *self);                  */
/* static size_t               default_type_size       (const type_t *self, const tval *val); */
/* static const struct_info_t *default_type_is_struct  (const type_t *self);                  */
static typed_t              default_type_cons_type  (const type_t *self);
static tval                *default_type_init       (const type_t *self, tval *cons);
static void                 default_type_free       (const type_t *self, tval *val);
static const tval          *default_type_has_default(const type_t *self);
static memory_tracker_t    *default_type_mem        (const type_t *self, tval *val_raw);
static tval                *default_type_dup        ( const type_t *self
                                                    , tval *dest
                                                    , const tval *src
                                                    , int rec_copy
                                                    , int dup_metadata
                                                    , ref_traversal_t *ref_traversal
                                                    );

static size_t default_type_is_struct_default_value        (const field_info_t *self, void *dest_field_mem);
static size_t default_type_is_struct_template_unused_value(const field_info_t *self, void *dest_field_mem);

const type_t type_defaults =
  { type_type

    /* memory_tracker_defaults */
  , /* memory      */ { memory_tracker_type, { memory_manager_type } }

  , /* typed       */ default_type_typed

  , /* name        */ NULL
  , /* info        */ NULL
  , /* size        */ NULL
  , /* is_struct   */ NULL

  , /* cons_type   */ default_type_cons_type
  , /* init        */ NULL
  , /* free        */ default_type_free
  , /* has_default */ default_type_has_default
  , /* mem         */ default_type_mem
  , /* dup         */ default_type_dup

  , /* parity      */ ""
  };

static const type_t        *default_type_typed      (const type_t *self)
  { return type_is_typed(self); }

/*
static const char          *default_type_name       (const type_t *self)
  { return "this"; }

static const char          *default_type_info       (const type_t *self)
  { return "typedef struct this_s this_t"; }

static size_t               default_type_size       (const type_t *self, const tval *val)
  { return sizeof(this_t); }

static const struct_info_t *default_type_is_struct  (const type_t *self)
  {
    static const this_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /-* typed_t type *-/
        { field_info_type
        , offsetof(this_t, type)
        , sizeof(empty.type)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* void field0 *-/
      , { field_info_type
        , offsetof(this_t, field0)
        , sizeof(empty.field0)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* void field1 *-/
      , { field_info_type
        , offsetof(this_t, field1)
        , sizeof(empty.field1)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* terminating_field_info: end of fields. *-/
      , { field_info_type, (size_t) (-1) }
      }
    , 3

    , NULL

    /-* No memory tracker field. *-/
    , 0
    , 0
    };

    return &struct_info;
  }
*/

static typed_t              default_type_cons_type  (const type_t *self)
  { return template_cons_type; }

/*
static tval                *default_type_init       (const type_t *self, tval *cons)
  {
    static default_t *defaults = NULL;
    if (!defaults)
    {
      static default_t this_defaults;

      this_defaults = default_this;

      defaults = &this_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }
*/

static void                 default_type_free       (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

/* By default there is no default value. */
/* TODO: by default, use struct_info's defaults?
 * TODO: would this result in an infinite loop?
 */
static const tval          *default_type_has_default(const type_t *self)
  { return NULL; /* TODO */; }

/*
 * TODO
 */
static memory_tracker_t    *default_type_mem        (const type_t *self, tval *val_raw)
  { return NULL; /* TODO */ }

static tval                *default_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal)
  {
    struct_dup(self->is_struct(self), dest, src, 1, rec_copy, dup_metadata, ref_traversal);

    return dest;
  }

/* ---------------------------------------------------------------- */

const type_t *type_is_typed(const type_t *self)
{
  return self;
}

const type_t *type_is_untyped(const type_t *self)
{
  return NULL;
}

const tval *type_has_no_default(const type_t *self)
{
  return NULL;
}

/* TODO */
TODO
const type_t        *type_typed      (const type_t *type);
const char          *type_name       (const type_t *type);
const char          *type_info       (const type_t *type);
size_t               type_size       (const type_t *type, const tval *val);
const struct_info_t *type_is_struct  (const type_t *type);
typed_t              type_cons_type  (const type_t *type);
tval                *type_init       (const type_t *type, tval *cons);
void                 type_free       (const type_t *type, tval *val);
const tval          *type_has_default(const type_t *type);
memory_tracker_t    *type_mem        (const type_t *type, tval *val_raw);
tval                *type_dup        ( const type_t *type
                                     , tval *dest
                                     , const tval *src
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *ref_traversal
                                     );

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

const template_cons_t * const default_template_cons = &template_cons_defaults;

const template_cons_t template_cons_defaults =
  { template_cons_type

  , /* dest                   */ NULL
  , /* memory_manager         */ NULL
  , /* initials               */ NULL
  , /* force_no_defaults      */ 0
  , /* initials_copy_rec      */ 0
  , /* ref_traversal          */ NULL
  , /* preserve_nocopy        */ 0
  , /* user                   */ NULL
  , /* allocate_only_with_num */ (size_t) 0
  , /* out_init_error_msg     */ NULL
  , /* init_error_msg_size    */ (size_t) 0
  };

/* ---------------------------------------------------------------- */

typed_t memory_manager_type_cons_type(const type_t *self)
{
  return template_cons_type;
}

tval *template_cons_dup_struct
  ( const template_cons_t   *cons
  , size_t                   size
  , const tval              *default_initials
  , const struct_info_t     *struct_info
  , memory_tracker_t      *(*mem)(tval *val_raw)
  )
{
  tval             *dest;

  int               is_allocate;
  int               is_allocate_only;

  const char       *is_err;

  memory_manager_t *memory_manager;

  if (!cons)
  {
    cons = default_template_cons;
    if (!cons)
      cons = &template_cons_defaults;
  }

  memory_manager = cons->memory_manager;
  if (!memory_manager)
    memory_manager = default_memory_manager;

  is_allocate_only = cons->allocate_only_with_num >= 1;

  is_allocate = !cons_dest || is_allocate_only;

  /* Do we need to dynamically allocate memory for this value? */
  if (is_allocate)
  {
    int    is_array;
    size_t number_to_allocate;

    if (cons->preserve_metadata)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: preserve_metadata flag is set, but the constructor dynamically allocates a value.\n"
            "  Since "preserve_metadata" skips memory tracker initialization, tracking this dynamic allocation can't be added to it.\n"
            "  Failed to initialize value.\n"
          );

      return NULL;
    }

    if (!mem && (!struct_info || !struct_info->has_memory_tracker))
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: constructor dynamically allocates memory, but there is no associated memory tracker!.\n"
            "  Failed to initialize new value, because \"mem\" is NULL and no \"struct_info\" with a memory tracker field reference was provided.\n"
          );

      return NULL;
    }

    number_to_allocate = cons->allocate_only_with_num;
    if (number_to_allocate <= 0)
      number_to_allocate = 1;

    is_array = number_to_allocate >= 2;

    if (!is_array && number_to_allocate == 1)
    {
      dest = memory_manager_malloc(memory_manager, size);
    }
    else
    {
      dest = memory_manager_calloc(memory_manager, number_to_allocate, size);
    }

    if (!dest)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: failed to allocate value memory!  Requested a total of '%d' bytes.\n"
          , (int) (number_to_allocate * size)
          );

      return NULL;
    }
  }

  /* Should we skip initialization? */
  if (is_allocate_only)
  {
    return dest;
  }

  if (!struct_info)
  {
    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_dup_struct: \"struct_info\" is NULL!\n"
          "  Failed to initialize a value without a representation of the struct.\n"
        , (int) (number_to_allocate * size)
        );

    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    return NULL;
  }

  /* Initialize memory tracker if available or necessary. */
  if (cons->preserve_metadata)
  {
    /* preserve_metadata is set: Skip memory tracker initialization. */
  }
  else
  {
    memory_tracker_t *memory_tracker;

    /* Note: if "mem" is provided, we should disregard     */
    /* "struct_info"'s references because "mem" overrides. */
    if      (mem)
    {
      memory_tracker = mem(dest);
    }
    else if (struct_info->has_memory_tracker)
    {
      memory_tracker = struct_value_has_memory_tracker(struct_info, dest);
    }

    /* Is a memory tracker required, but none available? */
    if (is_allocate && !memory_tracker)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: constructor dynamically allocates memory, but there is no associated memory tracker!.\n"
            "  Failed to initialize new value, because %s.\n"
          ,   mem
            ? "\"mem\" indicates that there should be no associated innate memory tracker for this value"
            : "\"mem\" is NULL and \"struct_info\" lacks an associating reference to a memory tracker field"
          );

      memory_manager_free(memory_manager, dest);

      return NULL;
    }

    /* Initialize memory tracker. */
    if (memory_tracker)
    {
      if (is_allocate)
        is_err = memory_tracker_initialize_no_buffers(memory_tracker, memory_manager, dest);
      else
        is_err = memory_tracker_initialize_no_buffers(memory_tracker, memory_manager, NULL);

      if (is_err)
      {
        if (cons->out_init_error_msg)
          snprintf
            ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
            , "Error: template_cons_dup_struct: initializing the memory tracker with \"memory_tracker_initialize_no_buffers\" failed!\n"
              "  Failed to initialize a value without successful initialization of its associated memory tracker.\n"
              "Error message returned from \"memory_tracker_initialize_no_buffers\": %s%s"
            , (const char *) is_err
            , ascii_ends_in_char(is_err, '\n') ? "" : "\n"
            );

        if (is_allocate)
          memory_manager_free(memory_manager, dest);

        return NULL;
      }
    }
  }

  /* Initialize fields. */
  is_err =
    struct_dup
      ( struct_info
      , dest
      , cons->initials
      , cons->force_no_defaults
      , cons->initials_copy_rec
      , cons->preserve_metadata
      , cons->ref_traversal
      );

  if (is_err)
  {
    size_t error_msg_len;
    int    error_msg_has_trailing_newline;

    size_t init_error_msg_size_terminator;

    init_error_msg_size_terminator = cons->init_error_msg_size - 1;
    if (init_error_msg_size_terminator < 0)
      init_error_msg_size_terminator = 0;

    error_msg_len = strlen(is_err);
    if (error_msg_len <= 0)
      error_msg_has_trailing_newline = 0;
    else
      error_msg_has_trailing_newline = is_err[error_msg_len - 1] == '\n';

    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_dup_struct: assigning the struct fields with \"struct_dup\" failed!\n"
          "  Failed to initialize a value without successful assignment of struct fields.\n"
          "Error message returned from \"struct_dup\": %s%s"
        , (const char *) is_err
        , error_msg_has_trailing_newline ? "" : "\n"
        );

    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    return NULL;
  }

  /* Done! */
  return dest;
}

/*
 * template_cons_get_type_mem:
 *
 * Return the type's "mem"
 */
memory_tracker_t *(*
    template_cons_get_type_mem(const type_t *type)
  )(const type_t *self, tval *val_raw)
{
  if (!type)
    return NULL;

  return type->mem;
}

/* ---------------------------------------------------------------- */
/* Utility functions.                                               */
/* ---------------------------------------------------------------- */

ptrdiff_t   field_pos (const void *base, const void *field)
{
  return ((const unsigned char *) field) - ((const unsigned char *) base);
}

void       *field_ref (ptrdiff_t   pos,  void       *base)
{
  return (void *) (((unsigned char *) base) + pos);
}

const void *field_cref(ptrdiff_t   pos,  const void *base)
{
  return (const void *) (((const unsigned char *) base) + pos);
}
