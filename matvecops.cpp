#include "matvecops.hpp"
#include <iostream>
#include <fstream>

void
matrix_from_file(std::string file_name, dense &mat) {

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


void
coo_from_file(std::string file_name, Matrix_Coo &coo) {
    std::ifstream input_stream;
    input_stream.open(file_name);
    int n = 0, m = 0;
    input_stream >> m >> n;
    int array_size = m * n;
    coo.rows.resize(array_size);
    coo.cols.resize(array_size);
    coo.values.resize(array_size);

    for (int i = 0; i < array_size; ++i) {
        input_stream >> coo.rows[i] >> coo.cols[i] >> coo.values[i];
    }
}

void
dense_from_coo(Matrix_Coo &coo, dense &mat) {
    mat.resize(coo.num_rows);
    for (int i = 0; i < coo.num_rows; ++i) {
        mat[i].resize(coo.num_cols);
    }
    for (int i = 0; i < coo.rows.size(); ++i)
        mat[coo.rows[i]][coo.cols[i]] = coo.values[i];
}

void
coo_from_dense(dense &mat, Matrix_Coo &coo) {
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


// Generate the three vectors A, IA, JA
void
csr_from_coo(Matrix_Coo &coo, Matrix_Csr &csr) {
    int m = coo.num_rows;
    int n = coo.num_cols;

    csr.num_rows = m;
    csr.num_cols = n;
    auto &A = csr.A;
    auto &IA = csr.IA;
    auto &JA = csr.JA;
    int NNZ = 0;

    IA.push_back(0);
    dense M{};
    dense_from_coo(coo, M);

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


void
csr_vector_mult(Matrix_Csr &csr, std::vector<scalar> &v, std::vector<scalar> &result) {
    result.resize(csr.num_cols);
    std::fill(result.begin(), result.end(), 0.0);
    for (int i = 0; i < csr.num_cols; ++i) {
        for (int j = csr.IA[i]; j < csr.IA[i + 1]; ++j) {
            auto idx = csr.JA[j];
            result[i] += csr.A[j] * v[idx];
        }
    }

}

void
matrix_csr_transpose(Matrix_Csr &csr, Matrix_Csr &csr_tran) {

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
    //Cumulative sum to set the column pointer of dense B
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
print_matrix(dense &mat) {
    std::cout << "dense in native format" << std::endl;
    for (int i = 0; i < mat.size(); ++i) {
        for (int j = 0; j < mat[0].size(); ++j)
            std::cout << mat[i][j];
        std::cout << "\n";
    }
}

void
print_coo(Matrix_Coo &coo) {
    std::cout << "dense in coo format" << std::endl;
    for (int i = 0; i < coo.rows.size(); ++i) {
        std::cout << coo.rows[i] << coo.cols[i] << coo.values[i] << std::endl;
    }
}

void
print_csr(Matrix_Csr &csr) {
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