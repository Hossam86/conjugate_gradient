#pragma once

#include <vector>

typedef double scalar;
typedef std::vector<std::vector<scalar>> dense;

struct Matrix_Coo {
    int num_rows;
    int num_cols;
    std::vector<int> rows;
    std::vector<int> cols;
    std::vector<scalar> values;
};

struct Matrix_Csr {
    int num_rows;
    int num_cols;
    std::vector<scalar> A;
    std::vector<int> IA;
    std::vector<int> JA;
};

void
matrix_from_file(std::string file_name, dense &mat);

void
coo_from_file(std::string file_name, Matrix_Coo &coo);

void
dense_from_coo(Matrix_Coo &coo, dense &mat);

void
coo_from_dense(dense &mat, Matrix_Coo &coo);

void
csr_from_coo(Matrix_Coo &coo, Matrix_Csr &csr);

void
csr_vector_mult(Matrix_Csr &csr, std::vector<scalar> &v, std::vector<scalar> &result);

void
matrix_csr_transpose(Matrix_Csr &csr, Matrix_Csr &csr_tran);

void
print_matrix(dense &mat);

void
print_coo(Matrix_Coo &coo);

void
print_csr(Matrix_Csr &csr);



