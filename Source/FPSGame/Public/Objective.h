// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Objective.generated.h"

UCLASS()
class FPSGAME_API AObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjective();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Mesh
	UPROPERTY(VisibleAnywhere, Category = "Components");
	class UStaticMeshComponent* MeshComp;

	//Collider
	UPROPERTY(VisibleAnywhere, Category = "Components");
	class USphereComponent* SphereColliderComp;

	FVector StartLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
