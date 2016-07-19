#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#else
#ifndef _WIN32
#   define BOOST_TEST_MODULE MatrixTests
#endif
#endif

#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_CASE( MatrixAdditionTest )
{
    // Construct tensors with their dimensionality
    Input a(2);
    Input b(2);

    // Index variables
    Index i, j;

    // The function declaration
    Func matAdd;

    // The function definition
    matAdd[i, j] = a[i, j] + b[i, j];

    // matAdd.distribute()
    // matAdd.parallelize()
    // matAdd.vectorize()

    Tensor<int> matA;
    Tensor<int> matB;

    Tensor c = matAdd(matA, matB);
}

BOOST_AUTO_TEST_CASE( MatrixMultiplicationTest )
{
    // Construct tensors with their dimensionality
    Tensor a(2);
    Tensor b(2);

    // Index variables
    Var i, j, k;

    // The function declaration
    Func matMult;

    // The function definition
    matMult[i, j] = sum(a[i, k] * b[k, j]);
        
    // matMult.distribute()
    // matMult.parallelize()
    // matMult.vectorize()

    // The result
    Tensor c = matMult(a, b);

    c.Print();
}