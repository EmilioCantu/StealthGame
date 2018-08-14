// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComp->SetSphereRadius(100);
	InnerSphereComp->SetupAttachment(MeshComp);

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	OuterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComp->SetSphereRadius(3000);
	OuterSphereComp->SetupAttachment(MeshComp);

	ExplodeTimer = 150.0f;
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//if (OtherActor)
	//{
	//	OtherActor->Destroy();
	//}


}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ExplodeTimer -= DeltaTime;
	//Find all overlapping components that can collide and may be physically simulating
	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphereComp->GetOverlappingComponents(OverlappingComps);

	if (ExplodeTimer > 0)
	{
		for (int32 i = 0; i < OverlappingComps.Num(); i++)
		{
			PrimComp = OverlappingComps[i];

			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				// the component we are looking for! It needs to be simulating in order to apply forces

				const float SphereRadius = OuterSphereComp->GetScaledSphereRadius();
				const float ForceStrength = -2000; //Negative value to make it pull towards the origin instead of pushing away

				PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
	}
	else
	{
		for (int32 i = 0; i < OverlappingComps.Num(); i++)
		{
			PrimComp = OverlappingComps[i];

			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				// the component we are looking for! It needs to be simulating in order to apply forces

				const float SphereRadius = OuterSphereComp->GetScaledSphereRadius();
				const float ForceStrength = 10000; //Negative value to make it pull towards the origin instead of pushing away

				PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			}
		}
		//ExplodeTimer = 15;
	}

}

