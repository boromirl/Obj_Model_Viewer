#include "render_widget.h"

#include "main_window.h"

namespace s21 {

RenderWidget::RenderWidget(MainWindow *parent)
    : QOpenGLWidget(parent), parent_(parent), model_(nullptr) {}

RenderWidget::~RenderWidget() {
  // Явно удаляем буфферы OpenGL (опциональная мера)
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void RenderWidget::SetModelData(const ModelData &data) {
  makeCurrent();

  model_ = &data;

  // vertices
  m_vertices.clear();
  m_vertices.reserve(model_->vertices.size() * 3);
  for (const Vertex vertex : model_->vertices) {
    m_vertices.emplace_back(vertex.x);
    m_vertices.emplace_back(vertex.y);
    m_vertices.emplace_back(vertex.z);
  }

  // edges
  m_indices.clear();
  m_indices.reserve(model_->edges.size() * 2);
  for (const Edge &edge : model_->edges) {
    m_indices.emplace_back(edge.first);
    m_indices.emplace_back(edge.second);
  }

  glBindVertexArray(VAO);  // все команды далее будут
                           // связаны с этим объектом

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
               m_vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
               m_indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  modelMatr = model_->tm;
  update();       // перерисовка виджета
  doneCurrent();  // выходим из контекста OpenGL
}

void RenderWidget::SetTransformationMatrix(const S21Matrix &matr) {
  modelMatr = matr;
}

void RenderWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);

  update();
}

void RenderWidget::initializeGL() {
  initializeOpenGLFunctions();  // обязательно

  // Включаем необходимые функции OpenGL
  // -----------------------------------
  glEnable(GL_PROGRAM_POINT_SIZE);  // изменение размера точек в шейдерах
  glEnable(GL_DEPTH_TEST);  // проверка глубины (для перспективы)

  // инициализация шейдеров
  m_pointShader.init("assets/shaders/point.vert", "assets/shaders/point.frag");
  m_lineShader.init("assets/shaders/line.vert", "assets/shaders/line.frag");

  // создание буфферов и объекта для OpenGL
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
}

