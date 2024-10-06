#include "sparse_matrix.h"

#define INITIAL_CAPACITY 1000

SparseMatrix *createSparseMatrix(const char *matrixFilePath)
{
    FILE *file = fopen(matrixFilePath, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file: %s\n", matrixFilePath);
        return NULL;
    }

    SparseMatrix *matrix = (SparseMatrix *)malloc(sizeof(SparseMatrix));
    if (!matrix)
    {
        fprintf(stderr, "Error: Memory allocation didn't go pretty well.\n");
        fclose(file);
        return NULL;
    }

    matrix->nonZeroCount = 0;
    matrix->elements = (Element *)malloc(sizeof(Element) * 1000); // initial capacity
    if (!matrix->elements)
    {
        fprintf(stderr, "Memory allocation didn't go pretty well.\n");
        fclose(file);
        free(matrix);
        return NULL;
    }

    char line[256];
    if (!fgets(line, sizeof(line), file) || sscanf(line, "rows=%d", &matrix->numRows) != 1)
    {
        fprintf(stderr, "Error: Invalid format for number of rows.\n");
        freeSparseMatrix(matrix);
        fclose(file);
        return NULL;
    }

    if (!fgets(line, sizeof(line), file) || sscanf(line, "cols=%d", &matrix->numCols) != 1)
    {
        fprintf(stderr, "Error: Invalid format for number of columns.\n");
        freeSparseMatrix(matrix);
        fclose(file);
        return NULL;
    }

    // Read elements from the file
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n')
            continue; // Skip empty lines

        int row, col, value;
        if (sscanf(line, "(%d, %d, %d)", &row, &col, &value) != 3)
        {
            fprintf(stderr, "Error: Invalid element format.\n");
            freeSparseMatrix(matrix);
            fclose(file);
            return NULL;
        }

        if (row >= matrix->numRows || col >= matrix->numCols)
        {
            fprintf(stderr, "Error: Element out of bounds (row: %d, col: %d).\n", row, col);
            freeSparseMatrix(matrix);
            fclose(file);
            return NULL;
        }

        setElement(matrix, row, col, value);
    }

    fclose(file);
    return matrix;
}

void freeSparseMatrix(SparseMatrix *matrix)
{
    if (matrix)
    {
        free(matrix->elements);
        free(matrix);
    }
}

int getElement(SparseMatrix *matrix, int row, int col)
{
    for (int i = 0; i < matrix->nonZeroCount; ++i)
    {
        if (matrix->elements[i].row == row && matrix->elements[i].col == col)
        {
            return matrix->elements[i].value;
        }
    }
    return 0;
}

void setElement(SparseMatrix *matrix, int row, int col, int value)
{
    if (row < 0 || row >= matrix->numRows || col < 0 || col >= matrix->numCols)
    {
        fprintf(stderr, "Error: Index out of bounds (row: %d, col: %d).\n", row, col);
        return;
    }

    // Search for the element if it already exists
    for (int i = 0; i < matrix->nonZeroCount; i++)
    {
        if (matrix->elements[i].row == row && matrix->elements[i].col == col)
        {
            if (value != 0)
            {
                matrix->elements[i].value = value;
            }
            else
            {
                // Remove element by swapping with last and decreasing nonZeroCount
                matrix->elements[i] = matrix->elements[--matrix->nonZeroCount];
            }
            return;
        }
    }

    // Add new element if value is non-zero
    if (value != 0)
    {
        if (matrix->nonZeroCount >= matrix->capacity)
        {
            // Reallocate memory for additional elements
            matrix->capacity *= 2;
            matrix->elements = (Element *)realloc(matrix->elements, sizeof(Element) * matrix->capacity);
            if (!matrix->elements)
            {
                fprintf(stderr, "Error: Memory reallocation failed.\n");
                return;
            }
        }

        matrix->elements[matrix->nonZeroCount++] = (Element){row, col, value};
    }
}

