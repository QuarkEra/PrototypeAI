// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_MonsterController.h"

#include "GodVent.h"
#include "Director.h"
#include "MonsterCharacter.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"


void AAI_MonsterController::ReceiveNewDirector(ADirector* NewDirector)
{
	if (ensure(NewDirector != nullptr))
	{
		this->MyDirector = NewDirector;
	}
}

const FVector& AAI_MonsterController::GetLastKnownPlayerLocation() const
{
	return LastKnownPlayerLocation;
}

// @TODO: DELETE THIS UNUSED
void AAI_MonsterController::SetLastKnownPlayerLocation(FVector NewLastKnownPlayerLocation)
{
	this->LastKnownPlayerLocation = NewLastKnownPlayerLocation;
}

const FVector& AAI_MonsterController::GetSeenPlayerLocation() const
{
	return SeenPlayerLocation;
}

void AAI_MonsterController::SetSeenPlayerLocation(FVector NewSeenPlayerLocation)
{
	this->SeenPlayerLocation = NewSeenPlayerLocation;
}

bool AAI_MonsterController::IsWantsToHunt() const
{
	return bWantsToHunt;
}

void AAI_MonsterController::SetWantsToHunt(const bool NewbWantsToHunt)
{
	this->bWantsToHunt = NewbWantsToHunt;
}

void AAI_MonsterController::SetWantsToVent(const bool NewbWantsToVent)
{
	this->bWantsToVent = NewbWantsToVent;
}

bool AAI_MonsterController::IsWantsToVent() const
{
	return bWantsToVent;
}

APawn* AAI_MonsterController::GetHuntedPawn() const
{
	return HuntedPawn;
}

void AAI_MonsterController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = GetPawn();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGodVent::StaticClass(), VentActors);

	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	MonsterCharacter = Cast<AMonsterCharacter>(ControlledPawn);
	if (MonsterCharacter != nullptr)
	{
		CharacterMovementComponent = MonsterCharacter->GetCharacterMovement();
	}

	if (ControlledPawn)
	{
		 SensingComponent = ControlledPawn->FindComponentByClass<UPawnSensingComponent>();
		if (SensingComponent)
		{
			SensingComponent->OnSeePawn.AddDynamic(this, &AAI_MonsterController::OnPawnSeen);
			SensingComponent->OnHearNoise.AddDynamic(this, &AAI_MonsterController::OnNoiseHeard);
			
			MySightRadius = SensingComponent->SightRadius;
		}
	}

	//SensingComponent->SetSensingUpdatesEnabled(false);
	//MonsterCharacter->GetMesh()->SetVisibility(false);
	//MonsterCharacter->SetActorEnableCollision(false);
	//MonsterCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//RoamTimer = 15.f;
	//PlayerSeenTimer = 4.5f;
	//MaxIdleTimer = 35.f;
	//PostHostilityTimer = 1.5f;
	//OutOfVentTimer = 15.f;
	
}

void AAI_MonsterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	/////////////////////////////////////////////////////////////////////////////
	// Use Switch case by state to decide what to do
	/////////////////////////////////////////////////////////////////////////////
	
	if (Result.Code == EPathFollowingResult::Success)
	{
		// The AI character has reached the destination
		UE_LOG(LogTemp, Warning, TEXT("AI character has reached the destination."));
		if (bTravellingToPoint)
		{
			bTravellingToPoint = false;
			GetWorld()->GetTimerManager().SetTimer(TH_FailedMovement, this, &AAI_MonsterController::GetNewAction, 5.f);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(TH_FailedMovement, this, &AAI_MonsterController::GetNewAction, 5.f);
		}
		
	}
	else
	{
		// The AI character failed to reach the destination
		UE_LOG(LogTemp, Warning, TEXT("AI character failed to reach the destination."));
		/*
		if (CurrentState != EMonsterState::Wandering_Out_Of_Vent)
		{
			SetNewState(CurrentState, HuntedPawn);
		}
		*/
		
	}
}

void AAI_MonsterController::InitMonster()
{
	MonsterCharacter->SetActorHiddenInGame(false);
	SensingComponent->SetSensingUpdatesEnabled(true);
	MonsterCharacter->SetActorEnableCollision(true);
	MonsterCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TimePawnSeen = 0.f;
	SearchAttempts = 0;
	PlacesToSearch = 9;
	MaxSearchAttempts = 2;
	//SetNewState(EMonsterState::Idle, nullptr);
	HuntedPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	SetNewState(EMonsterState::Wandering_Out_Of_Vent, HuntedPawn);
}

