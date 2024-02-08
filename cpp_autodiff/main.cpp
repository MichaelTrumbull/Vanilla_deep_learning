#include <vector>
#include "matrix.cpp"

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>
#include "ezML.cpp"

#define MAXCHAR 1000
const int DATACOLS = 785;
const int DATAROWS = 60000;

const float alpha = 0.1; //learning rate
const int iterations = 9;


int main()
{   

    // Load and parse data //
    Matrix allData(DATAROWS,DATACOLS);
    Matrix Y(DATAROWS,1);
    Matrix X(DATAROWS,DATACOLS-1);
    allData = loadData("../../MNIST/mnist_train.csv");
    Y = getTargets(allData);
    X = ( getInputs(allData).transpose() ) / 255.0;
    
    // INIT PARAMS //
    Matrix W1(10, 784);
    W1.randomize();
    Matrix b1(10,1);
    b1.randomize();
    Matrix W2(10,10);
    W2.randomize();
    Matrix b2(10,1);
    b2.randomize();
    
    Matrix Z1(10, 59000);
    Matrix A1(10, 59000);
    Matrix Z2(10, 59000);
    Matrix A2(10, 59000);

    Matrix dZ2(10,59000);
    Matrix dW2(10,10);
    float db2 = 0.0;
    Matrix dZ1(10,59000);
    Matrix dW1(10,784);
    float db1 = 0.0;

    
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
    
    for(int i=0; i<iterations; i++){
        forward_prop(W1, b1, W2, b2, X, Z1, A1, Z2, A2);
        backward_prop(Z1, A1, A2, W2, X, dZ2, dW2, db2, dZ1, dW1, db1, one_hot_Y);
        update_params(W1, b1, W2, b2, dW1, db1, dW2, db2, alpha);
        printf("iteration: %i\n", i);
        predictions = get_predictions(A2);
        printf("%f\n\n", get_accuracy(predictions, Y));
    }
    
}