#include "main_window.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

#include "settings_dialog.h"

namespace s21 {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      file_label_(new QLabel("No file loaded", this)),
      vertex_count_label_(new QLabel("0", this)),
      edge_count_label_(new QLabel("0", this)),
      render_widget_(new RenderWidget(this)),
      controller_(this) {
  // Читаем сохранённые настройки, если они есть или инициализируем если нет
  ReadSettings();

  // Собираем UI
  auto* central = new QWidget(this);
  auto* vbox = new QVBoxLayout(central);
  auto* hbox = new QHBoxLayout();

  hbox->addWidget(file_label_);
  hbox->addWidget(new QLabel("Vert:", this));
  hbox->addWidget(vertex_count_label_);
  hbox->addWidget(new QLabel("Eges:", this));
  hbox->addWidget(edge_count_label_);
  vbox->addLayout(hbox);
  vbox->addWidget(render_widget_, /*stretch=*/1);

  setCentralWidget(central);
  setWindowTitle(PROG_NAME);
  resize(WINDOW_WIDTH, WINDOW_HEIGHT);

  CreateActions();
  CreateMenus();
  CreateTransformToolBar();

  vbox->addWidget(transformToolBar_);

  // (далее нужно связать и другие кнопки/поля с лямбдами,
  // вызывающими controller_.Translate/Rotate/Scale/Reset)
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent* event) {
  WriteSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::ReadSettings() {
  QSettings settings(COMPANY_NAME, PROG_NAME);

  // Тип проекции
  projection_type_ = static_cast<ProjectionType>(
      settings.value("render/projection_type", DEFAULT_PROJECTION_TYPE)
          .toInt());

  // Фон
  background_color_ =
      ReadColorSetting("render/background_color", DEFAULT_BACKGROUND_COLOR);

  // Стиль рёбер
  edge_style_.style = static_cast<LineStyle>(
      settings.value("render/edge/style", DEFAULT_EDGE_STYLE).toInt());
  edge_style_.color = ReadColorSetting("render/edge/color", DEFAULT_EDGE_COLOR);
  edge_style_.thickness =
      settings.value("render/edge/thickness", DEFAULT_EDGE_THICKNESS)
          .toDouble();

  // Стиль вершин
  vertex_style_.shape = static_cast<VertexShape>(
      settings.value("render/vertex/shape", DEFAULT_VERTEX_SHAPE).toInt());
  vertex_style_.color =
      ReadColorSetting("render/vertex/color", DEFAULT_VERTEX_COLOR);
  vertex_style_.size =
      settings.value("render/vertex/size", DEFAULT_VERTEX_SIZE).toDouble();
}

void MainWindow::WriteSettings() {
  QSettings settings(COMPANY_NAME, PROG_NAME);

  // Тип проекции
  settings.setValue("render/projection_type",
                    static_cast<int>(projection_type_));

  // Фон
  WriteColorSetting("render/background_color", background_color_);

  // Стиль рёбер
  settings.setValue("render/edge/style", static_cast<int>(edge_style_.style));
  WriteColorSetting("render/edge/color", edge_style_.color);
  settings.setValue("render/edge/thickness", edge_style_.thickness);

  // Стиль вершин
  settings.setValue("render/vertex/shape",
                    static_cast<int>(vertex_style_.shape));
  WriteColorSetting("render/vertex/color", vertex_style_.color);
  settings.setValue("render/vertex/size", vertex_style_.size);

  settings.sync();
}

Color MainWindow::ReadColorSetting(const QString& key,
                                   const QString& defaultValue) const {
  QColor qcolor = QSettings(COMPANY_NAME, PROG_NAME)
                      .value(key, defaultValue)
                      .value<QColor>();
  return Color{static_cast<unsigned char>(qcolor.red()),
               static_cast<unsigned char>(qcolor.green()),
               static_cast<unsigned char>(qcolor.blue()),
               static_cast<unsigned char>(qcolor.alpha())};
}

void MainWindow::WriteColorSetting(const QString& key, const Color& color) {
  QColor qcolor(color.r, color.g, color.b, color.a);
  QSettings(COMPANY_NAME, PROG_NAME).setValue(key, qcolor);
}

void MainWindow::CreateActions() {
  // создаём действия для меню File
  file_open_action_ = new QAction("Open model", this);
  file_exit_action_ = new QAction("Exit", this);
  connect(file_open_action_, &QAction::triggered, this,
          &MainWindow::SlotMenuOpen);
  connect(file_exit_action_, &QAction::triggered, this,
          &MainWindow::SlotMenuExit);
}

void MainWindow::CreateMenus() {
  menuFile_ = menuBar()->addMenu("File");
  menuFile_->addAction(file_open_action_);
  menuFile_->addSeparator();
  menuFile_->addAction(file_exit_action_);

  menuSettings_ = menuBar()->addMenu("Settings");

  QAction* settings_action = new QAction("Settings", this);
  connect(settings_action, &QAction::triggered, this,
          &MainWindow::SlotMenuSettings);
  menuSettings_->addAction(settings_action);
}

void MainWindow::CreateTransformToolBar() {
  transformToolBar_ = addToolBar("Transform");
  transformToolBar_->addWidget(new QLabel("dx:", this));
  spinDx_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinDx_);
  transformToolBar_->addWidget(new QLabel("dy:", this));
  spinDy_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinDy_);
  transformToolBar_->addWidget(new QLabel("dz:", this));
  spinDz_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinDz_);
  btnTranslate_ = new QPushButton("Translate", this);
  transformToolBar_->addWidget(btnTranslate_);
  connect(btnTranslate_, &QPushButton::clicked, this,
          &MainWindow::SlotTranslate);
  transformToolBar_->addSeparator();
  // — Поворот —
  transformToolBar_->addWidget(new QLabel("αx:", this));
  spinAx_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinAx_);
  transformToolBar_->addWidget(new QLabel("αy:", this));
  spinAy_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinAy_);
  transformToolBar_->addWidget(new QLabel("αz:", this));
  spinAz_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinAz_);
  btnRotate_ = new QPushButton("Rotate", this);
  transformToolBar_->addWidget(btnRotate_);
  connect(btnRotate_, &QPushButton::clicked, this, &MainWindow::SlotRotate);
  transformToolBar_->addSeparator();
  // — Масштаб —
  transformToolBar_->addWidget(new QLabel("Scale:", this));
  spinScale_ = new QDoubleSpinBox(this);
  transformToolBar_->addWidget(spinScale_);
  btnScale_ = new QPushButton("Scale", this);
  transformToolBar_->addWidget(btnScale_);
  connect(btnScale_, &QPushButton::clicked, this, &MainWindow::SlotScale);
  transformToolBar_->addSeparator();
  btnReset_ = new QPushButton("Reset", this);
  transformToolBar_->addWidget(btnReset_);
  connect(btnReset_, &QPushButton::clicked, this, &MainWindow::SlotReset);

  // Инициализируем тулбар
  InitializeTransformToolBar();
}

