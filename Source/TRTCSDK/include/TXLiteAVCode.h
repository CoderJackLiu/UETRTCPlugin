#ifndef __TXLITEAVCODE_H__
#define __TXLITEAVCODE_H__

/**
 * Error Codes
 */
enum TXLiteAVError {

    /////////////////////////////////////////////////////////////////////////////////
    //       Basic error codes
    /////////////////////////////////////////////////////////////////////////////////

    /// No error.
    ERR_NULL = 0,

    /// Unclassified error.
    ERR_FAILED = -1,

    /// An invalid parameter was passed in when the API was called.
    ERR_INVALID_PARAMETER = -2,

    /// The API call was rejected.
    ERR_REFUSED = -3,

    /// The current API cannot be called.
    ERR_NOT_SUPPORTED = -4,

    /// Failed to call the API because the license is invalid.
    ERR_INVALID_LICENSE = -5,

    /// The request timed out.
    ERR_REQUEST_SERVER_TIMEOUT = -6,

    /// The server cannot process your request.
    ERR_SERVER_PROCESS_FAILED = -7,

    /// Disconnected from the server.
    ERR_DISCONNECTED = -8,

    /////////////////////////////////////////////////////////////////////////////////
    //       Video error codes
    /////////////////////////////////////////////////////////////////////////////////

    /// Failed to turn the camera on. This may occur when there is a problem with the camera configuration program (driver) on Windows or macOS. Disable and reenable the camera, restart the camera, or update the configuration program.
    ERR_CAMERA_START_FAIL = -1301,

    /// No permission to access to the camera. This usually occurs on mobile devices and may be because the user denied access.
    ERR_CAMERA_NOT_AUTHORIZED = -1314,

    /// Incorrect camera parameter settings (unsupported values or others).
    ERR_CAMERA_SET_PARAM_FAIL = -1315,

    /// The camera is being used. Try another camera.
    ERR_CAMERA_OCCUPY = -1316,

    /// Failed to start screen recording. If this occurs on a mobile device, it may be because the user denied screen sharing permission; if it occurs on Windows or macOS, check whether the parameters of the screen recording API are set as required.
    ERR_SCREEN_CAPTURE_START_FAIL = -1308,

    /// Screen recording failed. Screen recording is only supported on Android versions later than 5.0 and iOS versions later than 11.0.
    ERR_SCREEN_CAPTURE_UNSURPORT = -1309,

    /// Screen recording was stopped by the system.
    ERR_SCREEN_CAPTURE_STOPPED = -7001,

    /// No permission to publish the substream.
    ERR_SCREEN_SHARE_NOT_AUTHORIZED = -102015,

    /// Another user is publishing the substream.
    ERR_SCREEN_SHRAE_OCCUPIED_BY_OTHER = -102016,

    /// Failed to encode video frames. This may occur when a user on iOS switches to another app, which may cause the system to release the hardware encoder. When the user switches back, this error may be thrown before the hardware encoder is
    /// restarted.
    ERR_VIDEO_ENCODE_FAIL = -1303,

    /// Unsupported video resolution.
    ERR_UNSUPPORTED_RESOLUTION = -1305,

    /// Custom video capturing: Unsupported pixel format.
    ERR_PIXEL_FORMAT_UNSUPPORTED = -1327,

    /// Custom video capturing: Unsupported buffer type.
    ERR_BUFFER_TYPE_UNSUPPORTED = -1328,

    /// No available HEVC decoder found.
    ERR_NO_AVAILABLE_HEVC_DECODERS = -2304,

    /////////////////////////////////////////////////////////////////////////////////
    //       Audio error codes
    /////////////////////////////////////////////////////////////////////////////////

    /// Failed to turn the mic on. This may occur when there is a problem with the mic configuration program (driver) on Windows or macOS. Disable and reenable the mic, restart the mic, or update the configuration program.
    ERR_MIC_START_FAIL = -1302,

    /// No permission to access to the mic. This usually occurs on mobile devices and may be because the user denied access.
    ERR_MIC_NOT_AUTHORIZED = -1317,

    /// Failed to set mic parameters.
    ERR_MIC_SET_PARAM_FAIL = -1318,

    /// The mic is being used. The mic cannot be turned on when, for example, the user is having a call on the mobile device.
    ERR_MIC_OCCUPY = -1319,

    /// Failed to turn the mic off.
    ERR_MIC_STOP_FAIL = -1320,

