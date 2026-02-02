#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>
#include <stdexcept>

class BinaryReader {
public:
    BinaryReader(const uint8_t* data, size_t size)
        : data_(data), size_(size), pos_(0) {
    }

    BinaryReader(const std::vector<uint8_t>& buf)
        : data_(buf.data()), size_(buf.size()), pos_(0) {
    }

    // Read POD type
    template<typename T>
    T read() {
        static_assert(std::is_trivially_copyable_v<T>,
            "BinaryReader::read requires trivially copyable type");

        ensure(sizeof(T));
        T out;
        std::memcpy(&out, data_ + pos_, sizeof(T));
        pos_ += sizeof(T);
        return out;
    }

    // Read raw bytes (returns pointer into buffer)
    const uint8_t* readBytes(size_t count) {
        ensure(count);
        const uint8_t* p = data_ + pos_;
        pos_ += count;
        return p;
    }

    // Read string (uint16 length + bytes)
    std::string readString() {
        uint16_t len = read<uint16_t>();
        ensure(len);
        std::string s(reinterpret_cast<const char*>(data_ + pos_), len);
        pos_ += len;
        return s;
    }

    size_t remaining() const {
        return size_ - pos_;
    }

private:
    void ensure(size_t needed) const {
        if (pos_ + needed > size_) {
            throw std::runtime_error("BinaryReader: buffer underrun");
        }
    }

    const uint8_t* data_;
    size_t size_;
    size_t pos_;
};
