// Copyright Dan Stull


#include "UI/Widget/RPGUserWidget.h"


void URPGUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
/*
bool URPGUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation);
	if (IsValid(DragWidgetResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast return null."));
		return false;
	}

	const FVector2d DrawWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	const FVector2d DrawWindowOffsetResult = DrawWindowOffset - DragWidgetResult->DragOffset;

	DragWidgetResult->WidgetReference->AddToViewport();
	DragWidgetResult->WidgetReference->SetVisibility(ESlateVisibility::Visible);
	DragWidgetResult->WidgetReference->SetPositionInViewport(DrawWindowOffsetResult, true);

	return true;
	
	return false;
}
*/