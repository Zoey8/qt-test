#include "../include/capture_observer.h"
#include "libyuv/convert_argb.h"

#include <qdebug.h>
#include <qtmetamacros.h>

void media::CaptureObserver::OnFrame(const webrtc::VideoFrame& videoFrame) {
    rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
        videoFrame.video_frame_buffer()->ToI420());
    std::unique_ptr<unsigned char[]> imageBuffer(
        new unsigned char[buffer->width() * buffer->height() * 3]);
    libyuv::I420ToRGB24(buffer->DataY(), buffer->StrideY(), buffer->DataU(),
                        buffer->StrideU(), buffer->DataV(), buffer->StrideV(),
                        imageBuffer.get(), buffer->width() * 3, buffer->width(),
                        buffer->height());
    QImage image(imageBuffer.get(), buffer->width(), buffer->height(),
                 QImage::Format_BGR888);
    Q_EMIT RefreshFrameSignal(image.copy());
}
