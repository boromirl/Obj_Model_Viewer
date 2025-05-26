#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GLWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

private:
  // widgets
  QPushButton *translate_button;
  QLabel *translate_label;
  QLineEdit *x_translate_le;
  QLineEdit *y_translate_le;
  QLineEdit *z_translate_le;
  QPushButton *open_file_button;
};

#endif