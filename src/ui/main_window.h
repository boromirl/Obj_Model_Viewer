#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QToolBar>

#include "../controller/controller.h"
#include "render_widget.h"

namespace s21 {
/// @brief Название компании (для QSettings)
inline const QString COMPANY_NAME = "School_S21";
/// @brief Имя программы (для QSettings и заголовка окна)
inline const QString PROG_NAME = "3DViewer v2.0";
/// @brief Ширина окна
constexpr int WINDOW_WIDTH = 1200;
/// @brief Высота окна
constexpr int WINDOW_HEIGHT = 800;
/// @brief Дефолтный тип проекции
constexpr int DEFAULT_PROJECTION_TYPE = 0;  // Parallel
/// @brief Дефолтный цвет фона (в формате "R,G,B,A")
inline const QString DEFAULT_BACKGROUND_COLOR = "255,255,255,255";  // белый
/// @brief Дефолтный стиль рёбер
constexpr int DEFAULT_EDGE_STYLE = 0;  // Solid;
/// @brief Дефолтный цвет рёбер (в формате "R,G,B,A")
inline const QString DEFAULT_EDGE_COLOR = "0,0,0,255";  // чёрный
/// @brief Дефолтная толщина рёбер
constexpr double DEFAULT_EDGE_THICKNESS = 1.0;
/// @brief Дефолтный стиль вершин
constexpr int DEFAULT_VERTEX_SHAPE = 0;  // None;
/// @brief Дефолтный цвет вершин (в формате "R,G,B,A")
inline const QString DEFAULT_VERTEX_COLOR = "0,0,0,255";  // чёрный
/// @brief Дефолтный размер вершин
constexpr double DEFAULT_VERTEX_SIZE = 3.0;

/// @brief Тип проекции модели
enum class ProjectionType { Parallel, Central };
/// @brief Тип стиля рёбер модели
enum class LineStyle { Solid, Dashed };
/// @brief Тип стиля вершин модели
enum class VertexShape { None, Circle, Square };

/// @brief Формат изображения
enum class ImageFormat { BMP, JPEG };

/// @brief Цвет
/// @param r Красный [0,255]
/// @param g Зелёный [0,255]
/// @param b Синий [0,255]
/// @param a Альфа канал, прозрачность [0,255]
struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

/// @brief Стиль рёбер
/// @param style Тип стиля линии
/// @param color Цвет линии
/// @param thickness Толщина линии в пикселях
struct EdgeStyle {
  LineStyle style;
  Color color;
  double thickness;
};

/// @brief Стиль вершин
/// @param shape Тип формы вершины
/// @param color Цвет вершины
/// @param size Размер вершины в пикселях (диаметр или сторона квадрата)
struct VertexStyle {
  VertexShape shape;
  Color color;
  double size;
};

/// @brief Главное окно программы, в котором отображается виджет для
/// рендеринга
class MainWindow : public QMainWindow, public IView {
  Q_OBJECT

 public:
  /// @brief Конструктор главного окна
  /// @param parent Родительский виджет
  explicit MainWindow(QWidget* parent = nullptr);
  /// @brief Деструктор главного окна
  ~MainWindow() override;

  /// @brief Получение типа проекции
  /// @return Тип проекции
  ProjectionType GetProjectionType() const { return projection_type_; }
  /// @brief Установка типа проекции
  /// @param type Тип проекции
  void SetProjectionType(ProjectionType type) { projection_type_ = type; }

  /// @brief Получение цвета фона
  /// @return Цвет фона
  Color GetBackgroundColor() const { return background_color_; }
  /// @brief Установка цвета фона
  /// @param color Цвет фона
  void SetBackgroundColor(Color color) { background_color_ = color; }

  /// @brief Получение стиля рёбер
  /// @return Стиль рёбер
  EdgeStyle GetEdgeStyle() const { return edge_style_; }
  /// @brief Установка стиля рёбер
  /// @param style Стиль рёбер
  void SetEdgeStyle(EdgeStyle style) { edge_style_ = style; }

  /// @brief Получение стиля вершин
  /// @return Стиль вершин
  VertexStyle GetVertexStyle() const { return vertex_style_; }
  /// @brief Установка стиля вершин
  /// @param style Стиль вершин
  void SetVertexStyle(VertexStyle style) { vertex_style_ = style; }

