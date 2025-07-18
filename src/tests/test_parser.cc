#include <gtest/gtest.h>

#include <string>

#include "../utils/obj_parser.h"

TEST(SuiteParser, BasicStar) {
  std::string filepath("tests/test_assets/star.obj");
  s21::ModelData modelData;

  bool res = s21::ObjParser::Parse(filepath, &modelData);

  // файл читается
  ASSERT_TRUE(res);

  // Правильное кол-во вершин и рёбер
  ASSERT_EQ(modelData.vertices.size(), 10);
  ASSERT_EQ(modelData.edges.size(), 20);
}