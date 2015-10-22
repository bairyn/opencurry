/*
 * opencurry: ansi_c_ast_base.c
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
 *   - size_t
 */
#include <stddef.h>

/* stdlib.h:
 *   - malloc
 *   - free
 */
#include <stdlib.h>

#include "base.h"
#include "ansi_c_ast_base.h"

/* ---------------------------------------------------------------- */
/* ANSI C AST with memory references to components. */

ac_ast_element_type_info_t ac_ast_element_type_info[num_ac_ast_element_type_ids] =
  { { type_ac_anno, DEF_NUM_AC_ANNO, sizeof(ac_anno_t) }
  , { type_ac_ast,  DEF_NUM_AC_AST,  sizeof(ac_ast_t)  }
  };
const size_t ac_ast_element_type_info_num =
  sizeof(ac_ast_element_type_info) / sizeof(ac_ast_element_type_info[0]);

/*
 * new_ac_ast_bufs:
 *
 * Dynamically allocate a new "ac_ast_bufs_t" structure with "malloc" and then
 * initialize it with "init_ac_ast_bufs".
 */
ac_ast_bufs_t *new_ac_ast_bufs(void)
{
  ac_ast_bufs_t *ast = malloc(sizeof(ac_ast_bufs_t));

  init_ac_ast_bufs(ast);

  return ast;
}

/*
 * free_ac_ast_bufs_norecursive:
 *
 * Clean up an "ac_ast_bufs_t" structure with "clos_ac_ast_bufs_norecursive"
 * and then free the reference with "free".
 *
 * Inverse of "new_ac_ast_bufs".
 */
void free_ac_ast_bufs_norecursive(ac_ast_bufs_t *ast)
{
  if(!ast)
    return;

  clos_ac_ast_bufs_norecursive(ast);
  free(ast);
}

/*
 * free_ac_ast_bufs_recursive:
 *
 * Clean up an "ac_ast_bufs_t" structure with "clos_ac_ast_bufs_recursive"
 * and then free the reference with "free".
 *
 * Inverse of "new_ac_ast_bufs".
 */
void free_ac_ast_bufs_recursive(ac_ast_bufs_t *ast)
{
  if(!ast)
    return;

  clos_ac_ast_bufs_recursive(ast);
  free(ast);
}

/*
 * rest_free_ac_ast_bufs:
 *
 * Clean up an "ac_ast_bufs_t" structure with "clos_ac_ast_bufs_norecursive"
 * and then free the reference with "free".
 *
 * Return the linked structure "next" of the input before it was freed.
 */
ac_ast_bufs_t *rest_free_ac_ast_bufs(ac_ast_bufs_t *ast)
{
  ac_ast_bufs_t *next;

  if(!ast)
    return NULL;

  next = ast->next;

  free_ac_ast_bufs_norecursive(ast);

  return next;
}

/*
 * init_ac_ast_bufs:
 *
 * Initialize an existing ANSI C AST structure.
 *
 * Allocates memory for buffers.
 */
void init_ac_ast_bufs(ac_ast_bufs_t *ast)
{
  int             i, j;

  /* Reference to one block of memory. */
  unsigned char  *mem;

  /* We'll sum up the total bytes we'll allocate from each type of element. */
  size_t          total_size_slots       = 0;
  size_t          total_size_used_slots  = 0;
  size_t          offsets_slots[num_ac_ast_element_type_ids + 1];
  size_t          offsets_used [num_ac_ast_element_type_ids + 1];
  size_t          slot_counts  [num_ac_ast_element_type_ids + 1];

  for (i = 0; i < ac_ast_element_type_info_num; ++i)
  {
    ac_ast_element_type_info_t *info;

    info = &ac_ast_element_type_info[i];

    slot_counts[i] = info->def_num_elems;

    offsets_slots[i] = total_size_slots;
    offsets_used [i] = total_size_used_slots;

    total_size_slots      += slot_counts[i] * info->elem_size;
    total_size_used_slots += (slot_counts[i] + 7) / 8;
  }

  offsets_slots[i] = total_size_slots;
  offsets_used [i] = total_size_used_slots;


  /* Set up the AST. */
  ast->next = NULL;

  mem = (unsigned char *) malloc(total_size_slots + total_size_used_slots);

  for (i = 0; i < ac_ast_element_type_info_num; ++i)
  {
    ac_ast_buf_t *buf;

    buf = &ast->elems[i];

    buf->slots = (void          *) (mem + offsets_slots[i]);
    buf->used  = (unsigned char *) (mem + offsets_used [i]);

    buf->size_slots = offsets_slots[i + 1] - offsets_slots[i];
    buf->size_used  = offsets_used [i + 1] - offsets_used [i];

    buf->num_slots = slot_counts[i];


    for (j = 0; j < buf->size_used; ++j)
    {
      buf->used[j] = 0;
    }
    buf->num_used = 0;
    buf->next_free = 0;
  }
}

/*
 * clos_ac_ast_bufs:
 *
 * Cleans up initialization performed by "init_ac_ast_bufs", by freeing memory
 * it allocated.
 */
void clos_ac_ast_bufs_norecursive(ac_ast_bufs_t *ast)
{
  if(ast->elems->slots)
    free(ast->elems->slots);
}

/*
 * clos_ac_ast_bufs:
 *
 * Cleans up initialization performed by "init_ac_ast_bufs", by freeing memory
 * it allocated.
 *
 * Also recursively cleans up any linked structure by "next".
 */
void clos_ac_ast_bufs_recursive(ac_ast_bufs_t *ast)
{
  while (ast)
  {
    ac_ast_bufs_t *next = ast->next;

    clos_ac_ast_bufs_norecursive(ast);

    ast = next;
  }
}

/*
 * clos_ac_ast_bufs:
 *
 * Cleans up initialization performed by "init_ac_ast_bufs", by freeing memory
 * it allocated.
 *
 * Returns the structure's "next" field, whether NULL or a reference to
 * another.
 */
ac_ast_bufs_t *rest_ac_ast_bufs(ac_ast_bufs_t *ast)
{
  ac_ast_bufs_t *next;

  if (!ast)
    return NULL;

  next = ast->next;

  clos_ac_ast_bufs_norecursive(ast);

  return next;
}
