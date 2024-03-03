#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {
	private:
	int nRows, nCols;
	std::vector<std::vector<char> > mat;

	public:
	Matrix(int NRows, int NCols): nRows(NRows), nCols(NCols),
    mat(NRows, std::vector<char>(NCols, false)) {}

	void setItem(int row, int col, char item) {mat[row][col] = item;}
	char getItem(int row, int col) const {return mat[row][col];}
	int getRows() const {return nRows;}
	int getCols() const {return nCols;}
};
#endif
