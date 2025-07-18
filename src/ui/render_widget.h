#ifndef RENDER_WIDGET_H_
#define RENDER_WIDGET_H_

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>

#include "../engine/model_data.h"

// #include "main_window.h"
#include "shader.h"

namespace s21 {

/// @brief Формардное объявление класса основного окна
class MainWindow;

/// @brief Виджет, на котором рисуется каркас модели в 2D
class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

 public:
  /// @brief Конструктор виджета с параметрами по умолчанию
  /// @param parent Указатель на родительскиое основное окно
  explicit RenderWidget(MainWindow *parent);
  /// @brief Деструктор виджета
  ~RenderWidget();

  /// @brief Устанавливает новые данные модели для отрисовки
  /// @param data Ссылка на данные модели
  void SetModelData(const ModelData &data);

  /// @brief Устанавливает новую матрицу трансформации
  /// @param Ссылка на новую матрицу
  void SetTransformationMatrix(const S21Matrix &matr);

 protected:
  /// @brief Инициализация данных OpenGL
  void initializeGL() override;

  /// @brief Изменение размеров виджета
  /// @param w ширина виджета
  /// @param h высота виджета
  void resizeGL(int w, int h) override;

  /// @brief Основной цикл прорисовки виджета
  void paintGL() override;

 private:
  /// @brief Отправка матрицы в шейдер
  /// @param shaderProgram ID шейдёр программы
  /// @param matrix матрица 4x4
  /// @param glGetActiveUniformName имя униформа матрицы в шейдере
  void SendMatrixToShader(GLuint shaderProgram, const S21Matrix &matrix,
                          const char *glGetActiveUniformName);

  /// @brief Создаёт матрицу перспективы для проекции
  /// @param fov_degrees Field of view в градусах
  /// @param aspect Отношение ширины к высоте
  /// @param near   Расстояние до ближней плоскости отсечения
  /// @param far    Расстояние до дальней плоскости отсечения
  /// @return матрица проекции перспективы 4x4
  S21Matrix Perspective(float fov_degrees, float aspect, float near, float far);

  /// @brief Создаёт матрицу ортографической проекции
  /// @param left   Координата левой границы видимой области
  /// @param right  Координата правой границы видимой области
  /// @param bottom Координата нижней границы видимой области
  /// @param top    Координата верхней границы видимой области
  /// @param near   Расстояние до ближней плоскости отсечения
  /// @param far    Расстояние до дальней плоскости отсечения
  /// @return       Матрица ортографической проекции 4x4
  S21Matrix Ortho(float left, float right, float bottom, float top, float near,
                  float far);

  /// @brief Указатель на родительскиое основное окно
  MainWindow *parent_;
  /// @brief Ссылка на данные модели для отрисовки
  const ModelData *model_;

  /// @brief Vertex Array Object
  GLuint VAO = 0;
  /// @brief Vertex Buffer Object
  GLuint VBO = 0;
  /// @brief Element Buffer Object
  GLuint EBO = 0;

  /// @brief Векстор вершин, подготовленный для работы с OpenGL
  std::vector<float> m_vertices;
  /// @brief Вектор рёбер, подготовленный для работы с OpenGL
  std::vector<GLuint> m_indices;

  /// @brief Шейдер вершин
  Shader m_pointShader;
  /// @brief Шейдер рёбер
  Shader m_lineShader;

  /// @brief Модельная матрица
  S21Matrix modelMatr;
  /// @brief Видовая матрица
  S21Matrix viewMatr;
  /// @brief Проекционная матрица
  S21Matrix projectionMatr;
};

}  // namespace s21

#endif  // RENDER_WIDGET_H_
