#pragma once
#include "Live2/V2TXLivePlayer.hpp"
#include "TRTCCloudHeaderBase.h"
#include "TRTCLivePlayer.generated.h"


#if PLATFORM_WINDOWS
#define UpdateResource UpdateResource
#endif

//declare delegate when render target is available
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRenderTargetAvailable, UTexture2D*, VideoOutTexture);

//declare delegate when begin play
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBeginPlay, UTexture2D*, VideoOutTexture);

UCLASS(BlueprintType, Blueprintable)
class UTRTCLivePlayer : public UObject, public FTickableGameObject, public V2TXLivePlayerObserver
{
	GENERATED_BODY()

public:
	UTRTCLivePlayer();

	virtual ~UTRTCLivePlayer() override;

private:
	// FTickableObjectBase interface
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "TRTCPlayer")
	void InitializePlayer();

	UFUNCTION(BlueprintCallable, Category = "TRTCPlayer")
	void StartPlay(const FString& url) const;

	UFUNCTION(BlueprintCallable, Category = "TRTCPlayer")
	void StopPlay() ;

	UFUNCTION(BlueprintCallable, Category = "TRTCPlayer")
	void PauseVideo() ;

	UFUNCTION(BlueprintCallable, Category = "TRTCPlayer")
	void ResumeVideo() const;

	UFUNCTION(BlueprintPure, Category = "TRTCPlayer")
	bool IsPlaying() const;

	//FOnRenderTargetAvailable
	UPROPERTY(BlueprintAssignable, Category = "TRTCPlayer")
	FOnRenderTargetAvailable OnRenderTargetAvailable;

	//FOnPlayerBeginPlay
	UPROPERTY(BlueprintAssignable, Category = "TRTCPlayer")
	FOnPlayerBeginPlay OnPlayerBeginPlay;

private:
	//V2TXLivePlayerObserver interface
	virtual void onError(V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) override;
	virtual void onWarning(V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) override;
	virtual void onConnected(V2TXLivePlayer* player, void* extraInfo) override;
	virtual void onVideoPlaying(V2TXLivePlayer* player, bool firstPlay, void* extraInfo) override;
	virtual void onAudioPlaying(V2TXLivePlayer* player, bool firstPlay, void* extraInfo) override;
	virtual void onVideoLoading(V2TXLivePlayer* player, void* extraInfo) override;
	virtual void onAudioLoading(V2TXLivePlayer* player, void* extraInfo) override;
	virtual void onPlayoutVolumeUpdate(V2TXLivePlayer* player, int32_t volume) override;
	virtual void onStatisticsUpdate(V2TXLivePlayer* player, V2TXLivePlayerStatistics statistics) override;
	virtual void onSnapshotComplete(V2TXLivePlayer* player, const char* image, int length, int width, int height, V2TXLivePixelFormat format) override;
	virtual void onRenderVideoFrame(V2TXLivePlayer* player, const V2TXLiveVideoFrame* videoFrame) override;
	virtual void onReceiveSeiMessage(V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize) override;
	virtual void onVideoResolutionChanged(V2TXLivePlayer* player, int width, int height) override;

public:
	//RenderTarget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TRTCPlayer")
	UTexture2D* RenderTargetTexture = nullptr;

private:
	V2TXLivePlayer* live_player_ = nullptr;
	FUpdateTextureRegion2D* UpdateTextureRegion = nullptr;
	uint8* VideoBuffer = nullptr;
	uint32 TextureBufferSize = 0;
	FCriticalSection Mutex;
	bool VideoRefresh = false;
	uint32_t TextureWidth = 0;
	uint32_t TextureHeight = 0;
	bool IsBeginPlay = false;

private:
	void UpdateBuffer(char* RGBBuffer, uint32_t Width, uint32_t Height, uint32_t Size);

	void ResetBuffer();
	void RenderVideoFrameToRT();
};
