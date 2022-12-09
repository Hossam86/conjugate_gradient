#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matvecops.hpp"




int main(int argc, char *argv[]) {
    /* Get command line arguments */
    if (argc != 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << argv[0] << " <input file> <soln prefix>" << std::endl;
        return 0;
    }
    std::string input_matrix = argv[1];
    std::string solution = argv[2];


    dense mat{};
    matrix_from_file(input_matrix, mat);
    print_matrix(mat);

    Matrix_Coo coo{};
    coo_from_dense(mat, coo);
    print_coo(coo);

    Matrix_Csr csr{};
    csr_from_coo(coo, csr);
    print_csr(csr);

//    //test built-in function
//    std::cout << "built in funcs \n";
//
//    Matrix_Csr csr2{};
//    csr2.A = coo.values;
//    csr2.IA = coo.rows;
//    csr2.JA = coo.cols;
//    COO2CSR(csr2.A, csr2.IA, csr2.JA);
//    print_csr(csr2);

    // test transpose
    std::cout << "test transpose \n";
    Matrix_Csr csr_t;
    matrix_csr_transpose(csr, csr_t);
    std::cout << "A in csr form \n";
    print_csr(csr);
    std::cout << "AT in csr form \n";
    print_csr(csr_t);

    // test multi
    std::vector<scalar> v{1, 1, 1, 1};
    std::vector<scalar> result;
    csr_vector_mult(csr, v, result);
    std::cout << "result -->{";
    for (int i = 0; i < result.size(); ++i)
        std::cout << result[i] << ",";
    std::cout << "}" << std::endl;


    return 0;
}
