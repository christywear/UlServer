#pragma once
#include <protocol/net/NetTypes.h>
#include <type_traits>
#include <string>

// class obj for writing packets to buffer for networking
class BinaryWriter {
public:
    // declaring writer function
    explicit BinaryWriter(Buffer& b) : buf(b) {}

    template<typename T>
    // writing templated type data
    void write(const T& v) {
        // confirming type safety!
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for binary transfer!");

        // declaring byte type for data
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&v);

        // put data into buffer
        buf.insert(buf.end(), p, p + sizeof(T));
    }

    // HIGH-ENERGY ADDITION: Write a string with a length prefix!
    void writeString(const std::string& str) {
        // Underlight style: 16-bit length prefix followed by raw chars
        uint16_t length = static_cast<uint16_t>(str.size());
        write<uint16_t>(length);

        const uint8_t* p = reinterpret_cast<const uint8_t*>(str.data());
        buf.insert(buf.end(), p, p + length);
    }

private:
    // declaring buffer
    Buffer& buf;
};