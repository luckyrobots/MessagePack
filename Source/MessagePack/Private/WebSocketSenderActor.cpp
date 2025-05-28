// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebSocketSenderActor.h"
#include "WebSocketsModule.h"
#include "GenericPlatform/GenericPlatformMath.h" // For FMath::RandRange
#include "Misc/Guid.h" // For FGuid

AWebSocketSenderActor::AWebSocketSenderActor()
{
	PrimaryActorTick.bCanEverTick = false; // Not using Tick
}

void AWebSocketSenderActor::BeginPlay()
{
	Super::BeginPlay();
	ConnectWebSocket();
}

void AWebSocketSenderActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear timer
	GetWorldTimerManager().ClearTimer(SendMessageTimerHandle);

	// Close WebSocket connection if valid
	if (WebSocket.IsValid())
	{
		WebSocket->Close();
	}

	Super::EndPlay(EndPlayReason);
}

void AWebSocketSenderActor::ConnectWebSocket()
{
	FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");

	if (!WebSocket.IsValid())
	{
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL);

		WebSocket->OnConnected().AddUObject(this, &AWebSocketSenderActor::OnConnected);
		WebSocket->OnConnectionError().AddUObject(this, &AWebSocketSenderActor::OnConnectionError);
		WebSocket->OnClosed().AddUObject(this, &AWebSocketSenderActor::OnClosed);
		WebSocket->OnMessage().AddUObject(this, &AWebSocketSenderActor::OnMessageReceived); // Listening to messages just in case

		UE_LOG(LogTemp, Log, TEXT("Attempting to connect to WebSocket server at %s"), *ServerURL);
		WebSocket->Connect();
	}
}

void AWebSocketSenderActor::OnConnected()
{
	UE_LOG(LogTemp, Log, TEXT("Successfully connected to WebSocket server."));
	// Start timer to send messages
	GetWorldTimerManager().SetTimer(SendMessageTimerHandle, this, &AWebSocketSenderActor::SendRandomMessage, MessageIntervalSeconds, true, 0.0f);
}

void AWebSocketSenderActor::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Error, TEXT("WebSocket connection error: %s"), *Error);
	// Optionally, try to reconnect here or handle error
}

void AWebSocketSenderActor::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket connection closed. Status: %d, Reason: %s, Clean: %s"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
	GetWorldTimerManager().ClearTimer(SendMessageTimerHandle); // Stop sending messages
	WebSocket.Reset(); // Ensure WebSocket is marked as invalid
	// Optionally, try to reconnect here
}

void AWebSocketSenderActor::OnMessageReceived(const FString& MessageString)
{
	// This actor is primarily for sending, but we log received messages for debugging.
	UE_LOG(LogTemp, Log, TEXT("WebSocket message received: %s"), *MessageString);
}

template<typename T>
FGameMessage AWebSocketSenderActor::CreateGameMessageFromStruct(const T& StructData)
{
    FGameMessage MessageData;
    MessageData.Id = FGuid::NewGuid().ToString();
    
    // Serialize the struct to binary
    FMemoryWriter MemoryWriter(MessageData.Data, true);
    MemoryWriter << StructData;
    
    // Create a descriptive message with the struct type name
    MessageData.Message = FString::Printf(TEXT("Serialized %s"), *T::StaticStruct()->GetName());
    
    return MessageData;
}

FGameMessage AWebSocketSenderActor::CreateGameMessage()
{
	int32 RandomDataSize = FMath::RandRange(10, 50);
	FGameMessage MessageData;

	MessageData.Id = FGuid::NewGuid().ToString();
	MessageData.Message = FString::Printf(TEXT("Random message value: %d"), FMath::RandRange(0, 1000));
	MessageData.Data.SetNumUninitialized(RandomDataSize);
	
	// Generate some random binary data (e.g., 10-50 bytes)
	for (int32 i = 0; i < RandomDataSize; ++i)
	{
		MessageData.Data[i] = static_cast<uint8>(FMath::RandRange(0, 255));
	}

	return MessageData;
}


void AWebSocketSenderActor::SendRandomMessage()
{
	if (!WebSocket.IsValid() || !WebSocket->IsConnected())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send message, WebSocket is not connected."));
		// Attempt to reconnect if desired
		// ConnectWebSocket(); 
		return;
	}

	// Example struct for testing CreateGameMessageFromStruct


	FGameMessage MessageData = CreateGameMessage();

	TArray<uint8> PackedData = UMessagePackBlueprintLibrary::SerializeToMessagePack(MessageData);

	if (PackedData.Num() > 0)
	{
		WebSocket->Send(PackedData.GetData(), PackedData.Num(), true /*true for binary*/);
		UE_LOG(LogTemp, Log, TEXT("Sent MessagePack message with ID: %s, Text: '%s', DataSize: %d bytes"), *MessageData.Id, *MessageData.Message, MessageData.Data.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MessagePack serialization resulted in empty data. Not sending."));
	}
} 