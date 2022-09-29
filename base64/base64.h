/**
 * Copyright (c) 2020-2022, Haier
 *
 * base64 codec.
 *
 * Change Logs:
 * Date             Author              Notes
 * 2022-02-10       YangZhikang         first version
 */

#ifndef BASE64_H
#define BASE64_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Defines/Macros/Types -------------------------------------------------------------------------*/

/* 由base64字符串长度估算原始数据缓冲区长度 */
#define calc_raw_data_buf_size(base64_size)     ((base64_size) / 4 * 3)

/* 由原始数据长度估算base64缓冲区长度 */
#define calc_base64_buf_size(raw_data_size)     (((raw_data_size) + 2) / 3 * 4 + 1)


/*--- Global Variables -----------------------------------------------------------------------------*/


/*--- Global Constants -----------------------------------------------------------------------------*/


/*--- Global Prototypes ----------------------------------------------------------------------------*/

/**
 * @brief base64编码
 * 
 * @param _raw_data 待编码的原始数据
 * @param raw_data_len 原始数据长度
 * @param base64_buf base64缓冲区指针
 * @param base64_buf_len base64缓冲区长度
 * @return 成功返回编码后的字符串指针，失败返回NULL
 */
char *base64_encode(const void *_raw_data, size_t raw_data_len, char *base64_buf, size_t base64_buf_len);

/**
 * @brief base64解码
 * 
 * @param base64 待解码的base64字符串
 * @param _raw_data_buf 原始数据缓冲区指针
 * @param raw_data_buf_len 原始数据缓冲区长度
 * @return 成功返回解码后的原始数据长度，失败返回<0
 */
int base64_decode(const char *base64, void *_raw_data_buf, size_t raw_data_buf_len);

#ifdef __cplusplus
}
#endif

#endif /* BASE64_H */
