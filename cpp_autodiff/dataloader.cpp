#include <vector>
#include "autodiff.cpp"

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>

#define MAXCHAR 1000
const int DATACOLS = 785;
const int DATAROWS = 60000;
const float size_Y = 59000;
const float eps = 0.00000001;
const float m = 60000.0;
const float alpha = 0.1;
const int iterations = 9;

/*

THIS NEEDS TO BE REWORKED...
instead of holding data in a Matrix, build a new typedef
*/

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