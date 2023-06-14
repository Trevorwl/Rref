#include <string>
#include <vector>

#ifndef ROWDATA_H_
#define ROWDATA_H_

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * RowData.h                                                                                    *
 *                                                                                              *
 * Copyright 2023 Trevor Lash                                                                   *
 *                                                                                              *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of              *
 * this software and associated documentation files (the “Software”), to deal in the            *
 * Software without restriction, including without limitation the rights to use, copy,          *
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,          *
 * and to permit persons to whom the Software is furnished to do so, subject to the             *
 * following conditions: The above copyright notice and this permission notice shall be         *
 * included in all copies or substantial portions of the Software.THE SOFTWARE IS PROVIDED      *
 * “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED         *
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR      *
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
   This class represents a matrix row.

   A typical implementation would be
   to create a std::vector<rowData>,
   which would represent a matrix.

   After performing elementaryAdd()
   on a row, it is recommended to call
   setRowInfo() to make sure pivotIndex
   and zeroRow are updated.

   RowData objects can be sorted
   in natural order when in a data
   structure and std::sort is called.

          -Zero rows will be considered "smaller" than other rows,
   	      which allows them to be pushed to the beginning
          of a matrix

   	      -nonZero rows are compared based on their pivotIndex,
   	  	  and a row with a smaller pivotIndex will be moved towards
   	  	  the beginning of a matrix. This is useful for
   	  	  programs that need ref matrices.

   It one needs to perform rref on matrix,
   it is recommended not to use this class
   directly and instead use class Rref.

   Author: Trevor Lash

   Revised: 6/11/2023
 */
class RowData{

	public:

		/*
		   Width of matrix row
		 */
		int W;

		/*
		   Numbers in matrix row
		 */
		double* data;

		/*
		   Index of row's pivot

		   -1 if zeroRow = true
		   W-1 if only non-zero is data[W-1]
		 */
		int pivotIndex;

		/*
		   True if this row has
		   all zeros
		 */
		bool zeroRow;

		/*
		   Constructs a matrix row using a
		   tokenized string

		   Parameters:

		   numbers-> tokenized string
		   W-> expected length of row

		   Throws:

		   std::invalid argument->
		   row isn't of length W,
		   or W = 0
		 */
		RowData(std::vector<std::string>& numbers,
			   int W);

		/*
		   Constructs a matrix row.

		   Parameters:

		   row-> matrix row
		   W-> width of row. Undefined
		   behavior if this value is incorrect.
		 */
		RowData(double* row, int W);

		/*
		   Also makes a copy of double* data
		 */
		RowData(const RowData& other);

		/*
		   Moves double* data
		 */
		RowData(RowData&& other) noexcept;

		/*
		   Deletes double* data
		   if data!=nullptr
		 */
		~RowData();

		/*
		   sets pivotIndex and zeroRow.
		   Called in constructor
		   so you don't have to until
		   the row is modified.
		   It is recommended you call this after
		   calling elementaryAdd(RowData&,double)
		 */
		void setRowInfo();

		/*
		   Prints row
		 */
		void print();

		RowData& operator=(const RowData& other);

		/*
		   moves double* data instead of copying it
		 */
		RowData& operator=(RowData&& other) noexcept;

		/*
		   If a row is a zeroRow, it will be smaller
		   than nonzero rows, and equal to zeroRows.
		   Nonzero rows will be greater than zeroRows.

		   nonzero rows are compared based on their pivotIndex,
		   with operator<(RowData& that) evaluating to true
		   if this->pivotIndex < that.pivotIndex.
		   nonzero rows are equal if their pivotIndex
		   is the same.

		   These are useful for sorting methods
		   to get a matrix in ref or other
		   needed formats
		 */
		bool operator>(RowData& that);
		bool operator<(RowData& that);
		bool operator==(RowData& that);
		bool operator!=(RowData& that);
		bool operator<=(RowData& that);
		bool operator>=(RowData& that);

		/*
		   multiplies row by a scalar
		 */
		RowData& operator*=(double operand);

		/*
		   adds another row to this row
		 */
		RowData& operator+=(RowData& that);

		/*
		   divides this row by a scalar
		 */
		RowData& operator/=(double operand);

		/*
		   Evaluates to true
		   if this row is not a zeroRow,
		   false otherwise. Useful
		   in operations where you don't
		   want to evaluate zeroRows
		 */
		operator bool();

		/*
		   Access elements in double* data
		 */
		double& operator[](int i);

		/*
		   Represents an elementary row operation.
		   where k is a scalar, r1 = *this, r2 = other row

		           k * r1 + r2 -> r1

		   Result will be that r1[index] is 0,
		   with the rest of the row transformed.
		   r2 is not modified

		   Parameters:

		   r2-> other row
		   index-> coefficient to be evaluated
		 */
		RowData& elementaryAdd(RowData& r2, double index);

		double* begin();
		double* end();
};

#endif
