/**
 * Copyright (c) 2020-2022, Haier
 *
 * base64 codec.
 *
 * base64是一种基于64个可打印字符来表示二进制数据的表示方法。
 * 由于2^6=64，所以每6位为一个单位，对应某个可打印字符。三个字节共24位，对应于4个base64单位，即3个字节需要用4个可打印字符来表示。
 * 它常用来作为电子邮件的传输编码。在base64中的可打印字符包括大写英文字母A-Z，小写英文字母a-z、阿拉伯数字0-9，这样共有62个字符，
 * 此外两个可打印符号在不同的系统中而不同，通常用加号（+）和正斜杠（/）。外加“补全符号”，通常用等号（=）。
 * 
 * 完整的base64定义可见RFC 1421和RFC 2045。编码后的数据比原始数据略长，为原来的4/3。在电子邮件中，根据RFC 822的规定，每76个字符，
 * 还需要加上回车符和换行符。可以估算编码后数据长度大约为原长的135.1%。
 * 
 * base64编码的时候，将三个自己的数据，先后放入一个24位的缓冲区中，先来的自己占高位。数据不足3个字节的话，在缓冲区中剩下的位用0补足。
 * 然后，每次取出6（因为26=64）位，按照其值选择 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ 中的
 * 字符作为编码后的输出。不断进行，直到全部输入数据转换完成。如果最后剩下两个输入数据，在编码结果后加1个“=”；如果最后剩下一个输入数据，
 * 编码结果后加2个“=”；如果没有剩下任何数据，就什么都不要加。这样才可以保证资料还原的正确性。
 * 
 * Change Logs:
 * Date             Author              Notes
 * 2022-02-10       YangZhikang         first version
 */

#include "base64.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/*--- Defines/Macros/Types -------------------------------------------------------------------------*/


/*--- Prototypes -----------------------------------------------------------------------------------*/


/*--- Variables ------------------------------------------------------------------------------------*/


/*--- Constants ------------------------------------------------------------------------------------*/

/* base64编码查找表 */
static const char base64_encode_lut[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* base64解码查找表（使用tools/make_base64_lut.py工具生成） */
static const uint8_t base64_decode_lut[256] = 
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x00 ~ 0x07 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x08 ~ 0x0f */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x10 ~ 0x17 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x18 ~ 0x1f */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x20 ~ 0x27 */
    0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,     /* ASCII: 0x28 ~ 0x2f */
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,     /* ASCII: 0x30 ~ 0x37 */
    0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x38 ~ 0x3f */
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,     /* ASCII: 0x40 ~ 0x47 */
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,     /* ASCII: 0x48 ~ 0x4f */
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,     /* ASCII: 0x50 ~ 0x57 */
    0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x58 ~ 0x5f */
    0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,     /* ASCII: 0x60 ~ 0x67 */
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,     /* ASCII: 0x68 ~ 0x6f */
    0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,     /* ASCII: 0x70 ~ 0x77 */
    0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x78 ~ 0x7f */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x80 ~ 0x87 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x88 ~ 0x8f */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x90 ~ 0x97 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0x98 ~ 0x9f */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xa0 ~ 0xa7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xa8 ~ 0xaf */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xb0 ~ 0xb7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xb8 ~ 0xbf */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xc0 ~ 0xc7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xc8 ~ 0xcf */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xd0 ~ 0xd7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xd8 ~ 0xdf */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xe0 ~ 0xe7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xe8 ~ 0xef */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xf0 ~ 0xf7 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,     /* ASCII: 0xf8 ~ 0xff */
};


/*--- Global Function Implementation ---------------------------------------------------------------*/