  // Реализация IView:
  /// @brief Вызывается после успешной загрузки модели
  /// @param data Ссылка на данные модели
  /// @param vertex_count Количество вершин модели
  /// @param edge_count Количество ребер модели
  /// @param filename Имя файла модели
  void OnModelLoaded(const ModelData& data, std::size_t vertex_count,
                     std::size_t edge_count,
                     const std::string& filename) override;
  /// @brief Вызывается после каждого обновления (поворот/перемещение/масштаб)
  /// @param data Ссылка на данные модели
  void OnModelTransformed(const S21Matrix& tm) override;
  /// @brief Вызывается после изменения настроек отображения
  void OnRenderSettingsChanged() override;
  /// @brief Вызывается по окончании сохранения изображения
  /// @param filepath Путь к файлу изображения
  void OnImageSaved(const std::string& filepath) override;
  /// @brief Вызывается по окончании записи GIF
  /// @param filepath Путь к файлу изображения
  void OnGifRecorded(const std::string& filepath) override;
  /// @brief Вызывается при ошибке
  /// @param message Сообщение об ошибке
  void OnError(const std::string& message) override;

 protected:
  /// @brief Обработчик события закрытия окна
  /// @param event Событие закрытия окна
  void closeEvent(QCloseEvent* event) override;

 private slots:
  /// @brief Обработчик нажатия кнопки перемещения модели
  void SlotTranslate();
  /// @brief Обработчик нажатия кнопки поворота модели
  void SlotRotate();
  /// @brief Обработчик нажатия кнопки масштабирования модели
  void SlotScale();
  /// @brief Обработчик нажатия кнопки сброса трансформаций модели
  void SlotReset();

  /// @brief Слот для меню «Open…»
  void SlotMenuOpen();
  /// @brief Слот для меню «Exit»
  void SlotMenuExit();

  /// @brief Слот для меню «Settings»
  void SlotMenuSettings();

  // … прочие слоты …

 private:
  /// @brief Создание действий меню
  void CreateActions();
  /// @brief Создание меню
  void CreateMenus();
  /// @brief Создание панели инструментов для управления трансформацией
  void CreateTransformToolBar();
  /// @brief Инициализация тулбара трансформаций
  void InitializeTransformToolBar();

  /// @brief Считать из persistent storage (QSettings) все нужные параметры
  void ReadSettings();
  /// @brief Записать в persistent storage (QSettings) все нужные параметры
  void WriteSettings();
  /// @brief Чтение цвета из QSettings по заданному ключу
  /// @param key Ключ параметра
  /// @param defaultValue Значение по умолчанию
  /// @return Структура цвета
  Color ReadColorSetting(const QString& key, const QString& defaultValue) const;
  /// @brief Запись цвета в QSettings по заданному ключу
  /// @param key Ключ параметра
  /// @param color Цвет
  void WriteColorSetting(const QString& key, const Color& color);

  // UI-элементы:
  /// @brief Указатель на надпись имени файла
  QLabel* file_label_;
  /// @brief Указатель на надпись количества вершин
  QLabel* vertex_count_label_;
  /// @brief Указатель на надпись количества ребер
  QLabel* edge_count_label_;
  /// @brief Указатель на виджет рендеринга
  RenderWidget* render_widget_;

  /// @brief Главное меню «File»
  QMenu* menuFile_;
  /// @brief Действие меню «Open» (File → Open…)
  QAction* file_open_action_;
  /// @brief Действие меню «Exit» (File → Exit)
  QAction* file_exit_action_;

  /// @brief Главное меню «Settings»
  QMenu* menuSettings_;

  /// @brief Контроллер MVC
  Controller controller_;

  // Инструменты для трансформаций в тулбаре:
  /// @brief Указатель на тулбар трансформаций
  QToolBar* transformToolBar_;
  /// @brief Указатель на спинбокс для ввода величины перемещения по X, Y и Z
  QDoubleSpinBox *spinDx_, *spinDy_, *spinDz_;
  /// @brief Указатель на кнопку для применения перемещения
  QPushButton* btnTranslate_;
  /// @brief Указатель на спинбокс для ввода величины поворота по X, Y и Z
  QDoubleSpinBox *spinAx_, *spinAy_, *spinAz_;
  /// @brief Указатель на кнопку для применения поворота
  QPushButton* btnRotate_;
  /// @brief Указатель на спинбокс для ввода величины масштабирования
  QDoubleSpinBox* spinScale_;
  /// @brief Указатель на кнопку для применения масштабирования
  QPushButton* btnScale_;
  /// @brief Указатель на кнопку для сброса трансформаций
  QPushButton* btnReset_;

  // Настройки:
  /// @brief Тип проекции модели
  ProjectionType projection_type_;
  /// @brief Цвет фона
  Color background_color_;
  /// @brief Стиль рёбер модели
  EdgeStyle edge_style_;
  /// @brief Стиль вершин модели
  VertexStyle vertex_style_;
};

}  // namespace s21

#endif  // MAIN_WINDOW_H_
