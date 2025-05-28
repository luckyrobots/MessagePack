// Copyright Epic Games, Inc. All Rights Reserved.

#include "MessagePackBlueprintLibrary.h"

#ifdef __APPLE__
#pragma push_macro("nil")
#undef nil
#endif

#include "msgpack.hpp" // This needs to be accessible

#ifdef __APPLE__
#pragma pop_macro("nil")
#endif

TArray<uint8> UMessagePackBlueprintLibrary::SerializeToMessagePack(const FGameMessage& MessageData)
{
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(buffer);

    // Create a map with our message data
    packer.pack_map(3); 
    
    // Pack ID
    packer.pack(std::string("id")); // Use std::string
    packer.pack(TCHAR_TO_UTF8(*MessageData.Id));
    
    // Pack Message
    packer.pack(std::string("message")); // Use std::string
    packer.pack(TCHAR_TO_UTF8(*MessageData.Message));
    
    // Pack Data
    packer.pack(std::string("data")); // Use std::string
    packer.pack_bin(MessageData.Data.Num());
    if (MessageData.Data.Num() > 0)
    {
        packer.pack_bin_body(reinterpret_cast<const char*>(MessageData.Data.GetData()), MessageData.Data.Num());
    }

    // Convert to TArray<uint8>
    TArray<uint8> Result;
    if (buffer.size() > 0)
    {
        Result.Append(reinterpret_cast<const uint8*>(buffer.data()), buffer.size());
    }
    return Result;
}

// Placeholder for deserialization
// bool UMessagePackBlueprintLibrary::DeserializeFromMessagePack(const TArray<uint8>& PackedData, FGameMessage& OutMessageData)
// {
//     if (PackedData.Num() == 0) return false;
// 
//     try
//     {
//         msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(PackedData.GetData()), PackedData.Num());
//         msgpack::object obj = oh.get();
// 
//         // Assuming the root is a map
//         if (obj.type == msgpack::type::MAP)
//         {
//             // Example: obj.convert(OutMessageData); // This requires FGameMessage to have msgpack_unpack defined
//             // Or manually extract:
//             // const auto& map = obj.via.map;
//             // for (uint32_t i = 0; i < map.size; ++i) {
//             //     msgpack::object_kv& kv = map.ptr[i];
//             //     // ... inspect kv.key and kv.val
//             // }
//             return true; // Modify based on actual deserialization logic
//         }
//     }
//     catch (const std::exception& e)
//     {
//         UE_LOG(LogTemp, Error, TEXT("MessagePack Deserialization Error: %s"), UTF8_TO_TCHAR(e.what()));
//         return false;
//     }
//     return false;
// } 