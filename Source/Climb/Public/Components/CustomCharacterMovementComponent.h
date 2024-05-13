// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName="Climb Mode")
	};
}
/**
 * 
 */
UCLASS()
class CLIMB_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
#pragma region ClimbTraces
	
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start,const FVector& End,bool bShowDebugShape=false,bool bDrawPersistantShapes=false);
	FHitResult DoLineTraceSingleByObject(const FVector& Start,const FVector& End,bool bShowDebugShape=false,bool bDrawPersistantShapes=false);
	
#pragma endregion

#pragma region ClimbCore

	bool TraceClimbableSurfaces();
	FHitResult TraceFromEyeHeight(float TraceDistance,float TraceStartOffset=0.f);
	bool CanStartClimbing();
	
#pragma endregion 

#pragma region ClimbCoreVariables

	TArray<FHitResult> ClimbableSurfacesTracedResults;
	
#pragma endregion 
	
#pragma region ClimbBPVariables
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery> > ClimbableSurfaceTraceTypes;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceRadius=50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float ClimbCapsuleTraceHalfHeight=72.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Character Movement: Climbing",meta = (AllowPrivateAccess = "true"))
	float LineTraceDistance=100.f;
	
#pragma endregion

public:

	void ToggleClimbing(bool bEnableClimb);
	bool IsClimbing() const;
};
