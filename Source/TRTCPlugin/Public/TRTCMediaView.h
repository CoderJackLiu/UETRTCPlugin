#pragma once
#include "IMediaView.h"
#include "TRTCCloud.h"

class FTRTCMediaView : public IMediaView
{
public:
	//
	// /**
	//  * Initialize this object for the specified VLC media player.
	//  *
	//  * @param InPlayer The VLC media player.
	//  */
	// void Initialize(FLibvlcMediaPlayer& InPlayer);
	//
	/** Shut down this object. */
	void Shutdown();

public:
	//~ IMediaView interface

	virtual bool GetViewField(float& OutHorizontal, float& OutVertical) const override;
	virtual bool GetViewOrientation(FQuat& OutOrientation) const override;
	virtual bool SetViewField(float Horizontal, float Vertical, bool Absolute) override;
	virtual bool SetViewOrientation(const FQuat& Orientation, bool Absolute) override;

private:
	/** The current field of view (horizontal & vertical). */
	float CurrentFieldOfView = 0.f;

	/** The current view orientation. */
	FQuat CurrentOrientation;

	/** The VLC media player object. */
	ue::TRTCCloud* Player = nullptr;
	
	/** The VLC video viewpoint. */
	// FLibvlcVideoViewpoint* Viewpoint;
};