    /// Failed to turn the speaker on. This may occur when there is a problem with the speaker configuration program (driver) on Windows or macOS. Disable and reenable the speaker, restart the speaker, or update the configuration program.
    ERR_SPEAKER_START_FAIL = -1321,

    /// Failed to set speaker parameters.
    ERR_SPEAKER_SET_PARAM_FAIL = -1322,

    /// Failed to turn the speaker off.
    ERR_SPEAKER_STOP_FAIL = -1323,

    /// Failed to record computer audio, which may be because the audio driver is unavailable.
    ERR_AUDIO_PLUGIN_START_FAIL = -1330,

    /// No permission to install the audio driver.
    ERR_AUDIO_PLUGIN_INSTALL_NOT_AUTHORIZED = -1331,

    /// Failed to install the audio driver.
    ERR_AUDIO_PLUGIN_INSTALL_FAILED = -1332,

    /// The virtual sound card is installed successfully, but due to the restrictions of macOS, you cannot use it right after installation. Ask users to restart the app upon receiving this error code.
    ERR_AUDIO_PLUGIN_INSTALLED_BUT_NEED_RESTART = -1333,

    /// Failed to encode audio frames. This may occur if the SDK could not process the custom audio data passed in.
    ERR_AUDIO_ENCODE_FAIL = -1304,

    /// Unsupported audio sample rate.
    ERR_UNSUPPORTED_SAMPLERATE = -1306,

    /////////////////////////////////////////////////////////////////////////////////
    //       Network error codes
    /////////////////////////////////////////////////////////////////////////////////

    /// Failed to enter the room. For the reason, refer to the error message for -3301 in `onError`.
    ERR_TRTC_ENTER_ROOM_FAILED = -3301,

    /// IP and signature request timed out. Check your network connection and whether your firewall allows UDP.
    /// Try visiting the IP address 162.14.22.165:8000 or 162.14.6.105:8000 and the domain default-query.trtc.tencent-cloud.com:8000.
    ERR_TRTC_REQUEST_IP_TIMEOUT = -3307,

    /// Room entry request timed out. Check your network connection and whether VPN is used. You can also switch to 4G to run a test.
    ERR_TRTC_CONNECT_SERVER_TIMEOUT = -3308,

    /// Empty room entry parameters. Please check whether valid parameters were passed in to the `enterRoom:appScene:` API.
    ERR_TRTC_ROOM_PARAM_NULL = -3316,

    /// Incorrect room entry parameter. Check whether `TRTCParams.sdkAppId` is empty.
    ERR_TRTC_INVALID_SDK_APPID = -3317,

    /// Incorrect room entry parameter. Check whether `TRTCParams.roomId` or `TRTCParams.strRoomId` is empty. Note that you cannot set both parameters.
    ERR_TRTC_INVALID_ROOM_ID = -3318,

    /// Incorrect room entry parameter. Check whether `TRTCParams.userId` is empty.
    ERR_TRTC_INVALID_USER_ID = -3319,

    /// Incorrect room entry parameter. Check whether `TRTCParams.userSig` is empty.
    ERR_TRTC_INVALID_USER_SIG = -3320,

    /// Request to enter room denied. Check whether you called `enterRoom` twice to enter the same room.
    ERR_TRTC_ENTER_ROOM_REFUSED = -3340,

    /// Advanced permission control is enabled but failed to verify `TRTCParams.privateMapKey`.
    /// For details, see [Enabling Advanced Permission Control](https://intl.cloud.tencent.com/document/product/647/35157).
    ERR_TRTC_INVALID_PRIVATE_MAPKEY = -100006,

    /// The service is unavailable. Check if you have used up your package or whether your Tencent Cloud account has overdue payments.
    /// For details, see [Package Management](https://cloud.tencent.com/document/product/647/50492).
    ERR_TRTC_SERVICE_SUSPENDED = -100013,

    /// Failed to verify `UserSig`. Check whether `TRTCParams.userSig` is correct or valid.
    /// For details, see [UserSig Generation and Verification](https://intl.cloud.tencent.com/document/product/647/39074).
    ERR_TRTC_USER_SIG_CHECK_FAILED = -100018,

    /// The relay to CDN request timed out
    ERR_TRTC_PUSH_THIRD_PARTY_CLOUD_TIMEOUT = -3321,

    /// The On-Cloud MixTranscoding request timed out.
    ERR_TRTC_MIX_TRANSCODING_TIMEOUT = -3322,

    /// Abnormal response packets for relay.
    ERR_TRTC_PUSH_THIRD_PARTY_CLOUD_FAILED = -3323,

