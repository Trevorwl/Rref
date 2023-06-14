#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "RowData.h"

RowData::RowData(
		std::vector<std::string>& numbers,
		int W) {

	if((int)numbers.size() == 0){
		throw std::invalid_argument(
				"\nrref::rowData::rowData("
				"std::vector<std::string>&,int)->"
				"empty row");
	}

    if ((int)numbers.size() != W) {
		throw std::invalid_argument(
				"\nrref::rowData::rowData("
				"std::vector<std::string>&,int)->"
				"One row isn't proper length\n");
	}

    this -> W = W;
    data = new double[W];

    for (int i = 0; i < W; i++) {
		data[i] = std::atof(numbers[i].c_str());
	}

    zeroRow = false;

    setRowInfo();
}

RowData::RowData(double* row, int W) {
	if(!(row && W)){
		throw std::invalid_argument(
				"\nrref::rowData::rowData("
				"std::vector<std::string>&,int)->"
				"empty row");
	}

	this -> W = W;
	data = new double[W];
	std::copy(row, row + W, data);
	zeroRow = false;

	setRowInfo();
}

RowData::RowData(const RowData& other) {
	W = other.W;
	data = new double[W];
	std::copy(other.data, other.data + W, data);
	pivotIndex = other.pivotIndex;
	zeroRow = other.zeroRow;
}

RowData::RowData(RowData&& other) noexcept {
	W = other.W;
	data = other.data;
	other.data = nullptr;
	pivotIndex = other.pivotIndex;
	zeroRow = other.zeroRow;
}

RowData::~RowData() {
	if (data) {
		delete[] data;
	}
}

void RowData::setRowInfo() {
	for (int i = 0; i < W; i++) {
		if (data[i] != 0 || i == W - 1) {

			/*
			 * We searched the whole row,
			 * and all the elements were 0.
			 *
			 * PivotIndex is set to -1
			 * if user wishes to evaluate if a
			 * RowData is a zeroRow based on pivot index
			 */
			if (data[i] == 0) {
				zeroRow = true;
				pivotIndex = -1;
			} else {

				/*
				 * nonzero found, we found the pivot
				 */
				pivotIndex = i;
			}

			break;
		}
	}
}

void RowData::print() {
	for(auto& e : *this){
		printf(" %.2f ",e < 1E-3 ? 0 : e);
	}
}

RowData& RowData::operator=(const RowData& other) {
	if (this == &other) {
		return *this;
	}

	W = other.W;

	delete[] data;

	data = new double[W];
	std::copy(other.data, other.data + W, data);

	pivotIndex = other.pivotIndex;
	zeroRow = other.zeroRow;

	return *this;
}

RowData& RowData::operator=(RowData&& other) noexcept {
	W = other.W;

	delete[] data;

	data = other.data;
	other.data = nullptr;
	pivotIndex = other.pivotIndex;
	zeroRow=other.zeroRow;

	return *this;
}

bool RowData::operator>(RowData& that) {
	if ((!(*this) && !that) || !(*this)) {
		return false;
	}

	return !that || (pivotIndex > that.pivotIndex);
}

bool RowData::operator<(RowData& that) {
	if ((!(*this) && !that) || !(that)) {
			return false;
	}

	return !(*this) || (pivotIndex < that.pivotIndex);
}

bool RowData::operator==(RowData& that) {
	if((!(*this) && that) || (*this && !that)){
		return false;
	}

	return (!(*this) && !that)
			|| (pivotIndex == that.pivotIndex);
}

bool RowData::operator!=(RowData& that) {
	return !(*this == that);
}

bool RowData::operator<=(RowData& that){
	return (*this < that) || (*this == that);
}

bool RowData::operator>=(RowData& that){
	return (*this > that) || (*this == that);
}

RowData&  RowData::operator*=(double operand){
	std::transform(data, data + W, data,
			[operand](double e){
				return e * operand;
			});

	return *this;
}

RowData&  RowData::operator+=(RowData& that) {
	for (int i = 0; i < W; i++) {
		data[i] += that[i];
	}

	return *this;
}

/*
 * 1/operand is the same as division
 */
RowData& RowData::operator/=(double operand) {
	return (*this) *= (1 / operand);
}

RowData::operator bool() {
	return !zeroRow;
}

double& RowData::operator[](int i) {
	return data[i];
}

/*
 * r1[index] becomes 0 after addition when it equals -r2[index].
 * We need to find x in r1[index] * x = -r2[index]. Shifting over
 * r1 gives you -r2[index]/r1[index].
 *
 * We then multiply r1 by -r2[index]/r1[index].
 * and add r2 to it.
 */
RowData& RowData::elementaryAdd(RowData& r2, double index){
	return ((*this) *= ((r2[index] * -1) / (*this)[index]))
			+= r2;
}

double* RowData::begin(){
	return data;
}

double* RowData::end(){
	return data + W;
}

