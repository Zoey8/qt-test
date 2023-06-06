#include "../include/meet_window.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "ui_meet_window.h"

#include <qcameradevice.h>
#include <qmediadevices.h>
#include <qvariant.h>

MeetWindow::MeetWindow(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::MeetWindow),
      video_capture_observer_(std::unique_ptr<media::CaptureObserver>(
          new media::CaptureObserver())),
      video_capture_capability_(std::unique_ptr<webrtc::VideoCaptureCapability>(
          new webrtc::VideoCaptureCapability())),
      audio_device_module_(webrtc::AudioDeviceModule::Create(
          webrtc::AudioDeviceModule::kPlatformDefaultAudio,
          webrtc::CreateDefaultTaskQueueFactory().get())) {
    ui->setupUi(this);
    RefreshCameraDeviceList();
    audio_device_module_->Init();
    RefreshAudioInputDeviceList();
    RefreshAudioOutputDeviceList();
}

MeetWindow::~MeetWindow() { delete ui; }

void MeetWindow::RefreshCameraDeviceList() {
    this->ui->CameraDeviceSelector->clear();
    auto cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        qDebug() << "No camera device found";
        return;
    }
    for (size_t i = 0; i < cameras.length(); i++) {
        auto camera = cameras.at(i);
        this->ui->CameraDeviceSelector->addItem(camera.description(),
                                                QVariant(camera.id()));
        qDebug() << "Find camera device <name: " << camera.description() << "\t"
                 << "id: " << camera.id() << ">";
        for (auto& format : camera.videoFormats()) {
            qDebug() << "Format: " << format.maxFrameRate()
                     << format.pixelFormat() << format.resolution();
        }
    }
    this->ui->CameraDeviceSelector->setCurrentIndex(-1);
}

void MeetWindow::StartCaptureVideo() {}

void MeetWindow::StopCaptureVideo() {}

void MeetWindow::RefreshAudioInputDeviceList() {
    if (audio_device_module_->RecordingDevices() == 0) {
        qDebug() << "No audio input device found";
        return;
    }
    for (int id = 1; id < audio_device_module_->RecordingDevices(); id++) {
        char name[webrtc::kAdmMaxDeviceNameSize];
        if (audio_device_module_->RecordingDeviceName(id, name, nullptr) == 0) {
            this->ui->audioInputDeviceSelector->addItem(name, QVariant(id));
            qDebug() << "Find audio input device <name: " << name << "  "
                     << "id: " << id << ">";
        }
    }
    this->ui->audioInputDeviceSelector->setCurrentIndex(-1);
}

void MeetWindow::RefreshAudioOutputDeviceList() {
    if (audio_device_module_->PlayoutDevices() == 0) {
        qDebug() << "No audio output device found";
        return;
    }
    for (int id = 1; id < audio_device_module_->PlayoutDevices(); id++) {
        char name[webrtc::kAdmMaxDeviceNameSize];
        if (audio_device_module_->PlayoutDeviceName(id, name, nullptr) == 0) {
            this->ui->audioOutputDeviceSelector->addItem(name, QVariant(id));
            qDebug() << "Find audio output device <name: " << name << "  "
                     << "id: " << id << ">";
        }
    }
    this->ui->audioOutputDeviceSelector->setCurrentIndex(-1);
}

void MeetWindow::StartCaptureAudio() {
    auto audio_input_device_id =
        this->ui->audioOutputDeviceSelector->currentData().toInt();
    auto audio_input_device_name =
        this->ui->audioOutputDeviceSelector->currentText();
    if (audio_device_module_->SetRecordingDevice(audio_input_device_id) != 0) {
        qDebug() << "Set audio device failed";
    }
    qDebug() << "audio open on device: " << audio_input_device_name;
    if (audio_device_module_->InitRecording() != 0) {
        qDebug() << "Init audio device failed";
    }
    if (audio_device_module_->StartRecording() != 0) {
        qDebug() << "Start audio device failed";
    }
    if (audio_device_module_->Recording()) {
        qDebug() << "Audio device start success on device: "
                 << audio_input_device_name;
    }
}

void MeetWindow::StopCaptureAudio() {
    audio_device_module_->StopRecording();
    if (!audio_device_module_->Recording()) {
        qDebug() << "Audio device stop success";
    }
}
