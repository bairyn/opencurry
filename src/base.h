/*
 * opencurry: base.h
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
 * base.h
 * ------
 *
 * Header file included by each C file, including those for testing.
 */

#ifndef BASE_H
#define BASE_H
#include "base.h"

#define DEFAULT_DEBUG          0
#define DEFAULT_POSIX_PARALLEL 0
#define DEFAULT_ERROR_CHECKING 1

/* ---------------------------------------------------------------- */

#ifndef DEBUG
#  define DEBUG DEFAULT_DEBUG
#endif /* #ifndef DEBUG */

#ifndef POSIX_PARALLEL
#  define POSIX_PARALLEL DEFAULT_POSIX_PARALLEL
#endif /* #ifndef POSIX_PARALLEL */

/* Flag to check for programmer errors.             */
/* This does not apply to errors of any other sort. */
#ifndef ERROR_CHECKING
#  define ERROR_CHECKING DEFAULT_ERROR_CHECKING
#endif /* #ifndef ERROR_CHECKING */

/* ---------------------------------------------------------------- */

#if DEBUG
#  define WHEN_DEBUG(a)                        a
#  define WHEN_NDEBUG(a)
#  define IF_DEBUG(    when_true,  when_false) when_true
#  define UNLESS_DEBUG(when_false, when_true)  when_true
#else  /* #if DEBUG */
#  define WHEN_DEBUG(a)
#  define WHEN_NDEBUG(a)                       a
#  define IF_DEBUG(    when_true,  when_false) when_false
#  define UNLESS_DEBUG(when_false, when_true)  when_false
#endif /* #if DEBUG */

#if POSIX_PARALLEL
#  define WHEN_POSIX_PARALLEL(a)                        a
#  define WHEN_NPOSIX_PARALLEL(a)
#  define IF_POSIX_PARALLEL(    when_true,  when_false) when_true
#  define UNLESS_POSIX_PARALLEL(when_false, when_true)  when_true
#else  /* #if POSIX_PARALLEL */
#  define WHEN_POSIX_PARALLEL(a)
#  define WHEN_NPOSIX_PARALLEL(a)                       a
#  define IF_POSIX_PARALLEL(    when_true,  when_false) when_false
#  define UNLESS_POSIX_PARALLEL(when_false, when_true)  when_false
#endif /* #if POSIX_PARALLEL */

#if ERROR_CHECKING
#  define WHEN_ERROR_CHECKING(a)                        a
#  define WHEN_NERROR_CHECKING(a)
#  define IF_ERROR_CHECKING(    when_true,  when_false) when_true
#  define UNLESS_ERROR_CHECKING(when_false, when_true)  when_true
#else  /* #if ERROR_CHECKING */
#  define WHEN_ERROR_CHECKING(a)
#  define WHEN_NERROR_CHECKING(a)                       a
#  define IF_ERROR_CHECKING(    when_true,  when_false) when_false
#  define UNLESS_ERROR_CHECKING(when_false, when_true)  when_false
#endif /* #if ERROR_CHECKING */

/* ---------------------------------------------------------------- */

#endif /* ifndef BASE_H */
