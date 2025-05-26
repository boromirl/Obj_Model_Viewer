#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.setWindowTitle("Object Viewer");
  mainWindow.setFixedSize(1500, 800);
  mainWindow.show();

  return app.exec();
}