/*
 * opencurry: ptrs.h
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
 * ptrs.h
 * ------
 */

#ifndef PTRS_H
#define PTRS_H
/* stddef.h:
 *   - size_t
 *   - ptrdiff_t
 */
#include <stddef.h>

#include "base.h"

/* ---------------------------------------------------------------- */
/* Casting to and from pointers, possibly of different types.       */
/* ---------------------------------------------------------------- */

typedef void   *objp_cast_t;
typedef void *(*funp_cast_t)(void *, ...);

typedef       void   *objpm_cast_t;
typedef       void *(*funpm_cast_t)(void *, ...);
typedef const void   *objpc_cast_t;
typedef const void *(*funpc_cast_t)(void *, ...);

funp_cast_t objp_to_funp(objp_cast_t ptr);
objp_cast_t funp_to_objp(funp_cast_t ptr);

funpc_cast_t objpc_to_funpc(objpc_cast_t ptr);
objpc_cast_t funpc_to_objpc(funpc_cast_t ptr);

size_t     objp_to_size   (void     *ptr);
void      *size_to_objp   (size_t    ptr_rep);
ptrdiff_t  objp_to_ptrdiff(void     *ptr);
void      *ptrdiff_to_objp(ptrdiff_t ptr_rep);

size_t      objpc_to_size   (const void *ptr);
const void *size_to_objpc   (size_t      ptr_rep);
ptrdiff_t   objpc_to_ptrdiff(const void *ptr);
const void *ptrdiff_to_objpc(ptrdiff_t   ptr_rep);

#endif /* ifndef PTRS_H */
