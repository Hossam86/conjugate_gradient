#include "CGSolver.hpp"
#include "matvecops.hpp"


int CGSolver(std::vector<double> &val,
             std::vector<int> &row_ptr,
             std::vector<int> &col_idx,
             std::vector<double> &b,
             std::vector<double> &x,
             double tol) {

    int n = x.size();


    Matrix_Csr csr{};
    csr.A = val;
    csr.JA = col_idx;
    csr.IA = row_ptr;
    csr.num_rows = n;
    csr.num_cols = n;

    auto Au = csr_vector_mult(csr, x);
    auto R = vector_combination(1, b, -1, Au);
    auto P = R;
    auto l2norm_0 = vectorNorm(R);

    int k = 0;

    while (k < n) {
        // Store previous residual
        auto Rold = R;
        auto AP = csr_vector_mult(csr, P);

        auto alpha = dot(R, R) / fmax(dot(P, AP), NEARZERO);
        // Next estimate of solution
        x = vector_combination(1.0, x, alpha, P);
        // Residual
        R = vector_combination(1.0, R, -alpha, AP);

        // Convergence test
        auto l2norm = vectorNorm(R);
        if (l2norm / l2norm_0 < tol) {
            return k;
        }

        double beta = dot(R, R) / fmax(dot(Rold, Rold), NEARZERO);
        P = vector_combination(1.0, R, beta, P);             // Next gradient
        k++;
    }

    return -1;

}