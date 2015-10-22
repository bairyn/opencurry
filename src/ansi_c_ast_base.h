/*
 * opencurry: ansi_c_ast_base.h
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
 * ansi_c_ast_base.h
 * ------
 */


#ifndef ANSI_C_AST_BASE_H
#define ANSI_C_AST_BASE_H
/* stddef.h:
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

/* ---------------------------------------------------------------- */
/* TODO. */

/* TODO */
typedef int TODO;
typedef TODO ac_whit_t;
typedef TODO ac_comm_t;
typedef TODO ac_arbi_t;
typedef TODO ac_item_t;
#define DEF_NUM_AC_WHIT (8 * 1024)
#define DEF_NUM_AC_COMM (8 * 1024)
#define DEF_NUM_AC_ARBI (8 * 1024)
#define DEF_NUM_AC_ITEM (8 * 1024)

/* ---------------------------------------------------------------- */
/* Annotations. */

/*
 * Whitespace, comment, CPP, or arbitrary string.
 */

/*
 * ac_annotation
 *   -> ac_whitespace
 *    | ac_comment
 *    | ac_arbitrary
 */
#define DEF_NUM_AC_ANNO (7 * 1024)
typedef struct ac_anno_s ac_anno_t;
struct ac_anno_s
{
  enum ac_anno_e
  { ac_anno_whit = (0 << 0) | (0 << 0)
  , ac_anno_comm = (0 << 1) | (1 << 0)
  , ac_anno_arbi = (1 << 1) | (0 << 0)
  } type;

  union ac_anno_u
  {
    struct ac_anno_whit_s
    { ac_whit_t *whit;
    } whit;

    struct ac_anno_comm_s
    { ac_comm_t *comm;
    } anno;

    struct ac_anno_arbi_s
    { ac_arbi_t *arbi;
    } item;
  } values;
};

/* ---------------------------------------------------------------- */
/* ANSI C AST. */

/*
 * ac_ast
 *   -> (empty)
 *    | ac_annotation ac_ast
 *    | ac_item       ac_ast
 */
#define DEF_NUM_AC_AST (8 * 1024)
typedef struct ac_ast_s ac_ast_t;
struct ac_ast_s
{
  enum ac_ast_e
  { ac_ast_empt = (0 << 0) | (0 << 0)

  , ac_ast_anno = (1 << 1) | (0 << 0)
  , ac_ast_item = (1 << 1) | (1 << 0)
  } type;

  union ac_ast_u
  {
    /* (empty) */

    struct ac_ast_anno_s
    { ac_item_t *item;
      ac_ast_t  *ast;
    } anno;

    struct ac_ast_item_s
    { ac_item_t *item;
      ac_ast_t  *ast;
    } item;
  } values;
};

/* ---------------------------------------------------------------- */
/* ANSI C AST with memory references to components. */

enum ac_ast_element_type_id_e
{ type_ac_anno,
  type_ac_ast,

  num_ac_ast_element_type_ids
};
typedef enum ac_ast_element_type_id_e ac_ast_element_type_id_t;

typedef struct ac_ast_element_type_info_s ac_ast_element_type_info_t;
struct ac_ast_element_type_info_s
{
  /* Identifier for this type. */
  ac_ast_element_type_id_t id;

  /*
   * Default number of elements to allocate for this type in an
   * "ac_ast_buf_t".
   */
  size_t                   def_num_elems;

  /* Size of an element of this type. */
  size_t                   elem_size;
};

extern ac_ast_element_type_info_t ac_ast_element_type_info[num_ac_ast_element_type_ids];
extern const size_t ac_ast_element_type_info_num;

/*
 * ac_ast_buf_t:
 *
 * Information about memory allocated for a type of element.
 */
typedef struct ac_ast_buf_s ac_ast_buf_t;
struct ac_ast_buf_s
{
  /* Array of element slots. */
  void   *slots;

  /* Bitfield representing which slots are in use. */
  unsigned char *used;

  /*
   * Memory allocated to element slot buffer.
   *
   * Must be at least "elem_size * num_alloc_slots".
   */
  size_t  size_slots;

  /*
   * Memory allocated to element-slots-in-use buffer.
   *
   * Must be at least "(num_alloc_slots + 7) / 8", i.e. 1/8th of the number of
   * slots, rounded up to the nearest minimum multiple of 8, since each byte in
   * "used" represents 8 element slots.
   */
  size_t  size_used;


  /* Number of element slots allocated in buffer. */
  size_t  num_slots;

  /* Number of element slots in use in buffer. */
  size_t  num_used;

  /* Number of any free element slot. */
  size_t  next_free;
};

/*
 * ac_ast_bufs_t;
 *
 * A structure containing (possibly multiple) ASTs along with allocated memory
 * references to the components of them.
 *
 * This can be used to contain an AST along with data for memory management.
 */
typedef struct ac_ast_bufs_s ac_ast_bufs_t;
struct ac_ast_bufs_s
{
  /*
   * Normally NULL, but support a simple linked list for additional memory
   * allocation.
   */
  struct ac_ast_bufs_s *next;


  /*
   * Arrays of elements, for each type of element.
   */
  ac_ast_buf_t          elems[num_ac_ast_element_type_ids];
};

/* ---------------------------------------------------------------- */
/* ac_ast_bufs_t accessors. */

ac_ast_bufs_t *new_ac_ast_bufs(void);
void free_ac_ast_bufs_norecursive(ac_ast_bufs_t *ast);

void free_ac_ast_bufs_recursive(ac_ast_bufs_t *ast);

ac_ast_bufs_t *rest_free_ac_ast_bufs_recursive(ac_ast_bufs_t *ast);


void init_ac_ast_bufs(ac_ast_bufs_t *ast);
void clos_ac_ast_bufs_norecursive(ac_ast_bufs_t *ast);

void clos_ac_ast_bufs_recursive(ac_ast_bufs_t *ast);

ac_ast_bufs_t *rest_ac_ast_bufs(ac_ast_bufs_t *ast);

#endif /* ifndef ANSI_C_AST_BASE_H */
