#ifndef __MEET_WINDOW_H__
#define __MEET_WINDOW_H__

#include "../include/capture_observer.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_defines.h"

#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MeetWindow;
}
QT_END_NAMESPACE

class MeetWindow : public QWidget {
    Q_OBJECT

  public:
    explicit MeetWindow(QWidget *parent = nullptr);
    ~MeetWindow();

  private:
    Ui::MeetWindow *ui;

    rtc::scoped_refptr<webrtc::VideoCaptureModule> video_capture_module_;
    std::unique_ptr<webrtc::VideoCaptureCapability> video_capture_capability_;
    std::unique_ptr<media::CaptureObserver> video_capture_observer_;

    rtc::scoped_refptr<webrtc::AudioDeviceModule> audio_device_module_; 

    void RefreshCameraDeviceList();
    void StartCaptureVideo();
    void StopCaptureVideo();

    void RefreshAudioInputDeviceList();
    void RefreshAudioOutputDeviceList();
    void StartCaptureAudio();
    void StopCaptureAudio();
};

#endif // __MEET_WINDOW_H__