void AAI_MonsterController::OnPawnSeen(APawn* PawnSeen)
{
	bSeenPlayer = true;
	bWantsToVent = false;
	LastKnownPlayerLocation = PawnSeen->GetActorLocation();
	
	// GetWorld()->GetTimerManager().SetTimer(Roam_TimerHandle, this, &AAI_MonsterController::ReturnToVent,RoamTimer);
	
	if (CurrentState != EMonsterState::Hostile)
	{
		HuntedPawn = PawnSeen;
				
		const FVector PawnLocation = LastKnownPlayerLocation;
		const FVector MonsterLocation = MonsterCharacter->GetActorLocation();
		const double VectorDistance = UKismetMathLibrary::Vector_Distance2D(PawnLocation, MonsterLocation);

		SightDistanceMultiplier = FMath::GetMappedRangeValueClamped(FVector2d(MySightRadius, 0), FVector2d(1, 6), VectorDistance);
		SightDistanceMultiplier = FMath::Clamp(SightDistanceMultiplier, 0, MySightRadius);
		TimePawnSeen += (UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * SightDistanceMultiplier);
		
		UE_LOG(LogTemp, Display, TEXT("Distance to player: %f, Multiplier to speed up pawn detection by distance: %f"), VectorDistance, SightDistanceMultiplier)
		
		if (TimePawnSeen >= 0.25f)
		{
			// GetWorldTimerManager().ClearTimer(Roam_TimerHandle);
			
			// Increases Menace when Pawn is visible to monster
			MyDirector->ChangeMenaceGauge(TimePawnSeen * 2);
			float MG = MyDirector->CheckMenaceGauge();
			UE_LOG(LogTemp, Display, TEXT("Looking Around, Menace Gauge: %f"), MG)
			
			bIsHostile = true;
			SearchAttempts = 0;
			MonsterCharacter->MonsterHostileScream();
			
			if (PawnSeen != nullptr && bIsHostile)
			{
				SetNewState(EMonsterState::Hostile, PawnSeen);
			}
		}
		else
		{
			StopMovement();
			
			if (SearchAttempts == 1)
			{
				MonsterCharacter->MonsterScream();
				// SetFocus(PawnSeen->GetOwner(), EAIFocusPriority::Gameplay);
				// GetWorld()->GetTimerManager().SetTimer(PlayerSeen_TimerHandle, this, &AAI_MonsterController::ReturnToVent,PlayerSeenTimer);
			}
		}
	}
}

void AAI_MonsterController::OnNoiseHeard(APawn* HeardPawn, const FVector& NoiseLocation, float Volume)
 {
	NoiseHeardLocation = NoiseLocation;

	// Small MG increment for player making noise/distractions
	MyDirector->ChangeMenaceGauge(10.f);
	
	if (HeardPawn != nullptr)
	{
		HuntedPawn = HeardPawn;
		SetNewState(EMonsterState::Hunting_Out_Of_Vent, HuntedPawn);

		/*
		if (bInVent)
		{
			bWantsToHunt = true;
			
			NextState = EMonsterState::Hunting_In_Vent;
			// SetWantsToVent(false);
			AGodVent* NearestVent = GetNearestVentToHuntPlayerFrom(HeardPawn);
			// FVector VentLocation = NearestVent->GetActorLocation();
			MonsterCharacter->SetActorEnableCollision(true);
			SensingComponent->SetSensingUpdatesEnabled(false);

			if (MonsterCharacter->IsOverlappingActor(NearestVent))
			{
				ExitVent();
			}
			else
			{
				MoveToActor(NearestVent);
			}
			
		}
		else
		{
			SetNewState(EMonsterState::Hunting_Out_Of_Vent, HuntedPawn);
			HuntLocation(NoiseHeardLocation);
		}
		*/
	}
}

