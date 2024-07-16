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

<hr style="border: 2px solid white;">

 # UETRTC插件

## 描述
* 该插件旨在提供一种将腾讯的TRTC SDK集成到Unreal Engine 5.1中的简单方法。
* TRTC SDK版本：11.9.0.15031

## 特性
1. TRTC SDK集成；
2. TRTC视频播放器集成，您可以免费播放RTMP和TRTC URL；
3. 支持TRTC直播间/视频通话/语音通话/语音聊天室；
4. 使用UTRTCLivePlayer时进行RenderTargetTexture2D渲染；

## 安装

1. 将仓库克隆到您的Unreal Engine项目的插件文件夹中：
    ```
    cd <YourUnrealProject>/Plugins/
    git clone https://github.com/CoderJackLiu/UETRTCPlugin.git
    ```
2. 在Unreal Engine中打开项目。
3. 在插件菜单中启用插件。

## 使用方法
1. **播放RTMP/TRTC URL**
- TRTC播放器对我们的用户是免费的。
- 创建一个TRTCLivePlayer
    ```
    UTRTCLivePlayer* TRTCPlayer= NewObject<UTRTCLivePlayer>();
    TRTCPlayer->AddToRoot();
    TRTCPlayer->InitializePlayer();
    ```
- 开始播放
    ```
    TRTCPlayer->StartPlay(url);
    ```
- 停止播放
    ```
    TRTCPlayer->StopPlay();
    ```
- 暂停视频
    ```
    TRTCPlayer->PauseVideo();
    ```
- 您可以在**TRTCLivePlayer.h**文件中找到它的API。
---

2. **将TRTC用作TRTC直播间/视频通话/音频通话**
- 使用这些功能并不是免费的。您需要在腾讯云创建账户 https://console.cloud.tencent.com/trtc
---

## 参考资料
* [激活TRTC直播播放服务](https://console.cloud.tencent.com/trtc)
* [API文档](https://liteav.sdk.qcloud.com/doc/api/zh-cn/index.html)
* [Forked插件](https://github.com/LiteAVSDK/TRTC_UnrealEngine.git)