char *base64_encode(const void *_raw_data, size_t raw_data_len, char *base64_buf, size_t base64_buf_len)
{
    const uint8_t * const raw_data = _raw_data;

    size_t i, j;
    uint8_t current;

    /* 检查参数合法性 */
    if (raw_data == NULL || base64_buf == NULL)
        return NULL;
    if (base64_buf_len < calc_base64_buf_size(raw_data_len))
        return NULL;

    /* 每3个字节为一组进行处理 */
    for (i = 0, j = 0; i < raw_data_len; i += 3)
    {
        /* 处理第1个字节高6位 */
        current = (raw_data[i] >> 2) & 0x3f;
        base64_buf[j++] = base64_encode_lut[current];

        /* 处理第1个字节低2位 */
        current = (raw_data[i] << 4) & 0x30;
        if (i + 1 >= raw_data_len)
        {   /* 仅剩1字节 */
            base64_buf[j++] = base64_encode_lut[current];
            base64_buf[j++] = '=';
            base64_buf[j++] = '=';
            break;
        }
        /* 处理第2个字节高4位 */
        current |= (raw_data[i + 1] >> 4) & 0x0f;
        base64_buf[j++] = base64_encode_lut[current];

        /* 处理第2个字节低4位 */
        current = (raw_data[i + 1] << 2) & 0x3c;
        if (i + 2 >= raw_data_len)
        {   /* 仅剩2字节 */
            base64_buf[j++] = base64_encode_lut[current];
            base64_buf[j++] = '=';
            break;
        }
        /* 处理第3个字节高2位 */
        current |= (raw_data[i + 2] >> 6) & 0x03;
        base64_buf[j++] = base64_encode_lut[current];

        /* 处理第3个字节低6位 */
        current = (raw_data[i + 2]) & (0x3f);
        base64_buf[j++] = base64_encode_lut[current];
    }

    base64_buf[j] = '\0';
    return base64_buf;
}

int base64_decode(const char *base64, void *_raw_data_buf, size_t raw_data_buf_len)
{
    uint8_t * const raw_data_buf = _raw_data_buf;

    size_t i, j;
    uint8_t temp[4];
    size_t base64_len;
    size_t raw_data_len;

    /* 检查参数合法性 */
    if (base64 == NULL || raw_data_buf == NULL)
        return -1;
    if (base64[0] == '\0')
        return 0;
    base64_len = strlen(base64);
    if (base64_len % 4 != 0)
    {
#if 0
        return -1;
#else
        base64_len = base64_len / 4 * 4;    /* 截断到合法长度，而非直接返回错误，提高鲁棒性 */
        if (base64_len == 0)
            return -1;
#endif
    }
#if 0
    if (raw_data_buf_len < calc_raw_data_buf_size(base64_len))
        return -1;
#else
    /* 计算原始数据长度 */
    raw_data_len = base64_len / 4 * 3;
    if (base64[base64_len - 1] == '=')
    {
        raw_data_len--;
        if (base64[base64_len - 2] == '=')
            raw_data_len--;
    }
    if (raw_data_buf_len < raw_data_len)
        return -1;
#endif

    /* 每4个字符为一组进行处理 */
    for (i = 0, j = 0; base64[i] != '\0'; i += 4)
    {
        temp[0] = base64_decode_lut[(int)base64[i]];
        temp[1] = base64_decode_lut[(int)base64[i + 1]];
        temp[2] = base64_decode_lut[(int)base64[i + 2]];
        temp[3] = base64_decode_lut[(int)base64[i + 3]];

        if ((temp[0] == 0xff && base64[i] != '=')
            || (temp[1] == 0xff && base64[i + 1] != '=')
            || (temp[2] == 0xff && base64[i + 2] != '=')
            || (temp[3] == 0xff && base64[i + 3] != '='))
        {
            /* 无效编码 */
            break;
        }

        raw_data_buf[j++] = (uint8_t)((temp[0] << 2) | (temp[1] >> 4));
        if (base64[i + 2] == '=')
            break;

        raw_data_buf[j++] = (uint8_t)((temp[1] << 4) | (temp[2] >> 2));
        if (base64[i + 3] == '=')
            break;

        raw_data_buf[j++] = (uint8_t)((temp[2] << 6) | temp[3]);
    }

    return j;
}


/*--- Local Function Implementation ----------------------------------------------------------------*/

