/**
 * Copyright (c) 2020-2022, Haier
 *
 * base64 codec extension.
 *
 * Change Logs:
 * Date             Author              Notes
 * 2022-02-12       YangZhikang         first version
 */

#ifndef BASE64_EX_H
#define BASE64_EX_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Defines/Macros/Types -------------------------------------------------------------------------*/


/*--- Global Variables -----------------------------------------------------------------------------*/


/*--- Global Constants -----------------------------------------------------------------------------*/


/*--- Global Prototypes ----------------------------------------------------------------------------*/

/**
 * @brief base64图片解码
 * 
 * 传入的base64字符串应符合以下格式：
 * "data:image/XXX;base64,XXXXXXXXXXXXXXXXXXXXXXXXXXX"
 * 
 * @param base64 待解码的base64字符串
 * @param path 待存储的图片文件路径
 * @return 成功返回0，失败返回<0
 */
int base64_decode_image(const char *base64_img, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* BASE64_EX_H */
