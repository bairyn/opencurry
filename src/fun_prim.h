/*
 * opencurry: fun_prim.h
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
 * fun_prim.h
 * ------
 *
 * Primitive, unwrapped values.
 */


#ifndef FUN_PRIM_H
#define FUN_PRIM_H
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

/* Constants. */
extern const prim_t prim_void_val;

/* Constructors. */
prim_t prim_void(void);
prim_t prim_any(void *any);
prim_t prim_pointer(union prim_u *pointer);
prim_t prim_vchar(char vchar);
prim_t prim_schar(signed char schar);
prim_t prim_uchar(unsigned char uchar);
prim_t prim_vshort(short vshort);
prim_t prim_ushort(unsigned short ushort);
prim_t prim_vint(int vint);
prim_t prim_uint(unsigned int uint);
prim_t prim_vlong(int vlong);
prim_t prim_ulong(unsigned int ulong);
prim_t prim_vfloat(int vfloat);
prim_t prim_ufloat(unsigned int ufloat);
prim_t prim_vdouble(int vdouble);
prim_t prim_udouble(unsigned int udouble);
prim_t prim_next(union prim_u (*next)(union prim_u in));

/* Accessors. */

#endif /* ifndef FUN_PRIM_H */
