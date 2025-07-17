#include "settings_dialog.h"

#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>

namespace s21 {
SettingsDialog::SettingsDialog(MainWindow* parent)
    : QDialog(parent), parent_(parent) {
  setWindowTitle("Settings");
  setFixedWidth(SETTING_WINDOW_WIDTH);
  SetLayout();

  // Заполняем настройки текущими значениями из MainWindow
  LoadCurrentSettingsFromParent();
}

void SettingsDialog::SetLayout() {
  // Создаём главный вертикальный layout
  auto* mainLayout = new QVBoxLayout(this);

  // Вызываем «сборщики» отдельных блоков:
  CreateBackgroundBlock(mainLayout);
  CreateProjectionBlock(mainLayout);
  CreateEdgeBlock(mainLayout);
  CreateVertexBlock(mainLayout);
  CreateButtonsBlock(mainLayout);
}

void SettingsDialog::CreateBackgroundBlock(QVBoxLayout* parentLayout) {
  AddColorPickerRow(parentLayout, "Choose background color", bg_color_btn_,
                    bg_color_label_, &SettingsDialog::ChooseBackgroundColor);
}

void SettingsDialog::CreateProjectionBlock(QVBoxLayout* parentLayout) {
  parentLayout->addWidget(new QLabel("Projection type", this));
  projection_type_combobox_ = new QComboBox(this);
  projection_type_combobox_->addItems({"Parallel", "Central"});
  parentLayout->addWidget(projection_type_combobox_);
}

void SettingsDialog::CreateEdgeBlock(QVBoxLayout* parentLayout) {
  // Блок «Толщина ребра»
  edge_thickness_spinbox_ = new QDoubleSpinBox(this);
  edge_thickness_spinbox_->setRange(1, 10);
  edge_thickness_spinbox_->setSingleStep(0.1);
  parentLayout->addWidget(new QLabel("Edge thickness", this));
  parentLayout->addWidget(edge_thickness_spinbox_);

  // Блок «Тип ребра»
  edge_type_combobox_ = new QComboBox(this);
  edge_type_combobox_->addItems({"Solid", "Dashed"});
  parentLayout->addWidget(new QLabel("Edge type", this));
  parentLayout->addWidget(edge_type_combobox_);

  // Блок «Цвет ребра»
  AddColorPickerRow(parentLayout, "Choose edge color", edge_color_btn_,
                    edge_color_label_, &SettingsDialog::ChooseEdgeColor);
}

void SettingsDialog::CreateVertexBlock(QVBoxLayout* parentLayout) {
  // Блок «Тип вершин»
  vertex_type_combobox_ = new QComboBox(this);
  vertex_type_combobox_->addItems({"None", "Circle", "Square"});
  parentLayout->addWidget(new QLabel("Vertex type", this));
  parentLayout->addWidget(vertex_type_combobox_);

  // Блок «Размер вершин»
  vertex_size_spinbox_ = new QDoubleSpinBox(this);
  vertex_size_spinbox_->setRange(1.0, 10.0);
  vertex_size_spinbox_->setSingleStep(0.1);
  parentLayout->addWidget(new QLabel("Vertex size", this));
  parentLayout->addWidget(vertex_size_spinbox_);

  // Блок «Цвет вершин»
  AddColorPickerRow(parentLayout, "Choose vertex color", vertex_color_btn_,
                    vertex_color_label_, &SettingsDialog::ChooseVertexColor);
}

void SettingsDialog::CreateButtonsBlock(QVBoxLayout* parentLayout) {
  // Блок «Кнопки Apply / Reset / Cancel»
  auto* btns_layout = new QHBoxLayout;
  auto* apply_btn = new QPushButton("Apply", this);
  reset_btn_ = new QPushButton("Reset", this);
  auto* cancel_btn = new QPushButton("Cancel", this);

  btns_layout->addWidget(apply_btn);
  btns_layout->addWidget(reset_btn_);
  btns_layout->addWidget(cancel_btn);
  parentLayout->addLayout(btns_layout);

  connect(apply_btn, &QPushButton::clicked, this,
          &SettingsDialog::ApplySettings);
  connect(cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
  connect(reset_btn_, &QPushButton::clicked, this,
          &SettingsDialog::ResetSettings);
}

void SettingsDialog::AddColorPickerRow(QVBoxLayout* parentLayout,
                                       const QString& buttonText,
                                       QPushButton*& outButton,
                                       QLabel*& outLabel,
                                       void (SettingsDialog::*slot)()) {
  auto* layout = new QHBoxLayout;
  outButton = new QPushButton(buttonText, this);
  outLabel = new QLabel(this);
  outLabel->setFixedSize(30, 30);

  layout->addWidget(outButton);
  layout->addWidget(outLabel);
  parentLayout->addLayout(layout);

  connect(outButton, &QPushButton::clicked, this, slot);
}

void SettingsDialog::LoadCurrentSettingsFromParent() {
  // Загрузка Цвета фона
  Color bg = parent_->GetBackgroundColor();
  background_color_.setRgb(bg.r, bg.g, bg.b, bg.a);
  UpdateColorLabel(bg_color_label_, bg);

  // Загрузка “Проекции”
  ProjectionType proj = parent_->GetProjectionType();
  projection_type_combobox_->setCurrentIndex(static_cast<int>(proj));

  // Загрузка “Толщины ребра” и “Типа ребра”
  EdgeStyle edge = parent_->GetEdgeStyle();
  edge_thickness_spinbox_->setValue(edge.thickness);
  edge_type_combobox_->setCurrentIndex(static_cast<int>(edge.style));
  // Цвет ребра:
  edge_color_.setRgb(edge.color.r, edge.color.g, edge.color.b, edge.color.a);
  UpdateColorLabel(edge_color_label_, edge.color);

  // Загрузка “Типа вершин”, “Размер вершин”
  VertexStyle vertex = parent_->GetVertexStyle();
  vertex_type_combobox_->setCurrentIndex(static_cast<int>(vertex.shape));
  vertex_size_spinbox_->setValue(vertex.size);
  // Цвет вершин:
  vertex_color_.setRgb(vertex.color.r, vertex.color.g, vertex.color.b,
                       vertex.color.a);
  UpdateColorLabel(vertex_color_label_, vertex.color);
}

void SettingsDialog::ChooseBackgroundColor() {
  QColorDialog dialog(this);
  dialog.setOption(QColorDialog::ShowAlphaChannel);
  dialog.setCurrentColor(background_color_);

  if (dialog.exec() == QDialog::Accepted) {
    background_color_ = dialog.currentColor();
    UpdateColorLabel(bg_color_label_,
                     {static_cast<unsigned char>(background_color_.red()),
                      static_cast<unsigned char>(background_color_.green()),
                      static_cast<unsigned char>(background_color_.blue()),
                      static_cast<unsigned char>(background_color_.alpha())});
  }
}

void SettingsDialog::ChooseEdgeColor() {
  QColorDialog dialog(this);
  dialog.setOption(QColorDialog::ShowAlphaChannel);
  dialog.setCurrentColor(edge_color_);

  if (dialog.exec() == QDialog::Accepted) {
    edge_color_ = dialog.currentColor();
    UpdateColorLabel(edge_color_label_,
                     {static_cast<unsigned char>(edge_color_.red()),
                      static_cast<unsigned char>(edge_color_.green()),
                      static_cast<unsigned char>(edge_color_.blue()),
                      static_cast<unsigned char>(edge_color_.alpha())});
  }
}

void SettingsDialog::ChooseVertexColor() {
  QColorDialog dialog(this);
  dialog.setOption(QColorDialog::ShowAlphaChannel);
  dialog.setCurrentColor(vertex_color_);

  if (dialog.exec() == QDialog::Accepted) {
    vertex_color_ = dialog.currentColor();
    UpdateColorLabel(vertex_color_label_,
                     {static_cast<unsigned char>(vertex_color_.red()),
                      static_cast<unsigned char>(vertex_color_.green()),
                      static_cast<unsigned char>(vertex_color_.blue()),
                      static_cast<unsigned char>(vertex_color_.alpha())});
  }
}

void SettingsDialog::ApplySettings() {
  // Проекция:
  if (projection_type_combobox_->currentText() == "Parallel") {
    parent_->SetProjectionType(ProjectionType::Parallel);
  } else {
    parent_->SetProjectionType(ProjectionType::Central);
  }

  // Вершины:
  VertexStyle vertex;
  vertex.color = {static_cast<unsigned char>(vertex_color_.red()),
                  static_cast<unsigned char>(vertex_color_.green()),
                  static_cast<unsigned char>(vertex_color_.blue()),
                  static_cast<unsigned char>(vertex_color_.alpha())};
  vertex.size = vertex_size_spinbox_->value();
  if (vertex_type_combobox_->currentText() == "None") {
    vertex.shape = VertexShape::None;
  } else if (vertex_type_combobox_->currentText() == "Circle") {
    vertex.shape = VertexShape::Circle;
  } else {
    vertex.shape = VertexShape::Square;
  }
  parent_->SetVertexStyle(vertex);

  // Рёбра:
  EdgeStyle edge;
  edge.color = {static_cast<unsigned char>(edge_color_.red()),
                static_cast<unsigned char>(edge_color_.green()),
                static_cast<unsigned char>(edge_color_.blue()),
                static_cast<unsigned char>(edge_color_.alpha())};
  edge.thickness = edge_thickness_spinbox_->value();

  if (edge_type_combobox_->currentText() == "Solid") {
    {
      edge.style = LineStyle::Solid;
    }
  } else {
    { edge.style = LineStyle::Dashed; }
  }
  parent_->SetEdgeStyle(edge);

  // Фон:
  Color color{static_cast<unsigned char>(background_color_.red()),
              static_cast<unsigned char>(background_color_.green()),
              static_cast<unsigned char>(background_color_.blue()),
              static_cast<unsigned char>(background_color_.alpha())};
  parent_->SetBackgroundColor(color);

  parent_->OnRenderSettingsChanged();
  accept();  // Закрываем диалог
}

void SettingsDialog::ResetSettings() {
  // Сброс цвета фона
  const Color default_bg = ParseColorString(DEFAULT_BACKGROUND_COLOR);
  background_color_.setRgb(default_bg.r, default_bg.g, default_bg.b,
                           default_bg.a);
  UpdateColorLabel(bg_color_label_, default_bg);

  // Сброс проекции
  projection_type_combobox_->setCurrentIndex(DEFAULT_PROJECTION_TYPE);

  // Сброс толщины ребра и типа ребра
  edge_thickness_spinbox_->setValue(DEFAULT_EDGE_THICKNESS);
  edge_type_combobox_->setCurrentIndex(DEFAULT_EDGE_STYLE);

  // Сброс цвета ребра
  const Color default_edge = ParseColorString(DEFAULT_EDGE_COLOR);
  edge_color_.setRgb(default_edge.r, default_edge.g, default_edge.b,
                     default_edge.a);
  UpdateColorLabel(edge_color_label_, default_edge);

  // Сброс типа вершин и размера вершин
  vertex_type_combobox_->setCurrentIndex(
      static_cast<int>(DEFAULT_VERTEX_SHAPE));
  vertex_size_spinbox_->setValue(DEFAULT_VERTEX_SIZE);

  // Сброс цвета вершин
  const Color default_vertex = ParseColorString(DEFAULT_VERTEX_COLOR);
  vertex_color_.setRgb(default_vertex.r, default_vertex.g, default_vertex.b,
                       default_vertex.a);
  UpdateColorLabel(vertex_color_label_, default_vertex);
}

void SettingsDialog::UpdateColorLabel(QLabel* label, const Color& color) {
  const QString style = QString(
                            "background-color: rgba(%1,%2,%3,%4);"
                            "border: 1.2px solid black;"
                            "border-radius: 5px;")
                            .arg(color.r)
                            .arg(color.g)
                            .arg(color.b)
                            .arg(color.a);
  label->setStyleSheet(style);
}

Color SettingsDialog::ParseColorString(const QString& str) {
  const QStringList parts = str.split(',');
  Color color;
  color.r = static_cast<unsigned char>(parts[0].toInt());
  color.g = static_cast<unsigned char>(parts[1].toInt());
  color.b = static_cast<unsigned char>(parts[2].toInt());
  color.a = static_cast<unsigned char>(parts[3].toInt());
  return color;
}

}  // namespace s21
