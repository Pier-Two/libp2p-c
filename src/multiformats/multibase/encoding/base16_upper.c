#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "multiformats/multibase/multibase.h"

/* Determine system endianness */
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define BIG_ENDIAN_SYSTEM 1
#else
#define BIG_ENDIAN_SYSTEM 0
#endif

/* Little‑endian lookup table for uppercase encoding */
static const uint16_t hex_lookup_le_upper[256] = {
    0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930, 0x4130, 0x4230,
    0x4330, 0x4430, 0x4530, 0x4630, 0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731,
    0x3831, 0x3931, 0x4131, 0x4231, 0x4331, 0x4431, 0x4531, 0x4631, 0x3032, 0x3132, 0x3232, 0x3332,
    0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932, 0x4132, 0x4232, 0x4332, 0x4432, 0x4532, 0x4632,
    0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733, 0x3833, 0x3933, 0x4133, 0x4233,
    0x4333, 0x4433, 0x4533, 0x4633, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734,
    0x3834, 0x3934, 0x4134, 0x4234, 0x4334, 0x4434, 0x4534, 0x4634, 0x3035, 0x3135, 0x3235, 0x3335,
    0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935, 0x4135, 0x4235, 0x4335, 0x4435, 0x4535, 0x4635,
    0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x4136, 0x4236,
    0x4336, 0x4436, 0x4536, 0x4636, 0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737,
    0x3837, 0x3937, 0x4137, 0x4237, 0x4337, 0x4437, 0x4537, 0x4637, 0x3038, 0x3138, 0x3238, 0x3338,
    0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938, 0x4138, 0x4238, 0x4338, 0x4438, 0x4538, 0x4638,
    0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839, 0x3939, 0x4139, 0x4239,
    0x4339, 0x4439, 0x4539, 0x4639, 0x3041, 0x3141, 0x3241, 0x3341, 0x3441, 0x3541, 0x3641, 0x3741,
    0x3841, 0x3941, 0x4141, 0x4241, 0x4341, 0x4441, 0x4541, 0x4641, 0x3042, 0x3142, 0x3242, 0x3342,
    0x3442, 0x3542, 0x3642, 0x3742, 0x3842, 0x3942, 0x4142, 0x4242, 0x4342, 0x4442, 0x4542, 0x4642,
    0x3043, 0x3143, 0x3243, 0x3343, 0x3443, 0x3543, 0x3643, 0x3743, 0x3843, 0x3943, 0x4143, 0x4243,
    0x4343, 0x4443, 0x4543, 0x4643, 0x3044, 0x3144, 0x3244, 0x3344, 0x3444, 0x3544, 0x3644, 0x3744,
    0x3844, 0x3944, 0x4144, 0x4244, 0x4344, 0x4444, 0x4544, 0x4644, 0x3045, 0x3145, 0x3245, 0x3345,
    0x3445, 0x3545, 0x3645, 0x3745, 0x3845, 0x3945, 0x4145, 0x4245, 0x4345, 0x4445, 0x4545, 0x4645,
    0x3046, 0x3146, 0x3246, 0x3346, 0x3446, 0x3546, 0x3646, 0x3746, 0x3846, 0x3946, 0x4146, 0x4246,
    0x4346, 0x4446, 0x4546, 0x4646};

