#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <iostream>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
};

#endif