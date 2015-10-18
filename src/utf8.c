/*
 * opencurry: utf8.c
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
 * utf8.c
 * ------
 *
 * http://www.ietf.org/rfc/rfc3629.txt
 */

/* stddef.h:
 *   - size_t
 */
#include <stddef.h>

/* string.h:
 *   - memcpy
 */
#include <string.h>

#include "base.h"
#include "utf8.h"

#include "unicode.h"

/* utf8_encode_one:
 *
 * Encode a single unicode codepoint in UTF-8, writing 1-4 bytes.
 *
 * Input:
 *  codepoint_t input:
 *   unicode codepoint for character to encode.
 *
 * Output:
 *  char *dest (optional):
 *   1-4 bytes will be written to this address.
 *
 *   This can be set to NULL to obtain the number of bytes required for an
 *   encoding, without writing an encoding.
 *
 * Return:
 *  Number of bytes the encoding requires.  Will be 1-4.
 */
size_t utf8_encode_one(unsigned char *dest, codepoint_t input)
{
  if      (input <=  0x7F)
  {
    if (dest)
    {
      /* <Byte: 0> */
      /* 0xxx xxxx */

      /* ---------------------------------------------------------------- */
      /* Byte 0 */

      /* 0xxx xxxx                            */
      /* 0111 1111 & input                    */
      /* 0x7F      & input                    */
      /*             input (input is <= 0x7F) */
      dest[0] = input;
    }

    return 1;
  }
  else if (input <= 0x7FF)
  {
    if (dest)
    {
      /* <Byte: 0>  <Byte: 1> */
      /* 110x xxxx  10xx xxxx */

      /* ---------------------------------------------------------------- */
      /* Byte 1 */

      /* 10xx xxxx                       */
      /* 1000 0000 | (0011 1111 & input) */
      /* 0x80      | (0x3F      & input) */
      dest[1] = 0x80 | (0x3F & input);

      /* ---------------------------------------------------------------- */
      /* Byte 0 */

      /* 110x xxxx                              */
      /* 1100 0000 | (0001 1111 & (input >> 6)) */
      /* 0xC0      | (0x1F      & (input >> 6)) */
      dest[0] = 0xC0 | (0x1F & (input >> 6));
    }

    return 2;
  }
  else if (input <= 0xFFFF)
  {
    if (dest)
    {
      /* <Byte: 0>  <Byte: 1>  <Byte: 2> */
      /* 1110 xxxx  10xx xxxx  10xx xxxx */

      /* ---------------------------------------------------------------- */
      /* Byte 2 */

      /* 10xx xxxx                       */
      /* 1000 0000 | (0011 1111 & input) */
      /* 0x80      | (0x3F      & input) */
      dest[2] = 0x80 | (0x3F & input);

      /* ---------------------------------------------------------------- */
      /* Byte 1 */

      /* 10xx xxxx                              */
      /* 1000 0000 | (0011 1111 & (input >> 6)) */
      /* 0x80      | (0x3F      & (input >> 6)) */
      dest[1] = 0x80 | (0x3F & (input >> 6));

      /* ---------------------------------------------------------------- */
      /* Byte 0 */

      /* 1110 xxxx                               */
      /* 1110 0000 | (0000 1111 & (input >> 12)) */
      /* 0xE0      | (0x0F      & (input >> 12)) */
      dest[0] = 0xE0 | (0x0F & (input >> 12));
    }

    return 3;
  }
  else
  {
    if (dest)
    {
      /* <Byte: 0>  <Byte: 1>  <Byte: 2>  <Byte: 3> */
      /* 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx */

      /* ---------------------------------------------------------------- */
      /* Byte 3 */

      /* 10xx xxxx                       */
      /* 1000 0000 | (0011 1111 & input) */
      /* 0x80      | (0x3F      & input) */
      dest[3] = 0x80 | (0x3F & input);

      /* ---------------------------------------------------------------- */
      /* Byte 2 */

      /* 10xx xxxx                              */
      /* 1000 0000 | (0011 1111 & (input >> 6)) */
      /* 0x80      | (0x3F      & (input >> 6)) */
      dest[2] = 0x80 | (0x3F & (input >> 6));

      /* ---------------------------------------------------------------- */
      /* Byte 1 */

      /* 10xx xxxx                               */
      /* 1000 0000 | (0011 1111 & (input >> 12)) */
      /* 0x80      | (0x3F      & (input >> 12)) */
      dest[1] = 0x80 | (0x3F & (input >> 12));

      /* ---------------------------------------------------------------- */
      /* Byte 0 */

      /* 1111 0xxx                               */
      /* 1111 0000 | (0000 0111 & (input >> 18)) */
      /* 0xF0      | (0x07      & (input >> 18)) */
      dest[0] = 0xF0 | (0x07 & (input >> 18));
    }

    return 4;
  }
}

/*
 * utf8_encode:
 *
 * Encode a chunk of codepoints, invoking utf8_encode_one in succession.
 *
 * Input:
 *             unsigned char *dest:            Buffer to write to 
 *             size_t         dest_max_size:   Maximum number of bytes to write.
 *             codepoint_t   *input:           Array of codepoints to encode.
 *             size_t         num_chars:       Maximum number of input codepoints to write.
 *
 * Output:
 *  (optional) size_t        *out_num_encoded: Number of encoded codepoints.
 *
 * Return:
 *             size_t:                         Number of bytes written.
 */
size_t utf8_encode(unsigned char *dest, size_t dest_max_size, codepoint_t *input, size_t num_chars, size_t *out_num_encoded)
{
  size_t codepoints_written;

  size_t buf_written        = 0;

  for (codepoints_written = 0; codepoints_written < num_chars; ++codepoints_written)
  {
    size_t        width;
    unsigned char buf[4];

    width = utf8_encode_one(buf, input[codepoints_written]);

    if(buf_written + width > dest_max_size)
    {
      break;
    }
    else
    {
      memcpy((void *) (dest + buf_written), (const void *) buf, (size_t) width);
      buf_written += width;
    }
  }

  if (out_num_encoded)
  {
    *out_num_encoded = codepoints_written;
  }

  return buf_written;
}
