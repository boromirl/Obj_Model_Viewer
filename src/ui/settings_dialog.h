#ifndef SETTINGS_DIALOG_H_
#define SETTINGS_DIALOG_H_

#include <QColor>
#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "../controller/controller.h"
#include "main_window.h"

namespace s21 {

/// @brief Дефолтная ширина окна
constexpr int SETTING_WINDOW_WIDTH = 330;

/// @brief Класс окна настроек
class SettingsDialog : public QDialog {
  Q_OBJECT

 public:
  /// @brief Конструктор
  /// @param parent Указатель на родительскиое основное окно
  explicit SettingsDialog(MainWindow* parent);
  /// @brief Деструктор
  ~SettingsDialog() override = default;

 private slots:
  /// @brief Диалог выбора цвета фона
  void ChooseBackgroundColor();
  /// @brief Диалог выбора цвета рёбер
  void ChooseEdgeColor();
  /// @brief Диалог выбора цвета вершин
  void ChooseVertexColor();
  /// @brief Применить изменённые настройки в окне
  void ApplySettings();
  /// @brief Сбросить все поля окна в значения по умолчанию
  void ResetSettings();

 private:
  /// @brief Компоновщик интерфейса окна
  void SetLayout();
  /// @brief Создать блок «Цвет фона» (кнопка + цветной квадратик)
  void CreateBackgroundBlock(QVBoxLayout* parentLayout);
  /// @brief Создать блок «Проекция» (метка + combobox)
  void CreateProjectionBlock(QVBoxLayout* parentLayout);
  /// @brief Создать блок с параметрами ребра
  void CreateEdgeBlock(QVBoxLayout* parentLayout);
  /// @brief Создать блок с параметрами вершины
  void CreateVertexBlock(QVBoxLayout* parentLayout);
  /// @brief Создать блок кнопок Apply / Reset / Cancel
  void CreateButtonsBlock(QVBoxLayout* parentLayout);

  /// @brief Универсальный хелпер: создаёт строку «кнопка + квадратик» для
  /// выбора цвета
  void AddColorPickerRow(QVBoxLayout* parentLayout, const QString& buttonText,
                         QPushButton*& outButton, QLabel*& outLabel,
                         void (SettingsDialog::*slot)());

  /// @brief Заполнить все поля значениями из parent_
  void LoadCurrentSettingsFromParent();

  /// @brief Установить стиль (фон + рамку) для QLabel по заданному Color
  void UpdateColorLabel(QLabel* label, const Color& color);
  /// @brief Преобразование строки «r,g,b,a» в структуру Color
  /// @param str Цвет в формате "R,G,B,A"
  /// @return Color{r, g, b, a}
  static Color ParseColorString(const QString& str);

  /// @brief Указатель на родительскиое основное окно
  MainWindow* parent_;

  /// @brief Цвет фона в диалоговом окне настроек
  QColor background_color_;
  /// @brief Цвет рёбер в диалоговом окне настроек
  QColor edge_color_;
  /// @brief Цвет вершин в диалоговом окне настроек
  QColor vertex_color_;

  /// @brief Спинбокс для изменения толщины рёбер
  QDoubleSpinBox* edge_thickness_spinbox_;
  /// @brief Комбобокс для выбора типа рёбер
  QComboBox* edge_type_combobox_;
  /// @brief Комбобокс для выбора типа вершин
  QComboBox* vertex_type_combobox_;
  /// @brief Спинбокс для изменения размера вершин
  QDoubleSpinBox* vertex_size_spinbox_;
  /// @brief Комбобокс для выбора типа проекции
  QComboBox* projection_type_combobox_;

  /// @brief Кнопкa выбора цвета фона
  QPushButton* bg_color_btn_;
  /// @brief Кнопкa выбора цвета рёбер
  QPushButton* edge_color_btn_;
  /// @brief Кнопкa выбора цвета вершин
  QPushButton* vertex_color_btn_;
  /// @brief Кнопкa применения настроек
  QPushButton* reset_btn_;

  /// @brief Маркер текущего цвета фона
  QLabel* bg_color_label_;
  /// @brief Маркер текущего цвета рёбер
  QLabel* edge_color_label_;
  /// @brief Маркер текущего цвета вершин
  QLabel* vertex_color_label_;
};
}  // namespace s21

#endif  // SETTINGS_DIALOG_H_