FNavLocation AAI_MonsterController::HuntAroundPlayerLocation()
{
	if (NavSys)
	{
		FVector Origin;
		FNavLocation RandomLocation;
		float Radius = 1000.f;
			
		if (bSeenPlayer)
		{
			Origin = LastKnownPlayerLocation;
			Radius *= (SearchAttempts * 0.75);
		}
		else
		{
			Origin = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
			LastKnownPlayerLocation = Origin;
			bSeenPlayer = true;
		}
		
		UE_LOG(LogTemp, Display, TEXT("Radius for search...%f"), Radius)
		
		if (NavSys->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation))
		{
			// Debug for radius visual and a direct spot to head to within that radius
			/*DrawDebugSphere(
				GetWorld(),
				Origin,
				Radius,
				112,
				FColor::Green,
				false,
				4.f,
				0,
				1);
			DrawDebugSphere(
				GetWorld(),
				RandomLocation,
				32,
				12,
				FColor::Blue,
				false,
				4.f,
				0,
				1);
				*/
			return RandomLocation;
		}
	}
	return {};
}

void AAI_MonsterController::HuntLocation(FVector HuntCoords)
{
	MoveToLocation(HuntCoords, 25.f, true, true, false, true);
}

void AAI_MonsterController::StartRoamingFromHostile()
{
	TimePawnSeen = 0.f; // Player not currently seen
	SetNewState(EMonsterState::Roaming, nullptr);
}

void AAI_MonsterController::Roam()
{	
	const FNavLocation Location = HuntAroundPlayerLocation();
	HuntLocation(Location);
	//GetWorld()->GetTimerManager().SetTimer(Roam_TimerHandle, this, &AAI_MonsterController::ReturnToVent,RoamTimer);
}

void AAI_MonsterController::ReturnToVent()
{
	//MyDirector->GetNewAction();
	/*
	if (SearchAttempts <= MaxSearchAttempts && bSeenPlayer)
	{
		MyDirector->
	}
	else if (SearchAttempts > MaxSearchAttempts)
	{
		UE_LOG(LogTemp, Display, TEXT("Returning To Vent after Search Attempts: %i, SearchAttempts too high"), SearchAttempts)
		bSeenPlayer = false;
		bIsHostile = false;
		SearchAttempts = 0;
		TimePawnSeen = 0.f;
		SetNewState(EMonsterState::Idle, nullptr);
	}
	////////////////////////////////////////////////////////////////////
	// Director now doing this
	////////////////////////////////////////////////////////////////////
	else // hunted a noise, no other disturbance detected
	{
		bSeenPlayer = false;
		UE_LOG(LogTemp, Display, TEXT("Returning To Vent after Search Attempts: %i"), SearchAttempts)
		bIsHostile = false;
		SearchAttempts = 0;
		TimePawnSeen = 0.f;
		SetNewState(EMonsterState::Idle, nullptr);
	}
	*/
}

AGodVent* AAI_MonsterController::GetNearestVentToHideIn()
{
	AGodVent* NearestVent = nullptr;
	if (VentActors.Num() > 0)
	{
		FVector TargetVector = MonsterCharacter->GetActorLocation();
		double NearestVentLocation = DBL_MAX;
		
		for (auto Element : VentActors)
		{
			double Distance = UKismetMathLibrary::Vector_Distance2D(TargetVector, Element->GetActorLocation());
			
			if (Distance < NearestVentLocation)
			{
				NearestVentLocation = Distance;
				NearestVent = Cast<AGodVent>(Element);
			}
		}
	}
	
	return NearestVent;
}

// Used when Noise is Heard
AGodVent* AAI_MonsterController::GetNearestVentToHuntPlayerFrom(APawn* NewHuntedPawn)
{
	AGodVent* NearestVent = nullptr;
	
	if (VentActors.Num() > 0)
	{
		FVector TargetVector;
		double NearestVentLocation = DBL_MAX;
		
		for (const auto Element : VentActors)
		{
			if (NewHuntedPawn != nullptr)
			{
				TargetVector = UGameplayStatics::GetPlayerCharacter(this,0)->GetActorLocation();
			}

			const double Distance = UKismetMathLibrary::Vector_Distance2D(TargetVector, Element->GetActorLocation());
			
			if (Distance < NearestVentLocation)
			{
				NearestVentLocation = Distance;
				NearestVent = Cast<AGodVent>(Element);
			}
		}
	}
	
	return NearestVent;
}

