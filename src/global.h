/*
 * opencurry: global.h
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
 * global.h
 * --------
 *
 * Assorted global buffers and data, constant and per-context.
 */

#ifndef GLOBAL_H
#define GLOBAL_H
#include "base.h"

#include "util.h"

/* ---------------------------------------------------------------- */

#define DEFAULT_GLOBAL_BUF_LEN  8192
#define DEFAULT_GLOBAL_BUF_SIZE (DEFAULT_GLOBAL_BUF_LEN + 1)

/* ---------------------------------------------------------------- */
/* NULL.                                                            */
/* ---------------------------------------------------------------- */

extern void * const null;

extern void * const nullref[];
extern const size_t nullref_size;
extern const size_t nullref_num;
extern const size_t nullref_len;

extern void * const loopref[];
extern const size_t loopref_size;
extern const size_t loopref_num;
extern const size_t loopref_len;

extern void * const selfref[];
extern const size_t selfref_size;
extern const size_t selfref_num;
extern const size_t selfref_len;

/* ---------------------------------------------------------------- */
/* Constant buffers.                                                */
/* ---------------------------------------------------------------- */

#define ZEROES_BUF_SIZE DEFAULT_GLOBAL_BUF_SIZE
extern const unsigned char zeroes[ZEROES_BUF_SIZE];
extern const size_t        zeroes_size;
extern const size_t        zeroes_num;
extern const size_t        zeroes_len;

extern const unsigned char * const zeroes_bytes;
extern const size_t zeroes_bytes_size;
extern const size_t zeroes_bytes_num;
extern const size_t zeroes_bytes_len;

extern const char * const zeroes_chars;
extern const size_t zeroes_chars_size;
extern const size_t zeroes_chars_num;
extern const size_t zeroes_chars_len;

extern void * const zeroes_mem;
extern const size_t zeroes_mem_size;
extern const size_t zeroes_mem_num;
extern const size_t zeroes_mem_len;

/* ---------------------------------------------------------------- */

#define SPACES_BUF_LEN DEFAULT_GLOBAL_BUF_LEN
extern const unsigned char spaces[SPACES_BUF_LEN + 1];
extern const size_t        spaces_size;
extern const size_t        spaces_num;
extern const size_t        spaces_len;

#define TABS_BUF_LEN DEFAULT_GLOBAL_BUF_LEN
extern const unsigned char tabs[TABS_BUF_LEN + 1];
extern const size_t        tabs_size;
extern const size_t        tabs_num;
extern const size_t        tabs_len;

/* ---------------------------------------------------------------- */

#endif /* ifndef GLOBAL_H */
