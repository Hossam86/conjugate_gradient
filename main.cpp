#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "COO2CSR.hpp"

typedef double scalar;
typedef std::vector<std::vector<scalar>> matrix;

struct matrix_coo {
    int num_rows;
    int num_cols;
    std::vector<int> rows;
    std::vector<int> cols;
    std::vector<scalar> values;
};

struct matrix_csr {
    int num_rows;
    int num_cols;
    std::vector<scalar> A;
    std::vector<int> IA;
    std::vector<int> JA;
};


void
matrix_from_file(std::string file_name, matrix &mat) {

    std::ifstream input_stream;
    input_stream.open(file_name);
    int n = 0, m = 0;

    input_stream >> m >> n;
    mat.resize(m);
    for (int i = 0; i < m; ++i) {
        mat[i].resize(n);
    }

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            input_stream >> mat[i][j];
        }

    }
}

//transpose
void
matrix_csr_transpose(matrix_csr &csr, matrix_csr &csr_tran) {

    auto nnz = csr.A.size();
    csr_tran.A.resize(nnz);
    csr_tran.JA.resize(nnz);
    csr_tran.IA.resize(csr.num_rows + 1);

    csr_tran.IA[0] = 0;

    // Count elements in each column:
    std::vector<int> cnt(csr.num_cols);
    std::fill(cnt.begin(), cnt.end(), 0);

    for (int i = 0; i < nnz; ++i) {
        auto col = csr.JA[i];
        cnt[col] += 1;
    }
    //Cumulative sum to set the column pointer of matrix B
    for (int i = 1; i < csr.num_cols + 1; ++i) {
        csr_tran.IA[i] = csr_tran.IA[i - 1] + cnt[i - 1];
    }

    for (int i = 0; i < csr.num_rows; ++i) {
        for (int j = csr.IA[i]; j < csr.IA[i + 1]; ++j) {
            auto col = csr.JA[j];
            auto dest = csr_tran.IA[col];

            csr_tran.JA[dest] = i;
            csr_tran.A[dest] = csr.A[j];
            csr_tran.IA[col] += 1;
        }
    }
    // shift csr_tran.IA;
    csr_tran.IA.insert(std::begin(csr_tran.IA), 0);
    csr_tran.IA.pop_back();
}

void
matrix_coo_from_file(std::string file_name, matrix_coo &mat) {
    std::ifstream input_stream;
    input_stream.open(file_name);
    int n = 0, m = 0;
    input_stream >> m >> n;
    int array_size = m * n;
    mat.rows.resize(array_size);
    mat.cols.resize(array_size);
    mat.values.resize(array_size);

    for (int i = 0; i < array_size; ++i) {
        input_stream >> mat.rows[i] >> mat.cols[i] >> mat.values[i];
    }
}

void
from_coo_to_mat(matrix_coo &coo, matrix &mat) {
    mat.resize(coo.num_rows);
    for (int i = 0; i < coo.num_rows; ++i) {
        mat[i].resize(coo.num_cols);
    }
    for (int i = 0; i < coo.rows.size(); ++i)
        mat[coo.rows[i]][coo.cols[i]] = coo.values[i];
}

void
from_mat_to_coo(matrix &mat, matrix_coo &coo) {
    int array_size = mat.size() * mat[0].size();

    coo.num_rows = mat.size();
    coo.num_cols = mat[0].size();

    for (int i = 0; i < coo.num_rows; ++i) {
        for (int j = 0; j < coo.num_cols; ++j) {
            coo.values.push_back(mat[i][j]);
            coo.rows.push_back(i);
            coo.cols.push_back(j);
        }

    }
}

void
print_matrix(matrix &mat) {
    std::cout << "matrix in native format" << std::endl;
    for (int i = 0; i < mat.size(); ++i) {
        for (int j = 0; j < mat[0].size(); ++j)
            std::cout << mat[i][j];
        std::cout << "\n";
    }
}

void
print_coo(matrix_coo &coo) {
    std::cout << "matrix in coo format" << std::endl;
    for (int i = 0; i < coo.rows.size(); ++i) {
        std::cout << coo.rows[i] << coo.cols[i] << coo.values[i] << std::endl;
    }
}

void
print_csr(matrix_csr &csr) {
    std::cout << "A --> {";
    for (int i = 0; i < csr.A.size(); ++i) {
        std::cout << csr.A[i] << ",";
    }
    std::cout << "}" << std::endl;

    std::cout << "IA --> {";
    for (int i = 0; i < csr.IA.size(); ++i) {
        std::cout << csr.IA[i] << ",";
    }
    std::cout << "}" << std::endl;

    std::cout << "JA --> {";
    for (int i = 0; i < csr.JA.size(); ++i) {
        std::cout << csr.JA[i] << ",";
    }
    std::cout << "}" << std::endl;
}

// Generate the three vectors A, IA, JA
void
from_coo_to_csr(matrix_coo &coo, matrix_csr &csr) {
    int m = coo.num_rows;
    int n = coo.num_cols;

    csr.num_rows = m;
    csr.num_cols = n;
    auto &A = csr.A;
    auto &IA = csr.IA;
    auto &JA = csr.JA;
    int NNZ = 0;

    IA.push_back(0);
    matrix M{};
    from_coo_to_mat(coo, M);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (M[i][j] != 0) {
                A.push_back(M[i][j]);
                JA.push_back(j);

                // Count Number of Non Zero
                // Elements in row i
                NNZ++;
            }
        }
        IA.push_back(NNZ);
    }
}

int main(int argc, char *argv[]) {
    /* Get command line arguments */
    if (argc != 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << argv[0] << " <input file> <soln prefix>" << std::endl;
        return 0;
    }
    std::string input_matrix = argv[1];
    std::string solution = argv[2];


    matrix mat{};
    matrix_from_file(input_matrix, mat);
    print_matrix(mat);

    matrix_coo coo{};
    from_mat_to_coo(mat, coo);
    print_coo(coo);

    matrix_csr csr{};
    from_coo_to_csr(coo, csr);
    print_csr(csr);

    //test built-in function
    std::cout << "built in funcs \n";

    matrix_csr csr2{};
    csr2.A = coo.values;
    csr2.IA = coo.rows;
    csr2.JA = coo.cols;
    COO2CSR(csr2.A, csr2.IA, csr2.JA);
    print_csr(csr2);

    // test transpose
    std::cout << "test transpose \n";
    matrix_csr csr_t;
    matrix_csr_transpose(csr, csr_t);
    std::cout << "A in csr form \n";
    print_csr(csr);
    std::cout << "AT in csr form \n";
    print_csr(csr_t);

    return 0;
}