    /// Abnormal response packet for On-Cloud MixTranscoding.
    ERR_TRTC_MIX_TRANSCODING_FAILED = -3324,

    /// Signaling for publishing to the Tencent Cloud CDN timed out.
    ERR_TRTC_START_PUBLISHING_TIMEOUT = -3333,

    /// Signaling for publishing to the Tencent Cloud CDN was abnormal.
    ERR_TRTC_START_PUBLISHING_FAILED = -3334,

    /// Signaling for stopping publishing to the Tencent Cloud CDN timed out.
    ERR_TRTC_STOP_PUBLISHING_TIMEOUT = -3335,

    /// Signaling for stopping publishing to the Tencent Cloud CDN was abnormal.
    ERR_TRTC_STOP_PUBLISHING_FAILED = -3336,

    /// The co-anchoring request timed out.
    ERR_TRTC_CONNECT_OTHER_ROOM_TIMEOUT = -3326,

    /// The request to stop co-anchoring timed out.
    ERR_TRTC_DISCONNECT_OTHER_ROOM_TIMEOUT = -3327,

    /// Invalid parameter.
    ERR_TRTC_CONNECT_OTHER_ROOM_INVALID_PARAMETER = -3328,

    /// The current user is an audience member and cannot request or stop cross-room communication. Please call `switchRole` to switch to an anchor first.
    ERR_TRTC_CONNECT_OTHER_ROOM_AS_AUDIENCE = -3330,
};

/**
 * Warning codes
 */
enum TXLiteAVWarning {

    /////////////////////////////////////////////////////////////////////////////////
    //       Video warning codes
    /////////////////////////////////////////////////////////////////////////////////

    /// Failed to start the hardware encoder. Switched to software encoding.
    WARNING_HW_ENCODER_START_FAIL = 1103,

    /// The codec changed. The additional field `type` in `onWarning` indicates the codec currently in use. `1` indicates H.265, and `0` indicates H.264. This field is not supported on Windows.
    WARNING_CURRENT_ENCODE_TYPE_CHANGED = 1104,

    /// Insufficient CPU for software encoding. Switched to hardware encoding.
    WARNING_VIDEO_ENCODER_SW_TO_HW = 1107,

    /// The capturing frame rate of the camera is insufficient. This error occurs on some Android phones with built-in beauty filters.
    WARNING_INSUFFICIENT_CAPTURE_FPS = 1108,

    /// Failed to start the software encoder.
    WARNING_SW_ENCODER_START_FAIL = 1109,

    /// The capturing frame rate of the camera was reduced for balance between frame rate and performance.
    WARNING_REDUCE_CAPTURE_RESOLUTION = 1110,

    /// No available camera found.
    WARNING_CAMERA_DEVICE_EMPTY = 1111,

    /// The user didn’t grant the application camera permission.
    WARNING_CAMERA_NOT_AUTHORIZED = 1112,

    /// The user didn’t grant the application screen recording permission.
    WARNING_SCREEN_CAPTURE_NOT_AUTHORIZED = 1206,

    /// Failed to decode the current video frame.
    WARNING_VIDEO_FRAME_DECODE_FAIL = 2101,

    /// Failed to start the hardware decoder. The software decoder is used instead.
    WARNING_HW_DECODER_START_FAIL = 2106,

    /// The hardware decoder failed to decode the first I-frame of the current stream. The SDK automatically switched to the software decoder.
    WARNING_VIDEO_DECODER_HW_TO_SW = 2108,

    /// Failed to start the software decoder.
    WARNING_SW_DECODER_START_FAIL = 2109,

    /// Failed to render the video.
    WARNING_VIDEO_RENDER_FAIL = 2110,

    /////////////////////////////////////////////////////////////////////////////////
    //       Audio warning codes
    /////////////////////////////////////////////////////////////////////////////////

    /// No available mic found.
    WARNING_MICROPHONE_DEVICE_EMPTY = 1201,

    /// No available speaker found.
    WARNING_SPEAKER_DEVICE_EMPTY = 1202,

    /// The user didn’t grant the application mic permission.
    WARNING_MICROPHONE_NOT_AUTHORIZED = 1203,

    /// The audio capturing device is unavailable (which may be because the device is used by another application or is considered invalid by the system).
    WARNING_MICROPHONE_DEVICE_ABNORMAL = 1204,

    /// The audio playback device is unavailable (which may be because the device is used by another application or is considered invalid by the system).
    WARNING_SPEAKER_DEVICE_ABNORMAL = 1205,

