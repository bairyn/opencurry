/*
 * opencurry: bits.c
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

#include "base.h"
#include "bits.h"

/* uint: unsigned int */

unsigned int on_uint(void)
{
  return ON();
}

unsigned int off_uint(void)
{
  return OFF();
}

unsigned int true_uint(void)
{
  return TRUE();
}

unsigned int false_uint(void)
{
  return FALSE();
}

unsigned int yes_uint(void)
{
  return YES();
}

unsigned int no_uint(void)
{
  return NO();
}

unsigned int enabled_uint(void)
{
  return ENABLED();
}

unsigned int disabled_uint(void)
{
  return DISABLED();
}

unsigned int enable_uint(void)
{
  return ENABLE();
}

unsigned int disable_uint(void)
{
  return DISABLE();
}

unsigned int get_bit_uint(unsigned int pos, unsigned int val)
{
  return GET_BIT(pos, val);
}

unsigned int set_bit_uint(unsigned int pos, unsigned int bit, unsigned int val)
{
  return SET_BIT(pos, bit, val);
}

unsigned int flip_bit_uint(unsigned int pos, unsigned int val)
{
  return FLIP_BIT(pos, val);
}

unsigned int bit_group_starts_uint(unsigned int val)
{
  return BIT_GROUP_STARTS(val);
}

unsigned int bit_group_ends_uint(unsigned int val)
{
  return BIT_GROUP_ENDS(val);
}

unsigned int bit_group_no_starts_uint(unsigned int val)
{
  return BIT_GROUP_NO_STARTS(val);
}

unsigned int bit_group_no_ends_uint(unsigned int val)
{
  return BIT_GROUP_NO_ENDS(val);
}

unsigned int bit_one_contagious_uint(unsigned int val)
{
  return BIT_ONE_CONTAGIOUS(val);
}

unsigned int bit_zero_contagious_uint(unsigned int val)
{
  return BIT_ZERO_CONTAGIOUS(val);
}

unsigned int bits_is_odd_uint(unsigned int val)
{
  return BITS_IS_ODD(val);
}

unsigned int bits_is_even_uint(unsigned int val)
{
  return BITS_IS_EVEN(val);
}

unsigned int is_nonzero_uint(unsigned int val)
{
  return IS_NONZERO(val);
}

unsigned int is_zero_uint(unsigned int val)
{
  return IS_ZERO(val);
}

unsigned int bit_nat_pred_uint(unsigned int num)
{
  return BIT_NAT_PRED(num);
}

unsigned int one_bit_repeat_uint(unsigned int num)
{
  return ONE_BIT_REPEAT(num);
}

unsigned long one_bit_repeat_ulong(unsigned long num)
{
  return ONE_BIT_REPEAT(num);
}
