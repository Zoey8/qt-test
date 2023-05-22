#ifndef MEET_WINDOW_H
#define MEET_WINDOW_H

#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_defines.h"
#include "../include/capture_observer.h"

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

    void RefreshVideoDeviceList();
    void StartCaptureVideo();
    void StopCaptureVideo();
};
#endif  // MEET_WINDOW_H
