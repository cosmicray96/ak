#include "ak/coll/str32.h"

// Gened by Claude.
int32_t
ak_codepoint_from_utf8(const uint8_t* utf,
                       ak_codepoint* o_cp)
{
  if (!utf || !o_cp)
    return -1;

  uint8_t b0 = utf[0];

  /* 1-byte sequence: 0xxxxxxx */
  if ((b0 & 0x80) == 0x00) {
    *o_cp = b0;
    return 1;
  }

  /* 2-byte sequence: 110xxxxx 10xxxxxx */
  if ((b0 & 0xE0) == 0xC0) {
    uint8_t b1 = utf[1];
    if ((b1 & 0xC0) != 0x80)
      return -1;

    *o_cp =
      ((ak_codepoint)(b0 & 0x1F) << 6) |
      (ak_codepoint)(b1 & 0x3F);

    if (*o_cp < 0x0080)
      return -1; /* overlong */
    return 2;
  }

  /* 3-byte sequence: 1110xxxx 10xxxxxx
   * 10xxxxxx */
  if ((b0 & 0xF0) == 0xE0) {
    uint8_t b1 = utf[1];
    uint8_t b2 = utf[2];
    if ((b1 & 0xC0) != 0x80)
      return -1;
    if ((b2 & 0xC0) != 0x80)
      return -1;

    *o_cp =
      ((ak_codepoint)(b0 & 0x0F) << 12) |
      ((ak_codepoint)(b1 & 0x3F) << 6) |
      (ak_codepoint)(b2 & 0x3F);

    if (*o_cp < 0x0800)
      return -1; /* overlong */
    if (*o_cp >= 0xD800 && *o_cp <= 0xDFFF)
      return -1; /* surrogate half */
    return 3;
  }

  /* 4-byte sequence: 11110xxx 10xxxxxx
   * 10xxxxxx 10xxxxxx */
  if ((b0 & 0xF8) == 0xF0) {
    uint8_t b1 = utf[1];
    uint8_t b2 = utf[2];
    uint8_t b3 = utf[3];
    if ((b1 & 0xC0) != 0x80)
      return -1;
    if ((b2 & 0xC0) != 0x80)
      return -1;
    if ((b3 & 0xC0) != 0x80)
      return -1;

    *o_cp =
      ((ak_codepoint)(b0 & 0x07) << 18) |
      ((ak_codepoint)(b1 & 0x3F) << 12) |
      ((ak_codepoint)(b2 & 0x3F) << 6) |
      (ak_codepoint)(b3 & 0x3F);

    if (*o_cp < 0x010000)
      return -1; /* overlong */
    if (*o_cp > 0x10FFFF)
      return -1; /* beyond Unicode range */
    return 4;
  }

  return -1; /* invalid leading byte */
}

int32_t
ak_utf8_from_codepoint(ak_codepoint cp,
                       uint8_t* o_utf8)
{
  if (!o_utf8)
    return -1;

  /* 1-byte: U+0000 - U+007F */
  if (cp <= 0x007F) {
    o_utf8[0] = (uint8_t)cp;
    return 1;
  }

  /* 2-byte: U+0080 - U+07FF */
  if (cp <= 0x07FF) {
    o_utf8[0] = (uint8_t)(0xC0 | (cp >> 6));
    o_utf8[1] =
      (uint8_t)(0x80 | (cp & 0x3F));
    return 2;
  }

  /* reject surrogate halves */
  if (cp >= 0xD800 && cp <= 0xDFFF)
    return -1;

  /* 3-byte: U+0800 - U+FFFF */
  if (cp <= 0xFFFF) {
    o_utf8[0] = (uint8_t)(0xE0 | (cp >> 12));
    o_utf8[1] =
      (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
    o_utf8[2] =
      (uint8_t)(0x80 | (cp & 0x3F));
    return 3;
  }

  /* 4-byte: U+10000 - U+10FFFF */
  if (cp <= 0x10FFFF) {
    o_utf8[0] = (uint8_t)(0xF0 | (cp >> 18));
    o_utf8[1] =
      (uint8_t)(0x80 | ((cp >> 12) & 0x3F));
    o_utf8[2] =
      (uint8_t)(0x80 | ((cp >> 6) & 0x3F));
    o_utf8[3] =
      (uint8_t)(0x80 | (cp & 0x3F));
    return 4;
  }

  return -1; /* above U+10FFFF */
}

bool
ak_codepoint_valid(ak_codepoint cp)
{
  if (cp > 0x10FFFF)
    return false; /* above Unicode range */
  if (cp >= 0xD800 && cp <= 0xDFFF)
    return false; /* surrogate halves */
  return true;
}
