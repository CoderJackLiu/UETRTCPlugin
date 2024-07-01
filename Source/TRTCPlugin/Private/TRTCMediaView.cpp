#include "TRTCMediaView.h"

//todo liuqi   all need realize

bool FTRTCMediaView::GetViewField(float& OutHorizontal, float& OutVertical) const
{
	return IMediaView::GetViewField(OutHorizontal, OutVertical);
}

bool FTRTCMediaView::GetViewOrientation(FQuat& OutOrientation) const
{
	return IMediaView::GetViewOrientation(OutOrientation);
}

bool FTRTCMediaView::SetViewField(float Horizontal, float Vertical, bool Absolute)
{
	return IMediaView::SetViewField(Horizontal, Vertical, Absolute);
}

bool FTRTCMediaView::SetViewOrientation(const FQuat& Orientation, bool Absolute)
{
	return IMediaView::SetViewOrientation(Orientation, Absolute);
}
