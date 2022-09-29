/**
 * Copyright (c) 2020-2022, Haier
 *
 * base64 codec extension.
 *
 * Change Logs:
 * Date             Author              Notes
 * 2022-02-12       YangZhikang         first version
 */

#define LOG_TAG             "base64_ex"
#define LOG_LVL             LOG_LVL_INFO

#include "base64_ex.h"
#include "base64.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "log.h"

/*--- Defines/Macros/Types -------------------------------------------------------------------------*/


/*--- Prototypes -----------------------------------------------------------------------------------*/


/*--- Variables ------------------------------------------------------------------------------------*/


/*--- Constants ------------------------------------------------------------------------------------*/

int base64_decode_image(const char *base64_img, const char *path)
{
    char img_format[8];         /* 图像格式 */
    char header[32];            /* 头部："data:image/XXXXXXX;base64," */
    const char *base64_data;    /* 实际的base64数据 */
    int raw_data_buf_size;      /* 解码数据缓冲区大小 */
    void *raw_data_buf;         /* 解码数据缓冲区指针 */
    int raw_data_size;          /* 解码数据大小 */
    FILE *fp;
    const char *temp_str;
    int temp_int;

    if (base64_img == NULL || base64_img[0] == '\0' || path == NULL || path[0] == '\0')
    {
        log_e("Invalid arguments.");
        return -1;
    }

    /* 定位起始位置 @{ */
    base64_img = strstr(base64_img, "data:image/");
    if (base64_img == NULL)
    {
        log_e("Invalid format. Cannot find header \"data:image/\".");
        return -1;
    }
    /* 定位起始位置 @} */

    /* 获取图片格式 @{ */
    temp_str = strchr(base64_img, ';');
    if (temp_str == NULL)
    {
        log_e("Invalid format. Cannot find image format name.");
        return -1;
    }
    temp_int = temp_str - base64_img - strlen("data:image/");   /* 计算图片扩展名长度 */
    if (temp_int > sizeof(img_format) - 1)
    {
        log_e("Invalid image format name.");
        return -1;
    }
    strncpy(img_format, base64_img + strlen("data:image/"), temp_int);
    img_format[temp_int] = '\0';
    log_d("image format: %s", img_format);
    /* 获取图片格式 @} */

    /* 验证头部是否正确 @{ */
    strncpy(header, "data:image/", sizeof(header));
    strcat(header, img_format);
    strcat(header, ";base64,");
    if (strncmp(base64_img, header, strlen(header)) != 0)
    {
        log_e("Invalid format. Cannot find image header \"%s\".", header);
        return -1;
    }
    /* 验证头部是否正确 @} */

    base64_data = base64_img + strlen(header);

    /* 分配解码缓冲区 @{ */
    raw_data_buf_size = calc_raw_data_buf_size(strlen(base64_data));
    raw_data_buf = malloc(raw_data_buf_size);
    if (raw_data_buf == NULL)
    {
        log_e("No memory.");
        return -1;
    }
    /* 分配解码缓冲区 @} */

    /* base64解码 @{ */
    raw_data_size = base64_decode(base64_data, raw_data_buf, raw_data_buf_size);
    if (raw_data_size < 0)
    {
        log_e("base64 decode error.");
        free(raw_data_buf);
        return -1;
    }
    /* base64解码 @} */

    /* 存文件 @{ */
    fp = fopen(path, "wb");
    if (fp == NULL)
    {
        log_e("Failed to create image file: %s", path);
        free(raw_data_buf);
        return -1;
    }
    if (fwrite(raw_data_buf, raw_data_size, 1, fp) != 1)
    {
        log_e("Failed to write file.");
        fclose(fp);
        free(raw_data_buf);
        return -1;
    }
    fclose(fp);
    /* 存文件 @} */

    free(raw_data_buf);;

    return 0;
}



/*--- Local Function Implementation ----------------------------------------------------------------*/