SparseMatrix *addSparseMatrices(SparseMatrix *a, SparseMatrix *b)
{
    if (a->numRows != b->numRows || a->numCols != b->numCols)
    {
        fprintf(stderr, "Error: Matrices must have the same dimensions for addition.\n");
        return NULL;
    }

    SparseMatrix *result = (SparseMatrix *)malloc(sizeof(SparseMatrix));
    result->numRows = a->numRows;
    result->numCols = a->numCols;
    result->capacity = INITIAL_CAPACITY;
    result->nonZeroCount = 0;
    result->elements = (Element *)malloc(sizeof(Element) * result->capacity);

    // Add elements from both matrices
    for (int i = 0; i < a->nonZeroCount; i++)
    {
        setElement(result, a->elements[i].row, a->elements[i].col, a->elements[i].value + getElement(b, a->elements[i].row, a->elements[i].col));
    }

    for (int i = 0; i < b->nonZeroCount; i++)
    {
        if (getElement(a, b->elements[i].row, b->elements[i].col) == 0)
        {
            setElement(result, b->elements[i].row, b->elements[i].col, b->elements[i].value);
        }
    }

    return result;
}

SparseMatrix *subtractSparseMatrices(SparseMatrix *a, SparseMatrix *b)
{
    if (a->numRows != b->numRows || a->numCols != b->numCols)
    {
        fprintf(stderr, "Matrices dimensions do not match for subtraction.\n");
        return NULL;
    }

    SparseMatrix *result = (SparseMatrix *)malloc(sizeof(SparseMatrix));
    if (!result)
    {
        fprintf(stderr, "Memory allocation didn't go pretty well.\n");
        return NULL;
    }
    result->numRows = a->numRows;
    result->numCols = a->numCols;
    result->elements = (Element *)malloc(sizeof(Element) * 1000);
    result->nonZeroCount = 0;

    for (int i = 0; i < a->nonZeroCount; ++i)
    {
        int row = a->elements[i].row;
        int col = a->elements[i].col;
        int diffValue = a->elements[i].value - getElement(b, row, col);
        if (diffValue != 0)
        {
            setElement(result, row, col, diffValue);
        }
    }

    for (int i = 0; i < b->nonZeroCount; ++i)
    {
        int row = b->elements[i].row;
        int col = b->elements[i].col;
        if (getElement(a, row, col) == 0)
        { // Only add if not present in [a]
            setElement(result, row, col, -b->elements[i].value);
        }
    }

    return result;
}

SparseMatrix *multiplySparseMatrices(SparseMatrix *a, SparseMatrix *b)
{
    if (a->numCols != b->numRows)
    {
        fprintf(stderr, "Matrices dimensions do not match for multiplication.\n");
        return NULL;
    }

    SparseMatrix *result = (SparseMatrix *)malloc(sizeof(SparseMatrix));
    if (!result)
    {
        fprintf(stderr, "Memory allocation didn't go pretty well.\n");
        return NULL;
    }
    result->numRows = a->numRows;
    result->numCols = b->numCols;
    result->elements = (Element *)malloc(sizeof(Element) * 1000);
    result->nonZeroCount = 0;

    for (int i = 0; i < a->nonZeroCount; ++i)
    {
        for (int j = 0; j < b->nonZeroCount; ++j)
        {
            if (a->elements[i].col == b->elements[j].row)
            {
                int row = a->elements[i].row;
                int col = b->elements[j].col;
                int value = a->elements[i].value * b->elements[j].value;
                setElement(result, row, col, getElement(result, row, col) + value);
            }
        }
    }

    return result;
}

void printSparseMatrix(SparseMatrix *matrix)
{
    for (int i = 0; i < matrix->nonZeroCount; ++i)
    {
        printf("(%d, %d, %d)\n", matrix->elements[i].row, matrix->elements[i].col, matrix->elements[i].value);
    }
}