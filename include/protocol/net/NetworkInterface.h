#pragma once
#include <protocol/net/PacketManager.h>
#include <protocol/net/BinaryWriter.h>
#include <protocol/net/NetTypes.h> 
#include <string>
#include <type_traits> 
#include <cstdint>

// --------------------------------------------------------------------
// HELPER: Map C++ Types -> Your 'DataType' Enum
// --------------------------------------------------------------------
template <typename T>
constexpr DataType GetTypeID() {
    // FIX: Using CamelCase names to match NetTypes.h
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>) return DataType::Int32;
    else if constexpr (std::is_same_v<T, uint32_t>) return DataType::UInt32;
    else if constexpr (std::is_same_v<T, float>)    return DataType::Float;
    else if constexpr (std::is_same_v<T, bool>)     return DataType::Bool;
    else if constexpr (std::is_same_v<T, uint8_t>)  return DataType::Byte;
    else if constexpr (std::is_same_v<T, char>)     return DataType::Byte;
    else if constexpr (std::is_same_v<T, int16_t>)  return DataType::Int16;
    else if constexpr (std::is_same_v<T, uint16_t>) return DataType::UInt16;
    else return DataType::None;
}

class NetworkInterface {
private:
    class AutoPacker {
        BinaryWriter& w;
    public:
        AutoPacker(BinaryWriter& writer) : w(writer) {}

        template <typename T>
        void Pack(const T& val) {
            if constexpr (std::is_arithmetic_v<T>) {
                w.write<uint8_t>(static_cast<uint8_t>(GetTypeID<T>()));
                w.write<T>(val);
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                // FIX: Use DataType::String (not STRING)
                w.write<uint8_t>(static_cast<uint8_t>(DataType::String));
                w.writeString(val);
            }
        }
    };

public:
    template<typename... Args>
    static void Send(uint64_t id, int verb, Args... args) {
        auto buffer = std::make_shared<Buffer>();
        buffer->reserve(512);

        auto writer = std::make_shared<BinaryWriter>(*buffer);

        AutoPacker packer(*writer);
        (packer.Pack(args), ...);

        PacketManager::Get()->Dispatch(id, verb, writer);
    }
};