/*
 * opencurry: type_base_universal.c
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
 */
#include <stddef.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_universal.h"

#include "type_base_memory_tracker.h"
#include "type_base_type.h"

/* ---------------------------------------------------------------- */
/* Universal type.                                                  */
/* ---------------------------------------------------------------- */

/* Universal type. */
const type_t *universal_type(void)
  { return &universal_type_def; }

static const char   *universal_type_name      (const type_t *self);
static const type_t *universal_type_is_subtype( const type_t *self
                                              , const type_t *is_subtype
                                              );

const type_t universal_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ universal_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ universal_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_has_unknown_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ universal_type_is_subtype
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ NULL
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ NULL
  , /* cuser                  */ NULL
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char   *universal_type_name      (const type_t *self)
  { return "universal_t"; }

static const type_t *universal_type_is_subtype( const type_t *self
                                              , const type_t *is_subtype
                                              )
  { if (!self) return NULL; else return is_subtype; }
