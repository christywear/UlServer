#pragma once
#include <cstdint>
#include <vector>

using PlayerId = uint64_t;
using Buffer = std::vector<uint8_t>;

// CamelCase names avoid Windows Macro collisions
enum class DataType : uint8_t {
    None = 0,
    Byte,
    Int16,
    UInt16,
    Int32,
    UInt32,
    Float,
    String,
    Bool,
    Array
};