/* Big‑endian lookup table for uppercase encoding */
static const uint16_t hex_lookup_be_upper[256] = {
    0x3030, 0x3031, 0x3032, 0x3033, 0x3034, 0x3035, 0x3036, 0x3037, 0x3038, 0x3039, 0x3041, 0x3042,
    0x3043, 0x3044, 0x3045, 0x3046, 0x3130, 0x3131, 0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137,
    0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3144, 0x3145, 0x3146, 0x3230, 0x3231, 0x3232, 0x3233,
    0x3234, 0x3235, 0x3236, 0x3237, 0x3238, 0x3239, 0x3241, 0x3242, 0x3243, 0x3244, 0x3245, 0x3246,
    0x3330, 0x3331, 0x3332, 0x3333, 0x3334, 0x3335, 0x3336, 0x3337, 0x3338, 0x3339, 0x3341, 0x3342,
    0x3343, 0x3344, 0x3345, 0x3346, 0x3430, 0x3431, 0x3432, 0x3433, 0x3434, 0x3435, 0x3436, 0x3437,
    0x3438, 0x3439, 0x3441, 0x3442, 0x3443, 0x3444, 0x3445, 0x3446, 0x3530, 0x3531, 0x3532, 0x3533,
    0x3534, 0x3535, 0x3536, 0x3537, 0x3538, 0x3539, 0x3541, 0x3542, 0x3543, 0x3544, 0x3545, 0x3546,
    0x3630, 0x3631, 0x3632, 0x3633, 0x3634, 0x3635, 0x3636, 0x3637, 0x3638, 0x3639, 0x3641, 0x3642,
    0x3643, 0x3644, 0x3645, 0x3646, 0x3730, 0x3731, 0x3732, 0x3733, 0x3734, 0x3735, 0x3736, 0x3737,
    0x3738, 0x3739, 0x3741, 0x3742, 0x3743, 0x3744, 0x3745, 0x3746, 0x3830, 0x3831, 0x3832, 0x3833,
    0x3834, 0x3835, 0x3836, 0x3837, 0x3838, 0x3839, 0x3841, 0x3842, 0x3843, 0x3844, 0x3845, 0x3846,
    0x3930, 0x3931, 0x3932, 0x3933, 0x3934, 0x3935, 0x3936, 0x3937, 0x3938, 0x3939, 0x3941, 0x3942,
    0x3943, 0x3944, 0x3945, 0x3946, 0x4130, 0x4131, 0x4132, 0x4133, 0x4134, 0x4135, 0x4136, 0x4137,
    0x4138, 0x4139, 0x4141, 0x4142, 0x4143, 0x4144, 0x4145, 0x4146, 0x4230, 0x4231, 0x4232, 0x4233,
    0x4234, 0x4235, 0x4236, 0x4237, 0x4238, 0x4239, 0x4241, 0x4242, 0x4243, 0x4244, 0x4245, 0x4246,
    0x4330, 0x4331, 0x4332, 0x4333, 0x4334, 0x4335, 0x4336, 0x4337, 0x4338, 0x4339, 0x4341, 0x4342,
    0x4343, 0x4344, 0x4345, 0x4346, 0x4430, 0x4431, 0x4432, 0x4433, 0x4434, 0x4435, 0x4436, 0x4437,
    0x4438, 0x4439, 0x4441, 0x4442, 0x4443, 0x4444, 0x4445, 0x4446, 0x4530, 0x4531, 0x4532, 0x4533,
    0x4534, 0x4535, 0x4536, 0x4537, 0x4538, 0x4539, 0x4541, 0x4542, 0x4543, 0x4544, 0x4545, 0x4546,
    0x4630, 0x4631, 0x4632, 0x4633, 0x4634, 0x4635, 0x4636, 0x4637, 0x4638, 0x4639, 0x4641, 0x4642,
    0x4643, 0x4644, 0x4645, 0x4646};

/* Select the proper encoding lookup table */
#define HEX_LOOKUP_UPPER (BIG_ENDIAN_SYSTEM ? hex_lookup_be_upper : hex_lookup_le_upper)

/*
 * The decoding lookup table accepts only valid uppercase hex digits:
 *  - '0'–'9' (ASCII 0x30–0x39) are mapped to 0x00–0x09.
 *  - 'A'–'F' (ASCII 0x41–0x46) are mapped to 0x0A–0x0F.
 * All other characters are marked as invalid (0x80).
 */
static const uint8_t hex_values_upper_table[256] = {
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};

/**
 * @brief Encode data into Base16 (hexadecimal) using UPPERCASE letters.
 *
 * @param data     The input bytes to encode.
 * @param data_len Number of bytes in 'data'.
 * @param out      Output buffer for the encoded hex string.
 * @param out_len  Size of 'out' (must allow 2 * data_len + 1).
 * @return Number of hex characters written (excluding NUL) on success,
 *         or an error code on failure.
 */
int base16_upper_encode(const uint8_t *restrict data, size_t data_len, char *restrict out,
                        size_t out_len)
{
    if (data == NULL || out == NULL)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }
    if (out_len < data_len * 2 + 1)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    if ((((uintptr_t)out) & 1) == 0)
    {
        uint16_t *out16 = (uint16_t *)out;
        for (size_t i = 0; i < data_len; i++)
        {
            out16[i] = HEX_LOOKUP_UPPER[data[i]];
        }
        out16[data_len] = 0;
    }
    else
    {
        for (size_t i = 0; i < data_len; i++)
        {
            uint16_t val = HEX_LOOKUP_UPPER[data[i]];
            if (BIG_ENDIAN_SYSTEM)
            {
                out[2 * i] = (char)(val >> 8);
                out[2 * i + 1] = (char)(val & 0xFF);
            }
            else
            {
                out[2 * i] = (char)(val & 0xFF);
                out[2 * i + 1] = (char)(val >> 8);
            }
        }
        out[data_len * 2] = '\0';
    }

    return (int)(data_len * 2);
}