void RenderWidget::paintGL() {
  if (m_lineShader.ID == 0 || m_pointShader.ID == 0) {
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    return;
  }

  // Вид проекции
  // -------------
  ProjectionType pt = parent_->GetProjectionType();

  if (pt == ProjectionType::Central) {
    projectionMatr = Perspective(
        45.0f, (float)this->width() / (float)this->height(), 0.1f, 100.0f);
  } else {
    float aspect = (float)this->width() / (float)this->height();
    float orthoSize = 1.2f;  // контролирует уровень скейлинга, приближения.
                             // Если увеличивать, то модель будет отдаляться,
                             // если уменьшить, то модель приблизится

    projectionMatr =
        Ortho(-orthoSize * aspect,  // левая граница "области видимости"
              orthoSize * aspect,  // правая граница "области видимости"
              -orthoSize, orthoSize,  // нижняя, верхняя границы
              0.1f, 100.0f);
  }

  // Матрица вида (матрица камеры)
  // ---------------------------------
  float cameraDistance =
      3.0f;  // позиция камеры (можно отдалять, если модель не влезает)
  viewMatr = S21Matrix(4, 4);
  viewMatr.SetIdentity(1.0);
  viewMatr(3, 2) = -cameraDistance;

  // Фоновый цвет
  // ------------
  Color bg = parent_->GetBackgroundColor();
  // OpenGL ожидает float значения в интервале [0;1]
  glClearColor(bg.r / 255.0f, bg.g / 255.0f, bg.b / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(VAO);

  // Рёбра
  // ----------------

  EdgeStyle e_style = parent_->GetEdgeStyle();

  // работа с шейдерами рёбер
  m_lineShader.use();
  SendMatrixToShader(m_lineShader.ID, modelMatr, "model");
  SendMatrixToShader(m_lineShader.ID, viewMatr, "view");
  SendMatrixToShader(m_lineShader.ID, projectionMatr, "projection");
  m_lineShader.setVec4("lineColor", e_style.color.r / 255.0f,
                       e_style.color.g / 255.0f, e_style.color.b / 255.0f,
                       e_style.color.a / 255.0f);
  m_lineShader.setVec2("resolution", this->width(), this->height());
  m_lineShader.setFloat("dashLength", 10.0f);
  m_lineShader.setFloat("gapLength", 10.0f);

  if (e_style.style == LineStyle::Dashed) {
    m_lineShader.setBool("isDashed", true);
  } else {
    m_lineShader.setBool("isDashed", false);
  }

  // прорисовка рёбер
  glDisable(GL_POINT_SPRITE);  // не нужно при прорисовке линий
  glLineWidth(e_style.thickness);
  glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);

  // Вершины
  // -----------------

  VertexStyle v_style = parent_->GetVertexStyle();

  if (v_style.shape != VertexShape::None) {
    // работа с шейдерами вершин
    m_pointShader.use();
    SendMatrixToShader(m_pointShader.ID, modelMatr, "model");
    SendMatrixToShader(m_pointShader.ID, viewMatr, "view");
    SendMatrixToShader(m_pointShader.ID, projectionMatr, "projection");
    m_pointShader.setFloat("pointSize", (float)v_style.size);
    m_pointShader.setVec4("pointColor", v_style.color.r / 255.0f,
                          v_style.color.g / 255.0f, v_style.color.b / 255.0f,
                          v_style.color.a / 255.0f);
    if (v_style.shape == VertexShape::Circle) {
      m_pointShader.setBool("isCircle", true);
    } else if (v_style.shape == VertexShape::Square) {
      m_pointShader.setBool("isCircle", false);
    }

    // отрисовка вершин
    glEnable(GL_POINT_SPRITE);
    glDrawArrays(GL_POINTS, 0,
                 m_vertices.size() / 3);  // каждая вершина - 3 числа
  }

  // Деактивация и удаление объектов
  // -------------------------------
  glBindVertexArray(0);
  // doneCurrent();
}

void RenderWidget::SendMatrixToShader(GLuint shaderProgram,
                                      const S21Matrix &matrix,
                                      const char *uniformName) {
  std::vector<float> matrixData = matrix.ToFlatArray();

  GLint loc = glGetUniformLocation(shaderProgram, uniformName);
  if (loc == -1) {
    std::cerr << "Uniform " << uniformName << " not found in shader!"
              << std::endl;
    return;
  }

  glUniformMatrix4fv(loc, 1, GL_FALSE, matrixData.data());
}

S21Matrix RenderWidget::Perspective(float fov_degrees, float aspect, float near,
                                    float far) {
  S21Matrix per(4, 4);

  float fov_radians = fov_degrees * (M_PI / 180.0f);
  float tanHalFov = tan(fov_radians / 2.0f);

  per(0, 0) = 1.0f / (aspect * tanHalFov);
  per(1, 1) = 1.0f / tanHalFov;
  per(2, 2) = -(far + near) / (far - near);
  per(2, 3) = -1.0f;
  per(3, 2) = -(2.0f * far * near) / (far - near);

  return per;
}

S21Matrix RenderWidget::Ortho(float left, float right, float bottom, float top,
                              float near, float far) {
  S21Matrix ortho(4, 4);
  ortho.SetIdentity(1.0f);

  ortho(0, 0) = 2.0f / (right - left);
  ortho(1, 1) = 2.0f / (top - bottom);
  ortho(2, 2) = -2.0f / (far - near);

  ortho(3, 0) = -(right + left) / (right - left);
  ortho(3, 1) = -(top + bottom) / (top - bottom);
  ortho(3, 2) = -(far + near) / (far - near);

  return ortho;
}

}  // namespace s21
