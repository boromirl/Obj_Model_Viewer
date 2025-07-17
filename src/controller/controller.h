#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <string>

#include "../engine/model_engine.h"

namespace s21 {

/// @brief Интерфейс View для обратных вызовов контроля состояния
class IView {
 public:
  /// @brief Деструктор интерфейса
  virtual ~IView() = default;

  /// @brief Вызывается после успешной загрузки модели
  /// @param data Данные модели
  /// @param vertex_count Количество вершин модели
  /// @param edge_count Количество ребер модели
  /// @param filename Имя файла модели
  virtual void OnModelLoaded(const ModelData& data, std::size_t vertex_count,
                             std::size_t edge_count,
                             const std::string& filename) = 0;
  /// @brief Вызывается после каждого обновления (поворот/перемещение/масштаб)
  /// @param data Данные модели
  virtual void OnModelTransformed(const ModelData& data) = 0;
  /// @brief Вызывается после изменения настроек отображения
  virtual void OnRenderSettingsChanged() = 0;
  /// @brief Вызывается по окончании сохранения изображения
  /// @param filepath Путь к файлу изображения
  virtual void OnImageSaved(const std::string& filepath) = 0;
  /// @brief Вызывается по окончании записи GIF
  /// @param filepath Путь к файлу записи GIF
  virtual void OnGifRecorded(const std::string& filepath) = 0;
  /// @brief Вызывается при ошибке
  /// @param message Сообщение об ошибке
  virtual void OnError(const std::string& message) = 0;
};

/// @brief Тонкий контроллер между UI и движком
class Controller {
 public:
  /// @brief Конструктор контроллера (явный)
  /// @param view Указатель на View (MainWindow или аналог)
  explicit Controller(IView* view);
  /// @brief Деструктор контроллера
  ~Controller() = default;

  /// @brief Загрузка модели
  /// @param filepath Путь к файлу модели
  /// @return true если загрузка прошла успешно, иначе false
  bool LoadModel(const std::string& filepath);
  /// @brief Очистка модели
  void ClearModel();

  /// @brief Перемещение модели по осям X, Y, Z
  /// @param dx Смещение по x
  /// @param dy Смещение по y
  /// @param dz Смещение по z
  /// @return true если перемещение прошло успешно, иначе false
  bool Translate(double dx, double dy, double dz);
  /// @brief Поворот модели вокруг осей X, Y, Z
  /// @param ax_deg Угол поворота вокруг оси x в градусах
  /// @param ay_deg Угол поворота вокруг оси y в градусах
  /// @param az_deg Угол поворота вокруг оси z в градусах
  /// @return true если поворот прошел успешно, иначе false
  bool Rotate(double ax_deg, double ay_deg, double az_deg);
  /// @brief Масштабирование модели
  /// @param factor Коэффициент масштабирования
  /// @return true если масштабирование прошло успешно, иначе false
  bool Scale(double factor);
  /// @brief Сброс всех трансформаций в исходное состояние
  /// @return true если сброс прошел успешно, иначе false
  bool ResetTransformations();

  /// @brief Установка типа проекции
  /// @param type Тип проекции модели
  // void SetProjection(s21::ProjectionType type);
  /// @brief Установка стиля рёбер
  /// @param style Стиль рёбер
  // void SetEdgeStyle(const s21::EdgeStyle& style);
  /// @brief Установка стиля вершин
  /// @param style Стиль вершин
  // void SetVertexStyle(const s21::VertexStyle& style);
  /// @brief Установка цвета фона
  /// @param color Цвет фона
  // void SetBackgroundColor(const s21::Color& color);

  /// @brief Сохранение изображения в файл
  /// @param filepath Путь к файлу
  /// @param format Формат изображения
  /// @return true если сохранение прошло успешно, иначе false
  // bool SaveImage(const std::string& filepath, s21::ImageFormat format);
  /// @brief Создание скринкастов и их сохранение в файл
  /// @param filepath Путь к файлу
  /// @return true если запись прошла успешно, иначе false
  // bool RecordGif(const std::string& filepath);

 private:
  /// @brief Обертка для вызова обратного уведомления
  IView* view_;
  /// @brief Движок для работы с моделью
  ModelEngine engine_;

  /// @brief Проверка наличия модели и отправка ошибки, если нет
  /// @return true если модель загружена, иначе false
  bool EnsureModelLoaded();
};

}  // namespace s21

#endif /* CONTROLLER_H_ */