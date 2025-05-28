// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IWebSocket.h"
#include "MessagePackBlueprintLibrary.h" // For FGameMessage and serialization
#include "WebSocketSenderActor.generated.h"

UCLASS()
class MESSAGEPACK_API AWebSocketSenderActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWebSocketSenderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the actor is shutting down
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

// We are not using Tick() for this actor, timer is used instead.
// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

private:
	void ConnectWebSocket();
	void SendRandomMessage();

	TSharedPtr<IWebSocket> WebSocket;
	FTimerHandle SendMessageTimerHandle;

	UPROPERTY(EditAnywhere, Category = "WebSocket", meta = (DisplayName = "WebSocket Server URL"))
	FString ServerURL = TEXT("ws://localhost:8765");

	UPROPERTY(EditAnywhere, Category = "WebSocket", meta = (DisplayName = "Message Send Interval (Seconds)"))
	float MessageIntervalSeconds = 1.0f;

	// WebSocket Event Handlers
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessageReceived(const FString& MessageString); // Though we are only sending
}; 