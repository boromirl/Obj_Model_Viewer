#include "controller.h"

namespace s21 {

Controller::Controller(IView* view) : view_(view) {}

bool Controller::LoadModel(const std::string& filepath) {
  engine_.LoadModelFromFile(filepath);
  bool res = true;
  if (!engine_.HasModel()) {
    view_->OnError("Не удалось загрузить модель: " + filepath);
    res = false;
  } else {
    view_->OnModelLoaded(engine_.GetModelData(), engine_.GetVertexCount(),
                         engine_.GetEdgeCount(), engine_.GetFilename());
  }
  return res;
}

void Controller::ClearModel() {
  engine_.ClearModel();
  view_->OnModelTransformed(engine_.GetModelData().tm);
}

bool Controller::Translate(double dx, double dy, double dz) {
  bool res = true;
  if (!EnsureModelLoaded()) {
    res = false;
  } else {
    engine_.Translate(dx, dy, dz);
    view_->OnModelTransformed(engine_.GetModelData().tm);
  }
  return res;
}

bool Controller::Rotate(double ax_deg, double ay_deg, double az_deg) {
  bool res = true;
  if (!EnsureModelLoaded()) {
    res = false;
  } else {
    engine_.Rotate(ax_deg, ay_deg, az_deg);
    view_->OnModelTransformed(engine_.GetModelData().tm);
  }
  return res;
}

bool Controller::Scale(double factor) {
  bool res = true;
  if (!EnsureModelLoaded()) {
    res = false;
  } else {
    engine_.Scale(factor);
    view_->OnModelTransformed(engine_.GetModelData().tm);
  }
  return res;
}

bool Controller::ResetTransformations() {
  bool res = true;
  if (!EnsureModelLoaded()) {
    res = false;
  } else {
    engine_.ResetTransformations();
    view_->OnModelTransformed(engine_.GetModelData().tm);
  }
  return res;
}

// bool Controller::SaveImage(const std::string& filepath, ImageFormat format) {
//   bool res = true;
//   if (!EnsureModelLoaded()) {
//     res = false;
//   } else {
//     engine_.SaveImage(filepath, format);
//     view_->OnImageSaved(filepath);
//   }
//   return res;
// }

// bool Controller::RecordGif(const std::string& filepath) {
//   bool res = true;
//   if (!EnsureModelLoaded()) {
//     res = false;
//   } else {
//     engine_.RecordGif(filepath);
//     view_->OnGifRecorded(filepath);
//   }
//   return res;
// }

bool Controller::EnsureModelLoaded() {
  bool res = true;
  if (!engine_.HasModel()) {
    view_->OnError("Модель не загружена");
    res = false;
  }
  return res;
}

}  // namespace s21
