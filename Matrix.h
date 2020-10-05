#ifndef PARALLELPROGRAM2_MATRIX_H
#define PARALLELPROGRAM2_MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <type_traits>

#include <omp.h>

#include "Timer.h"


template <class T, size_t Dim, class = std::enable_if_t<std::is_arithmetic_v<T>>>
class Matrix {
public:
    Matrix() {
        Data = new T[Size];
    }

    Matrix(T* data) {
        Data = data;
    }

    Matrix(Matrix<T, Dim>& m) {
        Data = new T[Size];
        std::copy(m.Data, m.Data + Size, Data);
    }

    ~Matrix() {
        delete [] Data;
        deleteMatT();
    }

    constexpr size_t getDim() {
        return Dim;
    }

    constexpr size_t getSize() {
        return Size;
    }

    void zero() {
        fill(0);
    }

    void fill(T value) {
        std::fill(Data, Data + Size, value);
    }

    void randFill() {
        for(size_t i = 0; i < Size; ++i)
            Data[i] = std::rand() & 0xfff;
    }

    void print() const {
        for(size_t i = 0; i < Dim; ++i) {
            for(size_t j = 0; j < Dim; ++j) {
                printf("%2d ", operator()(i, j));
            }
            printf("\n");
        }
    }

    const Matrix<T, Dim>& transpose() {
        if(MatT == nullptr) {
#if RunTimers == 1
            Timer t("transpose");
#endif
            T *DataT = new T[Size];
            transposeData(Data, DataT);

            MatT = new Matrix<T, Dim>(DataT);
        }
        return *MatT;
    }

    // Умножение с вертикальным разложением (на столбцы)
    static void multiplyV(Matrix<T, Dim>& left, Matrix<T, Dim>& right, Matrix<T, Dim>& result) {
        multiplyDataVertical(left.Data, right.transpose().Data, result.Data);
    }


    // Умножение с горизонтальным разложением (на строки)
    static void multiplyH(const Matrix<T, Dim>& left, const Matrix<T, Dim>& right, Matrix<T, Dim>& result) {
        multiplyDataHorizontal(left.Data, right.Data, result.Data);
    }

    // Умножение с вертикальным разложением (на столбцы) в многопоточном режиме
    static void multiplyVMT(Matrix<T, Dim>& left, Matrix<T, Dim>& right, Matrix<T, Dim>& result) {
        multiplyDataVerticalMT(left.Data, right.transpose().Data, result.Data);
    }

    // Умножение с горизонтальным разложением (на строки) в многопоточном режиме
    static void multiplyHMT(const Matrix<T, Dim>& left, const Matrix<T, Dim>& right, Matrix<T, Dim>& result) {
        multiplyDataHorizontalMT(left.Data, right.Data, result.Data);
    }

    const Matrix<T, Dim>& operator*=(const Matrix<T, Dim>& other) {
        T* newData = new T[Size];
        std::fill(newData, newData + Size, 0);

//        multiplyDataVerticalMT(Data, other.transpose().Data, newData);
        multiplyDataHorizontalMT(Data, other.Data, newData);

        deleteMatT();
        delete[] Data;
        Data = newData;
        return *this;
    }

    Matrix<T, Dim> operator*(const Matrix<T, Dim>& right) const {
        T* newData = new T[Size];
        std::fill(newData, newData + Size, 0);

        multiplyDataHorizontalMT(Data, right.Data, newData);

        return Matrix<T, Dim>(newData);
    }

    T& operator()(const size_t index) const {
        return Data[index];
    }

    T& operator()(const size_t row, const size_t column) const {
        return Data[row * Dim + column];
    }

    void deleteMatT() {
        if (MatT) {
            delete MatT;
            MatT = nullptr;
        }
    }
private:
    T* Data = nullptr;
    static constexpr size_t Size = Dim*Dim;
    Matrix<T, Dim>* MatT = nullptr;

    static void transposeData(const T* data, T* result) {
        for(size_t i = 0; i < Dim; ++i) {
            for(size_t j = 0; j < Dim; ++j) {
                result[j*Dim + i] = data[i*Dim + j];
            }
        }
    }

    static void multiplyDataVertical(const T* left, const T* rightT, T* result) {
        // i - row, j - column
        for(size_t i = 0; i < Dim; ++i) {
            for (size_t j = 0; j < Dim; ++j) {
                T sum = 0;
                for (size_t k = 0; k < Dim; ++k) {
                    sum += left[i * Dim + k] * rightT[j * Dim + k];
                }
                result[i * Dim + j] = sum;
            }
        }
    }

    static void multiplyDataHorizontal(const T* left, const T* right, T* result) {
        for(size_t a = 0; a < Dim; ++a) {
            for (size_t b = 0; b < Dim; ++b) {
                for (size_t c = 0; c < Dim; ++c) {
                    result[a * Dim + c] += left[a * Dim + b] * right[b * Dim + c];
                }
            }
        }
    }

    static void multiplyDataVerticalMT(const T* left, const T* rightT, T* result) {
#pragma omp parallel for
        // i - row, j - column
        for(size_t i = 0; i < Dim; ++i) {
            for (size_t j = 0; j < Dim; ++j) {
                T sum = 0;
                for (size_t k = 0; k < Dim; ++k) {
                    sum += left[i * Dim + k] * rightT[j * Dim + k];
                }
                result[i * Dim + j] = sum;
            }
        }
    }

    static void multiplyDataHorizontalMT(const T* left, const T* right, T* result) {
#pragma omp parallel for
        for(size_t a = 0; a < Dim; ++a) {
            for (size_t b = 0; b < Dim; ++b) {
                for (size_t c = 0; c < Dim; ++c) {
                    result[a * Dim + c] += left[a * Dim + b] * right[b * Dim + c];
                }
            }
        }
    }
};


#endif //PARALLELPROGRAM2_MATRIX_NOPE_H
