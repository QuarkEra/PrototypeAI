// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_MonsterController.generated.h"

class ADirector;
class AGodVent;
class AGodCharacter;
class UNavigationSystemV1;
class AMonsterCharacter;
class UCharacterMovementComponent;
class UPawnSensingComponent;



UENUM()
enum class EMonsterState : uint8
{
	Idle,
	Venting, // @TODO: DELETE VENTING, was not even used prototyping
	Wandering_Out_Of_Vent,
	Wandering_In_Vent,
	Hunting_Out_Of_Vent,
	Hunting_In_Vent,
	Searching, // new for randomly leaving a vent to search for a player in a radial FNavLocation
	Roaming,
	Hunting,
	Hostile,
	Escaping,
	// expand and describe
	// Alien_InVent_Threat_Aware
	// Alien_OutVent_Distracted
	// Alien_OutVent_Hit_And_Run
	// ALien_Returning_To_Vent
	// GameplayTags better than enums for branching states?
	// inactive/active, vented/outvent, aware/unaware
	// searching/distracted, 
};

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
	* @TODO: DirectorAI AActor
	* Director (AActor) placed in scene, selection in editor of character or characters
	* to direct (Monster/s)
	*
	* Monster Character has mostly accessible BlueprintVisible properties for aggression,
	*
	*
	*/

protected:

	UPROPERTY()
	ADirector* MyDirector;

public:
	void ReceiveNewDirector(ADirector* NewDirector);
	
	bool bInVent;
	
protected:
	// Timers will be replaced with a manager Actor that is placed in the scene

	// _TimerHandle; how long before X task
	float RoamTimer; // Before ReturningToVent will likely search again
	float PlayerSeenTimer; // after Player recognised (delay to allow player to duck/hide) before searching via ReturnToVent
	float MaxIdleTimer; // when Entering vent remain there until PrepareToSearch
	float PostHostilityTimer; // used after BecomeHostile and in Roam(), search for player
	FTimerHandle Roam_TimerHandle;
	FTimerHandle PlayerSeen_TimerHandle;
	FTimerHandle MaxIdle_TimerHandle;
	FTimerHandle PostHostility_TimerHandle;

	// @TODO: first on the chopping board is the old patrol timer now that patrol is indefinite
	float OutOfVentTimer; // when leaving vent ReturnToVent/Search
	FTimerHandle OutOfVent_TimerHandle;


	/*
	* Director Functions
	*/
	void MoveToRandomVent();
	void ReturnToVent();
	// DONE void CheckMenaceGuage
	// void GiveAlienNewTask(AlienCharacter); 
	// new monster function void RecieveNewTask Adds 
	// activate and disable monster



	/*
	* Bluprint in CharacterActor to set up modular monsters per project
	*/
	UPROPERTY()
	UPawnSensingComponent* SensingComponent;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
	int SearchAttempts;
	int MaxSearchAttempts;
	float MySightRadius;
	float SightDistanceMultiplier;
	// @TODO: BP visible setup for sight hearing etc per monster



	/*
	* Character Functions
	*/

	// @TODO: Update MenaceGauge on pawn interactions including player lineofsight
	UFUNCTION()
	void OnPawnSeen(APawn* PawnSeen);
	UFUNCTION()
	void OnNoiseHeard(APawn* HeardPawn, const FVector& NoiseLocation, float Volume);
	void Roam();
	void BecomeHostile(const APawn* PawnToAttack);
	void HuntLocation(FVector HuntCoords);
	void StartRoamingFromHostile();
	void StartSearching(FNavLocation LocationToSearch);
	void StartHunting(const APawn* PawnHunted);
	void StartIdle();
	AGodVent* GetNearestVentToHuntPlayerFrom(APawn* NewHuntedPawn);
	AGodVent* GetNearestVentToHuntFrom(const FVector& NewNoiseHeardLocation);
	AGodVent* GetNearestVentToHideIn();
	FNavLocation HuntAroundPlayerLocation();

	void KillPlayer();
	bool PlayerCaught;
	FTimerHandle TH_ShortDelay_TimerHandle;
	void RestartLevel();

	/*
	* MonsterAI stuff that can remain post refactor
	*/

	FVector LastKnownPlayerLocation;
	FVector SeenPlayerLocation;
	FVector NoiseHeardLocation;
	// @TODO: no longer needed in instance
	UPROPERTY(VisibleInstanceOnly)
	float TimePawnSeen;
	UPROPERTY(VisibleInstanceOnly)
	bool bSeenPlayer;

	bool bIsSearchingForPlayer;
	bool bWantsToVent;
	bool bWantsToHunt;
	bool bIsHostile;


	FNavLocation RandomLocationToSearch;

	virtual void BeginPlay() override;

	UPROPERTY()
	APawn* HuntedPawn;
	UPROPERTY()
	APawn* ControlledPawn;
	UPROPERTY()
	AMonsterCharacter* MonsterCharacter;

	EMonsterState NextState;
	EMonsterState CurrentState;

	UPROPERTY()
	TArray<AActor*> VentActors;
	UPROPERTY()
	UNavigationSystemV1* NavSys;

	void GetNewAction();

	TArray<FNavLocation> NavPoints;
	UPROPERTY(EditAnywhere)
	int PlacesToSearch;

	void GetNextNavPoint();
	int NavPointIndex;
	FNavLocation NextPoint;
	FNavLocation BacktrackPoint;
	FNavLocation LastPoint;
	bool bTravellingToPoint;
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	FTimerHandle TH_FailedMovement;

	UPROPERTY()
	AGodCharacter* GodPlayer;
	
	float DotProductToOtherActor(AActor* OtherActor);

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

	// DIRECTOR ACCESSORS
	[[nodiscard]] APawn* GetHuntedPawn() const;
	UFUNCTION(BlueprintCallable)
	void InitMonster();
	// @TODO: DisableMonster()
	void EnterVent();
	void ExitVent();

	// NextState is set by monster and can be triggered as new/current
	// when vent is finally reached
	void SetNextState();
	// @TODO: did not know timer can be set with function params
	void StartEscaping();
	void Escape();
	void SetNewState(EMonsterState NewState, APawn* PawnHunted);

	void PrepareToSearch();
	void FillNavPoints(FNavLocation& NewLocation);
	void LookAround();
};