AGodVent* AAI_MonsterController::GetNearestVentToHuntFrom(const FVector& NewNoiseHeardLocation)
{
	AGodVent* NearestVent = nullptr;
	
	if (VentActors.Num() > 0)
	{
		FVector TargetVector;
		double NearestVentLocation = DBL_MAX;
		
		for (const auto Element : VentActors)
		{
			if (NewNoiseHeardLocation != FVector::ZeroVector)
			{
				TargetVector = NewNoiseHeardLocation;
			}

			const double Distance = UKismetMathLibrary::Vector_Distance2D(TargetVector, Element->GetActorLocation());
			
			if (Distance < NearestVentLocation)
			{
				NearestVentLocation = Distance;
				NearestVent = Cast<AGodVent>(Element);
			}
		}
	}
	
	return NearestVent;
}

// Not in use right now
void AAI_MonsterController::MoveToRandomVent()
{
	if (VentActors.Num() > 0)
	{
		int RandInt = FMath::RandRange(0, VentActors.Num());
		if (RandInt != NULL)
		{
			const FVector VentLocation = VentActors[RandInt]->GetActorLocation();
			MoveToLocation(VentLocation);
		}
	}
}

EMonsterState AAI_MonsterController::GetCurrentState() const
{
	return CurrentState;
}

void AAI_MonsterController::SetNewState(EMonsterState NewState, APawn* PawnHunted)
{
	CurrentState = NewState;
	switch (NewState)
	{
	case EMonsterState::Idle:
		StartIdle();
		break;
	case EMonsterState::Searching:
		LookAround();
		break;
	case EMonsterState::Hunting:
		StartHunting(PawnHunted);
		break;
	case EMonsterState::Hostile:
		BecomeHostile(PawnHunted);
		break;
	case EMonsterState::Escaping:
		Escape();
		break;
	case EMonsterState::Roaming:
		Roam();
		break;
	case EMonsterState::Venting:
		break;
	case EMonsterState::Wandering_Out_Of_Vent:
		GetNewAction();
		break;
	case EMonsterState::Wandering_In_Vent:
		break;
	case EMonsterState::Hunting_Out_Of_Vent:
		StartHunting(PawnHunted);
		break;
	case EMonsterState::Hunting_In_Vent:
		break;
	default: break;
	}
}

void AAI_MonsterController::StartIdle()
{
	MonsterCharacter->MonsterScream();
	CharacterMovementComponent->MaxWalkSpeed = 350;
	const AGodVent* VentToHideIn = GetNearestVentToHideIn();
	SetWantsToVent(true);
	SetWantsToHunt(false);
	MoveToLocation(VentToHideIn->GetActorLocation());
}

void AAI_MonsterController::PrepareToSearch()
{
	if (CurrentState == EMonsterState::Idle || CurrentState == EMonsterState::Escaping)
	{
		NextState = EMonsterState::Searching;
		
		UE_LOG(LogTemp, Display, TEXT("Preparing to search"))
		
		RandomLocationToSearch = HuntAroundPlayerLocation();
		bWantsToHunt = true;
		AGodVent* NearestVent = GetNearestVentToHuntPlayerFrom(GetHuntedPawn());
		MonsterCharacter->SetActorEnableCollision(true);
		SensingComponent->SetSensingUpdatesEnabled(false);
		
		if (MonsterCharacter->IsOverlappingActor(NearestVent))
		{
			ExitVent();
			SetNewState(NextState, GetHuntedPawn());
		}
		else
		{
			MoveToActor(NearestVent);
		}
	}
}

void AAI_MonsterController::FillNavPoints(FNavLocation& NewLocation)
{
	while (NavPoints.Num() < PlacesToSearch)
	{
		NavSys->GetRandomPoint(NewLocation);
		NavPoints.Add(NewLocation);
	}
}

void AAI_MonsterController::GetNewAction()
{
	MyDirector->GiveNewTask();
}

void AAI_MonsterController::GetNextNavPoint()
{
	if (NavPoints.Num() > 0 && NavPointIndex < NavPoints.Num())
	{
		NextPoint = NavPoints[NavPointIndex];

		if (NavPointIndex > 1)
		{
			// set previous navigation point
			LastPoint = NavPoints[NavPointIndex - 2];
		}

		if (NavPointIndex % 3 == 0 && NavPointIndex != 0)
		{
			// backtracking condition met get, last point for next
			NextPoint = LastPoint;
		}
	}
	if (NavPointIndex > NavPoints.Num())
	{
		NavPointIndex = 0;
		NavPoints.Empty();
		FillNavPoints(NextPoint);
	}
	NavPointIndex++;
}

