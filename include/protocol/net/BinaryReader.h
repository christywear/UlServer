#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class BinaryReader {
public:
    // We point to the existing data so we don't make unnecessary copies! 🏎️
    BinaryReader(const uint8_t* data, size_t size)
        : data_(data), size_(size), cursor_(0) {
    }

    // Template magic to read primitive types (int, float, uint32, etc.)
    template<typename T>
    T read() {
        if (cursor_ + sizeof(T) > size_) {
            throw std::runtime_error("BinaryReader: Out of bounds read!");
        }
        T value;
        std::memcpy(&value, data_ + cursor_, sizeof(T));
        cursor_ += sizeof(T);
        return value;
    }

    // Specialized read for strings
    std::string readString() {
        uint16_t length = read<uint16_t>(); // Read length prefix
        if (cursor_ + length > size_) {
            throw std::runtime_error("BinaryReader: String length exceeds buffer!");
        }
        std::string str(reinterpret_cast<const char*>(data_ + cursor_), length);
        cursor_ += length;
        return str;
    }

    size_t remaining() const { return size_ - cursor_; }

private:
    const uint8_t* data_;
    size_t size_;
    size_t cursor_;
};