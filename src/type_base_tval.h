/*
 * opencurry: type_base_tval.h
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
 * type_base_tval.h
 * ------
 *
 * tval: Values associated with a type representation.
 */

#ifndef TYPE_BASE_TVAL_H
#define TYPE_BASE_TVAL_H
#include "base.h"

#include "type_base_prim.h"
#include "type_base_typed.h"

/* ---------------------------------------------------------------- */
/* tval: Synopsis.                                                  */
/* ---------------------------------------------------------------- */

/* tval: typed_t polymorphic values.                                */

/* "tval *" is "void *" with the additional semantics that non-NULL */
/* references are polymorphic with references to "typed_t"          */
/*                                                                  */
/* In other words, the "tval" label means that "tval *" pointers    */
/* can be cast to "typed_t *" pointers.                             */
/*                                                                  */
/* Usually, a "tval *" refers to a value of a struct whose          */
/* first field is "typed_t type", where the underlying memory       */
/* of the struct value starts with th underlying memory of          */
/* the value of tihs field.                                         */

/* > typedef struct type_s struct_t;                                */
/* > typedef const type_t *(*typed_t)(void);                        */
/* >                                                                */
/* > /-* Values of this type are valid "tval"s. *-/                 */
/* > typedef struct mystruct_s mystruct_t;                          */
/* > struct mystruct_s                                              */
/* > {                                                              */
/* >   typed_t type;                                                */
/* >                                                                */
/* >   int a;                                                       */
/* >   int b;                                                       */
/* >   ...                                                          */
/* > };                                                             */

/* ---------------------------------------------------------------- */
/* tval: Documentation.                                             */
/* ---------------------------------------------------------------- */

