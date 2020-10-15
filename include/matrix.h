#ifndef APP_MATRIX_H
#define APP_MATRIX_H

#include <vector>
#include <stdexcept>

template<typename T>
class Matrix {
public:
    struct Row {
        Row(const Row &) = default;

        Row &operator=(const Row &) = default;

        Row(Row &&) = default;

        Row &operator=(Row &&) = default;

        const Matrix &matrix;
        size_t num = 0;

        T &operator[](size_t index) {
            auto &ref = const_cast<Matrix &>(matrix);
            return ref.data_[matrix.cols_ * num + index];
        }

        const T &operator[](size_t index) const {
            return matrix.data_[matrix.cols_ * num + index];
        }

        size_t size() const {
            return matrix.cols_;
        }
    };

    friend Row;

    Row operator[](size_t index) {
        return Row{*this, index};
    };

    Row operator[](size_t index) const {
        return Row{*this, index};
    };

    Matrix(size_t x, size_t y) : cols_(y), rows_(x) {
        data_ = std::vector<T>(cols_ * rows_, T{});
        data_.resize(cols_ * rows_, T{});
    }

    Matrix(std::initializer_list<std::initializer_list<T>> ls) {
        if (ls.size() == 0 || ls.begin()->size() == 0) {
            throw std::invalid_argument("can't be empty");
        }

        rows_ = ls.size();
        cols_ = ls.begin()->size();

        for (auto it : ls) {
            if (it.size() != cols_) {
                throw std::invalid_argument("wrong dimension");
            }
            std::copy(it.begin(), it.end(), std::back_inserter(data_));
        }
    }

    Matrix() = delete;

    size_t size() const {
        return rows_;
    }

private:
    std::vector<T> data_;
    size_t cols_;
    size_t rows_;
};

#endif //APP_MATRIX_H
