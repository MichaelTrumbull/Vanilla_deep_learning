#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>
#define MAXCHAR 1000

//#define DATACOLS 785
//#define DATAROWS 60000
const int DATACOLS = 785;
const int DATAROWS = 60000;
const float size_Y = 59000; // used for normalization. In the python version I compute size(Y) but its constant so who cares.
const float eps = 0.00000001;
const float m = 60000.0;
const float alpha = 0.05; //learning rate
const int iterations = 20;
///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  C tools  ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


float **getArray2D(int rows,int cols){
    /* Build 2d array in the heap of any dimensin. Returns via a double pointer. s*/
    // instead of using static in the stack, build a 2d array in the heap and return its reference.
    float **arr = (float **)malloc(rows * sizeof(float *));
    int i;
    for (i = 0; i < rows; i++)
        arr[i] = (float*)malloc(cols * sizeof(float)); 
    // LOOP HERE TO SET ALL TO ZERO
    for(i = 0; i<rows; i++){
        for(int j=0; j<cols; j++){
            arr[i][j] = 0;
        }
    }
    return arr;
}
float *getArray1D(int dim1){
    /* Build 1d array in the heap of any dimensin. Returns via a double pointer. s*/
    float *arr = (float*)malloc(dim1 * sizeof(float));
    return arr;
}

// THERE IS SOMETHING VERY WRONG WITH FREEARRAY2D AND I DONT KNOW WHY
void freeArray2D(float **arr, int dim1, int dim2){ 
    for(int i = 0; i < dim1; i++){
        free(arr[i]);
    }
    free(arr);
}
void freeArray1D(float *arr){
    free(arr);
}
float **loadData(char* PATH){
    /*
    Load data (skipping first row) from csv file. Each row is the flattened input with the first integer as the target.
    */
    int j;
    int i;
    char *token;
    FILE *file;
    file = fopen(PATH,"r");
    if (file == NULL){ 
        printf("FAILED");
        exit(1);
    }
    char line[7000];// Needs to be this big to hold the title row to then skip past it
    float **arr = getArray2D(DATAROWS,DATACOLS);
    i = 0;
    j = 0;
    while(fgets(line,sizeof(line),file) && i < DATAROWS){
        if (i>0){//skips the first line of the data
            token = strtok(line, ","); 
            j = 0;
            while(token!=NULL && j<DATACOLS){
                arr[i-1][j] = atof(token); //I skipped the first line so i=1 to start. That's why I use i-1.
                token = strtok(NULL, ",");
                j++;
            }
        }
        i++;
    }
    fclose(file);
    return arr;
}
float *getTargets(float** data){
    float *targets;
    targets = getArray1D(DATAROWS);
    for (int i = 0; i<DATAROWS;i++){
        targets[i] = data[i][0];
    }
    //printArray1D(targets,DATAROWS);
    return targets;
}
float **getInputs(float** data){
    float **inputs;
    inputs = getArray2D(DATAROWS,DATACOLS-1);
    int i;
    int j;
    for (i = 0; i<DATAROWS; i++){
        for (j = 1; j<DATACOLS;j++){
            inputs[i][j-1] = data[i][j];
        }
    }
    return inputs;
}
void printArray1D(float* arr, int len){
    for (int i = 0; i < len; i++){
        printf("%f ",arr[i]);
    }
}
void printArray2D(float** arr, int rows, int cols){
    for (int i = 0; i<rows;i++){
        printArray1D(arr[i],cols);
        printf("\n");
    }
    printf("\n");
}
float **copy(float **arr, int dim1, int dim2){
    float **arrB = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrB[i][j] = arr[i][j];
        }
    }
    return arrB;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  MATH functions  ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


