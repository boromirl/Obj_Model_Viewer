#ifndef __S21MATRIX_H__
#define __S21MATRIX_H__

#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

class S21Matrix {
 private:
  int rows_, cols_;
  float **matrix_;

 public:
  S21Matrix();                        // default constructor
  S21Matrix(int rows, int cols);      // parameterized constructor
  S21Matrix(const S21Matrix &o);      // copy constructor
  S21Matrix(S21Matrix &&o) noexcept;  // move constructor
  S21Matrix(int rows, int cols,
            std::initializer_list<float> init_list);  // init_list constructor
  ~S21Matrix();                                       // destructor

  void SetIdentity(float num);

  // operator overloads
  S21Matrix &operator=(const S21Matrix &other);
  bool operator==(const S21Matrix &other) const;
  float &operator()(int row, int col);
  const float &operator()(int row, int col) const;

  S21Matrix operator+(const S21Matrix &other);
  S21Matrix operator-(const S21Matrix &other);
  S21Matrix operator*(const S21Matrix &other);
  S21Matrix operator*(const float num);
  friend S21Matrix operator*(const float num, const S21Matrix &other);

  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix &operator*=(const S21Matrix &other);
  S21Matrix &operator*=(const float num);

  // methods
  bool EqMatrix(const S21Matrix &other) const;
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const float num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  float Determinant();
  S21Matrix InverseMatrix();

  // accessors
  int GetRows();
  int GetCols();
  // mutators
  void SetRows(int new_rows);
  void SetCols(int new_cols);

  bool isSquare();
  float FindCofactor(int row, int col);
  S21Matrix CreateMinorMatrix(int row, int col);

  float MultColByRow(const S21Matrix &other, int row, int col);
  void CopyMatrix(const S21Matrix &other);  // full copy of matrix with changes
                                            // in rows_ and cols_

  std::vector<float> ToFlatArray() const;

  void AllocateMemory();
  void DeallocateMemory();
};

#endif