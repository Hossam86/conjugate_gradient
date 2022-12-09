#include "CGSolver.hpp"
#include "matvecops.hpp"

const scalar NEARZERO = 1.0e-10;       // interpretation of "zero"

int CGSolver(std::vector<double> &val,
             std::vector<int> &row_ptr,
             std::vector<int> &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double tol) {


    double TOLERANCE = 1.0e-10;

    int n = x.size();

    auto R = b;
    auto P = R;
    int k = 0;

    Matrix_Csr csr{};
    csr.A = val;
    csr.JA = col_idx;
    csr.IA = row_ptr;

    while (k < n) {
        // Store previous residual
        auto Rold = R;
        auto AP = csr_vector_mult(csr, P);

        auto alpha = dot(R, R) / fmax(dot(P, AP), NEARZERO);
        // Next estimate of solution
        x = vector_combination(1.0, x, alpha, P);
        // Residual
        R = vector_combination(1.0, R, -alpha, AP);

        if (vectorNorm(R) < TOLERANCE) break;             // Convergence test

        double beta = dot(R, R) / fmax(dot(Rold, Rold), NEARZERO);
        P = vector_combination(1.0, R, beta, P);             // Next gradient
        k++;
    }

    return 0;

}