float **transpose(float **arr, int dim1, int dim2){
    float **arrT;
    arrT = getArray2D(dim2,dim1);
    int i;
    int j;
    for (i=0;i<dim1;i++){
        for (j=0;j<dim2;j++){
            arrT[j][i] = arr[i][j];
        }
    }
    return arrT;
}
void TESTtranspose(){
    float **testArr = getArray2D(2,3);
    testArr[0][0] = 0; testArr[0][1] = 1; testArr[0][2] = 2;
    testArr[1][0] = 3; testArr[1][1] = 4; testArr[1][2] = 5;
    float **testArrT = transpose(testArr,2,3);
    printArray2D(testArr,2,3);
    printf("\n");
    printArray2D(testArrT,3,2);
    freeArray2D(testArr, 2, 3);
    freeArray2D(testArrT, 2, 3);
}



/*
def rand(dim1,dim2): # offset value included?
    list_rand_floats = [[random.uniform(0,1) - 0.5 for _ in range(dim2)] for _ in range(dim1)]
    return list_rand_floats
*/
float **getRand(int dim1, int dim2){
    float **arr = getArray2D(dim1,dim2);
    for(int i = 0; i < dim1; i++){
        for(int j = 0; j < dim2; j++){
            arr[i][j] = ( (rand()%1000000) * 0.000001 ) - 0.5;//rand float from -0.5 to 0.5
        }
    }
    return arr;
}
void TESTgetRand(){
    float **arr = getRand(3,4);
    printArray2D(arr,3,4);
    freeArray2D(arr, 2, 3);
}



float **maximum2D(float **arr,int dim1, int dim2, float compare_val){
    float **arr2 = getArray2D(dim1,dim2);
    for(int i = 0; i < dim1; i++){
        for(int j = 0; j < dim2; j++){
            if (arr[i][j] < compare_val){
                arr2[i][j] = compare_val;
            }
            else{
                arr2[i][j] = arr[i][j];
            }
        }
    }
    return arr2;
}
void TESTmaximum2D(){
    float **arr = getRand(2,3);
    float **arr2 = maximum2D(arr, 2,3, 0);
    printArray2D(arr,2,3);
    printf("\n");
    printArray2D(arr2,2,3);
    freeArray2D(arr, 2, 3);
    freeArray2D(arr2, 2, 3);
}



float **exp2D(float **arr, int dim1, int dim2){
    float **arr2 = getArray2D(dim1,dim2);
    for(int i = 0; i < dim1; i++){
        for(int j = 0; j < dim2; j++){
            arr2[i][j] = exp(arr[i][j]);
        }
    }
    return arr2;
}
void TESTexp2D(){
    float **arr = getRand(2,3);
    float **arr2 = exp2D(arr, 2,3);
    printArray2D(arr,2,3);
    printf("\n");
    printArray2D(arr2,2,3);
    freeArray2D(arr, 2, 3);
    freeArray2D(arr2, 2, 3);
}



float **zeros(int dim1, int dim2){
// 2d array of zeros /////////////////////////////////////////////////////
    float **arr = getArray2D(dim1,dim2);
    for(int i = 0; i < dim1; i++){
        for(int j = 0; j< dim2; j++){
            arr[i][j] = 0;
        }
    }
    return arr;
}
void TESTzeros(){
    float **arr = zeros(2,3);
    printArray2D(arr,2,3);
    freeArray2D(arr, 2, 3);
}


float *arange1D(int n){
    float *arr = getArray1D(n);
    for(int i = 0; i < n; i++){
        arr[i] = i;
    }
    return arr;
}
void TESTarange1D(){
    float *arr = arange1D(7);
    printArray1D(arr, 7);
    freeArray1D(arr);
}


