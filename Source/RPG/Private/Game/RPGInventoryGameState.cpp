// Copyright Dan Stull


#include "Game/RPGInventoryGameState.h"

ARPGInventoryGameState::ARPGInventoryGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("DataTable'/Game/Data/ItemDB.ItemDB'"));
	ItemDB = BP_ItemDB.Object;
}

UDataTable* ARPGInventoryGameState::GetItemDB() const
{
	return ItemDB; 
}
