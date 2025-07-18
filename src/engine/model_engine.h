#ifndef MODEL_ENGINE_H_
#define MODEL_ENGINE_H_

#include <string>

#include "../S21Matrix/s21_matrix_oop.h"
#include "model_data.h"

/// @brief Константа для перевода из градусов в радианы
constexpr float DEG_TO_RAD = M_PI / 180.0f;
/// @brief Ширина gif изображения
constexpr int gif_width = 640;
/// @brief Высота gif изображения
constexpr int gif_height = 480;
/// @brief Количество кадров в секунду для gif изображения
constexpr int gif_fps = 10;
/// @brief Длительность записи в секундах для gif изображения
constexpr int gif_duration = 5;

namespace s21 {
/// @brief Формат изображения
// enum class ImageFormat { BMP, JPEG };

/// @brief Основной класс движка
class ModelEngine {
 public:
  /// @brief Конструктор по умолчанию
  ModelEngine();
  /// @brief Деструктор по умолчанию
  ~ModelEngine() = default;

  /// @brief Загрузка модели из .obj файла
  /// @param filepath Путь к файлу
  void LoadModelFromFile(const std::string &filepath);

  /// @brief Очистка текущей модели
  void ClearModel();

  /// @brief Проверяет, загружена ли модель
  /// @return true, если модель загружена, false в противном случае
  bool HasModel() const;

  /// @brief Получить данные модели (вершины, рёбра)
  /// @return Ссылка на данные модели
  const ModelData &GetModelData() const;

  /// @brief Получает количество вершин
  /// @return Количество вершин в модели
  std::size_t GetVertexCount() const;
  /// @brief Получает количество рёбер
  /// @return Количество рёбер в модели
  std::size_t GetEdgeCount() const;
  /// @brief Получает имя файла
  /// @return Имя файла модели
  const std::string GetFilename() const;

  /// @brief Перемещение модели по осям X, Y, Z
  /// @param dx Смещение по x
  /// @param dy Смещение по y
  /// @param dz Смещение по z
  void Translate(double dx, double dy, double dz);
  /// @brief Поворот модели вокруг осей X, Y, Z
  /// @param angle_x_deg Угол поворота вокруг оси x в градусах
  /// @param angle_y_deg Угол поворота вокруг оси y в градусах
  /// @param angle_z_deg Угол поворота вокруг оси z в градусах
  void Rotate(double angle_x_deg, double angle_y_deg, double angle_z_deg);
  /// @brief Масштабирование модели
  /// @param factor Коэффициент масштабирования
  void Scale(double factor);

  /// @brief Сброс всех трансформаций в исходное состояние
  void ResetTransformations();

  /// @brief Сохранение изображения в файл
  /// @param filepath Путь к файлу
  /// @param format Формат изображения
  // void SaveImage(const std::string& filepath, ImageFormat format);

  /// @brief Создание скринкастов и их сохранение в файл
  /// @param filepath Путь к файлу
  // void RecordGif(const std::string& filepath);

 private:
  /// @brief Имя файла модели
  std::string filename_;
  /// @brief Данные исходной модели (вершины и грани в исходной системе
  /// координат)
  ModelData model_;
  /// @brief Трансформированные данные модели (вершины и грани после
  /// трансформаций)
  ModelData transformed_;

  /// @brief Приводим модель к нормальному виду (расположение по центру,
  /// соответствующий масштаб)
  void FitModel();

  /// @brief Обновление трасформаций модели, вызывается после любой
  /// трансформации
  void UpdateModelMatrix();

  /// @brief Создание матрицы перемещения модели
  /// @param tx перемещение по оси x
  /// @param ty перемещение по оси y
  /// @param tz перемещение по оси z
  /// @return матрица перемещения 4x4
  S21Matrix TranslateMatrix(float tx, float ty, float tz);

  /// @brief Создание матрицы масштабирования модели
  /// @param sx масштабирование по x
  /// @param sy масштабирование по y
  /// @param sz масштабирование по z
  /// @return матрица масштабирования 4x4
  S21Matrix ScaleMatrix(float sx, float sy, float sz);

  /// @brief Создание матрицы вращения модели
  /// @param rx вращение по оси x
  /// @param ry вращение по оси y
  /// @param rz вращение по оси z
  /// @return матрица вращения 4x4
  S21Matrix RotateMatrix(float rx, float ry, float rz);

  /// @brief Создание матрицы вращения по оси x
  /// @param angleRad вращение по оси x в радианах
  /// @return матрица вращения 4x4
  S21Matrix RotateXMatrix(float angleRad);

  /// @brief Создание матрицы вращения по оси y
  /// @param angleRad вращение по оси y в радианах
  /// @return матрица вращения 4x4
  S21Matrix RotateYMatrix(float angleRad);

  /// @brief Создание матрицы вращения по оси z
  /// @param angleRad вращение по оси z в радианах
  /// @return матрица вращения 4x4
  S21Matrix RotateZMatrix(float angleRad);

  /// @brief Перевод из градусов в радианы
  /// @param degrees значение в градусах
  /// @return значение в радианах
  float ToRadians(float degrees);

  /// @brief матрица перемещения модели
  S21Matrix translationMat;
  /// @brief матрица масштабирования модели
  S21Matrix scaleMat;
  /// @brief матрица вращения модели относительно мировых координат
  S21Matrix worldRotation;

  /// @brief Перемещение модели по осям на данный момент
  float current_translation[3];

  /// @brief Масштабирование по осям на данный момент
  float current_scale;
};

}  // namespace s21

#endif  // MODEL_ENGINE_H_