/*
float argmax1D(float *arr, int dim){
    float hold_val = arr[0];
    float hold_index = 0;
    for(int i=0; i<dim; i++){
        if(arr[i]>hold_val){
            hold_val = arr[i];
            hold_index = i;
        }
    }
    return hold_index;
}*/
float *argmax2D(float **arr,int axis, int dim1, int dim2){
    if(axis == 0)
    {
        float **arrT = transpose(arr, dim1, dim2);
        float *arr2 = getArray1D(dim2);
        float hold_val;
        float hold_index;
        //printf("start loop\n");
        for(int i=0; i<dim2; i++){
            hold_val = arrT[i][0]; // HERE IS THE ISSUE. WHY IS IT AN ISSUE WHEN I = 4. MAYBE A TRANSPOSE ISSUE.
            hold_index = 0;
            for(int j=0; j<dim1; j++){
                //printf("i %i j %i\n", i,j);
                //printf("arrT[%i][%i] %f\n",i,j,arrT[i][j]);
                if(arrT[i][j]>hold_val){
                    hold_val = arrT[i][j];
                    hold_index = j;
                }

            }
            arr2[i] = hold_index;
            
        }
        //printf("FINISHED LOOPS\n");
        //freeArray2D(arrT, dim2, dim1);
        return arr2;
    }
    printf("argmax2D only implemented for axis = 0");
    exit(-1); // i won't even implement the axis = 1 case.
}
void TESTargmax2D(){
    int dim1 = 3;
    int dim2 = 4;
    float **testArr = getArray2D(dim1,dim2);
    
    testArr[0][0] = 1; testArr[0][1] = 2; testArr[0][2] = 3; testArr[0][3] = -1;
    testArr[1][0] = 3; testArr[1][1] = 8; testArr[1][2] = 1; testArr[1][3] = 0;
    testArr[2][0] = 9; testArr[2][1] = 1; testArr[2][2] = 2; testArr[2][3] = -8;
    printf("before argmax\n");
    float *arrArgmax = argmax2D(testArr, 0, dim1,dim2);

    printArray1D(arrArgmax, dim2);
    freeArray2D(testArr, dim1,dim2);
    freeArray1D(arrArgmax);
}



// set size(Y) as a constant called size_Y.
float ** dot(float ** arrA, float ** arrB, int dim1A, int dim2A, int dim1B, int dim2B){
    float **arrC = getArray2D(dim1A, dim2B);
    for(int i=0; i<dim1A; i++){
        for(int j=0; j<dim2B; j++){
            arrC[i][j] = 0;
            for(int k=0; k<dim1B; k++){
                arrC[i][j] += arrA[i][k] * arrB[k][j];
            }
        }
    }
    return arrC;
}
void TESTdot(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;

    int dim1B = 4;
    int dim2B = 3;
    float **arrB = getArray2D(dim1B,dim2B);
    arrB[0][0] = 3; arrB[0][1] = 2; arrB[0][2] = 1;
    arrB[1][0] = 5; arrB[1][1] = 4; arrB[1][2] = 3;
    arrB[2][0] = 8; arrB[2][1] = 7; arrB[2][2] = 6; 
    arrB[3][0] = 1; arrB[3][1] = 3; arrB[3][2] = 4;

    float **arrC = dot(arrA,arrB, dim1A, dim2A, dim1B, dim2B);
    printArray2D(arrC, dim1A, dim2B);

    freeArray2D(arrC, dim1A, dim2B);
    freeArray2D(arrA, dim1A, dim2A);
    freeArray2D(arrB, dim1B, dim2B);
}


// ignore def shape(A) from python file. It just return DATACOLS and DATAROWS
float **div_element_wise(float **arr, int dim1, int dim2, float n){
    float **arrB = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrB[i][j] = arr[i][j] / n;
        }
    }
    return arrB;
}
void TESTdiv_element_wise(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;

    float **arrB = div_element_wise(arrA, dim1A, dim2A, 4.0);
    printArray2D(arrB, dim1A, dim2A);

    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrA, dim1A, dim2A);
}


// add_vecs_element_wise is never actually used. So I won't implement it here.
float **add2DWith1D(float **arrA, float **arrB, int dim1, int dim2){ //add_2dw_with_1d
    float **arrC = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrC[i][j] = arrA[i][j] + arrB[i][0];
        }
    }

    return arrC;
}
void TESTadd2DWith1D(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    
    float **arrB = getArray2D(dim1A, 1);
    arrB[0][0] = 1; arrB[1][0] = 2; arrB[2][0] = 3;
    
    float **arrC = add2DWith1D(arrA,arrB,dim1A,dim2A);

    printArray2D(arrC,dim1A,dim2A);

    freeArray2D(arrA, dim1A, dim2A);
    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrC, dim1A, dim2A);
}


