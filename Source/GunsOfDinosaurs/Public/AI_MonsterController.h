// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_MonsterController.generated.h"

class AGodVent;
class UNavigationSystemV1;
class AMonsterCharacter;
class UCharacterMovementComponent;

UENUM()
enum class EMonsterState : uint8
{
	Idle,
	Venting, // @TODO: DELETE VENTING, was not even used prototyping
	Searching, // new for randomly leaving a vent to search for a player in a radial FNavLocation
	Roaming,
	Hunting,
	Hostile,
	Escaping,
};


class UPawnSensingComponent;
/**
 * 
 */
UCLASS()
class GUNSOFDINOSAURS_API AAI_MonsterController : public AAIController
{
	GENERATED_BODY()

	/*
	 * Recreating the AI of Alien: Isolation
	 *
	 * Now I have read a little more of A:I xenomorph behaviour
	 * the idea of having a separate controller to poke the monster out of vents is so obvious
	 * a separate vent controller AI would also be helpful for navigation
	 *
	 * speaking of navigation I recall navmesh filters etc but cannot find the documentation for it
	 * regardless I think what I have so far is pretty damn close for how hacky it is
	 * and for my first AI i have a soft spot for it's squashed bugs
	 * 
	 */

protected:
	// _TimerHandle; how long before X task
	float RoamTimer;							// Before ReturningToVent will likely search again
	float PlayerSeenTimer;						// after Player recognised (delay to allow player to duck/hide) before searching via ReturnToVent
	float MaxIdleTimer;							// when Entering vent remain there until PrepareToSearch
	float PostHostilityTimer;					// used after BecomeHostile and in Roam(), search for player
	float OutOfVentTimer;						// when leaving vent ReturnToVent/Search
	FTimerHandle Roam_TimerHandle;
	FTimerHandle PlayerSeen_TimerHandle;
	FTimerHandle MaxIdle_TimerHandle;
	FTimerHandle PostHostility_TimerHandle;
	FTimerHandle OutOfVent_TimerHandle;
	
	FVector LastKnownPlayerLocation;
	FVector SeenPlayerLocation;
	FVector NoiseHeardLocation;
	bool bIsSearchingForPlayer;
	float MySightRadius;
	float SightDistanceMultiplier;
	UPROPERTY(VisibleInstanceOnly)
	float TimePawnSeen;
	int SearchAttempts;
	int MaxSearchAttempts;
	UPROPERTY(VisibleInstanceOnly)
	bool bSeenPlayer;
	bool bWantsToVent;
	bool bWantsToHunt;
	bool bIsHostile;
	FNavLocation RandomLocationToSearch;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* PawnSeen);
	UFUNCTION()
	void OnNoiseHeard(APawn* HeardPawn, const FVector& NoiseLocation, float Volume);
	UPROPERTY()
	APawn* HuntedPawn;
	
	UPROPERTY()
	UPawnSensingComponent* SensingComponent;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY()
	APawn* ControlledPawn;
	UPROPERTY()
	AMonsterCharacter* MonsterCharacter;
	
	EMonsterState NextState;
	EMonsterState CurrentState;

	UPROPERTY()
	TArray<AActor*> VentActors;
	
	AGodVent* GetNearestVentToHuntPlayerFrom(APawn* NewHuntedPawn);
	AGodVent* GetNearestVentToHuntFrom(const FVector& NewNoiseHeardLocation);
	AGodVent* GetNearestVentToHideIn();
	
	void MoveToRandomVent();
	void ReturnToVent();
	
	UPROPERTY()
	UNavigationSystemV1* NavSys;
	
	FNavLocation HuntAroundPlayerLocation();
	void HuntLocation(FVector HuntCoords);
	void StartRoamingFromHostile();
	void Roam();
	void StartIdle();
	void PrepareToSearch();
	void StartSearching(FNavLocation LocationToSearch);
	
	void StartHunting(const APawn* PawnHunted);
	void BecomeHostile(const APawn* PawnToAttack);

	// kill player cease all movement, play zombie attack animation
	
	
public:
	// I trust Rider with whatever this extra stuff is
	[[nodiscard]] EMonsterState GetCurrentState() const;
	__declspec(property(get = GetCurrentState)) EMonsterState FCurrentState;
	[[nodiscard]] bool IsWantsToHunt() const;
	void SetWantsToHunt(const bool NewbWantsToHunt);
	__declspec(property(get = IsWantsToHunt, put = SetWantsToHunt)) bool FWantsToHunt;
	void SetWantsToVent(const bool NewbWantsToVent);
	__declspec(property(put = SetWantsToVent)) bool FWantsToVent1;
	[[nodiscard]] bool IsWantsToVent() const;
	__declspec(property(get = IsWantsToVent)) bool FWantsToVent;
	[[nodiscard]] const FVector& GetLastKnownPlayerLocation() const;
	void SetLastKnownPlayerLocation(FVector NewLastKnownPlayerLocation);
	__declspec(property(get = GetLastKnownPlayerLocation, put = SetLastKnownPlayerLocation)) FVector
	FLastKnownPlayerLocation;
	[[nodiscard]] const FVector& GetSeenPlayerLocation() const;
	void SetSeenPlayerLocation(FVector NewSeenPlayerLocation);
	__declspec(property(get = GetSeenPlayerLocation, put = SetSeenPlayerLocation)) FVector FSeenPlayerLocation;
	
	[[nodiscard]] APawn* GetHuntedPawn() const;

	UFUNCTION(BlueprintCallable)
	void InitMonster();
	
	void EnterVent();
	void ExitVent();
	
	void StartEscaping();
	void Escape();
	
	void SetNewState(EMonsterState NewState, APawn* PawnHunted);
	
};
