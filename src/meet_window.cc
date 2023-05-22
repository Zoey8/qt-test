#include "../include/meet_window.h"
#include "modules/video_capture/video_capture_factory.h"
#include "ui_meet_window.h"

#include <qvariant.h>

MeetWindow::MeetWindow(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MeetWindow),
      video_capture_observer_(std::unique_ptr<media::CaptureObserver>(
          new media::CaptureObserver())),
      video_capture_capability_(std::unique_ptr<webrtc::VideoCaptureCapability>(
          new webrtc::VideoCaptureCapability())) {
    ui->setupUi(this);
    RefreshVideoDeviceList();
}

MeetWindow::~MeetWindow() { delete ui; }

void MeetWindow::RefreshVideoDeviceList() {
    this->ui->videoDeviceSelector->clear();
    auto info = webrtc::VideoCaptureFactory::CreateDeviceInfo();
    if (info == nullptr || info->NumberOfDevices() == 0) {
        qDebug() << "No video device found";
        return;
    }
    for (size_t i = 0; i < info->NumberOfDevices(); i++) {
        const uint32_t kSize = 256;
        char name[kSize] = {0};
        char id[kSize] = {0};
        info->GetDeviceName(i, name, kSize, id, kSize);
        this->ui->videoDeviceSelector->addItem(name, QVariant(QString(id)));
        qDebug() << "Find video device <name: " << name << "\t"
                 << "id: " << id << ">";
    }
    this->ui->videoDeviceSelector->setCurrentIndex(-1);
}
