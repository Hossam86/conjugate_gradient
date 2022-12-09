#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef float scalar;
typedef std::vector<std::vector<scalar>> matrix;

struct matrix_coo {
    int row_count;
    int col_count;
    std::vector<int> rows;
    std::vector<int> cols;
    std::vector<scalar> values;
};

struct matrix_csr {

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
    matrix result(coo.row_count);
    for (int i = 0; i < coo.row_count; ++i) {
        result[i].resize(coo.col_count);
    }
    for (int i = 0; i < coo.rows.size(); ++i)
        result[coo.rows[i]][coo.cols[i]] = coo.values[i];
}

void
from_mat_to_coo(matrix &mat, matrix_coo &coo) {
    int array_size = mat.size() * mat[0].size();

    coo.row_count = mat.size();
    coo.col_count = mat[0].size();

    for (int i = 0; i < coo.row_count; ++i) {
        for (int j = 0; j < coo.col_count; ++j) {
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
from_coo_to_csr(matrix_coo, matrix_csr) {

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

    return 0;
}