void AAI_MonsterController::LookAround()
{
	if (NavSys)
	{
		bTravellingToPoint = true;
		CharacterMovementComponent->MaxWalkSpeed = 200;
		GetNextNavPoint();
		StartSearching(NextPoint);
	}
}

void AAI_MonsterController::StartSearching(FNavLocation LocationToSearch)
{
	UE_LOG(LogTemp, Display, TEXT("Starting Search"))
	MoveToLocation(LocationToSearch);
	
	// GetWorld()->GetTimerManager().SetTimer(Roam_TimerHandle, this, &AAI_MonsterController::ReturnToVent,RoamTimer);
}

void AAI_MonsterController::StartHunting(const APawn* PawnHunted)
{
	UE_LOG(LogTemp, Display, TEXT("Starting Hunt"))
	MonsterCharacter->MonsterScream();
	CharacterMovementComponent->MaxWalkSpeed = 450;
	MoveToLocation(NoiseHeardLocation);
}

void AAI_MonsterController::BecomeHostile(const APawn* PawnToAttack)
{
	// 10 points per Hostile response
	MyDirector->ChangeMenaceGauge(10.f);
	
	//GetWorld()->GetTimerManager().ClearTimer(PlayerSeen_TimerHandle);
	UE_LOG(LogTemp, Display, TEXT("Becoming Hostile"))
	CharacterMovementComponent->MaxWalkSpeed = 660;
	bWantsToVent = false;
	LastKnownPlayerLocation = PawnToAttack->GetOwner()->GetActorLocation();
	// MoveToActor(UGameplayStatics::GetPlayerCharacter(this, 0));
	SetSeenPlayerLocation(PawnToAttack->GetActorLocation());
	MoveToLocation(SeenPlayerLocation);
	//GetWorld()->GetTimerManager().ClearTimer(PostHostility_TimerHandle);
	//GetWorld()->GetTimerManager().SetTimer(PostHostility_TimerHandle, this, &AAI_MonsterController::StartRoamingFromHostile, PostHostilityTimer);
}

void AAI_MonsterController::StartEscaping()
{
	SetNewState(EMonsterState::Escaping, nullptr);
}

void AAI_MonsterController::Escape()
{
	UE_LOG(LogTemp, Display, TEXT("Escaping"))
	//GetWorld()->GetTimerManager().ClearTimer(OutOfVent_TimerHandle);	// Stop ReturnToVent in ExitVent() 
	CharacterMovementComponent->MaxWalkSpeed = 600;						// Gotta go fast
	const AGodVent* VentToHideIn = GetNearestVentToHideIn();			// Hide faster
	SetWantsToVent(true);												// Can enter vent when close
	SetWantsToHunt(false);												// Will not ExitVent() on overlap
	SensingComponent->SetSensingUpdatesEnabled(false);					// Do not muck around just run away
	MoveToLocation(VentToHideIn->GetActorLocation());				// Go
}

void AAI_MonsterController::EnterVent()
{
	bInVent = true;
	SetWantsToVent(false);
	bSeenPlayer = false;
	//GetWorld()->GetTimerManager().ClearTimer(PlayerSeen_TimerHandle);
	MonsterCharacter->GetMesh()->SetVisibility(false);
	MonsterCharacter->SetActorEnableCollision(false);
	MonsterCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterMovementComponent->StopActiveMovement();
	SensingComponent->SetSensingUpdatesEnabled(true);
	SensingComponent->SightRadius = 0;
	//GetWorld()->GetTimerManager().SetTimer(MaxIdle_TimerHandle, this, &AAI_MonsterController::PrepareToSearch, MaxIdleTimer);
}

void AAI_MonsterController::ExitVent()
{
	bInVent = false;
	SetWantsToVent(false);
	MonsterCharacter->GetMesh()->SetVisibility(true);
	MonsterCharacter->SetActorEnableCollision(true);
	MonsterCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SensingComponent->SetSensingUpdatesEnabled(true);
	SensingComponent->SightRadius = MySightRadius;
	//GetWorld()->GetTimerManager().SetTimer(OutOfVent_TimerHandle, this, &AAI_MonsterController::ReturnToVent,OutOfVentTimer);
	SetNewState(NextState, GetHuntedPawn());
}
