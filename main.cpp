#include <cstdio>
#include <iostream>

#include "Timer.h"
#include "Matrix.h"


template <class T, size_t Dim, size_t Cycles>
void test() {
    printf("Matrix size = %lu\n", Dim);
    Matrix<T, Dim> A;
    Matrix<T, Dim> B;
    Matrix<T, Dim> C;
    A.randFill();
    B.randFill();

    printf("Vertical:\n");
    for(size_t i = 0; i < Cycles; ++i) {
        C.zero();
        {
            Timer t;
            Matrix<T, Dim>::multiplyV(A, B, C);
        }
        B.deleteMatT();
    }
    printf("\n");

    printf("Horizontal:\n");
    for(size_t i = 0; i < Cycles; ++i) {
        C.zero();
        {
            Timer t;
            Matrix<T, Dim>::multiplyH(A, B, C);
        }
    }
}

template <class T, size_t Dim, size_t Cycles>
void testMT() {
    printf("Matrix size = %lu\n", Dim);
    Matrix<T, Dim> A;
    Matrix<T, Dim> B;
    Matrix<T, Dim> C;
    A.randFill();
    B.randFill();

    printf("Vertical Multithreaded:\n");
    for(size_t i = 0; i < Cycles; ++i) {
        C.zero();
        {
            Timer t;
            Matrix<T, Dim>::multiplyVMT(A, B, C);
        }
    }
    printf("\n");

    printf("Horizontal Multithreaded:\n");
    for(size_t i = 0; i < Cycles; ++i) {
        C.zero();
        {
            Timer t;
            Matrix<T, Dim>::multiplyHMT(A, B, C);
        }
    }
}


int main(int argc, char *argv[]) {
    std::srand(time(0));
    if(argc == 1) {
        int* DataA = new int[9];
        int* DataB = new int[9];
        printf("Input 9 numbers for matrix A\n");
        for (size_t i = 0; i < 9; ++i) {
            while(true)
            {
                std::cin >> DataA[i];
                if (!std::cin.fail()) break;
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                std::cout << "Bad entry. Enter a NUMBER: ";
            }
        }
        printf("Input 9 numbers for matrix B\n");
        for (size_t i = 0; i < 9; ++i) {
            while(true)
            {
                std::cin >> DataB[i];
                if (!std::cin.fail()) break;
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                std::cout << "Bad entry. Enter a NUMBER: ";
            }
        }


        Matrix<int, 3> A(DataA);
        Matrix<int, 3> B(DataB);
        Matrix<int, 3> C;

        printf("Matrix A:\n");
        A.print();
        printf("\n");

        printf("Matrix B:\n");
        B.print();
        printf("\n");

        printf("Vertical multiplication:\n");
        Matrix<int, 3>::multiplyV(A, B, C);
        C.print();

        // Сброс значений матрицы результата
        C.zero();

        printf("Horizontal multiplication:\n");
        Matrix<int, 3>::multiplyH(A, B, C);
        C.print();
    } else if (argc == 2) {
        if (strcmp(argv[1], "500") == 0) {
            test<int, 500, 5>();
        } else if (strcmp(argv[1], "600") == 0) {
            test<int, 600, 5>();
        } else if (strcmp(argv[1], "700") == 0) {
            test<int, 700, 5>();
        } else if (strcmp(argv[1], "800") == 0) {
            test<int, 800, 5>();
        } else if (strcmp(argv[1], "900") == 0) {
            test<int, 900, 5>();
        } else if (strcmp(argv[1], "1000") == 0) {
            test<int, 1000, 5>();
        } else {
            printf("Invalid matrix size. Available parameters: 500, 600, 700, 800, 900, 1000.\n");
        }
    } else if (argc == 3) {
        char* p;
        long threads = strtol(argv[2], &p, 10);
        if (*p != '\0') {
            printf("Invalid number of threads\n");
            return 1;
        }
        printf("Set number of threads to %ld\n", threads);
        omp_set_num_threads(threads);

        if (strcmp(argv[1], "500") == 0) {
            testMT<int, 500, 5>();
        } else if (strcmp(argv[1], "600") == 0) {
            testMT<int, 600, 5>();
        } else if (strcmp(argv[1], "700") == 0) {
            testMT<int, 700, 5>();
        } else if (strcmp(argv[1], "800") == 0) {
            testMT<int, 800, 5>();
        } else if (strcmp(argv[1], "900") == 0) {
            testMT<int, 900, 5>();
        } else if (strcmp(argv[1], "1000") == 0) {
            testMT<int, 1000, 5>();
        } else {
            printf("Invalid matrix size. Available parameters: 500, 600, 700, 800, 900, 1000.\n");
        }
    }
    return 0;
}
