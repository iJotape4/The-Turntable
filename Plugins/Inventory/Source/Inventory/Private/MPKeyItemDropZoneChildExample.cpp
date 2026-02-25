#include "MPKeyItemDropZoneChildExample.h"

#include "EventRouterSubsystem.h"
#include "MPInventoryEventPayloads.h"

AMPKeyItemDropZoneChildExample::AMPKeyItemDropZoneChildExample()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMPKeyItemDropZoneChildExample::BeginPlay()
{
	Super::BeginPlay();
	UEventRouterSubsystem::SubscribeToEvent(this, EventTopic.GetTagName(), &AMPKeyItemDropZoneChildExample::DummyTest);
}

void AMPKeyItemDropZoneChildExample::DummyTest(const FDummyTestEvent& event)
{
	UE_LOG(LogTemp, Warning, TEXT("Dummy Test"));
}

bool AMPKeyItemDropZoneChildExample::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!IsPlayerInRadius()) return false;
	
	UEventRouterSubsystem::BroadcastEvent(this, EventTopic.GetTagName(), FMatchKeyItemEvent{this});
	return true;
}

void AMPKeyItemDropZoneChildExample::AcceptItem(UMPItem* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Item accepted"));
}

void AMPKeyItemDropZoneChildExample::FinishPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Puzzle finished"));
	DispatchEvent(FDummyTestEvent());
}