/*
 * opencurry: fun.h
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
 * fun.h
 * ------
 */


#ifndef FUN_H
#define FUN_H
#include "base.h"

/* Non-function values. */
union prim_u
{
  void           *any;

  union prim_u   *pointer;

  char           vchar;
  signed char    schar;
  unsigned char  uchar;

  /* - short
   * - short int
   * - signed short
   * - signed short int
   */
  short          vshort;

  /* - unsigned short
   * - unsigned short int
   */
  unsigned short ushort;

  /* - int
   * - signed
   * - signed int
   */
  int            vint;

  /* - unsigned
   * - unsigned int
   */
  unsigned int   uint;

  /* - long
   * - long int
   * - signed long
   * - signed long int
   */
  long           vlong;

  /* - unsigned long
   * - unsigned long int
   */
  unsigned long  ulong;

  float          vfloat;
  double         vdouble;
  /* long double    ldouble; */

  union prim_u   (*next)(union prim_u in);
};
typedef union prim_u prim_t;

struct primpair_s
{
  union prim_u a;
  union prim_u b;
};
typedef struct primpair_s primpair_t;

/*
 * Functions and values:
 *
 * A val_t can be either a primitive value or a function.
 */
struct fun_s
{
  union  prim_u  out;
  struct fun_s  (*fun)(struct fun_s in);

  union  prim_u  state;

  struct primpair_s user0;
  struct primpair_s user1;
};
typedef struct fun_s fun_t;

typedef fun_t any_t;
typedef fun_t val_t;

#endif /* ifndef FUN_H */