float sum2D(float **arr, int dim1, int dim2){
    float sum = 0.0;
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            sum += arr[i][j];
        }
    }
    return sum;
}
float sum1D(float *arr, int dim1){
    float sum = 0.0;
    for(int i=0; i<dim1; i++){
            sum += arr[i];
    }
    return sum;
}
void TESTsum2D(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    printf("%f", sum2D(arrA,dim1A,dim2A));
    freeArray2D(arrA, dim1A, dim2A);
}


float **subtract_lists(float **arrA, float **arrB, int dim1, int dim2){
    //printf("subtract_lists(): get 2d array");
    float **arrC = getArray2D(dim1,dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            //printf("subtract_lists(): i %i j %i", i,j);
            arrC[i][j] = arrA[i][j] - arrB[i][j];
        }
    }
    return arrC;
}
void TESTsubtract_lists(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;

    int dim1B = 3;
    int dim2B = 4;
    float **arrB = getArray2D(dim1B,dim2B);
    arrB[0][0] = 1; arrB[0][1] = 4; arrB[0][2] = 7; arrB[0][3] = 10;
    arrB[1][0] = 2; arrB[1][1] = 5; arrB[1][2] = 8; arrB[1][3] = 11;
    arrB[2][0] = 3; arrB[2][1] = 6; arrB[2][2] = 9; arrB[2][3] = 12;

    float **arrC = subtract_lists(arrA,arrB, dim1A, dim2A);
    printArray2D(arrC, dim1A, dim2A);

    freeArray2D(arrC, dim1A, dim2B);
    freeArray2D(arrA, dim1A, dim2A);
    freeArray2D(arrB, dim1A, dim2A);
}


float **mult_element_wise2D(float val, float **arrA, int dim1, int dim2){
    float **arrB = getArray2D(dim1,dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrB[i][j] = arrA[i][j] * val;
        }
    }
    return arrB;
}
void TESTmult_element_wise2D(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    float **arrB = mult_element_wise2D(2.0, arrA, dim1A, dim2A);
    printArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrA, dim1A, dim2A);
}


float **each_element_is_greater2D(float **arrA, float val, int dim1, int dim2){
    float **arrB = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            if(arrA[i][j] > val){
                arrB[i][j] = 1.0;
            }else{
                arrB[i][j] = 0.0;
            }
        }
    }
    return arrB;
}
void TESTeach_element_is_greater(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    float **arrB = each_element_is_greater2D(arrA, 2.0, dim1A, dim2A);
    printArray2D(arrB, dim1A, dim2A);

    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrA, dim1A, dim2A);
}


float **mult_lists(float **arrA, float **arrB, int dim1, int dim2){
    float **arrC = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrC[i][j] = arrA[i][j] * arrB[i][j];
        }
    }
    return arrC;
}
void TESTmult_lists(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    float **arrB = mult_lists(arrA, arrA, dim1A, dim2A);
    printArray2D(arrB, dim1A, dim2A);

    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrA, dim1A, dim2A);
}


float **subtract_val_from_list2D(float **arrA, float val, int dim1, int dim2){
    float ** arrB = getArray2D(dim1, dim2);
    for(int i=0; i<dim1; i++){
        for(int j=0; j<dim2; j++){
            arrB[i][j] = arrA[i][j] - val;
        }
    }
    return arrB;
}
void TESTsubtract_val_from_list2D(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;
    float **arrB = subtract_val_from_list2D(arrA, 1.0, dim1A, dim2A);
    printArray2D(arrB, dim1A, dim2A);

    freeArray2D(arrB, dim1A, dim2A);
    freeArray2D(arrA, dim1A, dim2A);
}


