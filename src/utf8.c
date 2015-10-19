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
 * Encoding and decoding of UTF-8 as byte sequences, and codepoints as ints.
 *
 * codepoint_t  <-> utf-8
 * unsigned int <-> unsigned char *
 *
 * Implementation attempts to conform to http://www.ietf.org/rfc/rfc3629.txt .
 *
 * ----------------------------------------------------------------
 * -- Background.
 *
 * ASCII defines 128 characters by associating each one with a number.  ASCII
 * encodes these numbers in a byte: 0xxx xxxx, where the x's are the bits of
 * the number.
 *
 * For example, the byte 0x37 represents the character '7'.
 *
 * Unicode defines vastly more characters, but a byte is only 8 bits wide.  A
 * byte can only encode 256 unique values.  More bytes are needed to support
 * encoding unicode characters.
 *
 * Unicode uniquely assigns each character a number that represents the
 * character.  A unicode character's associated number is called a codepoint.
 *
 * Unicode codepoints range from U+0000 to U+10FFFF.
 *
 * For example, the codepoint U+2665 represents the character called "BLACK
 * HEART SUIT".
 *
 * ----------------------------------------------------------------
 * -- Unicode encodings.
 *
 * A unicode encoding specifies a way to convert a sequence of unicode
 * codepoints to a sequence a 8-bit bytes and visa versa.
 *
 * There are multiple common unicode encodings.
 *
 * This module implements one, caled UTF-8.
 *
 * ----------------------------------------------------------------
 * -- UTF-8.
 *
 * The UTF-8 encoding encodes each codepoint as 1-4 bytes.  Because a codepoint
 * can be encoded with a different number of bytes, it is called
 * "variable-width".
 *
 * UTF-8 is a superset of ASCII: all ascii characters have the same
 *
 * The procedures in this module allow users to convert between UTF-8 encoded
 * byte sequences, and sequences of codepoints (encoded as "unsigned int"s,
 * typedef'd here as codepoint_t).
 *
 * Thus a single unicode character can be decoded from a sequence of 1-4 bytes
 * by converting the sequence of bytes to a codepoint identifying which
 * character it is, and visa versa.
 *
 * ----------------------------------------------------------------
 * -- Important types.
 *
 * codepoint_t: unsigned int
 *   Type for codepoints, representing unicode characters.
 *
 * ----------------------------------------------------------------
 * -- Utf-8 implementation.
 *
 * This module implementation respects the various specified restrictions in
 * the UTF-8 definition, such as restricting codepoints to U+0000-U+10FFFF.
 *
 * The general conversion concept can be generalized to boundless integers, but
 * the main procedures of interest don't support this.  utf8_general_encode_one
 * and utf8_general_decode_one (TODO) implement the general principle for arbitrarily
 * large numbers.
 *
 * ----------------------------------------------------------------
 * -- UTF-8 format.
 *
 * Each codepoint is encoded as follows:
 *
 * If the first bit of the first byte is 0:
 *   Then the codepoint is encoded in 1 byte.
 *   The bits after the first bit encode the codepoint (0xxx xxxx).
 * Otherwise:
 *   Starting with the first bit of the first byte, possibly continuing into
 *   further bytes for sufficiently large bounds, consume each 1 bit until a 0
 *   bit is reached.  Count the number of 1-bits except for the first 1-bit of
 *   each byte.
 *
 *   This number of leading 1-bits is the number of continuation bytes that
 *   follow the byte containing the terminating 0 bit.
 *
 *   The bits remaining in this 0-terminating-bit-containing byte, if any,
 *   encode the first bits of the represented codepoint.  The 6 last bits of
 *   all the following continuation bytes encode 6 more bits of the codepoint.
 *
 * Thus in utf-8 each byte can be categorized into one of the following 3
 * kinds:
 *   * Single byte:       0___ ____
 *   * Counting byte:     11__ ____
 *   * Continuation byte: 10__ ____
 *
 * For example:
 *
 *   <Single byte>
 *   <0xxx xxxx>
 *   7 bits
 *   = 7 bits, encoded in 1 byte.
 *
 *   ----
 *   ----
 *
 *   1 leading 1-bits: 1 continuation bytes follow.
 *
 *   <Counting byte>     <Continuation byte>
 *   <110x xxxx>         <10xx xxxx>
 *   5 bits +            6 bits
 *   = 11 bits, encoded in 2 bytes.
 *
 *   ----
 *
 *   2 leading 1-bits: 2 continuation bytes follow.
 *
 *   <Counting byte>     <Continuation byte> <Continuation byte>
 *   <1110 xxxx>         <10xx xxxx>         <10xx xxxx>
 *   4 bits +            6 bits +            6 bits
 *   = 16 bits, encoded in 3 bytes.
 *
 *   ----
 *
 *   3 leading 1-bits: 3 continuation bytes follow.
 *
 *   <Counting byte>     <Continuation byte> <Continuation byte> <Continuation byte>
 *   <1111 0xxx>         <10xx xxxx>         <10xx xxxx>         <10xx xxxx>
 *   3 bits +            6 bits +            6 bits +            6 bits
 *   = 21 bits, encoded in 4 bytes.
 *
 *   ----
 *
 *   (Remember, don't count the first 1-bit of each counting byte as a leading
 *   1-bit.  So the number of continuation bytes is 7+4=11, not 8+5=13.)
 *
 *   7+4=11 leading 1-bits: 11 continuation bytes follow.
 *
 *   (7 leading 1-bits)  (4 leading 1-bits)  (contin. byte #1)   (contin. byte #2)
 *   <Counting byte>     <Counting byte>     <Continuation byte> <Continuation byte>
 *   <1111 1111>         <1111 10xx>         <10xx xxxx>         <10xx xxxx>
 *   0 bits +            2 bits +            6 bits +            6 bits +
 *
 *   (contin. byte #3)   (contin. byte #4)   (contin. byte #5)   (contin. byte #6)
 *   <Continuation byte> <Continuation byte> <Continuation byte> <Continuation byte>
 *   <10xx xxxx>         <10xx xxxx>         <10xx xxxx>         <10xx xxxx>
 *   6 bits +            6 bits +            6 bits +            6 bits
 *
 *   (contin. byte #7)   (contin. byte #8)   (contin. byte #9)   (contin. byte #10)
 *   <Continuation byte> <Continuation byte> <Continuation byte> <Continuation byte>
 *   <10xx xxxx>         <10xx xxxx>         <10xx xxxx>         <10xx xxxx>
 *   6 bits +            6 bits +            6 bits +            6 bits
 *
 *   (contin. byte #11)
 *   <Continuation byte>
 *   <10xx xxxx>
 *   6 bits
 *
 *   = 2 + 11*6 bits
 *
 *   = 68 bits, encoded in 13 bytes.
 *
 * ----------------------------------------------------------------
 * -- Overlong encoding.
 *
 * A codepoint can be encoded with an arbitrarily large number of
 * representations, by padding extra 0-bits.
 *
 * An overencoded input is one that encodes a codepoint with more bytes than is
 * needed.  The UTF-8 specification considers such input to be invalid, so we
 * need to check this.
 *
 * The above illustration helps explain how we can efficiently test for
 * overencoding.
 *
 * To extend the example, here's a table of utf-8 byte width and the
 * corresponding codepoint bit width:
 *
 * Once the UTF-8 byte width reaches 8, a second counting byte is introduced.
 *
 * The most codepoint bits a counting byte can encode is 5, where the counting
 * byte is "110x xxxx".
 *
 * __________________________________________________________________________
 * |-------------|-----------------------|------------|---------------------|
 * | UTF-8       | per-byte              | codepoint  | note                |
 * | byte width  | bit widths            | bit width  |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            |                     |
 * | 1           | 7                     | 7          | special case        |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 2           | 5 + 6                 | 11         | -1 bits (lead bit)  |
 * |             |                       |            | +5 bits (+counting) |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 3           | 4 + 6+6               | 16         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 4           | 3 + 6+6+6             | 21         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 5           | 2 + 6+6+6+6           | 26         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 6           | 1 + 6+6+6+6+6         | 31         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 7           | 0+5 + 6+6+6+6+6+6     | 41         | -1 bits (lead bit)  |
 * |             |                       |            | +5 bits (+counting) |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 8           | 0+4 + 6+6+6+6+6+6+6   | 46         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 * |             |                       |            | +6 bits (+contin.)  |
 * | 9           | 0+3 + 6+6+6+6+6+6+6+6 | 51         | -1 bits (lead bit)  |
 * |             |                       |            |                     |
 * |-------------|-----------------------|------------|---------------------|
 *
 * So we see that, starting with a UTF-8 byte with of 2 for 11 bits, every byte
 * adds 5 bits, and every 5 bytes adds an additional 5 bits.
 *
 * bits(1)           = 7
 * bits(2)           = 11
 * bits(bytes)       = bits(extra_bytes = bytes - 2)
 * bits(extra_bytes) = 11 + extra_bytes * 5 + (extra_bytes / 5) * 5
 *
 * As an example, we want to encode the codepoint 0x0F_FF_FF.  We need *at least*
 * 20 (4 + 8 + 8) bits, because of the position of the first 1 bit in our
 * codepoint 0x0F_FF_FF.
 *
 * Thus any UTF-8 byte width that encodes *at least* 24 bits can encode our
 * codepoint, because we can always add leading 0 bits.
 *
 * So our UTF-8 encoding needs at least 4 bytes.  (The shortest UTF-8 byte
 * width encoding at least 20 bits is 4 bytes, which encodes 21 bits.)
 *
 *
 * So given 1) the UTF-8 byte width, "width", 2) the encoded codepoint bits,
 * "bits", and 3) the codepoint it encodes, "codepoint", how can we determine
 * whether the encoding is overencoded?
 *
 * a) One simple way is to check if width-1 can also encode codepoint:
 *
 * greatest_codepoint(utf8_byte_width) = 2^(bits(utf8_byte_width)) - 1
 *
 * /8 (ignore "bits") 8/
 * is_overencoded(width, bits, codepoint) = codepoint <= greatest_codepoint(width-1)
 *
 * b) An even simpler way takes advantage of the fact that each byte adds
 * either 5 or 10 bits.
 *
 * If adding a byte results in a "width" such that "width"-2 is a multiple of
 * 5, then adding that byte adds 10 bits.
 *
 * Otherwise, it adds 5 bits.
 *
 * So if the first 5/10 bits, depending on these two cases, are all 0, then the
 * codepoint encoding is overencoded.
 *
 * /8 (ignore "codepoint") 8/
 * is_overencoded(1,     bits, codepoint) = 0
 * is_overencoded(2,     bits, codepoint) =
 *     bits == 0000 xxxx  ...
 * is_overencoded(width, bits, codepoint) =
 *   if (width-2) % 5 != 0:
 *     bits == 0000 0xxx  ...
 *   else:
 *     bits == 0000 0000  00xx xxxx  ...
 *
 * For example, a UTF-8 byte width of 7 adds 10 bits to the UTF-8 byte width of
 * 6.  So if the first 10 bits are 0, then we don't need those extra 10 bits.
 *
 * Because a byte width of 1 is a special case, we need to treat a byte-width
 * of 2 as a special case because it only encodes 4 more bits than an encoding
 * with a byte-width of 1.
 *
 * (A byte-width of 1 is the smallest possible width, so in this case the
 * codepoint is never overencoded.)
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

