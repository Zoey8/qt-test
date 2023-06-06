#ifndef __DEVICE_INFO_MAC_H__
#define __DEVICE_INFO_MAC_H__

#include "modules/video_capture/device_info_impl.h"

#include <qcameradevice.h>
#include <qmediadevices.h>

namespace media {
class DeviceInfoMac : public webrtc::videocapturemodule::DeviceInfoImpl {
  public:
    DeviceInfoMac();
    ~DeviceInfoMac() override;
    uint32_t NumberOfDevices() override;
    int32_t GetDeviceName(uint32_t device_number, char* device_name_uft8,
                          uint32_t device_name_length,
                          char* device_unique_id_utf8,
                          uint32_t device_unique_id_utf8_length,
                          char* product_unique_id_utf8 = nullptr,
                          uint32_t product_unique_id_utf8_length = 0) override;
    int32_t CreateCapabilityMap(const char* deviceUniqueIdUTF8) override
        RTC_EXCLUSIVE_LOCKS_REQUIRED(_apiLock);
    int32_t DisplayCaptureSettingsDialogBox(const char* device_unique_id_utf8,
                                            const char* dialog_title_utf8,
                                            void* parent_window,
                                            uint32_t position_x,
                                            uint32_t position_y) override;
    int32_t Init() override;
    static const webrtc::VideoType GetVideoType(
        const QVideoFrameFormat::PixelFormat& pixel_format);
};
}  // namespace media

#endif  // __DEVICE_INFO_MAC_H__