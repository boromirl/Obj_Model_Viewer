#ifndef OBJ_PARSER_H_
#define OBJ_PARSER_H_

#include <algorithm>
#include <string>

#include "../engine/model_data.h"

namespace s21 {

/// @brief Парсер файлов .obj (только вершины и поверхности)
class ObjParser {
 public:
  ObjParser() = delete;  // статический утилитарный класс

  /// @brief Разбирает .obj-файл и заполняет данные модели
  /// @param filepath Путь к obj-файлу
  /// @param out_model Ссылка на структуру модели для заполнения
  /// @return true при успешном парсинге хотя бы одной вершины, иначе false
  static bool Parse(const std::string &filepath, ModelData *out_model);

 private:
  /// @brief Вспомогательный: обрабатывает строку "f ..." и добавляет рёбра
  /// @param line Строка "f ..."
  /// @param out_model Ссылка на структуру модели для заполнения
  static void ParseFaceLine(const std::string &line, ModelData *out_model);
};

}  // namespace s21

#endif  // OBJ_PARSER_H_
