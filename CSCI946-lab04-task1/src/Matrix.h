/*
 * Matrix.h
 *
 *  Created on: 2016Äê8ÔÂ31ÈÕ
 *      Author: Administrator
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <typeinfo>

template<class T> class Matrix {
	friend std::istream& operator>>(std::istream &in, Matrix<T> &obj) {
		T item;
		//release
		if (obj.m_data != NULL) {
			for (int i = 0; i < obj.getRows(); i++) {
				delete[] obj.m_data[i];
			}
			delete[] obj.m_data;
		}
		//new
		obj.m_data = new T *[obj.getRows()];
		for (int i = 0; i < obj.getRows(); i++) {
			obj.m_data[i] = new T[obj.getCols()];
		}
		//input
		std::cout << "	Please input data for the ";
		for (int r = 0; r < obj.getRows(); r++)
			for (int c = 0; c < obj.getCols(); c++) {
				in >> item;
				obj(r, c) = item;
			}
		return in;
	}

	friend std::ostream& operator<<(std::ostream &out, const Matrix<T> &obj) {
		out << std::fixed << std::setprecision(3) << std::setiosflags(std::ios::right) << std::setfill(' ');
		for (int r = 0; r < obj.rows; r++) {
			for (int c = 0; c < obj.cols; c++) {
				out << std::setw(8);	//?? is it enough
				out << obj(r, c) << " ";
			}
			out << std::endl;
		}
		return out;
	}

private:
	T **m_data;
	int rows;
	int cols;
public:
	Matrix() {
		rows = 0;
		cols = 0;
		m_data = NULL;
	}

	Matrix(int r, int c) {
		rows = r;
		cols = c;
		// allocate memory for m_data[][]
		if (r > 0 && c > 0) {
			m_data = new T *[this->getRows()];
			for (int i = 0; i < this->getRows(); i++) {
				m_data[i] = new T[this->getCols()];
			}

			for (int r = 0; r < this->getRows(); r++) {
				for (int c = 0; c < this->getCols(); c++) {
					this->m_data[r][c] = 0;
				}
			}
		} else {
			m_data = NULL;
		}
	}

	~Matrix() {
		// deallocate memory
		if (m_data != NULL) {
			for (int i = 0; i < this->getRows(); i++) {
				delete[] m_data[i];
			}
			delete[] m_data;
		}
	}

	int getRows() const {
		return this->rows;
	}
	int getCols() const {
		return this->cols;
	}
	void setRows(int rows) {
		this->rows = rows;
	}
	void setCols(int columns) {
		this->cols = columns;
	}

	// get & set item(r,c)
	T& operator()(const int r, const int c) {
		return this->m_data[r][c];
	}
	const T& operator()(const int r, const int c) const {
		return this->m_data[r][c];
	}
	// deep copy
	Matrix<T>& operator=(const Matrix<T> &other) {
		if (this == &other) {
			return *this;
		} else {
			// release memory
			for (int i = 0; i < this->getRows(); i++) {
				delete[] this->m_data[i];
			}
			delete[] this->m_data;
			// allocate memory
			this->setRows(other.getRows());
			this->setCols(other.getCols());
			this->m_data = new T *[rows];
			for (int i = 0; i < this->getRows(); i++) {
				this->m_data[i] = new T[this->getCols()];
			}
			// copy data
			for (int r = 0; r < this->getRows(); r++) {
				for (int c = 0; c < this->getCols(); c++) {
					this->m_data[r][c] = other.m_data[r][c];
				}
			}
			return *this;
		}
	}
	;

	Matrix<T>& operator+(const double &param) {
		for (int r = 0; r < this->getRows(); r++) {
			for (int c = 0; c < this->getCols(); c++) {
				this->m_data[r][c] = this->m_data[r][c] + param;
			}
		}
		return *this;
	}

	Matrix<T>& operator*(const double &param) {
		for (int r = 0; r < this->getRows(); r++) {
			for (int c = 0; c < this->getCols(); c++) {
				this->m_data[r][c] = this->m_data[r][c] * param;
			}
		}
		return *this;
	}

	Matrix<T> operator*(const Matrix<T> &other) {
		if (this->getCols() != other.getRows())
			throw 2;
		Matrix<T> result(this->getRows(), other.getCols());
		for (int r = 0; r < this->getRows(); r++)
			for (int c = 0; c < other.getCols(); c++) {
				result.m_data[r][c] = 0;
				for (int i = 0; i < this->getCols(); i++) {
					result.m_data[r][c] = result.m_data[r][c] + this->m_data[r][i] * other.m_data[i][c];
				}
			}
		return result;
	}

	bool operator==(const Matrix<T> &other) {
		if (this->getRows() != other.getRows() || this->getCols() != other.getCols())
			throw 2;
		double a, b;
		for (int r = 0; r < this->getRows(); ++r) {
			for (int c = 0; c < this->getCols(); ++c) {
				a = ((double) ((int) ((this->m_data[r][c] + 0.0005) * 1000))) / 1000;
				b = ((double) ((int) ((other.m_data[r][c] + 0.0005) * 1000))) / 1000;
				if (a != b)
					return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix<T> &other) {
		if (this->getRows() != other.getRows() || this->getCols() != other.getCols())
			throw 2;
		double a, b;
		for (int r = 0; r < this->getRows(); ++r) {
			for (int c = 0; c < this->getCols(); ++c) {
				a = ((double) ((int) ((this->m_data[r][c] + 0.0005) * 1000))) / 1000;
				b = ((double) ((int) ((other.m_data[r][c] + 0.0005) * 1000))) / 1000;
				if (a != b)
					return true;
			}
		}
		return false;
	}

	Matrix<T> transpose() {
		Matrix<T> result(this->getCols(), this->getRows());
		for (int r = 0; r < this->getRows(); r++)
			for (int c = 0; c < this->getCols(); c++) {
				result.m_data[c][r] = this->m_data[r][c];
			}
		return result;
	}

	void setData(T *data, int size) {
		if (this->getRows() * this->getCols() != size)
			throw 3;
		int idx = 0;
		for (int r = 0; r < this->getRows(); ++r) {
			for (int c = 0; c < this->getCols(); ++c) {
				this->m_data[r][c] = data[idx];
				idx++;
			}
		}
	}

	void norm() {
		if (this->getCols() != 1 && this->getRows() != 1)
			throw 4;
		T tmp = 0;
		for (int r = 0; r < this->getRows(); r++)
			for (int c = 0; c < this->getCols(); c++) {
				//tmp = pow(this->m_data[r][c], 2) + tmp;
				tmp = this->m_data[r][c] + tmp;
			}
		//tmp = sqrt(tmp);
		for (int r = 0; r < this->getRows(); r++)
			for (int c = 0; c < this->getCols(); c++) {
				this->m_data[r][c] = this->m_data[r][c] / tmp;
			}
	}
};

#endif /* MATRIX_H_ */
