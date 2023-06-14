#ifndef RREF_H_
#define RREF_H_

#include <string>
#include <vector>

#include "RowData.h"

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Rref.h                                                                                       *
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
   This class contains a matrix represented by
   std::vector<RowData> rows(see RowData.h).
   Rref is performed on this matrix after
   initialization in a call to solve(),
   which contains the main loop for the program.

   The program exits solve() when
           -the matrix is a zero matrix(see bool zeroMatrix)
           -matrix is in rref form, which
           occurs after isRef() evaluates true

   As a student does in real life,
   solve() will keep making passes over
   the matrix until it is in ref form (see isRef()).
   These passes are performed via doAnRefPass().
   This function checks the pivots of the rows
   and performs row operations when necessary

   Once solve() finds that isRef() evaluates to
   true, toRref() is executed to transform the matrix
   from ref to rref. This occurs only once.

   Author: Trevor Lash

   Revised: 6/11/23
 */
class Rref {

	private:

		/*
		   Width of matrix.
		 */
		int W;

		/*
		   Height of matrix.
		 */
		int H;

		/*
		   Allows us to ignore
		   zero rows when making
		   passes over the matrix
		 */
		int firstNonZeroRow;

		/*
		   A flag that is set to true
		   if matrix is all zeros.
		   This lets solve() terminate
		 */
		bool zeroMatrix;

		/*
		   The matrix. Each RowData
		   contains one row of the matrix/
		   (see RowData.h)
		 */
		std::vector<RowData> rows;

    public:

		/*
		   Reads a matrix from a file and converts
		   it into rref.

		   The file should countain rows
		   of numbers, with each number separated by 1+ spaces.
		   All rows should have the same amount of numbers

		   Parameters:
		   url-> name of text file

		  Throws:
		  std::ifstream::failure-> file read error
		  std::invalid_argument-> row lengths are inconsistent
	     */
		Rref(std::string url);

		/*
		   Takes the user-provided matrix and converts it into rref form.
		   This is done on a copy of the user's matrix.

		   Clients are responsible for deleting the matrix passed
		   to this function.

		   rref makes a copy of the matrix. user's responsibility
		   to delete original matrix.
		 */
		Rref(double** matrix, int W, int H);
		Rref(const Rref& other);
		Rref(Rref&& other) noexcept;

		~Rref();

		Rref& operator=(const Rref& other);
		Rref& operator=(Rref&& other) noexcept;

		/*
		   Deep copy of matrix
		 */
		double** getMatrix();
		void printMatrix();

    private:

		/*
		   Main loop of program.
		   While Rref::rows
		   isn't a Rref::zeroMatrix and Rref::isRef() is false,
		   this continues to execute.
		   Rref::doAnRefPass() is called for each loop for an ref pass.
		   Rref::toRref() is called before exiting the loop
		   to convert Rref::rows from ref to rref
		 */
		void solve();

		/*
		   Updates matrix properties

		   Called in Rref::solve() after every ref pass
		   to update the matrix. It calls RowsInfo::setRowInfo()
		   on each rowInfo in Rref::rows (the matrix), sorts the matrix
		   by natural ordering(see RowInfo::operator<(RowInfo&)),
		   and determines the Rref::firstNonZeroRow, as well as whether
		   or not Rref::rows is a Rref::zeroMatrix.
		 */
		void setRowInfo();

		/*
		   See Rref::setRowInfo
		 */
		void doAnRefPass();

		/*
		   See Rref::setRowInfo
		 */
		void toRref();

		/*
		   Checks if matrix Rref::rows is in Rref

		   Checked after each matrix pass
		   in Rref::solve(). Once this evaluates
		   to true, matrix is ready to be
		   transformed into rref, and
		   solve calls Rref::toRref().

		   Returns:
		   true-> matrix is in ref
		   false-> matrix is not in ref
		 */
		bool isRef();
};

#endif
