#include <iostream>
#include <fstream>
#include <string>

// #include "heat.hpp"

int main(int argc, char *argv[]) {
    /* Get command line arguments */
    if (argc != 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  " << argv[0] << " <input file> <soln prefix>" << std::endl;
        return 0;
    }
    std::string input_matrix = argv[1];
    std::string solution = argv[2];

    std::ifstream input_stream;
    input_stream.open(input_matrix);
    int n = 0, m = 0;

    input_stream >> m >> n;
    float mat[m][n];
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            float temp;
            input_stream >> mat[i][j];
        }
    }

    // print matrix
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            std::cout << mat[i][j] << " ";
        std::cout << "\n";
    }

    return 0;
}
