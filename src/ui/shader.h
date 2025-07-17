#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader : protected QOpenGLFunctions_3_3_Core {
 public:
  // shader program ID
  unsigned int ID;

  Shader() = default;
  void init(const char *vertexPath, const char *fragmentPath);

  // активация шейдера
  void use();

  // Вспомогательные функции для установления значений униформов
  void setBool(const std::string &name, bool value);
  void setInt(const std::string &name, int value);
  void setFloat(const std::string &name, float value);
  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setVec2(const std::string &name, float x, float y);
};

#endif