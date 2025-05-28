// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

// Include msgpack essentials
#ifdef __APPLE__
#pragma push_macro("nil")
#undef nil
#endif
#include "msgpack.hpp" // Include the main msgpack header
#ifdef __APPLE__
#pragma pop_macro("nil")
#endif

#include "MessagePackBlueprintLibrary.generated.h"

// Removed forward declaration of msgpack::sbuffer as msgpack.hpp is now included

USTRUCT(BlueprintType)
struct FGameMessage // Consider making this more generic or allowing users to define their own structs
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessagePack")
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessagePack")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MessagePack")
    TArray<uint8> Data;
};

UCLASS()
class MESSAGEPACKMODULE_API UMessagePackBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MessagePack")
    static TArray<uint8> SerializeToMessagePack(const FGameMessage& MessageData);

    // Placeholder for deserialization, if needed in the future
    // UFUNCTION(BlueprintCallable, Category = "MessagePack")
    // static bool DeserializeFromMessagePack(const TArray<uint8>& PackedData, FGameMessage& OutMessageData);
}; 