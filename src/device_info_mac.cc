#include "../include/device_info_mac.h"
#include "common_video/libyuv/include/webrtc_libyuv.h"
#include "modules/video_capture/video_capture_defines.h"

#include <cstring>

namespace media {
DeviceInfoMac::DeviceInfoMac() : webrtc::videocapturemodule::DeviceInfoImpl() {}

DeviceInfoMac::~DeviceInfoMac() = default;

uint32_t DeviceInfoMac::NumberOfDevices() {
    return QMediaDevices::videoInputs().length();
}

int32_t DeviceInfoMac::GetDeviceName(
    uint32_t device_number, char* device_name_uft8, uint32_t device_name_length,
    char* device_unique_id_utf8, uint32_t device_unique_id_utf8_length,
    char* product_unique_id_utf8, uint32_t product_unique_id_utf8_length) {
    if (device_number >= NumberOfDevices()) {
        return -1;
    }
    const QCameraDevice& device =
        QMediaDevices::videoInputs().at(device_number);
    if (device_name_length <= device.description().length()) {
        return -1;
    }
    if (device_unique_id_utf8_length <= device.id().length()) {
        return -1;
    }
    memset(device_name_uft8, 0, device_name_length);
    memcpy(device_name_uft8, device.description().toUtf8().constData(),
           device_name_length);
    memset(device_unique_id_utf8, 0, device_unique_id_utf8_length);
    memcpy(device_unique_id_utf8, device.id().constData(),
           device_unique_id_utf8_length);
    return 0;
}

int32_t DeviceInfoMac::CreateCapabilityMap(const char* deviceUniqueIdUTF8) {
    for (const auto& camera_device : QMediaDevices::videoInputs()) {
        if (strcmp(camera_device.id().constData(), deviceUniqueIdUTF8) != 0) {
            continue;
        }
        std::vector<webrtc::VideoCaptureCapability> video_capture_capabilities;
        for (const auto& camera_format : camera_device.videoFormats()) {
            webrtc::VideoCaptureCapability video_capture_capability;
            video_capture_capability.width = camera_format.resolution().width();
            video_capture_capability.height =
                camera_format.resolution().height();
            video_capture_capability.maxFPS = camera_format.maxFrameRate();
            video_capture_capability.videoType =
                GetVideoType(camera_format.pixelFormat());
            video_capture_capabilities.push_back(video_capture_capability);
        }
        _captureCapabilities = video_capture_capabilities;
        return 0;
    }
    return -1;
}

int32_t DeviceInfoMac::DisplayCaptureSettingsDialogBox(
    const char* device_unique_id_utf8, const char* dialog_title_utf8,
    void* parent_window, uint32_t position_x, uint32_t position_y) {
    return -1;
}

int32_t DeviceInfoMac::Init() { return 0; }

const webrtc::VideoType DeviceInfoMac::GetVideoType(
    const QVideoFrameFormat::PixelFormat& pixel_format) {
    switch (pixel_format) {
        case QVideoFrameFormat::Format_NV12:
            return webrtc::VideoType::kNV12;
        case QVideoFrameFormat::Format_YUV420P:
            return webrtc::VideoType::kI420;
        case QVideoFrameFormat::Format_UYVY:
            return webrtc::VideoType::kUYVY;
        // TODO add more formats
        default:
            return webrtc::VideoType::kUnknown;
    }
}
}  // namespace media
