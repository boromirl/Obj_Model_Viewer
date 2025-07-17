#include "s21_matrix_oop.h"

void S21Matrix::AllocateMemory() {
  matrix_ = new float *[rows_]();
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new float[cols_]{0};
  }
}

void S21Matrix::DeallocateMemory() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
}

// default constructor
S21Matrix::S21Matrix() {
  rows_ = 3;
  cols_ = 3;
  AllocateMemory();
}

// parameterized constructor
S21Matrix::S21Matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Number of rows and columns must be positive.");
  }
  rows_ = rows;
  cols_ = cols;
  AllocateMemory();
}

// copy constructor
S21Matrix::S21Matrix(const S21Matrix &o) : rows_(o.rows_), cols_(o.cols_) {
  AllocateMemory();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = o.matrix_[i][j];
    }
  }
}

// move constructor
S21Matrix::S21Matrix(S21Matrix &&o) noexcept
    : rows_(o.rows_), cols_(o.cols_), matrix_(o.matrix_) {
  o.rows_ = 0;
  o.cols_ = 0;
  o.matrix_ = nullptr;
}

// init list constructor
S21Matrix::S21Matrix(int rows, int cols,
                     std::initializer_list<float> init_list) {
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Number of rows and columns must be positive.");
  }
  rows_ = rows;
  cols_ = cols;
  AllocateMemory();

  // Check if the size of init_list is correct
  if (init_list.size() != static_cast<size_t>(rows_ * cols_)) {
    throw std::invalid_argument(
        "Initializer list size does not match matrix dimensions.");
  }

  auto it = init_list.begin();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (it != init_list.end()) {
        matrix_[i][j] = *it;
        it++;
      }
    }
  }
}

// destructor
S21Matrix::~S21Matrix() { DeallocateMemory(); }

int S21Matrix::GetRows() { return rows_; }

int S21Matrix::GetCols() { return cols_; }

void S21Matrix::SetRows(int new_rows) {
  if (new_rows < 1) {
    throw std::invalid_argument("Number of rows must be positive.");
  } else if (rows_ != new_rows) {
    // initialize new matrix with different amount of rows
    float **new_matrix = new float *[new_rows];
    for (int i = 0; i < new_rows; i++) {
      new_matrix[i] = new float[cols_]{0};
    }
    // copy data into new matrix
    for (int i = 0; i < std::min(rows_, new_rows); i++) {
      for (int j = 0; j < cols_; j++) {
        new_matrix[i][j] = matrix_[i][j];
      }
    }

    // free old matrix
    DeallocateMemory();

    // set matrix to the new array
    matrix_ = new_matrix;
    rows_ = new_rows;
  }
};

void S21Matrix::SetCols(int new_cols) {
  if (new_cols < 1) {
    throw std::invalid_argument("Number of columns must be positive.");
  } else if (cols_ != new_cols) {
    // initialize new matrix with different amount of colummns
    float **new_matrix = new float *[rows_];
    for (int i = 0; i < rows_; i++) {
      new_matrix[i] = new float[new_cols]{0};
    }
    // copy data into new matrix
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < std::min(cols_, new_cols); j++) {
        new_matrix[i][j] = matrix_[i][j];
      }
    }

    // free old matrix
    DeallocateMemory();

    // set matrix to the new array
    matrix_ = new_matrix;
    cols_ = new_cols;
  }
}

//
void S21Matrix::CopyMatrix(const S21Matrix &other) {
  SetRows(other.rows_);
  SetCols(other.cols_);

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

bool S21Matrix::isSquare() { return (rows_ == cols_); }

// найти алгебраическое дополнение
float S21Matrix::FindCofactor(int row, int col) {
  float result = 0.0;
  if (rows_ == 1) {
    result = 1;
  } else {
    S21Matrix minorMatrix = CreateMinorMatrix(row, col);
    result = minorMatrix.Determinant();

    result *= pow(-1.0, (float)(row + col));
  }
  return result;
}

// создание минорной матрицы путем вычеркивания троки и столбца
S21Matrix S21Matrix::CreateMinorMatrix(int row, int col) {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }

  S21Matrix result(rows_ - 1, cols_ - 1);

  int current_row = 0;
  for (int i = 0; i < result.rows_; i++) {
    int current_col = 0;
    if (current_row == row) current_row++;
    for (int j = 0; j < result.rows_; j++) {
      if (current_col == col) current_col++;
      result.matrix_[i][j] = matrix_[current_row][current_col];
      current_col++;
    }
    current_row++;
  }

  return result;
}

