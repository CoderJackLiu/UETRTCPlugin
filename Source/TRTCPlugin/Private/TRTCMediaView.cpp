#include "TRTCMediaView.h"

#include "TRTCLogMacro.h"

//todo liuqi   all need realize

void FTRTCMediaView::Shutdown()
{
	Player = nullptr;
}

bool FTRTCMediaView::GetViewField(float& OutHorizontal, float& OutVertical) const
{
	OutHorizontal = CurrentFieldOfView;
	OutVertical = CurrentFieldOfView;
	return true;
}

bool FTRTCMediaView::GetViewOrientation(FQuat& OutOrientation) const
{
	OutOrientation = CurrentOrientation;
	return true;
}

bool FTRTCMediaView::SetViewField(float Horizontal, float Vertical, bool Absolute)
{
	return false;
	//
	// if ((Player == nullptr) || (Viewpoint == nullptr))
	// {
	// 	return false;
	// }
	//
	// if (!Absolute)
	// {
	// 	Horizontal = CurrentFieldOfView + Horizontal;
	// }
	//
	// FVector Euler = CurrentOrientation.Euler();

	// Viewpoint->Roll = Euler.X;
	// Viewpoint->Pitch = Euler.Y;
	// Viewpoint->Yaw = Euler.Z;
	// Viewpoint->FieldOfView = FMath::ClampAngle(Horizontal, 10.0f, 360.0f);
	//
	// UE_LOG(LogTRTCMedia, VeryVerbose, TEXT("Setting viewpoint to %f %f %f / %f."), Viewpoint->Roll, Viewpoint->Pitch, Viewpoint->Roll, Viewpoint->FieldOfView);

	// if (FVlc::VideoUpdateViewpoint(Player, Viewpoint, true) != 0)
	// {
	// 	return false;
	// }

	CurrentFieldOfView = Horizontal;

	return true;
}

bool FTRTCMediaView::SetViewOrientation(const FQuat& Orientation, bool Absolute)
{
	return false;
	
	// if ((Player == nullptr) || (Viewpoint == nullptr))
	// {
	// 	return false;
	// }
	//
	// FQuat NewOrientation;
	//
	// if (Absolute)
	// {
	// 	NewOrientation = Orientation;
	// }
	// else
	// {
	// 	NewOrientation = Orientation * CurrentOrientation;
	// }
	//
	// FVector Euler = NewOrientation.Euler();
	//
	// Viewpoint->Roll = Euler.X;
	// Viewpoint->Pitch = Euler.Y;
	// Viewpoint->Yaw = Euler.Z;
	// Viewpoint->FieldOfView = CurrentFieldOfView;
	//
	// UE_LOG(LogTRTCMedia, VeryVerbose, TEXT("Setting viewpoint to %f %f %f / %f."), Viewpoint->Roll, Viewpoint->Pitch, Viewpoint->Roll, Viewpoint->FieldOfView);
	//
	// // if (FVlc::VideoUpdateViewpoint(Player, Viewpoint, true) != 0)
	// // {
	// // 	return false;
	// // }
	//
	// CurrentOrientation = NewOrientation;
	// return true;
}
