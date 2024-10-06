#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int row;
    int col;
    int value;
} Element;

typedef struct {
    int numRows;
    int numCols;
    Element* elements;
    int nonZeroCount;
    int capacity;
} SparseMatrix;

/**
 * Creates a sparse matrix from a specified file.
 * 
 * @param matrixFilePath The path to the file containing matrix data.
 * @return A pointer to the created SparseMatrix, or NULL on failure.
 */
SparseMatrix* createSparseMatrix(const char* matrixFilePath);

/**
 * Frees the allocated memory for a SparseMatrix.
 * 
 * @param matrix The SparseMatrix to be freed.
 */
void freeSparseMatrix(SparseMatrix* matrix);

/**
 * Retrieves the value at a specific row and column in the matrix.
 * 
 * @param matrix The SparseMatrix.
 * @param row The row index.
 * @param col The column index.
 * @return The value at the specified location, or 0 if not set.
 */
int getElement(SparseMatrix* matrix, int row, int col);

/**
 * Sets the value at a specific row and column in the matrix.
 * 
 * @param matrix The SparseMatrix.
 * @param row The row index.
 * @param col The column index.
 * @param value The value to set.
 */
void setElement(SparseMatrix* matrix, int row, int col, int value);

/**
 * Adds two sparse matrices.
 * 
 * @param a The first SparseMatrix.
 * @param b The second SparseMatrix.
 * @return A new SparseMatrix containing the result, or NULL on failure.
 */
SparseMatrix* addSparseMatrices(SparseMatrix* a, SparseMatrix* b);

/**
 * Subtracts the second sparse matrix from the first.
 * 
 * @param a The first SparseMatrix.
 * @param b The second SparseMatrix.
 * @return A new SparseMatrix containing the result, or NULL on failure.
 */
SparseMatrix* subtractSparseMatrices(SparseMatrix* a, SparseMatrix* b);

/**
 * Multiplies two sparse matrices.
 * 
 * @param a The first SparseMatrix.
 * @param b The second SparseMatrix.
 * @return A new SparseMatrix containing the result, or NULL on failure.
 */
SparseMatrix* multiplySparseMatrices(SparseMatrix* a, SparseMatrix* b);

/**
 * Prints the non-zero elements of the sparse matrix.
 * 
 * @param matrix
 */
void printSparseMatrix(SparseMatrix* matrix);

#endif 