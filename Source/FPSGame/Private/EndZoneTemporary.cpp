// Fill out your copyright notice in the Description page of Project Settings.

#include "EndZoneTemporary.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/BoxComponent.h"

// Sets default values
AEndZoneTemporary::AEndZoneTemporary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetBoxExtent(FVector(200.0f));

	//Set box as root component
	RootComponent = BoxComponent;

	//Otherwise box collider
	BoxComponent->SetHiddenInGame(false);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndZoneTemporary::HandleOverlap);
}

// Called when the game starts or when spawned
void AEndZoneTemporary::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEndZoneTemporary::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		AFPSCharacter* mCharacter = Cast<AFPSCharacter>(OtherActor);
		if (mCharacter == nullptr) {
			return;
		}
		if (mCharacter->bIsCarryingObjective) {
			AFPSGameMode* gMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (gMode) {
				gMode->CompleteMission(mCharacter);
			}
		}
	}
}

// Called every frame
void AEndZoneTemporary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

