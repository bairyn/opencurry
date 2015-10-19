/*
 * opencurry: utf8.h
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
 * utf8.h
 * ------
 *
 * http://www.ietf.org/rfc/rfc3629.txt
 */


#ifndef UTF8_H
#define UTF8_H
/* stddef.h:
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

#include "unicode.h"

/* dest must support at least 4 bytes. */
size_t utf8_encode_one(unsigned char *dest, codepoint_t input);

/* Returns number of encoded bytes. */
/* If num_encoded is not null, sets number of encoded codepoints. */
size_t utf8_encode(unsigned char *dest, size_t dest_max_size, const codepoint_t *input, size_t num_chars, size_t *out_num_encoded);


/* https://en.wikipedia.org/wiki/UTF-8#Invalid_byte_sequences */
enum utf8_decode_error_status 
{
  /* No error. */
  utf8_decode_no_error                                         = 0,


  /* Premature end of input.
   *
   * More bytes need to be read to decode a codepoint, but input_max_size
   * indicates the buffer ends prematurely.
   *
   * The least significant bit can be checked to match both of these errors:
   *  - utf8_decode_insufficient_continuation_bytes_eof
   *  - utf8_decode_insufficient_continuation_bytes_sufficient_input
   *
   * (Note that if both conditions are satisfied, this value trumps; for
   * example, in case 3 continuation bytes are expected, 2 extra bytes are
   * available for input, but only the first of the 2 extra bytes is a
   * continuation byte.)
   */
  utf8_decode_insufficient_continuation_bytes_eof              = ((1 << 1) << 1) | 1,

  /*
  utf8_decode_red_invalid_byte                                 = ((1 << 2) << 1),
  */

  /* Too many continuation bytes. */
  utf8_decode_unexpected_continuation_byte                     = ((1 << 3) << 1),

  /* Not enough continuation bytes, but more input is available.
   *
   * The least significant bit can be checked to match both of these errors:
   *  - utf8_decode_insufficient_continuation_bytes_eof
   *  - utf8_decode_insufficient_continuation_bytes_sufficient_input
   */
  utf8_decode_insufficient_continuation_bytes_sufficient_input = ((1 << 4) << 1) | 1,

  /* The input represents a codepoint that could, thus must, be encoded with
   * fewer bytes.
   *
   * Without enforcing absense of overlong encoding of codepoints, then a
   * codepoint could be encoded with *at least* n bytes, rather than only with
   * n bytes.
   *
   * (On pain of security risks, utf8_decode_error_behaviour_t supports
   * potentially-unsafe values that cause decoding algorithms to return the
   * corresponding codepoint for overencoded codepoints anyway, but in this
   * case the error status is still set.
   */
  utf8_decode_overlong_encoding                                = ((1 << 5) << 1),

  /* A 4-byte sequences decodes to a value greater than 0x10FFFF. */
  utf8_decode_out_of_bounds                                    = ((1 << 6) << 1)
};
typedef enum utf8_decode_error_status utf8_decode_error_status_t;

/* What to do when decoding fails on invalid input.
 *
 * Values of this type determine which codepoint to return.
 *
 * If the width exceeds the available bytes of input, the bytes beyond what is
 * available from the input are treated as 0x00.
 *
 * Values are from https://en.wikipedia.org/wiki/UTF-8#Invalid_byte_sequences .
 */
#define UTF8_DECODE_ERROR_PO_BIT (1 << 1)
enum utf8_decode_error_behaviour
{
  /*
   * Tell decoding algorithms to select the default behaviour,
   * DEFAULT_UTF8_DECODE_ERRROR_BEHAVIOUR.
   */
  utf8_default_decode_error_behaviour           = 0,

  /*
   * When decoding a stream of codepoints, rather than just one,
   * stop decoding and return on invalid input.
   *
   * Otherwise, behave as though this were set to
   * "utf8_default_decode_error_behaviour".
   */
  utf8_stop_on_decode_error                     = 1,

  /* U+FFFD, width of 1.
   *
   * Replace the first invalid byte with the replacement character U+FFFD, and
   * continue parsing at the next byte.
   *
   * This is the default behaviour.
   */
  utf8_replacement_character_uFFFD_behaviour    = (2 << 2),

  /* U+DCxx, width of 1.
   *
   * This setting causes decoding procedures to replace the first byte of
   * invalid input with the invalid codepoint U+DCxx, where xx is the value of
   * the byte.
   *
   * (Alternatively, U+DC80 + 0x7F & value of byte.  Also, U+DC00 + first byte.)
   *
   * Since the first bit of an invalid byte must be 1, the resulting, invalid
   * codepoint will be in the range U+DC80 - U+DCFF.
   */
  utf8_invalid_character_uDCxx_behaviour        = (3 << 2),

  /* U+00xx, width of 1.
   *
   * Directly interpret the first byte of invalid input as the code point.
   */
  utf8_codepoint_u00xx_behaviour                = (4 << 2),


  /* Don't sanitize overencoded UTF-8 codepoints (security risk!):
   *
   * utf8_%_behaviour -> utf8_po_%_behaviour
   *
   * By padding with leading 0 bits, an arbitrarily large number of extra bytes
   * can be added to a UTF-8 encoding to represent the same codepoint.
   *
   * Normally UTF-8 forbids this, but these error behaviour values will cause
   * decoding algorithms to return the encoded codepoint anyway (while still
   * setting the appropriate error status.)
   *
   * (For example, if a codepoint requires at least 2 encoded bytes to be
   * encoded in UTF-8, then the codepoint can be encoded with not just 2 bytes,
   * but with any number of bytes greater than 2 also (2, 3, 7, 42, 1024,
   * etc.))
   */
  utf8_po_replacement_character_uFFFD_behaviour = (2 << 2) | UTF8_DECODE_ERROR_PO_BIT,
  utf8_po_invalid_character_uDCxx_behaviour     = (3 << 2) | UTF8_DECODE_ERROR_PO_BIT,
  utf8_po_codepoint_u00xx_behaviour             = (4 << 2) | UTF8_DECODE_ERROR_PO_BIT
};
typedef enum utf8_decode_error_behaviour utf8_decode_error_behaviour_t;
#define DEFAULT_UTF8_DECODE_ERRROR_BEHAVIOUR            utf8_replacement_character_uFFFD_behaviour
#define UTF8_DECODE_ERROR_INSUFFICIENT_INPUT_BYTE_VALUE 0x00

codepoint_t utf8_decode_one(const unsigned char *input, size_t input_max_size, utf8_decode_error_behaviour_t error_behaviour, size_t *out_width, utf8_decode_error_status_t *out_error_status);

codepoint_t utf8_decode_one_erroneous(const unsigned char *input, size_t input_max_size, utf8_decode_error_behaviour_t error_behaviour, size_t *out_width);


/* Returns number of decoded codepoints. */
size_t utf8_decode
  ( codepoint_t *dest, size_t dest_max_size, const unsigned char *input, size_t input_max_size, int allow_trailing_bytes, utf8_decode_error_behaviour_t error_behaviour
  , size_t *outnum__bytes_read, size_t *out_num_trailing_bytes, utf8_decode_error_status_t *out_error_status_sum
  );

#endif /* ifndef UTF8_H */
