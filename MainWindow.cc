#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
  setCentralWidget(splitter);

  // left panel(controls)
  QWidget *leftPanel = new QWidget();
  QGridLayout *leftLayout = new QGridLayout(leftPanel);

  translate_label = new QLabel("Translate");
  translate_button = new QPushButton("TRANSLATE");
  x_translate_le = new QLineEdit();
  y_translate_le = new QLineEdit();
  z_translate_le = new QLineEdit();
  open_file_button = new QPushButton("OPEN FILE");

  leftLayout->addWidget(translate_label, 0, 0, 2, 2, Qt::AlignCenter);
  leftLayout->addWidget(new QLabel("x:"), 2, 0);
  leftLayout->addWidget(x_translate_le, 2, 1);
  leftLayout->addWidget(new QLabel("y:"), 3, 0);
  leftLayout->addWidget(y_translate_le, 3, 1);
  leftLayout->addWidget(new QLabel("z:"), 4, 0);
  leftLayout->addWidget(z_translate_le, 4, 1);
  leftLayout->addWidget(translate_button, 5, 0, 1, 2);
  leftLayout->addWidget(open_file_button, 6, 0, 1, 2);
  leftLayout->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 0,
      1, 2);

  // right panel(view)
  GLWidget *glWidget = new GLWidget();

  // splitter
  splitter->addWidget(leftPanel);
  splitter->addWidget(glWidget);
}

MainWindow::~MainWindow() {}