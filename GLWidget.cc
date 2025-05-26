#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget() {}

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void GLWidget::paintGL() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }