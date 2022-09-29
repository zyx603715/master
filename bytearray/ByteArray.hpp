#ifndef BYTE_ARRAY_HPP
#define BYTE_ARRAY_HPP

#include <string>
#include <cstring>

#if 1
using byte_t = unsigned char;
#else
using byte_t = char;
#endif

class ByteArray : public std::basic_string<byte_t>
{
public:
    ByteArray() = default;
    ByteArray(size_type size, byte_t byte = 0x00) : std::basic_string<byte_t>(size, byte) {}
    ByteArray(const byte_t *data, size_type size) : std::basic_string<byte_t>(data, size) {}
    // ByteArray(const char *str) : std::basic_string<byte_t>(reinterpret_cast<const byte_t *>(str)) {}
    ByteArray(const char *str) : ByteArray(reinterpret_cast<const byte_t *>(str), str ? strlen(str) + 1 : 0) {}

    const byte_t *data() const noexcept
    {
        return std::basic_string<byte_t>::data();
    }

    byte_t *data() noexcept
    {
        return const_cast<byte_t *>(std::basic_string<byte_t>::data());
    }
};


#endif  /* BYTE_ARRAY_HPP */
