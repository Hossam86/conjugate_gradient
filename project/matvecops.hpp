#pragma once

#include <vector>

typedef double scalar;
typedef std::vector<std::vector<scalar> > Dense;

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
matrix_from_file(std::string file_name, Dense &mat);

void
coo_from_file(std::string file_name, Matrix_Coo &coo);

void
dense_from_coo(Matrix_Coo &coo, Dense &mat);

void
coo_from_dense(Dense &mat, Matrix_Coo &coo);

void
csr_from_coo(Matrix_Coo &coo, Matrix_Csr &csr);

std::vector<scalar>
csr_vector_mult(Matrix_Csr &csr, const std::vector<scalar> &v);

void
matrix_csr_transpose(Matrix_Csr &csr, Matrix_Csr &csr_tran);

void
print_matrix(Dense &mat);

void
print_coo(Matrix_Coo &coo);

void
print_csr(Matrix_Csr &csr);

scalar
dot(const std::vector<scalar> &a, const std::vector<scalar> &b);

// Linear combination of vectors
std::vector<scalar>
vector_combination(scalar a, const std::vector<scalar> &U, scalar b, const std::vector<scalar> &V);

// Vector norm
scalar
vectorNorm(const std::vector<scalar> &V);