#include "util.h"

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
size_t utf8_encode(unsigned char *dest, size_t dest_max_size, const codepoint_t *input, size_t num_chars, size_t *out_num_encoded)
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


/*
 * utf8_decode_one: Decode a single codepoint.
 *
 * Input:
 *   const unsigned char *input:           UTF-8 bytes.
 *   size_t               input_max_size:  Max number of input bytes.
 *   utf8_decode_error_behaviour_t
 *                        error_behaviour: How to behave on invalid input.
 *
 * Output:
 *   (optional) size_t *out_width:
 *     Number of bytes assumed from the input to generate the parsed codepoint.
 *
 *     This value is always at most "input_max_size".
 *
 *     If more codepoints are to be decoded from the input stream, this value
 *     represents the distance at which decoding should resume.  For example, a
 *     value of "2" indicates that the next character should be parsed at 2
 *     bytes after the last position.
 *
 *     Note: this value is not necessarily the same as the number of bytes
 *     required to encode the returned codepoint, because on invalid input, a
 *     multi-byte replacement character could be returned where decoding should
 *     continue at the next byte.
 *
 *     Note: on invalid input, if input_max_size is 0, out_width will be 0.
 *
 * Return:
 *   codepoint_t: Decoded codepoint.
 *
 * If no bytes are available for input, 
 */
