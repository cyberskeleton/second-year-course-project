#include <iostream>
#include "intervals_c/headers/tests_c.h"
#include "intervals_cpp/headers/cross_tests.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    cout << "Running C tests implementing basic logic..." << endl;
    run_tests();
    cout << "...end of C tests\n" << endl;

    cout << "Running C++ cross_tests wrapping basic implementation up..." << endl;
    cross_tests::run_cpp_tests();
    cout << "...end of C++ cross_tests\n" << endl;

    return 0;
}