/* Background: first-field polymorphism.                            */
/*                                                                  */
/* Given a struct, for example:                                     */
/* > typedef struct intpair_s intpair_t;                            */
/* > struct intpair_s                                               */
/* > {                                                              */
/* >   int a;                                                       */
/* >   int b;                                                       */
/* > };                                                             */
/*                                                                  */
/* Then a pointer to a value of the struct can also be              */
/* interpreted as a pointer to the first field value of a struct,   */
/* because the memory of a value of "intpair_t" begins with the     */
/* first field.                                                     */
/*                                                                  */
/* For example:                                                     */
/* > static const intpair_t pair_3_7 =                              */
/* >   { /-* a *-/ 3                                                */
/* >   , /-* b *-/ 7                                                */
/* >   };                                                           */
/* >                                                                */
/* > static const intpair_t *my_pair = &pair_3_7;                   */
/* >                                                                */
/* > static const int * const my_a = (const int *) my_pair;         */
/*                                                                  */
/* Here "*my_a" would evaluate to 3.                                */
/*                                                                  */
/* A "tval *" uses this concept for type representation values      */
/* as the first field rather than "int"s.                           */
/*                                                                  */
/* ---------------------------------------------------------------- */
/*                                                                  */
/* Background: type_t.                                              */
/*                                                                  */
/* "type_t" is a struct, "type_s".  A "type_t" value describes      */
/* a type, containing data about how to handle initialization,      */
/* memory allocation, etc.                                          */
/*                                                                  */
/* For a simpler API, we might define our "intpair_t" struct        */
/* above with an additional "type_t *" field, to allow "intpair_t"  */
/* values to polymorphically be cast to a "type_t *" pointer,       */
/* using first-field polymorphism:                                  */
/*                                                                  */
/* > typedef struct intpair_s intpair_t;                            */
/* > struct intpair_s                                               */
/* > {                                                              */
/* >   type_t *type;                                                */
/* >                                                                */
/* >   int a;                                                       */
/* >   int b;                                                       */
/* > };                                                             */
/*                                                                  */
/* We can also define a second struct conforming to the same        */
/* first-field polyhorphism:                                        */
/*                                                                  */
/* > typedef struct errcode_s errcode_t;                            */
/* > struct errcode_s                                               */
/* > {                                                              */
/* >   type_t *type;                                                */
/* >                                                                */
/* >   int  error_code;                                             */
/* >   char error_msg[ERR_MSG_SIZE];                                */
/* > };                                                             */
/*                                                                  */
/* This allows us to retrieve the first, "type_t *" field from      */
/* a pointer to either "intpair_t" or "errcode_t':                  */
/*                                                                  */
/* > type_t **get_type(void *val)                                   */
/* > {                                                              */
/* >   void    *value;                                              */
/* >   type_t **type;                                               */
/* >                                                                */
/* >   value = ((void    ) *) val;                                  */
/* >   type  = ((type_t *) *) val;                                  */
/* >                                                                */
/* >   return type;                                                 */
/* > }                                                              */
/*                                                                  */
/* Here we just reinterpret a pointer to either "intpair_t",        */
/* "errcode_t", or any other conformant structs, as a pointer       */
/* to "type_t *".                                                   */
/*                                                                  */
/* ---------------------------------------------------------------- */
/*                                                                  */
/* Background: typed_t.                                             */
/*                                                                  */
/* "type_t" is defined in the "type_base_type" module as a struct.  */
/*                                                                  */
/* However, the standard type of type representations for           */
/* a given type is "typed_t", not "type_t"; this is defined         */
/* in "type_base_typed".                                            */
/*                                                                  */
/* What is typed_t?                                                 */
/*                                                                  */
/* Many constant top-level type representations can be defined with */
/* a static initializer, providing a "const type_t *" available for */
/* use.                                                             */
/*                                                                  */
/* But static initializers have their limitations.  They can't,     */
/* for example, call out to functions, nor can they refer to        */
/* other variables.                                                 */
/*                                                                  */
/* Adding a layer of indirection via a call to a C function         */
/* grants flexibility, that, for instance, supports both.           */
/*                                                                  */
/* This is the purpose of "typed_t".  A "typed_t" is a function     */
/* pointer to a function that takes zero arguments and returns      */
/* a "const type_t *" pointer*                                      */
/*                                                                  */
/* > typedef const type_t *(*typed_t)(void);                        */
/* >                                                                */
/* > const type_t *intpair_type(void);                              */
/* >                                                                */
/* > void foo()                                                     */
/* > {                                                              */
/* >   typed_t which_type = intpair_type;                           */
/* >                                                                */
/* >   const type_t *type_def = which_type();                       */
/* >                                                                */
/* >   do_something_with_type_def(type_def);                        */
/* > }                                                              */
/*                                                                  */
/* Thus the standard reference to type descriptions (type_t)'s      */
/* is not a pointer to "type_t", but a pointer to a function        */
/* with this type:                                                  */
/* > const type_t *foo_type(void);                                  */
/*                                                                  */
/* ("void" in the argument list means that the C function           */
/* takes zero arguments.)                                           */
/*                                                                  */
/* We can obtain a "const type_t *" reference by just calling       */
/* "foo_type", or a function pointer to "foo_type":                 */
/* > void foo()                                                     */
/* > {                                                              */
/* >   const type_t *foo_type_def = foo_type();                     */
/* >                                                                */
/* >   do_something_with_type_def(foo_type_def);                    */
/* > }                                                              */
/*                                                                  */
/* For users writing a type representation for a type, i.e.         */
/* defining a "typed_t" pointer or the type of its reference:       */
/* "const type_t foo_type(void);", they can still use static        */
/* initializers to define a "type_t" when the requirements are      */
/* simple enough, and just define "foo_type" to be a                */
/* simple wrapper that directly returns a reference to this         */
/* struct.                                                          */
/*                                                                  */
/* More complex type representation definitions will also work;     */
/* users don't need to know the difference, since both kinds        */
/* of definition provide the same "typed_t"-based type.             */
/*                                                                  */
/* ---------------------------------------------------------------- */
/*                                                                  */
/* What is a tval?                                                  */
/*                                                                  */
/* First-field polymorphism is the underlying idea behind "tvals".  */
/*                                                                  */
/* Internally, "tval *" is just typedef'd to "void *".              */
/*                                                                  */
/* Wherever a "tval *" occurs instead of just "void *",             */
/* the pointer to the value is expected to be polymorphic with a    */
/* pointer to "typed_t", interpreting "tval" as "typed_t".          */
/*                                                                  */
/* Thus, generally speaking, a "tval" value is a value of any       */
/* struct type whose first field has type "typed_t".                */
/* (More accurately, any value whose underlying memory starts with  */
/* a valid representation of "typed_t", e.g. a "typed_t" function   */
/* pointer itself.)                                                 */
/*                                                                  */
/* This isn't enforced internally, where "tval" is equivalent to    */
/* "void" (and thus "tval *" to "void *"): "tval", then,            */
/* is effectively a label that means that it is safe to interpret   */
/* its memory as type "typed_t".                                    */

/* ---------------------------------------------------------------- */
/* tval: Values associated with a type, via "typed_t" polymorphism. */
/* ---------------------------------------------------------------- */

typedef void tval;

/* TODO: also: type_t *self method */
#ifdef TODO /* TODO: needs "type_container" implemented first. */
typed_t        typedof        (const tval *val);
#endif /* #ifdef TODO */
const type_t  *typeof         (const tval *val);
const type_t  *typeof_indirect(const tval *val);
typed_t        tval_get_typed (const tval *val);
const typed_t *tval_to_typed  (const tval *val);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_TVAL_H */
