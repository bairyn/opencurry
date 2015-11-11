/*
 * opencurry: type_base_ext.h
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
 * type_base_ext.h
 * ---------------
 *
 * "type_base.h" extensions.
 *
 * type_base declarations that require inclusion of additional header files.
 *
 * This exists in a separate header file to reduce the number of header file
 * inclusions for less commonly used declarations.
 */

#ifndef TYPE_BASE_EXT_H
#define TYPE_BASE_EXT_H
/* Header files for default values for  */
/* type representations for primitive C */
/* data types.                          */
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "base.h"

#include "type_base.h"

/* ---------------------------------------------------------------- */
/* Primitive C data types.                                          */
/* ---------------------------------------------------------------- */

/* <math.h> */
typedef div_t  primdiv_t;
typedef ldiv_t primldiv_t;

extern const primdiv_t  div_default;
extern const primldiv_t ldiv_default;

/* <setjmp.h> */
typedef jmp_buf jmp_buf_t;

extern const jmp_buf_t jump_buffer_placeholder, jmpbuf_default;

/* <signal.h> */
typedef sig_atomic_t primsig_atomic_t;

/* <stdarg.h> */
typedef va_list va_list_t;

/* <stddef.h> */
typedef size_t    primsize_t;
typedef ptrdiff_t primptrdiff_t;

extern const primsize_t    size_default;
extern const primptrdiff_t ptrdiff_default;

/* <stdio.h> */
typedef FILE   file_t;
typedef fpos_t primfpos_t;

/* <time.h> */
typedef struct tm tm_t;
typedef time_t    primtime_t;
typedef clock_t   primclock_t;

/* ---------------------------------------------------------------- */

typedef jmp_buf_t primjmp_buf_t;
typedef va_list_t primva_list_t;
typedef file_t    primfile_t;
typedef tm_t      primtm_t;

/* ---------------------------------------------------------------- */

/* Mutable vs. const typedefs. */

typedef       primdiv_t        mprimdiv_t;
typedef const primdiv_t        cprimdiv_t;

typedef       jmp_buf_t        mjmp_buf_t;
typedef const jmp_buf_t        cjmp_buf_t;

typedef       primsig_atomic_t mprimsig_atomic_t;
typedef const primsig_atomic_t cprimsig_atomic_t;

typedef       va_list_t        mva_list_t;
typedef const va_list_t        cva_list_t;

typedef       primsize_t       mprimsize_t;
typedef       primptrdiff_t    mprimptrdiff_t;
typedef const primsize_t       cprimsize_t;
typedef const primptrdiff_t    cprimptrdiff_t;

typedef       file_t           mfile_t;
typedef       primfpos_t       mprimfpos_t;
typedef const file_t           cfile_t;
typedef const primfpos_t       cprimfpos_t;

typedef       tm_t             mtm_t;
typedef       primtime_t       mprimtime_t;
typedef       primclock_t      mprimclock_t;
typedef const tm_t             ctm_t;
typedef const primtime_t       cprimtime_t;
typedef const primclock_t      cprimclock_t;

typedef       primjmp_buf_t    mprimjmp_buf_t;
typedef       primva_list_t    mprimva_list_t;
typedef       primfile_t       mprimfile_t;
typedef       primtm_t         mprimtm_t;
typedef const primjmp_buf_t    cprimjmp_buf_t;
typedef const primva_list_t    cprimva_list_t;
typedef const primfile_t       cprimfile_t;
typedef const primtm_t         cprimtm_t;

#endif /* ifndef TYPE_BASE_EXT_H */
