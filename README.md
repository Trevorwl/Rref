# Rref library
  ***********************************************************
<ins>__Rref.h__ </ins>

-Pass a double** matrix to an rref constructor to obtain a solution.
  
-One can also create an mxn matrix in a text file and pass its url to an rref contstructor.
  
An example is
  
    1 2 4 3
  
    2 3 4 1
  
    6 2 3 5
  
  ***********************************************************
  
<ins>__RowData.h: The data representation for the matrix rows in class Rref.__</ins>

Class RowData can also be used in your own matrix programs to make matrix processing easier.

__Contains:__
   
-width of matrix row

-numbers in row
  
-index of pivot
  
-flag that indicates whether or not a row is all zeros
  
RowData::elementaryAdd is provided
to allow for elementary row opearaions(see documentation).

Operators are provided to allow for natural ordering of RowData objects.
This allows for rows to be sorted in ascending order based on
the index of their first nonzero.