S21Matrix S21Matrix::CalcComplements() {
  if (!isSquare()) {
    throw std::out_of_range("Matrix is not square.");
  }
  S21Matrix result(rows_, cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[i][j] = FindCofactor(i, j);
    }
  }

  return result;
}

float S21Matrix::Determinant() {
  if (!isSquare()) {
    throw std::out_of_range("Matrix is not square.");
  }
  float result = 0;
  if (rows_ == 1) {
    result = matrix_[0][0];
  } else if (rows_ == 2) {
    result = (matrix_[0][0] * matrix_[1][1]) - (matrix_[0][1] * matrix_[1][0]);
  } else if (rows_ > 2) {
    for (int j = 0; j < cols_; j++) {
      // нахождение определителя по первой строке
      // умножение каждого элеменнта строки на его алгебраическое дополнение
      result += (matrix_[0][j] * FindCofactor(0, j));
    }
  }

  return result;
}

bool S21Matrix::EqMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }

  const float epsilon = 1e-12;  // precision

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (std::fabs(matrix_[i][j] - other.matrix_[i][j] > epsilon)) {
        return false;
      }
    }
  }

  return true;
}

S21Matrix S21Matrix::InverseMatrix() {
  float determinant = Determinant();
  if (determinant == 0) {
    throw std::out_of_range(
        "Determinant is equal to 0. Matrix can't be inverted.");
  }
  S21Matrix complementsMatrix = CalcComplements();

  S21Matrix transposedMatrix = complementsMatrix.Transpose();

  transposedMatrix.MulNumber(1.0 / determinant);

  S21Matrix result(rows_, cols_);
  result.CopyMatrix(transposedMatrix);

  return result;
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) {
    throw std::out_of_range(
        "Incorrect input, number of columns of the first matrix should be "
        "equal to the number of rows of the second matrix");
  } else {
    S21Matrix temp(rows_, other.cols_);
    for (int i = 0; i < temp.rows_; i++) {
      for (int j = 0; j < temp.cols_; j++) {
        temp.matrix_[i][j] = MultColByRow(other, i, j);
      }
    }
    CopyMatrix(temp);
  }
}

float S21Matrix::MultColByRow(const S21Matrix &other, int row, int col) {
  float result = 0.0;
  for (int i = 0; i < cols_; i++) {
    result += matrix_[row][i] * other.matrix_[i][col];
  }

  return result;
}

void S21Matrix::MulNumber(const float num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

float &S21Matrix::operator()(int row, int col) {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

const float &S21Matrix::operator()(int row, int col) const {
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

// перегрузка сложения
S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix res(*this);
  res.SumMatrix(other);
  return res;
}

// перегрузка сложения с присваиванием
S21Matrix &S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

// перегрузка вычитания
S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix res(*this);
  res.SubMatrix(other);
  return res;
}

// перегрузка вычитания с присваиванием
S21Matrix &S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

// перегрузка сравнения
bool S21Matrix::operator==(const S21Matrix &other) { return EqMatrix(other); }

// перегрузка умножения матрицы на матрицу
S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix res(*this);
  res.MulMatrix(other);
  return res;
}

// перегрузка умножения матрицы на матрицу с присваиванием
S21Matrix &S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

// перегрузка умножения матрицы на число
S21Matrix S21Matrix::operator*(const float num) {
  S21Matrix res(*this);
  res.MulNumber(num);
  return res;
}

// перегрузка умножения матрицы на число (матрица справа)
S21Matrix operator*(const float num, const S21Matrix &other) {
  S21Matrix res(other);
  res.MulNumber(num);
  return res;
}

// перегрузка умножения матрицы на число присваиванием
S21Matrix &S21Matrix::operator*=(const float num) {
  MulNumber(num);
  return *this;
}

// перегрузка приравнивания
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  CopyMatrix(other);
  return *this;
}

void S21Matrix::SubMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::out_of_range(
        "Incorrect input, matrices should have the same size.");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] -= other.matrix_[i][j];
      }
    }
  }
}

void S21Matrix::SumMatrix(const S21Matrix &other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::out_of_range(
        "Incorrect input, matrices should have the same size.");
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] += other.matrix_[i][j];
      }
    }
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

void S21Matrix::SetIdentity(float num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (i == j)
        (*this)(i, j) = num;
      else
        (*this)(i, j) = 0.0f;
    }
  }
}

std::vector<float> S21Matrix::ToFlatArray() const {
  std::vector<float> result(rows_ * cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result[i * cols_ + j] = matrix_[i][j];
    }
  }
  return result;
}