float *compare_bool_lists(float *arrA, float *arrB, int dim){
    float *arrC = getArray1D(dim);
    for(int i=0; i<dim; i++){
        if( fabs( arrA[i] - arrB[i] ) < eps){
            arrC[i] = 1;
        }else{
            arrC[i] = 0;
        }
    }
    return arrC;
}
void TESTcompare_bool_lists(){
    int dim = 4;
    float *arrA = getArray1D(dim);
    arrA[0] = 3.0; arrA[1] = 0.0; arrA[2] = 6.0; arrA[3] = 9.0; 

    float *arrB = getArray1D(dim);
    arrB[0] = 3.0; arrB[1] = 2.0; arrB[2] = 6.0; arrB[3] = 8.0; 

    float *arrC = compare_bool_lists(arrA, arrB, dim);
    printArray1D(arrC, dim);

    freeArray1D(arrA);
    freeArray1D(arrB);
    freeArray1D(arrC);
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// 2 LAYER FEED FORWARD NETWORK FUNCTIONS ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float **ReLU(float **arr, int dim1, int dim2){
    return maximum2D(arr, dim1, dim2, 0.0);
}

float **softmax(float **arr, int dim1, int dim2){
    return div_element_wise( exp2D(arr, dim1, dim2), dim1, dim2, sum2D(exp2D(arr, dim1, dim2), dim1, dim2) );
}
void TESTsoftmax(){
    float ** arr = getRand(10, 10);
    printArray2D(arr, 10,10);
    printf("\n");
    printArray2D(softmax(arr,10,10),10,10);
    freeArray2D(arr, 10,10);
}


/* SHAPES
W1:  (10, 784)
b1:  (10, 1)
W2:  (10, 10)
b2:  (10, 1)
X:  (784, 59000)
Z1:  (10, 59000)
A1:  (10, 59000)
Z2:  (10, 59000)
A2:  (10, 59000)
*/
void forward_prop(float ***W1, float ***b1, float ***W2, float ***b2, float ***X, 
                  float ***Z1, float ***A1, float ***Z2, float ***A2){ // try to not return things and just modify matricies via their references.
    //freeArray2D(Z1, 10, 59000);
    *Z1 = add2DWith1D( dot(*W1, *X, 10, 784, 784, 59000) , *b1, 10, 59000);
    //freeArray2D(A1, 10, 59000);
    *A1 = ReLU(*Z1, 10, 59000);
    //freeArray2D(Z2, 10, 59000);
    *Z2 = add2DWith1D( dot(*W2, *A1, 10, 10, 10, 59000) , *b2, 10, 59000);
    //freeArray2D(A2, 10, 59000);
    //printf("A2 before\n");
    //printArray2D(A2, 5, 5);
    *A2 = softmax(*Z2, 10, 59000);
    //printf("A2 after\n");
    //printArray2D(A2, 5, 5);

}

void TESTforward_prop(){
    printf("Testing forward_prop()\n");
    float ** W1 = getRand(10, 784); //rand(10, 784)
    float ** b1 = getRand(10, 1); //rand(10, 1) 
    float ** W2 = getRand(10, 10); //rand(10, 10) 
    float ** b2 = getRand(10, 1); //rand(10, 1) 
        
    float ** Z1 = getArray2D(10,59000);
    float ** A1 = getArray2D(10,59000);
    float ** Z2 = getArray2D(10,59000);
    float ** A2 = getArray2D(10,59000);

    float ** X = getRand(784, 59000);

    printf("Z1\n");
    printArray2D(Z1, 5,5);
    printf("A1\n");
    printArray2D(A1, 5,5);
    printf("Z2\n");
    printArray2D(Z2, 5,5);
    printf("A2\n");
    printArray2D(A2, 5,5);

    forward_prop(&W1, &b1, &W2, &b2, &X, &Z1, &A1, &Z2, &A2);
    printf("Z1\n");
    printArray2D(Z1, 5,5);
    printf("A1\n");
    printArray2D(A1, 5,5);
    printf("Z2\n");
    printArray2D(Z2, 5,5);
    printf("A2\n");
    printArray2D(A2, 5,5);



}