    /// Failed to decode the current audio frame.
    WARNING_AUDIO_FRAME_DECODE_FAIL = 2102,

    /// Failed to write recorded audio into the file.
    WARNING_AUDIO_RECORDING_WRITE_FAIL = 7001,

    /////////////////////////////////////////////////////////////////////////////////
    //       Network warning codes
    /////////////////////////////////////////////////////////////////////////////////i

    /// The current user is an audience member and cannot publish audio or video. Please switch to an anchor first.
    WARNING_IGNORE_UPSTREAM_FOR_AUDIENCE = 6001,

};

#define ERR_ROOM_ENTER_FAIL ERR_TRTC_ENTER_ROOM_FAILED
#define ERR_ROOM_REQUEST_IP_TIMEOUT ERR_TRTC_REQUEST_IP_TIMEOUT
#define ERR_ROOM_REQUEST_ENTER_ROOM_TIMEOUT ERR_TRTC_CONNECT_SERVER_TIMEOUT

#define ERR_ENTER_ROOM_PARAM_NULL ERR_TRTC_ROOM_PARAM_NULL
#define ERR_SDK_APPID_INVALID ERR_TRTC_INVALID_SDK_APPID
#define ERR_ROOM_ID_INVALID ERR_TRTC_INVALID_ROOM_ID
#define ERR_USER_ID_INVALID ERR_TRTC_INVALID_USER_ID
#define ERR_USER_SIG_INVALID ERR_TRTC_INVALID_USER_SIG
#define ERR_ROOM_REQUEST_ENTER_ROOM_REFUSED ERR_TRTC_ENTER_ROOM_REFUSED
#define ERR_SERVER_INFO_PRIVILEGE_FLAG_ERROR ERR_TRTC_INVALID_PRIVATE_MAPKEY
#define ERR_SERVER_INFO_SERVICE_SUSPENDED ERR_TRTC_SERVICE_SUSPENDED
#define ERR_SERVER_INFO_ECDH_GET_TINYID ERR_TRTC_USER_SIG_CHECK_FAILED
#define ERR_SERVER_CENTER_NO_PRIVILEDGE_PUSH_SUB_VIDEO ERR_SCREEN_SHARE_NOT_AUTHORIZED
#define ERR_SERVER_CENTER_ANOTHER_USER_PUSH_SUB_VIDEO ERR_SCREEN_SHRAE_OCCUPIED_BY_OTHER
#define ERR_PUBLISH_CDN_STREAM_REQUEST_TIME_OUT ERR_TRTC_PUSH_THIRD_PARTY_CLOUD_TIMEOUT
#define ERR_PUBLISH_CDN_STREAM_SERVER_FAILED ERR_TRTC_PUSH_THIRD_PARTY_CLOUD_FAILED
#define ERR_CLOUD_MIX_TRANSCODING_REQUEST_TIME_OUT ERR_TRTC_MIX_TRANSCODING_TIMEOUT
#define ERR_CLOUD_MIX_TRANSCODING_SERVER_FAILED ERR_TRTC_MIX_TRANSCODING_FAILED

#define ERR_ROOM_REQUEST_START_PUBLISHING_TIMEOUT ERR_TRTC_START_PUBLISHING_TIMEOUT
#define ERR_ROOM_REQUEST_START_PUBLISHING_ERROR ERR_TRTC_START_PUBLISHING_FAILED
#define ERR_ROOM_REQUEST_STOP_PUBLISHING_TIMEOUT ERR_TRTC_STOP_PUBLISHING_TIMEOUT
#define ERR_ROOM_REQUEST_STOP_PUBLISHING_ERROR ERR_TRTC_STOP_PUBLISHING_FAILED

#define ERR_ROOM_REQUEST_CONN_ROOM_TIMEOUT ERR_TRTC_CONNECT_OTHER_ROOM_TIMEOUT
#define ERR_ROOM_REQUEST_DISCONN_ROOM_TIMEOUT ERR_TRTC_DISCONNECT_OTHER_ROOM_TIMEOUT
#define ERR_ROOM_REQUEST_CONN_ROOM_INVALID_PARAM ERR_TRTC_CONNECT_OTHER_ROOM_INVALID_PARAMETER
#define ERR_CONNECT_OTHER_ROOM_AS_AUDIENCE ERR_TRTC_CONNECT_OTHER_ROOM_AS_AUDIENCE

typedef enum TXLiteAVError TXLiteAVError;
typedef enum TXLiteAVWarning TXLiteAVWarning;

#endif /* __TXLITEAVCODE_H__ */
