// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

//For getting and using online subsystem
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

//For creating custom debug log
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

//User for opening a level
#include "Kismet/GameplayStatics.h"

//Get current player state
#include "GameFramework/PlayerState.h"

//SEARCH_KEYWORDS, _PRESENCE, _LOBBIES, TEC.
#include "Online/OnlineSessionNames.h"

const FName SESSION_NAME = "AIEI_Session";
TSharedPtr<class FOnlineSessionSearch > searchSettings;

//Custom debug logging
#define DISPLAY_LOG(fmt, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT(fmt), ##__VA_ARGS__));

AMyPlayerController::AMyPlayerController() {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	UE_LOG(LogTemp, Warning, TEXT("[PlayerController found subsystem %s"), *subSystem->GetSubsystemName().ToString());

	Login();
}

void AMyPlayerController::Login() {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem) {
		IOnlineIdentityPtr identityInterface = subSystem->GetIdentityInterface();
		if (identityInterface.IsValid()) {
			ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);
			if (localPlayer != NULL) {
				int controllerId = localPlayer->GetControllerId();
				if (identityInterface->GetLoginStatus(controllerId) != ELoginStatus::LoggedIn) {
					identityInterface->AddOnLoginCompleteDelegate_Handle(controllerId, FOnLoginCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnLoginCompleteDelegate));
					identityInterface->AutoLogin(controllerId);
				}
			}
		}
	}
}

void AMyPlayerController::OnLoginCompleteDelegate(int32 localUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem) {
		IOnlineIdentityPtr identityInterface = subSystem->GetIdentityInterface();
		if (identityInterface.IsValid()) {
			ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);
			if (localPlayer != NULL) {
				FUniqueNetIdRepl uniqueNetId = PlayerState->GetUniqueId();
				uniqueNetId.SetUniqueNetId(FUniqueNetIdWrapper(UserId).GetUniqueNetId());
				PlayerState->SetUniqueId(uniqueNetId);

				int controllerId = localPlayer->GetControllerId();
				ELoginStatus::Type status = identityInterface->GetLoginStatus(controllerId);
				DISPLAY_LOG("Login Status: %s", ELoginStatus::ToString(status));
			}
		}
		else {
			DISPLAY_LOG(" Login FAILED ");
		}
	}
}

bool AMyPlayerController::HostSession() {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem) {
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid()) {
			TSharedPtr<class FOnlineSessionSettings> sessionSettings = MakeShareable(new FOnlineSessionSettings());
			sessionSettings->NumPublicConnections = 4; // 4 Online Connection
			sessionSettings->NumPrivateConnections = 4; // 4 LAN Connection
			sessionSettings->bShouldAdvertise = true; // should broadcast
			sessionSettings->bAllowJoinInProgress = true; // allow join after session begins
			sessionSettings->bAllowInvites = true; // allow invite receival

			sessionSettings->bUsesPresence = true; // use friendlist
			sessionSettings->bAllowJoinViaPresence = true;// join via friendlist
			sessionSettings->bUseLobbiesIfAvailable = true;// Use lobby if available in subsystem

			// Via Online service and Lan !!
			sessionSettings->Set(SEARCH_KEYWORDS, FString("AIEI_UNREAL_GAME"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnCreateSessionCompleteDelegate));
			TSharedPtr<const FUniqueNetId> uniqueNetId = GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
			bool res = sessionInterface->CreateSession(*uniqueNetId, SESSION_NAME, *sessionSettings);

			if (res) {
				DISPLAY_LOG("CREATED SESSION");
			}
			else {
				DISPLAY_LOG("FAILED TO CREATE SESSION");
			}
		}
	}

	return false;
}

void AMyPlayerController::OnCreateSessionCompleteDelegate(FName InSessionName, bool bWasSuccessful) {
	if (bWasSuccessful) {
		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap")), true, "listen");
	}
}

void AMyPlayerController::FindSession() {
	IOnlineSubsystem* subSystem = Online::GetSubsystem(GetWorld());

	if (subSystem) {
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();
		if (sessionInterface.IsValid()) {
			searchSettings = MakeShareable(new FOnlineSessionSearch());

			searchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			searchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			searchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("AIEI_UNREAL_GAME"), EOnlineComparisonOp::Equals);

			sessionInterface->AddOnEndSessionCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnFindSessionCompleteDelegate));
			TSharedRef<FOnlineSessionSearch> searchSettingsRef = searchSettings.ToSharedRef();
			TSharedPtr<const FUniqueNetId> uniqueNetId = GetLocalPlayer()->GetPreferredUniqueNetId().GetUniqueNetId();

			bool res = sessionInterface->FindSessions(*uniqueNetId, searchSettingsRef);

			if (res) {
				DISPLAY_LOG("FOUND SESSION ");
			}
			else {
				DISPLAY_LOG("FAILED TO FIND SESSION");
			}
		}
	}
}