float **ReLU_deriv(float **arr, int dim1, int dim2){
    return each_element_is_greater2D(arr, 0.0, dim1, dim2);
}
void TESTReLU_deriv(){

}
void backward_prop(float ***Z1, float ***A1, float ***A2, float ***W2, float ***X,  
                    float ***dZ2, float ***dW2, float *db2, float ***dZ1, float ***dW1, float *db1, 
                    float ***one_hot_Y){
    *dZ2 = subtract_lists( *A2 , *one_hot_Y , 10, 59000); //I HAVE DIMS WRONG HERE?
    *dW2 = mult_element_wise2D( 1 / m , dot(*dZ2, transpose(*A1, 10, 59000), 10, 59000, 59000, 10) , 10, 10);
    *db2 = 1 / m * sum2D(*dZ2, 10, 59000);
    *dZ1 = mult_lists( dot(transpose(*W2, 10, 10), *dZ2, 10, 10, 10, 59000) , ReLU_deriv(*Z1, 10, 59000), 10, 59000 );
    *dW1 = mult_element_wise2D( 1 / m , dot(*dZ1, transpose(*X, 784, 59000), 10, 59000, 59000, 784), 10, 784 );
    *db1 = 1 / m * sum2D(*dZ1, 10, 59000);
    //printf("backward_prop sum2D(dZ1, 10, 59000) %f\n", sum2D(*dZ1, 10, 59000));
}
void TESTbackward_prop(){
    float ** W1 = getRand(10, 784); //rand(10, 784)
    float ** b1 = getRand(10, 1);
    float ** W2 = getRand(10, 10); //rand(10, 10) 
    float ** b2 = getRand(10, 1); //rand(10, 1) 
        
    //float ** Z1 = getRand(10,59000);
    //float ** A1 = getRand(10,59000);
    //float ** A2 = getRand(10,59000);
    float ** Z1 = getArray2D(10,59000);
    float ** A1 = getArray2D(10,59000);
    float ** Z2 = getArray2D(10,59000);
    float ** A2 = getArray2D(10,59000);

    float ** X = getRand(784, 59000);

    forward_prop(&W1, &b1, &W2, &b2, &X, &Z1, &A1, &Z2, &A2);

    float ** dZ2 = getArray2D(10,59000);
    float ** dW2 = getArray2D(10,10);
    float db2 = 0.0;
    float ** dZ1 = getArray2D(10,59000);
    float ** dW1 = getArray2D(10,784);
    float db1 = 0.0;

    float ** one_hot_Y = getRand(10, 59000);

    printf("dZ2\n");
    printArray2D(dZ2, 5,5);
    printf("dW2\n");
    printArray2D(dW2, 5,5);
    printf("db1\n");
    printf("%f\n", db1);
    printf("dZ1\n");
    printArray2D(dZ1, 5,5);
    printf("dW1\n");
    printArray2D(dW1, 5,5);
    printf("db2\n");
    printf("%f\n", db2);


    printf("----\n");
    backward_prop(&Z1, &A1, &A2, &W2, &X, &dZ2, &dW2, &db2, &dZ1, &dW1, &db1, &one_hot_Y);

    printf("dZ2\n");
    printArray2D(dZ2, 5,5);
    printf("dW2\n");
    printArray2D(dW2, 5,5);
    printf("db1\n");
    printf("%f\n", db1);
    printf("dZ1\n");
    printArray2D(dZ1, 5,5);
    printf("dW1\n");
    printArray2D(dW1, 5,5);
    printf("db2\n");
    printf("%f\n", db2);
}
void update_params(float ***W1, float ***b1, float ***W2, float ***b2, float ***dW1, float *db1, float ***dW2, float *db2, float alpha){
    float **tmpArr;

    //tmpArr = copy(W1, 10, 784);
    //freeArray2D(W1, 10, 784);
    *W1 = subtract_lists( *W1 , mult_element_wise2D( alpha , *dW1, 10, 784), 10, 784);
    //freeArray2D(tmpArr, 10, 784);

    //tmpArr = copy(b1, 10, 1);
    //freeArray2D(b1, 10, 1);
    *b1 = subtract_val_from_list2D(*b1, alpha * (*db1), 10, 1);
    //freeArray2D(tmpArr, 10, 1);


    //tmpArr = copy(W2, 10, 10);
    //freeArray2D(W2, 10, 10);
    *W2 = subtract_lists( *W2 , mult_element_wise2D( alpha , *dW2, 10, 10), 10, 10 );
    //freeArray2D(tmpArr, 10, 10);

    //tmpArr = copy(b2, 10, 1);
    //freeArray2D(b2, 10, 1);
    *b2 = subtract_val_from_list2D(*b2, alpha * (*db2), 10, 1);
    //freeArray2D(tmpArr, 10, 1);

}

