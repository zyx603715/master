/**
 * Copyright (c) 2021-2022, Haier
 *
 * unit test for base64.
 * 
 * Change Logs:
 * Date             Author              Notes
 * 2022-02-10       YangZhikang         first version
 */

#define LOG_TAG             "Test"
#define LOG_LVL             LOG_LVL_DEBUG

#include "base64.h"
#include "base64_ex.h"
#include <string.h>
#include "log.h"

static const uint8_t test_raw_data[] =
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
};

static char base64_buf[calc_base64_buf_size(sizeof(test_raw_data))];
static uint8_t raw_data_buf[sizeof(test_raw_data)];

static const char *test_base64_img = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAPoAAAD6AQAAAACgl2eQAAACuUlEQVR42u2ZwZFjIQxEIRGUfxYbCiQC26/B5W9v1d7QyS7P+BveQSOphcSU9f/Xn/IDfsAPuAT0UuqaJWasqYc69Dy8mAcMvVlorVS4VqLX4fU8gK8z9F2/Zml8q17MBcbqdbaAqDJZn/kAO1hYvF1iZQMESwv4aHWFrGjvn2jeBcjP8f36zurLAK9Ze3Gg5J8eS+8vdV8GetFukVr4rFZuk4paJAKKigTr/NBD2MZZZ6xEQOmqNKkIFmdRPWxmTQRkGEmjDVWQTvLIxLKtTAO01WUkCSNvYTDm9kzgVbdIGGVtcbD0nAlMS4aq1UvzgUI5f4g3AdiuehkJ5ex5F7EEYLCo44TS0S3ZcfInEVCKBH2FigZGBmr+EO99wBmDUhpnWRAmF9OaCSg84djQaiFeLR2XZQFO2N1gnL1C+RgjEwjLhALS4njsq4jdB4qPtOBYQbd997yPYn4fIE6dvntYQ5wu9Fsf4r0NdMVlnJgNqjpnPHpOBFywaPbQrs6WSnHfukkDdJArNIGIeRBGz/PsBhMA5aecI6x5nRLW41HEEgD5iOh4DPKh7lL+HAbvA93jl9doMx210uIt3gSAWWPrltRFNcxl8l0iQNk+89/0XIylrCUC9pWsk2Rormyze41EYJzuf55d+YgRYKQCaCY4XWm92242PaznAb6e0SGieK1Tzl43FmmAmqqdKczkuIgJ/aMrvg84XM7aecqIh7OxEoFTRvccinot3vb4KxKAEu61MdKXFVKuB6NEwJ3ecPNPhGi8CdV7EEsAdt9LnvqyiJw5Q0Ae0G0RSbPc7Vg7/CQCvh+jhBfbVmrfzd7KBHxJdS4PPRTv26OaDJCotJhcm+mgb/sGLRfYM6CdtK9IZouVCZAiTGG0Or67a7HnwzzgFA26TUK29iSyPrP6MvD7X9IP+AHpwF/KjfT2txe2jwAAAABJRU5ErkJggg==";


int main(int argc, char *argv[])
{
    char *base64;

    test_assert(base64_encode(test_raw_data, sizeof(test_raw_data), base64_buf, sizeof(base64_buf) - 3) == NULL)

    base64 = base64_encode(test_raw_data, sizeof(test_raw_data), base64_buf, sizeof(base64_buf));
    test_assert(base64 != NULL);
    log_d("base64: %s", base64);

    test_assert(base64_decode(base64, raw_data_buf, sizeof(raw_data_buf) - 1) == -1);
    test_assert(base64_decode(base64, raw_data_buf, sizeof(raw_data_buf)) == sizeof(test_raw_data));
    test_assert(memcmp(raw_data_buf, test_raw_data, sizeof(raw_data_buf)) == 0);

    test_assert(base64_decode_image("ata:image/png;base64,iVBORw0KG", "./tmp/test_base64_img.png") == -1);
    test_assert(base64_decode_image("data:image/png; base64,iVBORw0KG", "./tmp/test_base64_img.png") == -1);
    test_assert(base64_decode_image("data:image/ZXCVBNMA;base64,iVBORw0KG", "./tmp/test_base64_img.png") == -1);
    test_assert(base64_decode_image(test_base64_img, "./tmp/test_base64_img.png") == 0);

    return 0;
}
