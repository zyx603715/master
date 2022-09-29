/**
 * Copyright (c) 2021-2022, Haier
 *
 * unit test for ByteArray.
 * 
 * Change Logs:
 * Date             Author              Notes
 * 2022-05-29       YangZhikang         first version
 */

#include "log.h"
#include "ByteArray.hpp"
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    byte_t data[256];

    for (int i = 0; i < sizeof(data); i++)
        data[i] = (byte_t)i;

    ByteArray array(data, sizeof(data));

    test_assert(array.size() == sizeof(data));
    test_assert(array.data()[0] == 0);
    test_assert(array.data()[sizeof(data) - 1] == (byte_t)(sizeof(data) - 1));

    ByteArray array2(100);
    test_assert(array2.size() == 100);
    test_assert(array2.data()[0] == 0x00);
    array2.data()[0] = 0x0f;
    test_assert(array2.data()[0] == 0x0f);

    ByteArray array3("Hello, world !");
    // test_assert(array3.size() == strlen("Hello, world !"));
    test_assert(array3.size() == sizeof("Hello, world !"));
    log_d("array3: %s", array3.data());

    ByteArray array4(nullptr);
    test_assert(array4.size() == 0);
    array4 = "Hello";
    test_assert(array4.size() == sizeof("Hello"));
    log_d("array4: %s", array4.data());
    array4 = "";
    test_assert(array4.size() == 1);

    return 0;
}