/**
 * @brief Decode a Base16 (hexadecimal) string of UPPERCASE letters.
 *
 * @param in       Input hex string (only uppercase letters 0–9A–F).
 * @param data_len Length of 'in' in characters (must be even).
 * @param out      Output buffer for decoded bytes.
 * @param out_len  Size of 'out' in bytes (must be at least data_len/2).
 * @return Number of bytes written to 'out' on success, or error code on
 * failure.
 */
int base16_upper_decode(const char *restrict in, size_t data_len, uint8_t *restrict out,
                        size_t out_len)
{
    if (in == NULL || out == NULL)
    {
        return MULTIBASE_ERR_NULL_POINTER;
    }
    if ((data_len & 1) != 0)
    {
        return MULTIBASE_ERR_INVALID_INPUT_LEN;
    }

    size_t required_out_len = data_len / 2;
    if (out_len < required_out_len)
    {
        return MULTIBASE_ERR_BUFFER_TOO_SMALL;
    }

    const unsigned char *in_ptr = (const unsigned char *)in;
    uint8_t *out_ptr = out;
    size_t i = 0;
    size_t bulk_end = required_out_len - (required_out_len % 8);

    while (i < bulk_end)
    {
        uint8_t v0 = hex_values_upper_table[in_ptr[0]];
        uint8_t v1 = hex_values_upper_table[in_ptr[1]];
        uint8_t v2 = hex_values_upper_table[in_ptr[2]];
        uint8_t v3 = hex_values_upper_table[in_ptr[3]];
        uint8_t v4 = hex_values_upper_table[in_ptr[4]];
        uint8_t v5 = hex_values_upper_table[in_ptr[5]];
        uint8_t v6 = hex_values_upper_table[in_ptr[6]];
        uint8_t v7 = hex_values_upper_table[in_ptr[7]];
        uint8_t v8 = hex_values_upper_table[in_ptr[8]];
        uint8_t v9 = hex_values_upper_table[in_ptr[9]];
        uint8_t v10 = hex_values_upper_table[in_ptr[10]];
        uint8_t v11 = hex_values_upper_table[in_ptr[11]];
        uint8_t v12 = hex_values_upper_table[in_ptr[12]];
        uint8_t v13 = hex_values_upper_table[in_ptr[13]];
        uint8_t v14 = hex_values_upper_table[in_ptr[14]];
        uint8_t v15 = hex_values_upper_table[in_ptr[15]];

        if ((v0 | v1 | v2 | v3 | v4 | v5 | v6 | v7 | v8 | v9 | v10 | v11 | v12 | v13 | v14 | v15) &
            0x80)
        {
            return MULTIBASE_ERR_INVALID_CHARACTER;
        }

        out_ptr[0] = (uint8_t)((v0 << 4) | v1);
        out_ptr[1] = (uint8_t)((v2 << 4) | v3);
        out_ptr[2] = (uint8_t)((v4 << 4) | v5);
        out_ptr[3] = (uint8_t)((v6 << 4) | v7);
        out_ptr[4] = (uint8_t)((v8 << 4) | v9);
        out_ptr[5] = (uint8_t)((v10 << 4) | v11);
        out_ptr[6] = (uint8_t)((v12 << 4) | v13);
        out_ptr[7] = (uint8_t)((v14 << 4) | v15);

        in_ptr += 16;
        out_ptr += 8;
        i += 8;
    }

    while (i < required_out_len)
    {
        uint8_t hi = hex_values_upper_table[in_ptr[0]];
        uint8_t lo = hex_values_upper_table[in_ptr[1]];
        if ((hi | lo) & 0x80)
        {
            return MULTIBASE_ERR_INVALID_CHARACTER;
        }
        *out_ptr++ = (uint8_t)((hi << 4) | lo);
        in_ptr += 2;
        i++;
    }

    return (int)required_out_len;
}