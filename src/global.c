/*
 * opencurry: global.c
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

#include "base.h"
#include "global.h"

#include "util.h"

/* ---------------------------------------------------------------- */
/* NULL.                                                            */
/* ---------------------------------------------------------------- */

void * const null = (void *) NULL;

void * const nullref[] = { NULL, NULL };
const size_t nullref_size = ARRAY_SIZE(   nullref);
const size_t nullref_num  = ARRAY_NUM(    nullref);
const size_t nullref_len  = ARRAY_LEN_ALL(nullref);

void * const loopref[] = { (void *) loopref, NULL };
const size_t loopref_size = ARRAY_SIZE(   loopref);
const size_t loopref_num  = ARRAY_NUM(    loopref);
const size_t loopref_len  = ARRAY_LEN_ALL(loopref);

void * const selfref[] = { (void *) &selfref, NULL };
const size_t selfref_size = ARRAY_SIZE(   selfref);
const size_t selfref_num  = ARRAY_NUM(    selfref);
const size_t selfref_len  = ARRAY_LEN_ALL(selfref);

/* ---------------------------------------------------------------- */
/* Constant buffers.                                                */
/* ---------------------------------------------------------------- */

const unsigned char zeroes[ZEROES_BUF_SIZE] = { 0x00 };
const size_t        zeroes_size = ARRAY_SIZE(   zeroes);
const size_t        zeroes_num  = ARRAY_NUM(    zeroes);
const size_t        zeroes_len  = ARRAY_LEN_ALL(zeroes);

const unsigned char * const zeroes_bytes = (const unsigned char *) (&zeroes[0]);
const size_t zeroes_bytes_size = sizeof(zeroes);
const size_t zeroes_bytes_num  = sizeof(zeroes) / sizeof(*zeroes_bytes);
const size_t zeroes_bytes_len  = SIZE_LESS_NULL(sizeof(zeroes) / sizeof(*zeroes_bytes));

const char * const zeroes_chars = (const char *) (&zeroes[0]);
const size_t zeroes_chars_size  = sizeof(zeroes);
const size_t zeroes_chars_num   = sizeof(zeroes) / sizeof(*zeroes_chars);
const size_t zeroes_chars_len   = SIZE_LESS_NULL(sizeof(zeroes) / sizeof(*zeroes_chars));

void * const zeroes_mem = (void *) (&zeroes[0]);
const size_t zeroes_mem_size = sizeof(zeroes);
const size_t zeroes_mem_num  = sizeof(zeroes) / sizeof(unsigned char);
const size_t zeroes_mem_len  = SIZE_LESS_NULL(sizeof(zeroes) / sizeof(unsigned char));

/* ---------------------------------------------------------------- */

const unsigned char spaces[SPACES_BUF_LEN + 1] = { CAT(REPLICATE_COMMA_, SPACES_BUF_LEN)(' '), '\x00' };
const size_t        spaces_size = ARRAY_SIZE(   spaces);
const size_t        spaces_num  = ARRAY_NUM(    spaces);
const size_t        spaces_len  = ARRAY_LEN_ALL(spaces);

const unsigned char tabs[TABS_BUF_LEN + 1] = { CAT(REPLICATE_COMMA_, SPACES_BUF_LEN)(' '), '\x00' };
const size_t        tabs_size = ARRAY_SIZE(   spaces);
const size_t        tabs_num  = ARRAY_NUM(    spaces);
const size_t        tabs_len  = ARRAY_LEN_ALL(spaces);