float *get_predictions(float **arr, int dim1, int dim2){
    return argmax2D(arr, 0, dim1, dim2);
}

float get_accuracy(float *predictions, float *Y){
    return sum1D( compare_bool_lists(predictions, Y, 59000), 59000 ) / size_Y;
}
/*
def get_accuracy(predictions, Y):
    return sum(compare_bool_lists(predictions , Y)) / size(Y)
*/

void TESTAssignNewArray(){
    int dim1A = 3;
    int dim2A = 4;
    float **arrA = getArray2D(dim1A,dim2A);
    arrA[0][0] = 1; arrA[0][1] = 2; arrA[0][2] = 3; arrA[0][3] = -1;
    arrA[1][0] = 3; arrA[1][1] = 8; arrA[1][2] = 1; arrA[1][3] = 0;
    arrA[2][0] = 9; arrA[2][1] = 1; arrA[2][2] = 2; arrA[2][3] = -8;

    int dim1B = dim1A;
    int dim2B = dim2A;
    float **arrB = getArray2D(dim1B,dim2B);
    arrB[0][0] = 1; arrB[0][1] = 4; arrB[0][2] = 7; arrB[0][3] = 10;
    arrB[1][0] = 2; arrB[1][1] = 5; arrB[1][2] = 8; arrB[1][3] = 11;
    arrB[2][0] = 3; arrB[2][1] = 6; arrB[2][2] = 9; arrB[2][3] = 12;

    printArray2D(arrA, dim1A, dim2A);
    printArray2D(arrB, dim1A, dim2A);
    arrA = arrB;
    printArray2D(arrA, dim1A, dim2A);

    //freeArray2D(arrA, dim1A, dim2A);
    //freeArray2D(arrB, dim1A, dim2A);
}

