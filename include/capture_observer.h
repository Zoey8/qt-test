#ifndef _CAPTURE_OBSERVER_H
#define _CAPTURE_OBSERVER_H

#include <modules/video_capture/video_capture.h>
#include <qimage.h>
#include <qobject.h>
#include <qtmetamacros.h>

namespace media {
class CaptureObserver : public QObject,
                        public rtc::VideoSinkInterface<webrtc::VideoFrame> {
    Q_OBJECT

  public:
    void OnFrame(const webrtc::VideoFrame &videoFrame);
  Q_SIGNALS:
    void RefreshFrameSignal(const QImage &frame);
};
}  // namespace ui

#endif