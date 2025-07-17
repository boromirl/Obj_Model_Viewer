#include "model_engine.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../utils/obj_parser.h"

namespace s21 {

ModelEngine::ModelEngine() {
  translationMat = S21Matrix(4, 4);
  scaleMat = S21Matrix(4, 4);
  worldRotation = S21Matrix(4, 4);

  translationMat.SetIdentity(1.0f);
  scaleMat.SetIdentity(1.0f);
  worldRotation.SetIdentity(1.0f);

  current_scale = 1.0f;
  current_translation[0] = 0.0f;
  current_translation[1] = 0.0f;
  current_translation[2] = 0.0f;
}

void ModelEngine::LoadModelFromFile(const std::string &filepath) {
  // Сброс старой модели
  ClearModel();
  // Сброс данных о трансформации
  ResetTransformations();

  if (ObjParser::Parse(filepath, &model_)) {
    // при успешном парсинге сохраняем имя и копируем во transformed_
    std::filesystem::path p(filepath);
    filename_ = p.filename().string();
    FitModel();  // приводим модель к нужному виду (нормализованный размер,
                 // расположение по центру)

    transformed_ = model_;
  }
}

void ModelEngine::ClearModel() {
  model_.vertices.clear();
  model_.edges.clear();

  transformed_.tm = S21Matrix(4, 4);
  transformed_.tm.SetIdentity(1.0f);
  transformed_.vertices.clear();
  transformed_.edges.clear();
  filename_.clear();
}

bool ModelEngine::HasModel() const { return !model_.vertices.empty(); }

const ModelData &ModelEngine::GetModelData() const { return transformed_; }

std::size_t ModelEngine::GetVertexCount() const {
  return model_.vertices.size();
}

std::size_t ModelEngine::GetEdgeCount() const { return model_.edges.size(); }

const std::string ModelEngine::GetFilename() const { return filename_; }

void ModelEngine::FitModel() {
  if (model_.vertices.empty()) {
    return;
  }

  const Vertex &first = model_.vertices.front();
  float minX = first.x, maxX = first.x;
  float minY = first.y, maxY = first.y;
  float minZ = first.z, maxZ = first.z;

  for (const Vertex &vertex : model_.vertices) {
    minX = std::min(minX, vertex.x);
    minY = std::min(minY, vertex.y);
    minZ = std::min(minZ, vertex.z);
    maxX = std::max(maxX, vertex.x);
    maxY = std::max(maxY, vertex.y);
    maxZ = std::max(maxZ, vertex.z);
  }

  float centerX = (minX + maxX) * 0.5f;
  float centerY = (minY + maxY) * 0.5f;
  float centerZ = (minZ + maxZ) * 0.5f;

  float sizeX = maxX - minX;
  float sizeY = maxY - minY;
  float sizeZ = maxZ - minZ;

  float maxExtent = std::max({sizeX, sizeY, sizeZ});

  model_.tm = S21Matrix(4, 4);
  model_.tm.SetIdentity(1.0f);
  // вычисляем масштаб, при котором модель влезает в экран
  float scaleFactor = 2.0 / maxExtent;
  model_.tm = TranslateMatrix(-centerX, -centerY, -centerZ) *
              ScaleMatrix(scaleFactor, scaleFactor, scaleFactor) * model_.tm;
}

void ModelEngine::Translate(double dx, double dy, double dz) {
  // вычисляем текущее перемещение
  current_translation[0] += dx / 10.0f;
  current_translation[1] += dy / 10.0f;
  current_translation[2] += dz / 10.0f;
  translationMat.SetIdentity(1.0f);
  translationMat = TranslateMatrix(
      current_translation[0], current_translation[1], current_translation[2]);
  UpdateModelMatrix();
}

void ModelEngine::Rotate(double angle_x_deg, double angle_y_deg,
                         double angle_z_deg) {
  // Create new rotations (world axes)
  S21Matrix newRot = RotateMatrix(angle_x_deg, angle_y_deg, angle_z_deg);
  // Apply new rotation BEFORE existing rotation (world-space)
  worldRotation = worldRotation * newRot;

  UpdateModelMatrix();
}

void ModelEngine::Scale(double factor) {
  current_scale *= factor;
  scaleMat = ScaleMatrix(current_scale, current_scale, current_scale);
  UpdateModelMatrix();
}

void ModelEngine::UpdateModelMatrix() {
  transformed_.tm = model_.tm * scaleMat * worldRotation * translationMat;
}

void ModelEngine::ResetTransformations() {
  transformed_ = model_;
  current_translation[0] = 0.0f;
  current_translation[1] = 0.0f;
  current_translation[2] = 0.0f;

  current_scale = 1.0f;

  worldRotation = S21Matrix(4, 4);
  worldRotation.SetIdentity(1.0f);

  translationMat = S21Matrix(4, 4);
  translationMat.SetIdentity(1.0f);

  scaleMat = S21Matrix(4, 4);
  scaleMat.SetIdentity(1.0f);
}

S21Matrix ModelEngine::TranslateMatrix(float tx, float ty, float tz) {
  S21Matrix tm(4, 4);
  tm.SetIdentity(1.0f);

  tm(3, 0) = tx;
  tm(3, 1) = ty;
  tm(3, 2) = tz;

  return tm;
}

S21Matrix ModelEngine::ScaleMatrix(float sx, float sy, float sz) {
  S21Matrix sm(4, 4);
  sm.SetIdentity(1.0f);

  sm(0, 0) = sx;
  sm(1, 1) = sy;
  sm(2, 2) = sz;

  return sm;
}

float ModelEngine::ToRadians(float degrees) { return degrees * DEG_TO_RAD; }

S21Matrix ModelEngine::RotateMatrix(float rx, float ry, float rz) {
  S21Matrix rm = RotateZMatrix(ToRadians(rz)) * RotateYMatrix(ToRadians(ry)) *
                 RotateXMatrix(ToRadians(rx));
  return rm;
}

S21Matrix ModelEngine::RotateXMatrix(float angleRad) {
  S21Matrix rx(4, 4);
  rx.SetIdentity(1.0f);

  float cosA = cos(angleRad);
  float sinA = sin(angleRad);

  rx(1, 1) = cosA;
  rx(2, 1) = -sinA;
  rx(1, 2) = sinA;
  rx(2, 2) = cosA;

  return rx;
}

S21Matrix ModelEngine::RotateYMatrix(float angleRad) {
  S21Matrix ry(4, 4);
  ry.SetIdentity(1.0f);

  float cosA = cos(angleRad);
  float sinA = sin(angleRad);

  ry(0, 0) = cosA;
  ry(2, 0) = sinA;
  ry(0, 2) = -sinA;
  ry(2, 2) = cosA;

  return ry;
}

S21Matrix ModelEngine::RotateZMatrix(float angleRad) {
  S21Matrix rz(4, 4);
  rz.SetIdentity(1.0f);

  float cosA = cos(angleRad);
  float sinA = sin(angleRad);

  rz(0, 0) = cosA;
  rz(1, 0) = -sinA;
  rz(0, 1) = sinA;
  rz(1, 1) = cosA;

  return rz;
}

// void ModelEngine::SetProjectionType(ProjectionType type) {
//   projection_type_ = type;
//   // TODO: вызвать ApplyTransformation с матрицей проекции или ещё как
// }

// ProjectionType ModelEngine::GetProjectionType() const {
//   return projection_type_;
// }

// void ModelEngine::SetEdgeStyle(const EdgeStyle& style) { edge_style_ =
// style;
// }

// const EdgeStyle ModelEngine::GetEdgeStyle() const { return edge_style_; }

// void ModelEngine::SetVertexStyle(const VertexStyle& style) {
//   vertex_style_ = style;
// }

// const VertexStyle ModelEngine::GetVertexStyle() const { return
// vertex_style_;
// }

// void ModelEngine::SetBackgroundColor(const Color& color) {
//   background_color_ = color;
// }

// const Color ModelEngine::GetBackgroundColor() const {
//   return background_color_;
// }

// void ModelEngine::SaveImage(const std::string& /*filepath*/,
//                             ImageFormat /*format*/) {
//   // Реализация сохранения через выбранную графическую библиотеку
// }

// void ModelEngine::RecordGif(const std::string& /*filepath*/) {
//   // Реализация записи GIF:
//   //   - каждый 1/gif_fps сек захватывать кадр
//   //   - собрать gif_duration*gif_fps кадров
//   //   - кодировать в GIF-файл
// }
}  // namespace s21
