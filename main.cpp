#include <iostream>
#include <string>
#include <vector>

#include "matvecops.hpp"
#include "CGSolver.hpp"

void print(std::string title, const std::vector<scalar> &V) {
    std::cout << title << '\n';

    int n = V.size();
    for (int i = 0; i < n; i++) {
        double x = V[i];
        if (abs(x) < NEARZERO) x = 0.0;
        std::cout << x << '\t';
    }
    std::cout << '\n';
}

std::vector<scalar>
matrixTimesVector(const Dense &A, const std::vector<scalar> &V)     // Matrix times vector
{
    int n = A.size();
    std::vector<scalar> C(n);
    for (int i = 0; i < n; i++) C[i] = dot(A[i], V);
    return C;
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


    std::cout << "print coo" << std::endl;
    Matrix_Coo coo{};
    coo_from_file(input_matrix, coo);
    print_coo(coo);

    std::cout << "print csr" << std::endl;
    Matrix_Csr csr{};
    csr_from_coo(coo, csr);
    print_csr(csr);

    /* test solver 1*/

//    Dense A = {{4, 1},
//               {1, 3}};
//    std::vector<scalar> B = {1, 2};
//
//    double tol = 1e-5;
//    Matrix_Coo coo{};
//    coo_from_dense(A, coo);
//    Matrix_Csr csr{};
//    csr_from_coo(coo, csr);
//    std::vector<scalar> X(B.size(), 0);
//    CGSolver(csr.A, csr.IA, csr.JA, B, X, tol);
//
//    std::cout << "Solves AX = B\n";
//
//    print("\nB:", B);
//    print("\nX:", X);
//    print("\nCheck AX:", matrixTimesVector(A, X));


    /* test solver 2*/
    double tol = 1e-5;
    std::vector<scalar> B(csr.num_rows, 0.0);
    std::vector<scalar> X(csr.num_rows, 0.0);
    CGSolver(csr.A, csr.IA, csr.JA, B, X, tol);
    print("\nX:", X);


    return 0;
}
