#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "multiformats/multibase/multibase.h"

/* The Base64 URL alphabet (RFC 4648, Table 2) */
static const char base64url_alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

/* Helper function mapping a Base64 URL character to its 6-bit value.
 * Returns 0–63 for a valid character or -1 if invalid.
 */
static inline int base64url_char_to_val(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9')
    {
        return c - '0' + 52;
    }
    if (c == '-')
    {
        return 62;
    }
    if (c == '_')
    {
        return 63;
    }
    return -1;
}

/**
 * @brief Encode data into Base64 URL format with padding using the URL- and
 *        filename-safe alphabet.
 *
 * @param data     The input data to be encoded.
 * @param data_len The length of the input data.
 * @param out      The buffer to store the encoded Base64 URL string.
 * @param out_len  The size of the output buffer.
 * @return The number of characters written (excluding the null terminator),
 *         or an error code indicating a null pointer, integer overflow, or
 *         insufficient buffer size.
 */
int multibase_base64_url_pad_encode(const uint8_t *data, size_t data_len, char *out, size_t out_len)
{
    if (data == NULL || out == NULL)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }
    if (data_len > SIZE_MAX - 2)
    {
        return MULTIBASE_ERR_OVERFLOW;
    }

    size_t groups = (data_len + 2) / 3;
    if (groups > SIZE_MAX / 4)
    {
        return MULTIBASE_ERR_OVERFLOW;
    }
    size_t encoded_len = groups * 4;
    if (out_len < encoded_len + 1)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    size_t i = 0;
    size_t j = 0;
    while (i + 3 <= data_len)
    {
        uint32_t triple = ((uint32_t)data[i] << 16) | ((uint32_t)data[i + 1] << 8) | (uint32_t)data[i + 2];
        out[j++] = base64url_alphabet[(triple >> 18) & 0x3F];
        out[j++] = base64url_alphabet[(triple >> 12) & 0x3F];
        out[j++] = base64url_alphabet[(triple >> 6) & 0x3F];
        out[j++] = base64url_alphabet[triple & 0x3F];
        i += 3;
    }

    size_t remainder = data_len - i;
    if (remainder == 1)
    {
        uint32_t triple = ((uint32_t)data[i]) << 16;
        out[j++] = base64url_alphabet[(triple >> 18) & 0x3F];
        out[j++] = base64url_alphabet[(triple >> 12) & 0x3F];
        out[j++] = '=';
        out[j++] = '=';
    }
    else if (remainder == 2)
    {
        uint32_t triple = (((uint32_t)data[i]) << 16) | (((uint32_t)data[i + 1]) << 8);
        out[j++] = base64url_alphabet[(triple >> 18) & 0x3F];
        out[j++] = base64url_alphabet[(triple >> 12) & 0x3F];
        out[j++] = base64url_alphabet[(triple >> 6) & 0x3F];
        out[j++] = '=';
    }

    out[j] = '\0';
    return (int)encoded_len;
}

/**
 * @brief Decode data from Base64 URL format with padding using the URL- and
 *        filename-safe alphabet.
 *
 * @param in       The input Base64 URL encoded string.
 * @param data_len The length of the input encoded data.
 * @param out      The buffer to store the decoded data.
 * @param out_len  The size of the output buffer.
 * @return The number of bytes written to the output buffer, or an error code
 *         indicating a null pointer, invalid input length, integer overflow,
 *         invalid character, or insufficient buffer size.
 */
int multibase_base64_url_pad_decode(const char *in, size_t data_len, uint8_t *out, size_t out_len)
{
    if (in == NULL || out == NULL)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }
    if (data_len == 0)
    {
        return 0;
    }
    if (data_len % 4 != 0)
    {
        return MULTIBASE_ERR_INVALID_INPUT_LEN;
    }

    size_t pad_count = 0;
    if (in[data_len - 1] == '=')
    {
        pad_count++;
    }
    if (in[data_len - 2] == '=')
    {
        pad_count++;
    }

    size_t groups = data_len / 4;
    if (groups > SIZE_MAX / 3)
    {
        return MULTIBASE_ERR_OVERFLOW;
    }
    size_t decoded_len = groups * 3 - pad_count;
    if (out_len < decoded_len)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    size_t i = 0;
    size_t j = 0;

    for (size_t group = 0; group < groups - 1; group++)
    {
        int v0 = base64url_char_to_val(in[i++]);
        int v1 = base64url_char_to_val(in[i++]);
        int v2 = base64url_char_to_val(in[i++]);
        int v3 = base64url_char_to_val(in[i++]);

        if (v0 == -1 || v1 == -1 || v2 == -1 || v3 == -1)
        {
            return MULTIBASE_ERR_INVALID_CHARACTER;
        }
        uint32_t triple = (v0 << 18) | (v1 << 12) | (v2 << 6) | v3;
        out[j++] = (triple >> 16) & 0xFF;
        out[j++] = (triple >> 8) & 0xFF;
        out[j++] = triple & 0xFF;
    }

    char c0 = in[i++];
    char c1 = in[i++];
    char c2 = in[i++];
    char c3 = in[i++];
    int v0 = base64url_char_to_val(c0);
    int v1 = base64url_char_to_val(c1);
    if (v0 == -1 || v1 == -1)
    {
        return MULTIBASE_ERR_INVALID_CHARACTER;
    }

    if (c2 == '=' && c3 == '=')
    {
        uint32_t triple = (v0 << 18) | (v1 << 12);
        out[j++] = (triple >> 16) & 0xFF;
    }
    else if (c3 == '=')
    {
        int v2 = base64url_char_to_val(c2);
        if (v2 == -1)
        {
            return MULTIBASE_ERR_INVALID_CHARACTER;
        }
        uint32_t triple = (v0 << 18) | (v1 << 12) | (v2 << 6);
        out[j++] = (triple >> 16) & 0xFF;
        out[j++] = (triple >> 8) & 0xFF;
    }
    else
    {
        int v2 = base64url_char_to_val(c2);
        int v3 = base64url_char_to_val(c3);
        if (v2 == -1 || v3 == -1)
        {
            return MULTIBASE_ERR_INVALID_CHARACTER;
        }
        uint32_t triple = (v0 << 18) | (v1 << 12) | (v2 << 6) | v3;
        out[j++] = (triple >> 16) & 0xFF;
        out[j++] = (triple >> 8) & 0xFF;
        out[j++] = triple & 0xFF;
    }
    return (int)decoded_len;
}