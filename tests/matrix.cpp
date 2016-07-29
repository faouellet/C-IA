# define BOOST_TEST_MODULE Main

#include <boost/test/unit_test.hpp>

#include "../src/func.h"
#include "../src/matrix.h"

#include <vector>

using namespace DistLang;

BOOST_AUTO_TEST_CASE( MatrixAdditionTest )
{
    // Construct tensors with their dimensionality
    Matrix<int> matA;
    Matrix<int> matB;

    // Index variables
    Index i, j;

    // The function declaration
    Func matAdd;

    // The function definition
    //matAdd(i, j) = matA(i, j) + matB(i, j);
    matAdd(i, j) = matA(i, j);

    // matAdd.distribute()
    // matAdd.parallelize()
    // matAdd.vectorize()
    
    Matrix<int> matC;
    matAdd.Execute(matC);
}

/*BOOST_AUTO_TEST_CASE( MatrixMultiplicationTest )
{
    // Construct tensors with their dimensionality
    Tensor<int> a(2);
    Tensor<int> b(2);

    // Index variables
    Index i, j, k;

    // The function declaration
    Func matMult;

    // The function definition
    matMult(i, j, k) = sum(a(i, k) * b(k, j));
        
    // matMult.distribute()
    // matMult.parallelize()
    // matMult.vectorize()

    // The result
    Tensor<int> c;
    matMult.execute(c);
}*/