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

int cmp_case_int(int check, int baseline, int when_lt, int when_eq, int when_gt)
{
  return CMP_CASE(check, baseline, when_lt, when_eq, when_gt);
}

unsigned int cmp_case_uint(unsigned int check, unsigned int baseline, unsigned int when_lt, unsigned int when_eq, unsigned int when_gt)
{
  return CMP_CASE(check, baseline, when_lt, when_eq, when_gt);
}

unsigned long int cmp_case_ulong(unsigned long check, unsigned long baseline, unsigned long when_lt, unsigned long when_eq, unsigned long when_gt)
{
  return CMP_CASE(check, baseline, when_lt, when_eq, when_gt);
}

int cmp_int(int check, int baseline)
{
  return CMP(check, baseline);
}

int cmp_uint(unsigned int check, unsigned int baseline)
{
  return CMP(check, baseline);
}

int cmp_ulong(unsigned long check, unsigned long baseline)
{
  return CMP(check, baseline);
}

int sign_case_int(int num, int when_negative, int when_zero, int when_positive)
{
  return SIGN_CASE(num, when_negative, when_zero, when_positive);
}

long sign_case_long(long num, long when_negative, long when_zero, long when_positive)
{
  return SIGN_CASE(num, when_negative, when_zero, when_positive);
}

int sign_int(int num)
{
  return SIGN(num);
}

long sign_long(long num)
{
  return SIGN(num);
}


int cmp_distance_int(int check, int baseline)
{
  return CMP_DISTANCE(check, baseline);
}

int cmp_distance_uint(unsigned int check, unsigned int baseline)
{
  return CMP_DISTANCE(check, baseline);
}

int cmp_distance_ulong(unsigned long check, unsigned long baseline)
{
  return CMP_DISTANCE(check, baseline);
}

/* ---------------------------------------------------------------- */

const int most_significant_bit_pos_byte_map[1 << 8] =
  MOST_SIGNIFICANT_BIT_POS_BYTE_MAP;

const int           max_byte_bits =
  MAX_BYTE_BITS;
const unsigned char max_byte      =
  MAX_BYTE;

/* Get the position of the most significant bit. */
int most_significant_bit_pos_char(char num)
{
  return most_significant_bit_pos_uchar((unsigned char) (int) num);
}
int most_significant_bit_pos_schar(signed char num)
{
  return most_significant_bit_pos_uchar((unsigned char) (int) num);
}

int most_significant_bit_pos_uchar(unsigned char num)
{
  return most_significant_bit_pos_byte_map[num];
}

int most_significant_bit_pos_ulong(unsigned long num)
{
  int pos = 0;

  for (pos = 0; num > MAX_BYTE; pos += MAX_BYTE_BITS, num >>= MAX_BYTE_BITS)
    ;

  return pos + most_significant_bit_pos_uchar((unsigned char) (num & MAX_BYTE));
}