void TESTpassDynamicArrays(float ***arr){// https://stackoverflow.com/questions/34844003/changing-array-inside-function-in-c
    float **arrB = getArray2D(4,4);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            arrB[i][j] = i+j;
        }
    }
    *arr = mult_element_wise2D(2, arrB, 4, 4);
}
int main(){
    //TESTtranspose();
    //TESTargmax2D();
    //printf("FINSIHED TESTS");
    //TESTsubtract_lists(); // subtract_lists is called to get dZ2. its failing for some reason.
    //TESTAssignNewArray(); // This tests what happens when i assign a new pointer value
    //TESTgetRand();
    //TESTmult_lists(); //dZ1 is not being filled?

    //TESTforward_prop();
    //TESTbackward_prop();

    // TESTpassDynaimcArrays
    /*
    float ** TESTarr = getArray2D(4,4);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            TESTarr[i][j] = 0;
        }
        printf("\n");
    }

    TESTpassDynamicArrays(&TESTarr);

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%f ", TESTarr[i][j]);
        }
        printf("\n");
    }
    */

    if (true)
    {
        printf("Loading data\n");

    // LOAD DATA //
    
    float **data;
    data = loadData("../../MNIST/mnist_train.csv");

    // SPLIT DATA INTO INPUT AND TARGET //
    float *Y;
    float **X;
    Y = getTargets(data); 
    // transpose data first or X ? 
    X = div_element_wise(transpose(getInputs(data),DATAROWS,DATACOLS-1), 784, 59000, 255.0); // might need to be trasnposed????
    
    //printArray1D(data[4], 784);
    //X = getRand(784, 59000);
    
    //shape(Y_train) 59000
    //shape(X_train) (784, 59000)
    
    
    //printArray1D(X[0],DATACOLS - 1);
    
    // INIT PARAMS //
    float ** W1 = getRand(10, 784); //rand(10, 784)
    float ** b1 = getRand(10, 1); //rand(10, 1) 
    float ** W2 = getRand(10, 10); //rand(10, 10) 
    float ** b2 = getRand(10, 1); //rand(10, 1) 
    
    //printArray2D(b1, 10,1);
    
    float ** Z1 = getArray2D(10,59000);
    float ** A1 = getArray2D(10,59000);
    float ** Z2 = getArray2D(10,59000);
    float ** A2 = getArray2D(10,59000);

    float ** dZ2 = getArray2D(10,59000);
    float ** dW2 = getArray2D(10,10);
    float db2 = 0.0;
    float ** dZ1 = getArray2D(10,59000);
    float ** dW1 = getArray2D(10,784);
    float db1 = 0.0; //                 When I send this I need to send it as (&db1), is received as *db1 then modified with *db1

    
    // build one_hot_Y
    // shape(one_hot_Y) (10, 59000) *But I used a different dimension? Its hard to read the python.
    float **tmp_one_hot_Y = getArray2D(59000, 10);
    int temp_val;
    
    for(int i=0; i<59000; i++){
        temp_val = Y[i];
        tmp_one_hot_Y[i][temp_val] = 1; // This might need a transpose ?????
    }
    float **one_hot_Y = transpose(tmp_one_hot_Y, 59000, 10);
    //freeArray2D(tmp_one_hot_Y, 59000, 10);

    //printf("%f\n", Y[30]);
    //printArray1D(one_hot_Y[30], 10);

    //

    // GRADIENT DESCENT
    //init_params() not needed since they have already been init above in main()
    float *predictions;

    
    for(int i=0; i<iterations; i++){
        //printf("db1 %f\n", db1);

        //printf("forward prop\n");
        forward_prop(&W1, &b1, &W2, &b2, &X, &Z1, &A1, &Z2, &A2);

        //printf("A2 after forward prop in main()\n");
        //printArray2D(A2, 5, 5);

        //printf("back prop\n");
        backward_prop(&Z1, &A1, &A2, &W2, &X, &dZ2, &dW2, &db2, &dZ1, &dW1, &db1, &one_hot_Y);
        //printf("update\n");
        update_params(&W1, &b1, &W2, &b2, &dW1, &db1, &dW2, &db2, alpha);
        printf("iteration: %i\n", i);
        predictions = get_predictions(A2, 10, 59000);
        printf("%f\n\n", get_accuracy(predictions, Y));
        //freeArray1D(predictions);
    }
    
    
    //def gradient_descent(X, Y, alpha, iterations):
    //    W1, b1, W2, b2 = init_params()
    //    for i in range(iterations):
    //        Z1, A1, Z2, A2 = forward_prop(W1, b1, W2, b2, X)
    //        dW1, db1, dW2, db2 = backward_prop(Z1, A1, Z2, A2, W1, W2, X, Y)
    //        W1, b1, W2, b2 = update_params(W1, b1, W2, b2, dW1, db1, dW2, db2, alpha)
    //        if True:#i % 10 == 0:
    //            print("Iteration: ", i)
    //            predictions = get_predictions(A2)
    //            print(get_accuracy(predictions, Y))
    //    return W1, b1, W2, b2

    
    } // end if statement
    //free(data);
	return 0;
}