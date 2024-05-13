// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomCharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Climb/ClimbCharacter.h"
#include "Climb/DebugHelper.h"

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


#pragma region ClimbTraces

TArray<FHitResult> UCustomCharacterMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start,
                                                                                  const FVector& End, bool bShowDebugShape,bool bDrawPersistantShapes)
{
	TArray<FHitResult> OutCapsuleTraceHitResults;

	EDrawDebugTrace::Type DebugTraceType=EDrawDebugTrace::None;
	if(bShowDebugShape)
	{
		DebugTraceType=EDrawDebugTrace::ForOneFrame;
		if(bDrawPersistantShapes)
		{
			DebugTraceType=EDrawDebugTrace::Persistent;
		}
	}
	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbCapsuleTraceRadius,
		ClimbCapsuleTraceHalfHeight,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		DebugTraceType,
		OutCapsuleTraceHitResults,
		false
	);

	return OutCapsuleTraceHitResults;
}

FHitResult UCustomCharacterMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End,
	bool bShowDebugShape,bool bDrawPersistantShapes)
{
	FHitResult OutLineTraceHitResult;

	EDrawDebugTrace::Type DebugTraceType=EDrawDebugTrace::None;
	if(bShowDebugShape)
	{
		DebugTraceType=EDrawDebugTrace::ForOneFrame;
		if(bDrawPersistantShapes)
		{
			DebugTraceType=EDrawDebugTrace::Persistent;
		}
	}
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		DebugTraceType,
		OutLineTraceHitResult,
		false
	);
	return OutLineTraceHitResult;
}
#pragma endregion

#pragma region ClimbCore
void UCustomCharacterMovementComponent::ToggleClimbing(bool bEnableClimb)
{
	if(bEnableClimb)
	{
		if(CanStartClimbing())
		{
			//enter climbing state
			Debug::Print(TEXT("Can Start Climbing"));
		}
		else
		{
			Debug::Print(TEXT("Can Not Start Climbing"));
		}
	}
	else
	{
		//stop climbing
	}
}

bool UCustomCharacterMovementComponent::CanStartClimbing()
{
	if(IsFalling()) return false;
	if(!TraceClimbableSurfaces()) return false;
	if(!TraceFromEyeHeight(LineTraceDistance).bBlockingHit) return false;
	return true;
}

bool UCustomCharacterMovementComponent::IsClimbing() const
{
	return MovementMode==MOVE_Custom&&CustomMovementMode==ECustomMovementMode::MOVE_Climb;
}
bool UCustomCharacterMovementComponent::TraceClimbableSurfaces()
{
	FVector StartOffset=UpdatedComponent->GetForwardVector()*30.f;
	FVector Start=UpdatedComponent->GetComponentLocation()+StartOffset;
	FVector End=Start+UpdatedComponent->GetForwardVector();
	ClimbableSurfacesTracedResults=DoCapsuleTraceMultiByObject(Start,End,true,true);
	return !ClimbableSurfacesTracedResults.IsEmpty();
}

FHitResult UCustomCharacterMovementComponent::TraceFromEyeHeight(float TraceDistance,float TraceStartOffset)
{
	const FVector ComponentLocation=UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset=UpdatedComponent->GetUpVector()*(CharacterOwner->BaseEyeHeight+TraceStartOffset);
	const FVector Start=ComponentLocation+EyeHeightOffset;
	const FVector End=Start+UpdatedComponent->GetForwardVector()*TraceDistance;
	return DoLineTraceSingleByObject(Start,End,true,true);
}
#pragma endregion 
