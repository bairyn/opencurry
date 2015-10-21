/*
 * opencurry: anci_c_ast.h
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
 * anci_c_ast.h
 * ------
 */

#ifndef ANSI_C_AST_H
#define ANSI_C_AST_H
#include "base.h"

#ifdef TODO

#define AC_INDEX_BITS 42
typedef unsigned long ac_index_t;

/* ---------------------------------------------------------------- */
/* String fragments. */

#define AC_STR_IOTA_BUF_SIZE 64
struct ac_str_iota_s
{
  unsigned char buf[AC_STR_IOTA_BUF_SIZE];
  size_t        size;  /* Total size of all bits, starting with element. */

  ac_index_t    next_i : AC_INDEX_BITS;
};
typedef struct ac_str_iota_s ac_str_iota_t;

#define AC_STR_SMALL_BUF_SIZE 1024
struct ac_str_small_s
{
  unsigned char buf[AC_STR_SMALL_BUF_SIZE];
  size_t        size;  /* Total size of all bits, starting with element. */

  ac_index_t    next_i : AC_INDEX_BITS;
};
typedef struct ac_str_small_s ac_str_small_t;

#define AC_STR_CHUNK_BUF_SIZE (32 * 1024)
struct ac_str_chunk_s
{
  unsigned char buf[AC_STR_CHUNK_BUF_SIZE];
  size_t        size;  /* Total size of all bits, starting with element. */

  ac_index_t    next_i : AC_INDEX_BITS;
};
typedef struct ac_str_chunk_s ac_str_chunk_t;

struct ac_str_s
{
  enum ac_str_type_e
  { ac_str_type_iota  = (0 << 1) | (0 << 0)
  , ac_str_type_small = (0 << 1) | (1 << 0)

  , ac_str_type_chunk = (1 << 1) | (0 << 0)
  } type       : 2;
  ac_index_t i : AC_INDEX_T_BITS;
  ac_index_t   : (64 - 2 - AC_INDEX_BITS);
};
typedef struct ac_str_s ac_str_t;

/* ---------------------------------------------------------------- */
/* Annotations. */

/*
 * Arbitrary string of no particular type.
 *
 * Can be used to represent an arbitrary string between some, but not all,
 * parts of an AST.
 */
struct ac_untyped_str_s
{
  ac_index_t str_i : AC_INDEX_BITS;
};
typedef struct ac_untyped_str_s ac_untyped_str_t;

/*
 * Whitespace that can be arbitrarily formatted.
 */
struct ac_whitespace_implicit_s
{
};
typedef struct ac_whitespace_implicit_s ac_whitespace_implicit_t;

/*
 * Explicitly formatted whitespace.
 */
struct ac_whitespace_explicit_s
{
  ac_index_t str_i : AC_INDEX_BITS;
};
typedef struct ac_whitespace_explicit_s ac_whitespace_explicit_t;

/*
 * Whitespace.
 */
struct ac_whitespace_s
{
  enum ac_whitespace_type_e
  { ac_whitespace_type_implicit = (0 << 0)
  , ac_whitespace_type_explicit = (1 << 0)
  } type       : 1;
  ac_index_t i : AC_INDEX_T_BITS;
  ac_index_t   : (64 - 1 - AC_INDEX_BITS);
};
typedef struct ac_whitespace_s ac_whitespace_t;

/*
 * Comment.
 */
struct ac_whitespace_implicit_s
{
};
typedef struct ac_whitespace_implicit_s ac_whitespace_implicit_t;

/*
 * Whitespace, comment, CPP, or fixed string.
 */
struct ac_annotation_s
{
  enum ac_annotation_type_e
  { ac_annotation_type_untyped_str (0 << 1) | (0 << 0)
  , ac_annotation_type_whitespace  (0 << 1) | (1 << 0)

  , ac_annotation_type_comment     (1 << 1) | (0 << 0)
  } type       : 2;
  ac_index_t i : AC_INDEX_T_BITS;
  ac_index_t   : (64 - 2 - AC_INDEX_BITS);
};
typedef struct ac_annotation_s ac_annotatoin_t;

/* ---------------------------------------------------------------- */
/* "Items": top-level declarations. */

/*
 * Item: a top-level declaration.
 */
struct ac_item_s
{
};
typedef struct ac_item_s ac_item_t;

/* ---------------------------------------------------------------- */
/* TODO. */

struct ac_ast_s
{
  ac_item_t *items;
  size_t     items_n;
};
typedef struct ac_ast_s ac_ast_t;









/*
 * todo
 */
struct ac_ast_s
{
  ac_ast_chunk_t foo;
};
typedef struct ac_ast_s ac_ast_t;




/* ---------------------------------------------------------------- */
/* typedefs. */

typedef struct ac_ast_s ac_ast_t;
typedef struct ac_ast_chunk_s ac_ast_chunk_t;

/* ---------------------------------------------------------------- */
/* struct definitions. */

/*
 * annotation:
 *
 * Comment, CPP, or custom whitespace.
 */

struct ac_ast_chunk_s
{
};

#endif /* TODO */

#endif /* ifndef ANSI_C_AST_H */
