#include <gtest/gtest.h>

#include "../engine/model_engine.h"

bool compareWithPrecision(S21Matrix& mat1, S21Matrix& mat2,
                          float precision = 1e-5f) {
  for (int i = 0; i < mat1.GetRows(); i++) {
    for (int j = 0; j < mat1.GetCols(); j++) {
      if (abs(mat1(i, j) - mat2(i, j)) > precision) return false;
    }
  }

  return true;
}

TEST(SuiteTransformations, FileLoading1) {
  s21::ModelEngine me;

  ASSERT_FALSE(me.HasModel());

  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  ASSERT_TRUE(me.HasModel());

  ASSERT_EQ(me.GetFilename(), "star.obj");
  ASSERT_EQ(me.GetVertexCount(), 10);
  ASSERT_EQ(me.GetEdgeCount(), 20);
}

TEST(SuiteTransformations, FileLoading2) {
  s21::ModelEngine me;

  ASSERT_FALSE(me.HasModel());

  std::string filename = "tests/test_assets/lowpoly_tree.obj";
  me.LoadModelFromFile(filename);

  ASSERT_TRUE(me.HasModel());

  ASSERT_EQ(me.GetFilename(), "lowpoly_tree.obj");
  ASSERT_EQ(me.GetVertexCount(), 281);
  ASSERT_EQ(me.GetEdgeCount(), 581);
}

TEST(SuiteTransformations, FileLoading3) {
  s21::ModelEngine me;

  ASSERT_FALSE(me.HasModel());

  std::string filename = "tests/test_assets/empty.obj";
  me.LoadModelFromFile(filename);

  ASSERT_FALSE(me.HasModel());
}

TEST(SuiteTransformations, ClearModel) {
  s21::ModelEngine me;

  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  ASSERT_TRUE(me.HasModel());

  S21Matrix identity(4, 4);
  identity.SetIdentity(1.0f);

  s21::ModelData md = me.GetModelData();
  ASSERT_FALSE(identity == md.tm);

  me.ClearModel();
  md = me.GetModelData();

  ASSERT_TRUE(identity == md.tm);
}

TEST(SuiteTransformations, Translate1) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(4, 4,
                   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0.1, 0.1, 0.1, 1});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Translate(1, 1, 1);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(md.tm == testMatr);
}

TEST(SuiteTransformations, Translate2) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(4, 4, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -20, 4.2, 3, 1});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Translate(-200, 42, 30);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(md.tm == testMatr);
}

TEST(SuiteTransformations, Rotate1) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(4, 4,
                   {0.999695f, 0.0177543f, -0.0171425f, 0.0f, -0.0174497f,
                    0.99969f, 0.0177543f, 0.0f, 0.0174524f, -0.0174497f,
                    0.999695f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Rotate(1, 1, 1);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(compareWithPrecision(testMatr, md.tm));
}

TEST(SuiteTransformations, Rotate2) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(
      4, 4,
      {0.888302, -0.38207f, -0.254836f, 0.0f, 0.377062f, 0.923519f, -0.0702579f,
       0.0f, 0.262189f, -0.0336786f, 0.964429f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Rotate(2, 15.2, -23);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(compareWithPrecision(testMatr, md.tm));
}

TEST(SuiteTransformations, Scale1) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(4, 4,
                   {2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                    2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Scale(2);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(compareWithPrecision(testMatr, md.tm));
}

TEST(SuiteTransformations, Scale2) {
  // Создаем объект и загружаем файл
  s21::ModelEngine me;
  std::string filename = "tests/test_assets/star.obj";
  me.LoadModelFromFile(filename);

  // матрица, после нормализации координат
  S21Matrix fitMatr = me.GetModelData().tm;

  // Правильная матрица трансформации
  S21Matrix trMatr(4, 4,
                   {-20.3f, 0.0f, 0.0f, 0.0f, 0.0f, -20.3f, 0.0f, 0.0f, 0.0f,
                    0.0f, -20.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});

  // Создаем правильную матрицу, которая должна получиться в результате
  // трансформации
  S21Matrix testMatr(4, 4);
  testMatr = fitMatr * trMatr;

  me.Scale(-20.3);
  s21::ModelData md = me.GetModelData();

  ASSERT_TRUE(compareWithPrecision(testMatr, md.tm));
}