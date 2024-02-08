#include "matrix.cpp"
#include <vector>
const int DATACOLS = 785;
const int DATAROWS = 60000;

const float size_Y = 59000; // used for normalization. In the python version I compute size(Y) but its constant so who cares.
const float eps = 0.00000001;
const float m = 60000.0;

Matrix ReLU(Matrix & m)
{
    return m.max(0.0);
}

Matrix softmax(Matrix & m)
{
    return m.exp() / (m.exp()).sum();
}

void forward_prop(Matrix & W1, Matrix & b1, Matrix & W2, Matrix & b2, Matrix & X, 
                  Matrix & Z1, Matrix & A1, Matrix & Z2, Matrix & A2)
{
    Z1 = W1.matmul(X) + b1;
    A1 = ReLU(Z1);
    Z2 = W2.matmul(A1) + b2;
    A2 = softmax(Z2);
}

Matrix ReLU_deriv(Matrix & m)
{
    return m > 0.0;
}

void backward_prop(Matrix & Z1, Matrix & A1, Matrix & A2, Matrix & W2, Matrix & X,  
                    Matrix & dZ2, Matrix & dW2, float & db2, Matrix & dZ1, Matrix & dW1, float & db1, 
                    Matrix & one_hot_Y)
{
    dZ2 = A2 - one_hot_Y;

    Matrix A1T(A1.rows, A1.cols);
    A1T = A1.transpose();
    dW2 = dZ2.matmul(A1T) * (1/m);

    db2 = dZ2.sum() * (1/m);

    Matrix ReLU_deriv_Z1(Z1.rows, Z1.cols);
    ReLU_deriv_Z1 = ReLU_deriv(Z1);
    dZ1 = (W2.transpose()).matmul(dZ2) * ReLU_deriv_Z1;

    Matrix XT(X.rows, X.cols);
    XT = X.transpose();
    dW1 = dZ1.matmul(XT) * (1/m);

    db1 = dZ1.sum() * (1/m);
}

void update_params(Matrix & W1, Matrix & b1, Matrix & W2, Matrix & b2, Matrix & dW1, float & db1, Matrix & dW2, float & db2, float alpha)
{
    dW1 = dW1 * alpha; // needs to be on separate line or the operator overloading doesn't recognize it correctly
    W1 = W1 - dW1;

    db1 = db1 * alpha;
    b1 = b1 - db1;

    dW2 = dW2 * alpha;
    W2 = W2 - dW2;

    db2 = db2 * alpha;
    b2 = b2 - db2;
}

Matrix get_predictions(Matrix & m)
{
    return m.argmax();
}
float get_accuracy(Matrix & predictions, Matrix & Y)
{
    return (predictions == Y).sum() / size_Y;
}

Matrix loadData(char const * PATH)
{
    //Load data (skipping first row) from csv file. Each row is the flattened input with the first integer as the target.
    char *token;
    FILE *file;
    file = fopen(PATH,"r");
    if (file == NULL)
    { 
        printf("FAILED");
        exit(1);
    }
    char line[7000];// Needs to be this big to hold the title row to then skip past it
    Matrix m(DATAROWS,DATACOLS);
    int i = 0;
    int j = 0;
    while(fgets(line,sizeof(line),file) && i < DATAROWS)
    {
        if (i>0)
        {//skips the first line of the data
            token = strtok(line, ","); 
            j = 0;
            while(token!=NULL && j<DATACOLS)
            {
                m.set(i-1, j, atof(token)); //I skipped the first line so i=1 to start. That's why I use i-1.
                //arr[i-1][j] = atof(token); //I skipped the first line so i=1 to start. That's why I use i-1.
                token = strtok(NULL, ",");
                j++;
            }
        }
        i++;
    }
    fclose(file);
    return m;
}
Matrix getTargets(Matrix & data)
{
    Matrix m(DATAROWS,1);
    for (int i=0; i<DATAROWS; i++)
    {
        m.set(i,0,data(i,0));
    }
    return m;
}
Matrix getInputs(Matrix & data)
{
    Matrix m(DATAROWS,DATACOLS-1);
    for (int i=0; i<DATAROWS; i++)
    {
        for (int j=1; j<DATACOLS; j++)
        {
            m.set(i,j-1,data(i,j));
        }
    }
    return m;
}