codepoint_t utf8_decode_one(const unsigned char *input, size_t input_max_size, utf8_decode_error_behaviour_t error_behaviour, size_t *out_width, utf8_decode_error_status_t *out_error_status)
{
  if (input_max_size < 1)
  {
    if (out_error_status)
      *out_error_status = utf8_decode_insufficient_continuation_bytes_eof;

    return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
  }

  /* ---------------------------------------------------------------- */
  /* 1 byte.                                                          */

  /* 0xxx xxxx                         */
  /* 1000 0000 & input[0] == 0000 0000 */
  if      ((0x80 & input[0]) == 0x00)
  {
    /* Expecting utf-8 1-byte-encoded codepoint. */
    /* 0xxxxxxx */

    if (out_error_status)   *out_error_status   = utf8_decode_no_error;

    if (out_width)          *out_width          = 1;

    /*
    return 0x7F & input[0];  /8 redundant 8/
    */
    return        input[0];
  }

  /* ---------------------------------------------------------------- */
  /* 2 bytes.                                                         */

  /* 110x xxxx                         */
  /* 1110 0000 & input[0] == 1100 0000 */
  else if ((0xE0 & input[0]) == 0xC0)
  {
    /* Expecting utf-8 2-byte-encoded codepoint. */
    /* 110x x xxx  10xxx xxx */

    /* Do we have enough input? */
    if (input_max_size < 2)
    {
      if (out_error_status)
        *out_error_status = utf8_decode_insufficient_continuation_bytes_eof;

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Are the remaining input bytes continuation bytes? */
    /* 10xx xxxx */
    /* 1100 0000 & input[i] == 1000 0000 */
    if ((0xC0 & input[1]) != 0x80)
    {
      if (out_error_status)
      {
        *out_error_status =
          utf8_decode_insufficient_continuation_bytes_sufficient_input;
      }

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Check for overencoding:
     *
     * A UTF-8 byte width of 2 encodes 4 more bits than a byte-width of 1, so
     * check whether the first 4 bits are 0.
     *
     * If they are, the codepoint encoding is overencoded, because the
     * codepoint can be encoded with one byte.
     */
    /* 0001 1110 & input[0] == 0 */
    if ((0x1E & input[0]) == 0)
    {
      if (out_error_status) *out_error_status = utf8_decode_overlong_encoding;

      if (!(error_behaviour & UTF8_DECODE_ERROR_PO_BIT))
        return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Write output. */
    else
    {
      if (out_error_status)   *out_error_status   = utf8_decode_no_error;
    }

    if (out_width)          *out_width          = 2;

    /* Extract bits. */
    return
      /*  110x xxxx */
      /* (0001 1111 & input[0]) << 6 */
      ((0x1F & input[0]) << 6)
      /*  10xx xxxx */
      /*  0011 1111 & input[1] */
    | (0x3F & input[1])
    ;
  }

  /* ---------------------------------------------------------------- */
  /* 3 bytes.                                                         */

  /* 1110 xxxx                         */
  /* 1111 0000 & input[0] == 1110 0000 */
  else if ((0xF0 & input[0]) == 0xE0)
  {
    /* Expecting utf-8 3-byte-encoded codepoint. */
    /* 1110 xxxx  10xx xxxx  10xx xxxx */

    /* Do we have enough input? */
    if (input_max_size < 3)
    {
      if (out_error_status)
        *out_error_status = utf8_decode_insufficient_continuation_bytes_eof;

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Are the remaining input bytes continuation bytes? */
    /* 10xx xxxx */
    /* 1100 0000 & input[i] == 1000 0000 */
    if
      (  (0xC0 & input[1]) != 0x80
      || (0xC0 & input[2]) != 0x80
      )
    {
      if (out_error_status)
      {
        *out_error_status =
          utf8_decode_insufficient_continuation_bytes_sufficient_input;
      }

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Check for overencoding:
     *
     * A UTF-8 byte width of 3 encodes 5 more bits than a byte-width of 2, so
     * check whether the first 5 bits are 0.
     *
     * If they are, the codepoint encoding is overencoded, because the
     * codepoint can be encoded with fewer bytes.
     */
    /* 0000 1111 & input[0] == 0 */
    /* 0010 0000 & input[1] == 0 */
    if
      (  (0x0F & input[0]) == 0
      && (0x20 & input[1]) == 0
      )
    {
      if (out_error_status) *out_error_status = utf8_decode_overlong_encoding;

      if (!(error_behaviour & UTF8_DECODE_ERROR_PO_BIT))
        return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Write output. */
    else
    {
      if (out_error_status)   *out_error_status   = utf8_decode_no_error;
    }

    if (out_width)          *out_width          = 3;

    /* Extract bits. */
    return
      /*  1110 xxxx                   */
      /* (0000 1111 & input[0]) << 12 */
      ((0x0F & input[0]) << 12)
      /*  10xx xxxx                   */
      /* (0011 1111 & input[1]) << 6  */
    | ((0x3F & input[1]) << 6)
      /*  10xx xxxx                   */
      /*  0011 1111 & input[2]        */
    | (0x3F & input[2])
    ;
  }

  /* ---------------------------------------------------------------- */
  /* 4 bytes.                                                         */

  /* 1111 0xxx                         */
  /* 1111 1000 & input[0] == 1111 0000 */
  else if ((0xF8 & input[0]) == 0xF0)
  {
    /* Expecting utf-8 4-byte-encoded codepoint. */
    /* 1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx */

    /* Do we have enough input? */
    if (input_max_size < 4)
    {
      if (out_error_status)
        *out_error_status = utf8_decode_insufficient_continuation_bytes_eof;

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Are the remaining input bytes continuation bytes? */
    /* 10xx xxxx */
    /* 1100 0000 & input[i] == 1000 0000 */
    if
      (  (0xC0 & input[1]) != 0x80
      || (0xC0 & input[2]) != 0x80
      || (0xC0 & input[3]) != 0x80
      )
    {
      if (out_error_status)
      {
        *out_error_status =
          utf8_decode_insufficient_continuation_bytes_sufficient_input;
      }

      return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Check for overencoding:
     *
     * A UTF-8 byte width of 4 encodes 5 more bits than a byte-width of 3, so
     * check whether the first 5 bits are 0.
     *
     * If they are, the codepoint encoding is overencoded, because the
     * codepoint can be encoded with fewer bytes.
     */
    /* 0000 0111 & input[0] == 0 */
    /* 0011 0000 & input[1] == 0 */
    if
      (  (0x07 & input[0]) == 0
      && (0x30 & input[1]) == 0
      )
    {
      if (out_error_status) *out_error_status = utf8_decode_overlong_encoding;

      if (!(error_behaviour & UTF8_DECODE_ERROR_PO_BIT))
        return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
    }

    /* Write output. */
    else
    {
      if (out_error_status)   *out_error_status   = utf8_decode_no_error;
    }

    if (out_width)          *out_width          = 4;

    /* Extract bits. */
    return
      /*  1111 0xxx                   */
      /* (0000 0111 & input[0]) << 18 */
      ((0x07 & input[0]) << 18)
      /*  10xx xxxx                   */
      /* (0011 1111 & input[1]) << 12 */
    | ((0x3F & input[1]) << 12)
      /*  10xx xxxx                   */
      /* (0011 1111 & input[2]) << 6  */
    | ((0x3F & input[2]) << 6)
      /*  10xx xxxx                   */
      /*  0011 1111 & input[3]        */
    | (0x3F & input[3])
    ;
  }

  /* ---------------------------------------------------------------- */
  /* Continuation byte.                                               */

  /* 10xx xxxx                         */
  /* 1100 0000 & input[0] == 1000 0000 */
  else if ((0xC0 & input[0]) == 0x80)
  {
    if (out_error_status)
      *out_error_status = utf8_decode_unexpected_continuation_byte;

    return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
  }

  /* ---------------------------------------------------------------- */
  /* >= 5 bytes.                                                      */

  else
  {
    if (out_error_status)
      *out_error_status = utf8_decode_out_of_bounds;

    return utf8_decode_one_erroneous(input, input_max_size, error_behaviour, out_width);
  }
}

/*
 * utf8_decode_one_erroneous: alternative decoding on arbitrary input.
 *
 * This is called on invalid input.  The codepoint returned depends on the
 * value of "error_behaviour".
 *
 * Input:
 *   unsigned char                 *input:
 *     Bytes to decode.
 *
 *   size_t                         input_max_size:
 *     Maximum number of bytes available for decoding.
 *
 *   utf8_decode_error_behaviour_t  error_behaviour:
 *     How to select a codepoint based on input.
 *
 *     See documentation for utf8_decode_error_behaviour_t for more details.
 *
 * Outut:
 *   size_t                        *out_width:
 *     Number of bytes from input interpreted to represent the codepoint.
 *
 *     This value is always at most "input_max_size".
 *
 *     For each of the currently supported behaviours, this will be 1 or 0,
 *     depending on whether input_max_size is positive.
 *
 *     This is different from the number of bytes required to *encode* the
 *     codepoint.  utf8_encode_one() with a NULL dest argument can be used to
 *     obtain this value.
 *
 *   size_t                        *out_bytes_consumed:
 *     Number of bytes read to represent codepoint.
 *
 * Return:
 *   codepoint_t:
 *     Codepoint derived from input, based on "error_behaviour".
 */
codepoint_t utf8_decode_one_erroneous(const unsigned char *input, size_t input_max_size, utf8_decode_error_behaviour_t error_behaviour, size_t *out_width)
{
  codepoint_t codepoint;

  /* If error_behaviour is unrecognized, set to default. */
  switch(error_behaviour)
  {
    case utf8_default_decode_error_behaviour:
    case utf8_stop_on_decode_error:
    default:
      error_behaviour = DEFAULT_UTF8_DECODE_ERRROR_BEHAVIOUR;
      break;

    case utf8_replacement_character_uFFFD_behaviour:
    case utf8_invalid_character_uDCxx_behaviour:
    case utf8_codepoint_u00xx_behaviour:
    case utf8_po_replacement_character_uFFFD_behaviour:
    case utf8_po_invalid_character_uDCxx_behaviour:
    case utf8_po_codepoint_u00xx_behaviour:
      break;
  }

  /* Set codepoint. */
  switch(error_behaviour)
  {
    default:
      report_bug("utf8_decode_one_erroneous: DEFAULT_UTF8_DECODE_ERRROR_BEHAVIOUR set to unrecognized value!  Proceeding anyway.");

    case utf8_replacement_character_uFFFD_behaviour:
    case utf8_po_replacement_character_uFFFD_behaviour:
      if(out_width) *out_width = (input_max_size >= 1 ? 1 : 0);
      codepoint = 0xFFFD;
      break;

    case utf8_invalid_character_uDCxx_behaviour:
    case utf8_po_invalid_character_uDCxx_behaviour:
      if(out_width) *out_width = (input_max_size >= 1 ? 1 : 0);
      codepoint = 0xDC80 | (input_max_size >= 1 ? *input : UTF8_DECODE_ERROR_INSUFFICIENT_INPUT_BYTE_VALUE);
      break;

    case utf8_codepoint_u00xx_behaviour:
    case utf8_po_codepoint_u00xx_behaviour:
      if(out_width) *out_width = (input_max_size >= 1 ? 1 : 0);
      codepoint = 0x0080 | (input_max_size >= 1 ? *input : UTF8_DECODE_ERROR_INSUFFICIENT_INPUT_BYTE_VALUE);
      break;
  }

  return codepoint;
}

/*
 * utf8_decode: decode a chunk of utf-8 encoded codepoints.
 *
 * Returns number of decoded *codepoints*.
 *
 * Input:
 *   codepoint_t                  *dest:                 Pointer to array of codepoints
 *   size_t                        dest_max_size:        Maximum number of codepoints to decode.
 *
 *   const unsigned char          *input:                UTF-8 encoded byte sequence to decode.
 *   size_t                        input_max_size:       Maxmimum number of bytes to decode.
 *
 *   int                           allow_trailing_bytes:
 *     How to handle premature end of input.
 *
 *     A chunk of UTF-8 encoded bytes might end in the middle of a multi-byte
 *     codepoint encoding.
 *
 *     Set this to true if more input might be available, and false if no more
 *     input is available.
 *
 *     When "utf8_decode_insufficient_continuation_bytes_eof" is encountered:
 *
 *     If true:
 *       - Don't parse trailing bytes.
 *       - Set "out_num_trailing_bytes" to number of trailing bytes if not
 *         NULL.
 *     If false:
 *       - Treat final bytes as invalid input, inserting a replacement
 *         character according to "error_behaviour".
 *
 *     When true, whether there are trailing bytes can be determined by
 *     checking whether the value of "out_num_trailing_bytes" is set to 0 or
 *     not.
 *
 *   utf8_decode_error_behaviour_t error_behaviour:
 *     What to do when invalid input is encountered.
 *
 *     Set to "utf8_stop_on_decode_error" to stop decoding and return on
 *     invalid input.
 *
 * Output:
 *   (optional) size_t                     *out_bytes_read:
 *      Total number of bytes read for each decoded codepoint.  (Bytes for
 *      invalid or incomplete input are not included.
 *
 *   (optional) size_t                     *out_num_trailing_bytes:
 *      If allow_trailing_bytes is "true", set to number of leftover bytes in
 *      case of premature end of input, otherwise set to 0.
 *
 *      If allow_trailing_bytes is "false", set to 0.
 *
 *   (optional) utf8_decode_error_status_t *out_sum_error_status:
 *      Indicates whether decoding was succesful.
 *
 *      On invalid UTF-8 input, this is set to the OR-sum of each error.
 *
 * Return:
 *   size_t:
 *     Number of decoded *codepoints*.
 *
 *     (The number of parsed bytes from the input is set to "out_bytes_read" if
 *     not NULL.
 */
size_t utf8_decode
  ( codepoint_t *dest, size_t dest_max_size, const unsigned char *input, size_t input_max_size, int allow_trailing_bytes, utf8_decode_error_behaviour_t error_behaviour
  , size_t *out_bytes_read, size_t *out_num_trailing_bytes, utf8_decode_error_status_t *out_sum_error_status
  )
{
  size_t num_bytes_read         = 0;
  size_t num_decoded_codepoints;

  utf8_decode_error_status_t error_status_sum = utf8_decode_no_error;

  for(num_decoded_codepoints = 0; num_decoded_codepoints < dest_max_size; ++num_decoded_codepoints)
  {
    size_t                     width;
    codepoint_t                codepoint;

    utf8_decode_error_status_t error_status;

    /* Have we reached the end of the input? */
    if (num_bytes_read >= input_max_size)
    {
      break;
    }

    /* Decode a codepoint. */
    codepoint =
      utf8_decode_one
        ( input + num_bytes_read, input_max_size - num_bytes_read, error_behaviour
        , &width, &error_status
        );

    /* Invalid input? */
    if (error_status != utf8_decode_no_error)
    {
      /* Trailing bytes? */
      if
        (  allow_trailing_bytes
        && error_status == utf8_decode_insufficient_continuation_bytes_eof
        )
      {
        if (out_num_trailing_bytes)
        {
          *out_num_trailing_bytes = input_max_size - num_bytes_read;
        }

        /* Stop decoding. */
        break;
      }
      else
      {
        error_status_sum |= error_status;

        /* Should we stop? */
        if (error_behaviour == utf8_stop_on_decode_error)
        {
          /* Stop decoding. */
          break;
        }
        else
        {
          /* Write replacement codepoint, and continue. */
          dest[num_decoded_codepoints]  = codepoint;
          num_bytes_read               += width;
        }
      }
    }
    else
    {
      /* Write decoded codepoint to output buffer ("dest"). */
      dest[num_decoded_codepoints]  = codepoint;
      num_bytes_read               += width;
    }
  }

  if (out_bytes_read)       *out_bytes_read       = num_bytes_read;
  if (out_sum_error_status) *out_sum_error_status = error_status_sum;

  return num_decoded_codepoints;
}
