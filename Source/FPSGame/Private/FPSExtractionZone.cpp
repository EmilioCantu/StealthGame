// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	overlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	overlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	overlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	overlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	overlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = overlapComp;

	overlapComp->SetHiddenInGame(false);

	overlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	decalComp->DecalSize = FVector(200.0f);
	decalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!!"));

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	if (MyPawn && MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn);
		}
	}
}