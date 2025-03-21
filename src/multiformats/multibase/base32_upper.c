#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "multiformats/multibase/multibase.h"

/* The upper-case Base32 alphabet (RFC 4648, Table 3) */
static const char base32_upper_alphabet[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

/**
 * @brief Encode data into a Base32 format using uppercase letters.
 *
 * This implementation is agnostic to any multibase prefixing.
 *
 * @param data The input data to be encoded.
 * @param data_len The length of the input data.
 * @param out The buffer to store the Base32 encoded string.
 * @param out_len The size of the output buffer.
 * @return The number of characters written to the output buffer,
 *         or an error code indicating a null pointer or insufficient buffer size.
 */
int base32_upper_encode(const uint8_t *data, size_t data_len, char *out, size_t out_len)
{
    size_t required;
    size_t full_blocks, rem;
    size_t i, j;
    size_t pos = 0;

    if (!data || !out)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }

    if (data_len == 0)
    {
        if (out_len < 1)
        {
            return MULTIBASE_ERR_BUFFER_TOO_SMALL;
        }
        out[0] = '\0';
        return 0;
    }

    full_blocks = data_len / 5;
    rem = data_len % 5;
    required = (((full_blocks + (rem ? 1 : 0)) * 8)) + 1;
    if (out_len < required)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    for (i = 0; i < full_blocks; i++)
    {
        const uint8_t *chunk = data + (i * 5);
        uint8_t index0 = chunk[0] >> 3;
        uint8_t index1 = ((chunk[0] & 0x07) << 2) | (chunk[1] >> 6);
        uint8_t index2 = (chunk[1] >> 1) & 0x1F;
        uint8_t index3 = ((chunk[1] & 0x01) << 4) | (chunk[2] >> 4);
        uint8_t index4 = ((chunk[2] & 0x0F) << 1) | (chunk[3] >> 7);
        uint8_t index5 = (chunk[3] >> 2) & 0x1F;
        uint8_t index6 = ((chunk[3] & 0x03) << 3) | (chunk[4] >> 5);
        uint8_t index7 = chunk[4] & 0x1F;

        out[pos++] = base32_upper_alphabet[index0];
        out[pos++] = base32_upper_alphabet[index1];
        out[pos++] = base32_upper_alphabet[index2];
        out[pos++] = base32_upper_alphabet[index3];
        out[pos++] = base32_upper_alphabet[index4];
        out[pos++] = base32_upper_alphabet[index5];
        out[pos++] = base32_upper_alphabet[index6];
        out[pos++] = base32_upper_alphabet[index7];
    }

    if (rem)
    {
        uint8_t tail[5] = {0, 0, 0, 0, 0};
        for (j = 0; j < rem; j++)
        {
            tail[j] = data[full_blocks * 5 + j];
        }
        uint8_t indices[8];
        indices[0] = tail[0] >> 3;
        if (rem == 1)
        {
            indices[1] = (tail[0] & 0x07) << 2;
        }
        else
        {
            indices[1] = ((tail[0] & 0x07) << 2) | (tail[1] >> 6);
            indices[2] = (tail[1] >> 1) & 0x1F;
            if (rem == 2)
            {
                indices[3] = (tail[1] & 0x01) << 4;
            }
            else
            {
                indices[3] = ((tail[1] & 0x01) << 4) | (tail[2] >> 4);
                indices[4] = ((tail[2] & 0x0F) << 1);
                if (rem == 4)
                {
                    indices[4] |= (tail[3] >> 7);
                    indices[5] = (tail[3] >> 2) & 0x1F;
                    indices[6] = ((tail[3] & 0x03) << 3) | (tail[4] >> 5);
                }
            }
        }
        size_t valid_chars;
        if (rem == 1)
        {
            valid_chars = 2;
        }
        else if (rem == 2)
        {
            valid_chars = 4;
        }
        else if (rem == 3)
        {
            valid_chars = 5;
        }
        else
        {
            valid_chars = 7;
        }

        for (j = 0; j < valid_chars; j++)
        {
            out[pos++] = base32_upper_alphabet[indices[j]];
        }
        for (j = valid_chars; j < 8; j++)
        {
            out[pos++] = '=';
        }
    }

    out[pos] = '\0';
    return pos;
}

