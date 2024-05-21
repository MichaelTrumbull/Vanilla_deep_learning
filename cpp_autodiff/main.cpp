#include <vector>
#include "autodiff.cpp"

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>
#include <set>


#define MAXCHAR 1000
const int DATACOLS = 785;
const int DATAROWS = 60000;
const float size_Y = 59000;
const float eps = 0.00000001;
const float m = 60000.0;
const float alpha = 0.1;
const int iterations = 9;


int main()
{

    Neuron n;
    n.p();
    float arr[] = {2,3};
    Val a = n(arr);
    a.p();
    






    // Load and parse data //
    /*
    Matrix allData(DATAROWS,DATACOLS);
    Matrix Y(DATAROWS,1);
    Matrix X(DATAROWS,DATACOLS-1);
    allData = loadData("../../MNIST/mnist_train.csv");
    Y = getTargets(allData);
    X = ( getInputs(allData).transpose() ) / 255.0;
    

    
    // build one_hot_Y // This is used as a target during learning.
    Matrix tmp_one_hot_Y(59000, 10);
    int temp_val;
    
    for(int i=0; i<59000; i++){
        temp_val = Y(i,0);
        tmp_one_hot_Y.set(i, temp_val, 1);
    }
    Matrix one_hot_Y(10, 59000);
    one_hot_Y = tmp_one_hot_Y.transpose();

    Matrix predictions(1,59000);
    */
    

    
}

/*
    Val x1(2.0);
    Val x2(0.0);
    Val w1(-3.0);
    Val w2(1.0);
    Val b(6.8813735870195432);
    Val x1w1 = x1*w1;
    Val x2w2 = x2*w2;
    Val x1w1x2w2 = x1w1 + x2w2;
    Val n = x1w1x2w2 + b;
    Val o = n.tanh();
    o.backward();
    printf("x1      ");
    x1.p();
    printf("x2      ");
    x2.p();
    printf("w1      ");
    w1.p();
    printf("w2      ");
    w2.p();
    printf("x1w1    ");
    x1w1.p();
    printf("x2w2    ");
    x2w2.p();
    printf("b       ");
    b.p();
    printf("x1w1x2w2");
    x1w1x2w2.p();
    printf("n       ");
    n.p();
    printf("o       ");
    o.p();

*/