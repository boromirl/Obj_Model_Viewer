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

TEST(SuiteParser, WrongFilePath) {
  std::string filepath("wrong/filepath");
  s21::ModelData modelData;

  bool res = s21::ObjParser::Parse(filepath, &modelData);
  ASSERT_FALSE(res);
}

TEST(SuiteParser, EmptyFile) {
  std::string filepath("tests/test_assets/empty.obj");
  s21::ModelData modelData;

  bool res = s21::ObjParser::Parse(filepath, &modelData);

  ASSERT_FALSE(res);
  ASSERT_EQ(modelData.vertices.size(), 0);
  ASSERT_EQ(modelData.edges.size(), 0);
}

TEST(SuiteParser, NegativeIndices) {
  std::string filepath("tests/test_assets/negative_star.obj");
  s21::ModelData modelData;

  bool res = s21::ObjParser::Parse(filepath, &modelData);

  // файл читается
  ASSERT_TRUE(res);

  // Правильное кол-во вершин и рёбер
  ASSERT_EQ(modelData.vertices.size(), 10);
  ASSERT_EQ(modelData.edges.size(), 20);
}

TEST(SuiteParser, BiggerFile) {
  std::string filepath("tests/test_assets/lowpoly_tree.obj");
  s21::ModelData modelData;

  bool res = s21::ObjParser::Parse(filepath, &modelData);

  // файл читается
  ASSERT_TRUE(res);

  // Правильное кол-во вершин и рёбер
  ASSERT_EQ(modelData.vertices.size(), 281);
  ASSERT_EQ(modelData.edges.size(), 581);
}