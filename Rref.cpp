#include <algorithm>
#include <cstdio>
#include <fstream>
#include <regex>
#include <stdexcept>

#include "Rref.h"

Rref::Rref(std::string url) {
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit
			| std::ifstream::badbit);

	try {
		ifs.open(url);

		std::string buf;
		std::regex r("[ ]+");

		W  = -1;

		while (!ifs.eof()) {
			std::getline(ifs, buf);

			std::vector<std::string> tokens{
				std::sregex_token_iterator{
				buf.begin(), buf.end(), r, -1}, {}};

			if (W == -1) {
				W = tokens.size();
			}

			rows.push_back(RowData{tokens, W});
		}

		ifs.close();

		if ((H = rows.size()) == 0) {
			throw std::ifstream::failure("No data");
		}

		setRowInfo();
		solve();

	} catch(std::ifstream::failure& e) {
		if (ifs.is_open()) {
			ifs.close();
		}

		char location[128] = "\nrref::rref(std::string)->";
		char msg[256];
		sprintf(msg, "%s%s", location, e.what());

		throw std::ifstream::failure(msg);
	}
}

Rref::Rref(double** matrix, int W, int H) {
	if (!matrix) {
		throw std::invalid_argument(
				"\nrref::rref(double**, int, int)->"
				"null matrix\n");
	}

	if (W <= 0 || H <= 0) {
		throw std::invalid_argument(
		        "\nrref::rref(double**, int, int)->"
				"invalid size\n");
	}

	this -> W = W;
	this -> H = H;

	for (int i = 0; i < H; i++) {
		rows.push_back(
				RowData(matrix[i], W));
	}

	setRowInfo();
    solve();
}

Rref::Rref(const Rref& other) {
	W = other.W;
	H = other.H;
	rows = other.rows;
	firstNonZeroRow = other.firstNonZeroRow;
	zeroMatrix = other.zeroMatrix;
}

Rref::Rref(Rref&& other) noexcept{
	W = other.W;
	H = other.H;
	rows = std::move(other.rows);
	firstNonZeroRow = other.firstNonZeroRow;
	zeroMatrix = other.zeroMatrix;
}

Rref::~Rref() {}

Rref& Rref::operator=(const Rref& other) {
	if(this == &other){
		return *this;
	}

	W = other.W;
	H = other.H;
	rows = other.rows;
	firstNonZeroRow = other.firstNonZeroRow;
	zeroMatrix = other.zeroMatrix;

	return *this;
}

Rref& Rref::operator=(Rref&& other) noexcept{
	W = other.W;
	H = other.H;
	rows = std::move(other.rows);
	firstNonZeroRow = other.firstNonZeroRow;
	zeroMatrix = other.zeroMatrix;

	return *this;
}

double** Rref::getMatrix() {
	double** data = new double*[H];

	for (int i = 0; i < H; i++) {
		double* copy = new double[W];
		std::copy(rows[i].data, rows[i].data + W, copy);
		data[i] = copy;
	}

	return data;
}

void Rref::printMatrix() {
	for (auto& e : rows) {
		e.print();
		printf("\n\n");
	}
}

bool Rref::isRef() {
	int prevPivot = -1;

	for (int i = firstNonZeroRow; i < H; i++) {
		if(i == firstNonZeroRow){
			prevPivot = rows[i].pivotIndex;
			continue;
		}

		/*
		   In ref, the pivot of the previous row
		   needs to be of a smaller index than the
		   current row
		 */
		if ((rows[i].pivotIndex <= prevPivot)) {
			return false;
		}

		prevPivot = rows[i].pivotIndex;
	}

	return true;
}

void Rref::solve() {
	while (true) {
		if (zeroMatrix) {
			break;
		}

		if (!isRef()) {
			doAnRefPass();
		} else {
			toRref();
			break;
		}

		/*
		   Update each RowInfo in Rref::rows
		   to allow for processing in
		   future calls to isRef(), doAnRefPass, and
		   toRref()
		 */
		setRowInfo();
	}
}

void Rref::setRowInfo() {
	for (int i = firstNonZeroRow; i < H; i++) {
		rows[i].setRowInfo();
	}

	/*
	   Moves all zero rows to the top
	   of the matrix Rref::rows so we can
	   ignore them.

	   Puts matrix in hair-step form,
	   or attempts to do so.
	 */
	std::sort(rows.begin(), rows.end());


	int i = 0;
	for ( ; i < H; i++) {
		if (rows[i]) {
			firstNonZeroRow = i;
			break;
		}
	}

	zeroMatrix = (i == H);
}


void Rref::doAnRefPass() {
	for (int i = H - 1; i > firstNonZeroRow; i--) {
		int pivot1 = rows[i].pivotIndex;

		/*
		   We only want to do a row operation
		   if pivot of previous row is in
		   same place as current row
		 */
		if (pivot1 == rows[i-1].pivotIndex) {

			/*
			   row addition. see RowData::elementaryAdd
			 */
			rows[i].elementaryAdd(rows[i - 1], pivot1);
		}
	}
}

void Rref::toRref() {
	for (int i = firstNonZeroRow; i < H; i++) {
		int index = rows[i].pivotIndex;

		/*
		   Check the pivot column
		   to see if a number
		   other than zero is present.
		   Perform row addition if
		   necessary.
		 */
		for (int j = firstNonZeroRow; j < i; j++) {

			if (rows[j][index] != 0) {
			    rows[j].elementaryAdd(rows[i], index);
			}
		}
	}

	/*
	   Make pivot of each row 1. We have now
	   solved the matrix.
	 */
	for (int i = firstNonZeroRow; i < H; i++) {
		rows[i] /= rows[i][rows[i].pivotIndex];
	}
}
