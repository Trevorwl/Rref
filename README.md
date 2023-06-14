# Rref
Author: Trevor Lash
Last modified: 6/14/23

A simple rref library.

Features:

Rref.h-> 
  -Pass a double** matrix to an rref constructor to obtain a solution.
  -One can also create an mxn matrix in a text file and pass its url to an rref contstructor.
  An example is
  1 2 4 3
  2 3 4 1
  6 2 3 5
  
RowData.h-> The data representation for the matrix rows in class Rref.
Can also be used in your own matrix programs to make matrix processing easier.
   Contains:
  -width of matrix row
  -row numbers
  -index of pivot
  -flag that indicates whether or not a row is all zeros
  
RowData::elementaryAdd(RowData& r2, double index) is provided
to allow for elementary row opearaions.
  r2->other row
  index->index of row element being evaluated

Operators are provided to allow for natural ordering of RowData objects.
This allows for rows to be sorted in ascending order based on
the index of their first nonzero.


