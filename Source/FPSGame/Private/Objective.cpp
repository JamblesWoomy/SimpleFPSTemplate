// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"
#include "GameFramework/GameStateBase.h"
#include "FPSCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AObjective::AObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereColliderComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereColliderComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereColliderComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereColliderComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereColliderComp->SetupAttachment(MeshComp);

	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called when the game starts or when spawned
void AObjective::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		StartLocation = GetActorLocation();
	}
}

// Called every frame
void AObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		double ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		FVector NewLocation = StartLocation + FVector(0.0f, 0.0f, 20 * cos(ServerTime * 2.5f));
		SetActorLocation(NewLocation);
	}
}

void AObjective::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	if (GetLocalRole() == ROLE_Authority) {
		AFPSCharacter* mCharacter = Cast<AFPSCharacter>(OtherActor);
		if (mCharacter) {
			//mCharacter->bIsCarryingObjective = true;
			Destroy();
		}
	}
}

