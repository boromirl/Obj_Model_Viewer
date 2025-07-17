#include "obj_parser.h"

#include <fstream>
// #include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

bool ObjParser::Parse(const std::string &filepath, ModelData *out_model) {
  out_model->vertices.clear();
  out_model->edges.clear();

  std::ifstream file(filepath);
  if (!file.is_open()) {
    return false;
  }

  std::string line;
  bool has_vertex = false;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string tag;
    iss >> tag;

    if (tag == "v") {
      // строка вершины: v x y z
      float x, y, z;
      if (iss >> x >> y >> z) {
        out_model->vertices.push_back({x, y, z});
        has_vertex = true;
      }
    } else if (tag == "f") {
      // строка грани: f i1[/..] i2[/..] i3[/..] ...
      ParseFaceLine(line, out_model);
    }
    // прочие теги игнорируем
  }

  // делаем рёбра уникальными
  std::sort(out_model->edges.begin(), out_model->edges.end());
  auto it = std::unique(out_model->edges.begin(), out_model->edges.end());
  out_model->edges.resize(std::distance(out_model->edges.begin(), it));

  file.close();

  return has_vertex;
}

void ObjParser::ParseFaceLine(const std::string &line, ModelData *out_model) {
  std::istringstream iss(line);
  std::string tag;
  iss >> tag;  // считываем и пропускаем саму букву "f"

  std::vector<std::size_t> indices;
  std::string token;
  while (iss >> token) {
    // берем часть до первого '/'
    auto slash_pos = token.find('/');
    std::string idx_str =
        (slash_pos == std::string::npos) ? token : token.substr(0, slash_pos);
    int idx = std::stoi(idx_str);
    // поддержка отрицательных индексов
    if (idx < 0) {
      idx = static_cast<int>(out_model->vertices.size()) + idx + 1;
    }
    // переводим в 0-based
    indices.push_back(static_cast<std::size_t>(idx - 1));
  }

  // строим рёбра: между каждым соседом
  for (std::size_t i = 0; i + 1 < indices.size(); ++i) {
    // вершины всегда хранятся парами (меньшая, большая) для проверки
    // уникальности
    out_model->edges.emplace_back(std::min(indices[i], indices[i + 1]),
                                  std::max(indices[i], indices[i + 1]));
  }
  // если грань замкнутая (больше 2 точек) — замыкаем
  if (indices.size() > 2) {
    out_model->edges.emplace_back(std::min(indices.back(), indices.front()),
                                  std::max(indices.back(), indices.front()));
  }
}

}  // namespace s21
