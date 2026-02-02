#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include <cstdint>

using Buffer = std::vector<uint8_t>;

class BinaryWriter {
public:
    explicit BinaryWriter(Buffer& out)
        : buffer_(out) {
    }

    // Write trivially-copyable POD types
    template<typename T>
    void write(const T& value) {
        static_assert(std::is_trivially_copyable_v<T>,
            "BinaryWriter::write requires trivially copyable type");

        const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
        buffer_.insert(buffer_.end(), p, p + sizeof(T));
    }

    // Write raw bytes
    void writeBytes(const void* data, size_t size) {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
        buffer_.insert(buffer_.end(), p, p + size);
    }

    // Write string (uint16 length + bytes)
    void writeString(const std::string& s) {
        uint16_t len = static_cast<uint16_t>(s.size());
        write(len);
        writeBytes(s.data(), len);
    }

    // Inside BinaryWriter class public:
    const uint8_t* data() const { return buffer_.data(); }

    size_t size() const { return buffer_.size(); }

private:
    Buffer& buffer_;
};
