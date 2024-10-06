#include "sparse_matrix.h"

int main() {
    char file1[256], file2[256];
    printf("Enter the path of the first matrix file: ");
    scanf("%255s", file1); // Prevent buffer overflowin'
    SparseMatrix* matrix1 = createSparseMatrix(file1);
    if (!matrix1) return EXIT_FAILURE; // Handle erro

    printf("Enter the path of the second matrix file: ");
    scanf("%255s", file2); // Prevent buffer overflowin'
    SparseMatrix* matrix2 = createSparseMatrix(file2);
    if (!matrix2) {
        freeSparseMatrix(matrix1);
        return EXIT_FAILURE; // Handle err
    }

    int choice;
    printf("Choose an operation:\n1. Addition\n2. Subtraction\n3. Multiplication\n");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input.\n");
        freeSparseMatrix(matrix1);
        freeSparseMatrix(matrix2);
        return EXIT_FAILURE; // Handle err
    }

    SparseMatrix* result = NULL;
    switch (choice) {
        case 1:
            result = addSparseMatrices(matrix1, matrix2);
            break;
        case 2:
            result = subtractSparseMatrices(matrix1, matrix2);
            break;
        case 3:
            result = multiplySparseMatrices(matrix1, matrix2);
            break;
        default:
            fprintf(stderr, "Invalid choice.\n");
            freeSparseMatrix(matrix1);
            freeSparseMatrix(matrix2);
            return EXIT_FAILURE; // Handle err
    }

    if (result) {
        printf("Result:\n");
        printSparseMatrix(result);
        freeSparseMatrix(result);
    } else {
        fprintf(stderr, "Error in matrix operation.\n");
    }

    freeSparseMatrix(matrix1);
    freeSparseMatrix(matrix2);

    return EXIT_SUCCESS;
}