/**
 * @brief Decode a Base32 encoded string using uppercase letters.
 *
 * @param in The input Base32 encoded string.
 * @param out The buffer to store the decoded binary data.
 * @param out_len The size of the output buffer.
 * @return The number of bytes written to the output buffer,
 *         or an error code indicating a null pointer, insufficient buffer size, or invalid input.
 */
int base32_upper_decode(const char *in, size_t data_len, uint8_t *out, size_t out_len)
{
    size_t in_len, pos = 0;
    size_t i, j;
    size_t decoded_len = 0;

    if (!in || !out)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }

    if (data_len == 0)
    {
        return 0;
    }

    in_len = data_len;

    if (in_len % 8 != 0)
    {
        return MULTIBASE_ERR_INVALID_INPUT_LEN;
    }

    uint8_t decode_table[256];
    for (i = 0; i < 256; i++)
    {
        decode_table[i] = 0xFF;
    }
    for (i = 0; i < 26; i++)
    {
        decode_table[(unsigned char)('A' + i)] = i;
    }
    for (i = 0; i < 6; i++)
    {
        decode_table[(unsigned char)('2' + i)] = 26 + i;
    }

    size_t blocks = in_len / 8;
    for (i = 0; i < blocks; i++)
    {
        const char *block = in + i * 8;
        size_t valid_chars = 0;
        for (j = 0; j < 8; j++)
        {
            if (block[j] == '=')
            {
                break;
            }
            valid_chars++;
        }
        if (i < blocks - 1 && valid_chars != 8)
        {
            return MULTIBASE_ERR_INVALID_INPUT_LEN;
        }
        if (valid_chars == 8)
        {
            decoded_len += 5;
        }
        else if (valid_chars == 7)
        {
            decoded_len += 4;
        }
        else if (valid_chars == 5)
        {
            decoded_len += 3;
        }
        else if (valid_chars == 4)
        {
            decoded_len += 2;
        }
        else if (valid_chars == 2)
        {
            decoded_len += 1;
        }
        else
        {
            return MULTIBASE_ERR_INVALID_INPUT_LEN;
        }
        for (; j < 8; j++)
        {
            if (block[j] != '=')
            {
                return MULTIBASE_ERR_INVALID_INPUT_LEN;
            }
        }
    }

    if (decoded_len > out_len)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    for (i = 0; i < blocks; i++)
    {
        const char *block = in + i * 8;
        uint8_t indices[8];
        size_t valid_chars = 0;
        for (j = 0; j < 8; j++)
        {
            if (block[j] == '=')
            {
                break;
            }
            uint8_t val = decode_table[(unsigned char)block[j]];
            if (val == 0xFF)
            {
                return MULTIBASE_ERR_INVALID_CHARACTER;
            }
            indices[j] = val;
            valid_chars++;
        }

        if (valid_chars == 8)
        {
            out[pos++] = (indices[0] << 3) | (indices[1] >> 2);
            out[pos++] = ((indices[1] & 0x03) << 6) | (indices[2] << 1) | (indices[3] >> 4);
            out[pos++] = ((indices[3] & 0x0F) << 4) | (indices[4] >> 1);
            out[pos++] = ((indices[4] & 0x01) << 7) | (indices[5] << 2) | (indices[6] >> 3);
            out[pos++] = ((indices[6] & 0x07) << 5) | indices[7];
        }
        else if (valid_chars == 7)
        {
            out[pos++] = (indices[0] << 3) | (indices[1] >> 2);
            out[pos++] = ((indices[1] & 0x03) << 6) | (indices[2] << 1) | (indices[3] >> 4);
            out[pos++] = ((indices[3] & 0x0F) << 4) | (indices[4] >> 1);
            out[pos++] = ((indices[4] & 0x01) << 7) | (indices[5] << 2) | (indices[6] >> 3);
        }
        else if (valid_chars == 5)
        {
            out[pos++] = (indices[0] << 3) | (indices[1] >> 2);
            out[pos++] = ((indices[1] & 0x03) << 6) | (indices[2] << 1) | (indices[3] >> 4);
            out[pos++] = ((indices[3] & 0x0F) << 4) | (indices[4] >> 1);
        }
        else if (valid_chars == 4)
        {
            out[pos++] = (indices[0] << 3) | (indices[1] >> 2);
            out[pos++] = ((indices[1] & 0x03) << 6) | (indices[2] << 1) | (indices[3] >> 4);
        }
        else if (valid_chars == 2)
        {
            out[pos++] = (indices[0] << 3) | (indices[1] >> 2);
        }
    }
    return pos;
}