void MainWindow::InitializeTransformToolBar() {
  spinDx_->setRange(-1000, 1000);
  spinDx_->setSingleStep(1);
  spinDx_->setValue(0.0);

  spinDy_->setRange(-1000, 1000);
  spinDy_->setSingleStep(1);
  spinDy_->setValue(0.0);

  spinDz_->setRange(-1000, 1000);
  spinDz_->setSingleStep(1);
  spinDz_->setValue(0.0);

  spinAx_->setRange(-360, 360);
  spinAx_->setSingleStep(5);
  spinAx_->setValue(0.0);

  spinAy_->setRange(-360, 360);
  spinAy_->setSingleStep(5);
  spinAy_->setValue(0.0);

  spinAz_->setRange(-360, 360);
  spinAz_->setSingleStep(5);
  spinAz_->setValue(0.0);

  spinScale_->setRange(0.01, 100.0);
  spinScale_->setSingleStep(0.1);
  spinScale_->setValue(1.0);
}

void MainWindow::SlotTranslate() {
  controller_.Translate(spinDx_->value(), spinDy_->value(), spinDz_->value());
}

void MainWindow::SlotRotate() {
  controller_.Rotate(spinAx_->value(), spinAy_->value(), spinAz_->value());
}

void MainWindow::SlotScale() { controller_.Scale(spinScale_->value()); }

void MainWindow::SlotReset() { controller_.ResetTransformations(); }

void MainWindow::SlotMenuOpen() {
  QString path = QFileDialog::getOpenFileName(this, "Open OBJ", QString(),
                                              "OBJ Files (*.obj)");
  if (!path.isEmpty()) {
    controller_.LoadModel(path.toStdString());
  }
}

void MainWindow::SlotMenuExit() { close(); }

void MainWindow::SlotMenuSettings() {
  SettingsDialog dialog(this);  // передаём родителя
  dialog.exec();
}

// IView-колбэки:

void MainWindow::OnModelLoaded(const ModelData& data, std::size_t vertex_count,
                               std::size_t edge_count,
                               const std::string& filename) {
  // Обновляем метки
  file_label_->setText(QString::fromStdString(filename));
  vertex_count_label_->setText(QString::number(vertex_count));
  edge_count_label_->setText(QString::number(edge_count));
  // Передаём данные на отрисовку
  render_widget_->SetModelData(data);
  render_widget_->update();
  // Обновляем заголовок окна
  setWindowTitle(PROG_NAME + " (" + QString::fromStdString(filename) + ")");
  // Можно обновить статус-бар
}

void MainWindow::OnModelTransformed(const ModelData& data) {
  // При любых трансформациях просто перерисовываем
  render_widget_->SetModelData(data);
  render_widget_->update();
}

void MainWindow::OnRenderSettingsChanged() {
  // Если изменились стили/проекция/фон —
  // попросим RenderWidget перечитать их из controller
  // (или передать через отдельный сеттер)
  render_widget_->update();
}

void MainWindow::OnImageSaved(const std::string& filepath) {
  QMessageBox::information(
      this, "Image Saved",
      QString::fromStdString("Image has been saved to:\n%1")
          .arg(QString::fromStdString(filepath)));
}

void MainWindow::OnGifRecorded(const std::string& filepath) {
  QMessageBox::information(
      this, "GIF Recorded",
      QString::fromStdString("Image has been saved to:\n%1")
          .arg(QString::fromStdString(filepath)));
}

void MainWindow::OnError(const std::string& message) {
  QMessageBox::critical(this, "Error", QString::fromStdString(message));
}

}  // namespace s21
