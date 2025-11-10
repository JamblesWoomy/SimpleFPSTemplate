// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();

	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void Login();
	void OnLoginCompleteDelegate(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	bool HostSession(); // creation of host and server
	void OnCreateSessionCompleteDelegate(FName InSessionName, bool bWasSuccessful);
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void FindSession(); // locate session
	void OnFindSessionCompleteDelegate(bool bWasSuccessful);
	
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void JoinSession(); // connect player to client
	void OnJoinSessionCompleteDelegate(FName sessionName, EOnJoinSessionCompleteResult::Type Result);
	
	//Quit
	UFUNCTION(BlueprintCallable, Category = "OnlineSession")
	void QuitSession();

	bool bIsInServer = false;
};
