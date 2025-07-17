#ifndef MODEL_DATA_H_
#define MODEL_DATA_H_

#include <array>
#include <vector>

#include "../S21Matrix/s21_matrix_oop.h"

namespace s21 {

/// @brief Структура с координатами вершины модели
/// @param x Координата x
/// @param y Координата y
/// @param z Координата z
struct Vertex {
  float x;
  float y;
  float z;
};

/// @brief Ребро модели, образованное двумя вершинами
using Edge = std::pair<std::size_t, std::size_t>;

/// @brief Структуры с данными о модели
struct ModelData {
  /// @brief Вершины модели
  std::vector<Vertex> vertices;
  /// @brief Рёбра модели
  std::vector<Edge> edges;
  /// @brief Матрица трансформации модели
  S21Matrix tm;
};

}  // namespace s21

#endif  // MODEL_DATA_H_