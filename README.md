# UETRTCPlugin

## Description
* The plugin is designed to provide an easy way to integrate Tencent's TRTC SDK with Unreal Engine 5.1. 
* TRTC SDK version : 11.9.0.15031

## Features
1. TRTC SDK integration;
2. TRTC Video Player Intergration which you can play RTMP and TRTC url for free;
3. TRTC living Room / Video Call /Audio Call / Voice Chat Room are okay;
4. RenderTargetTexture2D rendering when use UTRTCLivePlayer;

## Installation

1. Clone the repository to your Unreal Engine project's Plugins folder:
    ```
    cd <YourUnrealProject>/Plugins/
    git clone https://github.com/CoderJackLiu/UETRTCPlugin.git
    ```
2. Open the project in Unreal Engine.
3. Enable the plugin in the Plugins menu.

## How to use
1. **Play a RTMP /TRTC URL**
- TRTC player is free for our users.
- Create a TRTCLivePlayer 
    ```
    UTRTCLivePlayer* TRTCPlayer= NewObject<UTRTCLivePlayer>();
    TRTCPlayer->AddToRoot();
    TRTCPlayer->InitializePlayer();
    ```
- Start play
    ```
    TRTCPlayer->StartPlay(url);
    ```
- StopPlay
    ```
    TRTCPlayer->StopPlay();
    ```
- PauseVideo
    ```
    TRTCPlayer->PauseVideo();
    ```
- You can find it`s API in **TRTCLivePlayer.h** file.
---

2. **Use TRTC as TRTC live room /Video call /audio call**
- It is not free for us to use . You should create your accound in tencent cloud https://console.cloud.tencent.com/trtc
---

## Reference
* [Activate TRTC Live Play Service ](https://console.cloud.tencent.com/trtc)
* [API Doc](https://liteav.sdk.qcloud.com/doc/api/zh-cn/index.html)
* [Forked Plugin](https://github.com/LiteAVSDK/TRTC_UnrealEngine.git)


    
 