#include <vector> // do i include this here?
#include <iostream>
// https://www.youtube.com/watch?v=OmzU_-zbE7o&list=PL_xRyXins848nDj2v-TJYahzvs-XW9sVV&index=7


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////// from c attempt DELETE LATER//////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Matrix
{
public:
    int rows;
    int cols;
    std::vector<std::vector<float> > v2;
    // ??? should i make a constructor with no inputs that does not initialize a v2. 
    //It could be faster when I am going to change the reference any ways
    Matrix(){}
    Matrix(int n, int m): rows(n), cols(m), v2(n, std::vector<float>(m, 0)){} //using constructor list initializer method
    ~Matrix()
    {
       // printf("NEED TO IMPLEMENT MEMORY FREEING RAII\n");
    }

    ///////////////////////// Operator overloading /////////////////////////
    float& operator() (int i, int j)
    {
        return v2[i][j];
    }
    Matrix operator + (float rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j, v2[i][j] + rhs);
            }
        }
        return m;
    }
    Matrix operator + (Matrix & rhs)
    {
        Matrix m(rows, cols);
        if (cols == rhs.cols && rows == rhs.rows)
        {
            for(int i=0; i<rows; i++)
            {
                for(int j=0; j<cols; j++)
                {
                    m.set(i,j,v2[i][j] + rhs(i,j));
                }
            }
            return m;
        }
        if (rows == rhs.rows && rhs.cols == 1)
        {
            for(int i=0; i<rows; i++)
            {
                for(int j=0; j<cols; j++)
                {
                    m.set(i,j,v2[i][j] + rhs(i,0));
                }
            }
            return m;
        }
        printf("The matrix being added must be 1d or the exact same shape.");
        exit(-1);
    }
    Matrix operator - (float rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j, v2[i][j] - rhs);
            }
        }
        return m;
    }
    Matrix operator - (Matrix & rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j, v2[i][j] - rhs(i,j) );
            }
        }
        return m;
    }
    Matrix operator * (Matrix & rhs) // multiply element wise
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j,v2[i][j] * rhs(i,j));
            }
        }
        return m;
    }
    Matrix operator * (float rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j, v2[i][j] * rhs );
            }
        }
        return m;
    }
    Matrix operator / (float rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j, v2[i][j] / rhs);
            }
        }
        return m;
    }
    Matrix operator > (float rhs)//each_element_is_greater2D
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                if(v2[i][j] > rhs)
                {
                    m.set(i,j,1);
                }
                else
                {
                    m.set(i,j,0);
                }
            }
        }
        return m;
    }
    Matrix operator == (Matrix & rhs)
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                if( fabs( v2[i][j] - rhs(i,j) ) < 0.001)
                {
                    m.set(i,j,1);
                }else
                {
                    m.set(i,j,0);
                }
            }
            
        }
        return m;
    }

    ///////////////////////// Functions /////////////////////////
    Matrix matmul(Matrix & rhs)
    {
        Matrix m(rows, rhs.cols);
        for(int i=0; i<rows; i++){
            for(int j=0; j<rhs.cols; j++){
                m.set(i,j,0);
                for(int k=0; k<cols; k++){
                    m.set(i,j, m(i,j) + v2[i][k] * rhs(k,j));
                }
            }
        }
        return m;
    }
    int size()
    {
        return rows * cols;
    }
    void randomize() //getRand implementation
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                v2[i][j] = ( (rand()%1000000) * 0.000001 ) - 0.5; //rand float from -0.5 to 0.5
            }
        }
    }
    
    void print()
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                printf("%f ", v2[i][j]);
            }
            printf("\n");
        }
    }
    void print(int i)
    {
        for(int j=0; j<cols; j++)
        {
            printf("%f ", v2[i][j]);
        }
        printf("\n");
    }
    void shape()
    {
        printf("%i rows, %i cols", rows, cols);
    }
    Matrix transpose()
    {
        Matrix m(cols, rows);
        for (int i=0; i<rows; i++)
        {
            for (int j=0; j<cols; j++)
            {
                m.set(j, i, v2[i][j]);
            }
        }
        
        return m;
    }
    Matrix max(float val) // maximum2D implementation
    {
        Matrix m(rows, cols);
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                if (v2[i][j] < val)
                {
                    m.set(i,j,val);
                }
                else
                {
                    m.set(i,j,v2[i][j]);
                }
            }
        }
        return m;
    }
    void set(std::vector<std::vector<float> > & vect)
    {
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                v2[i][j] = vect[i][j];
            }
        }
    }
    void set(int i, int j, float val)
    {
        v2[i][j] = val;
    }
    Matrix exp()
    {
        Matrix m(rows, cols);
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                m.set(i,j,std::exp(v2[i][j]));
            }
        }
        return m;
    }
    Matrix argmax(int axis = 0)
    {
        if(axis == 0)
        {
            Matrix m(1, cols);
            float hold_val;
            float hold_index;
            for(int i=0; i<cols; i++){
                hold_val = v2[0][i];
                hold_index = 0;
                for(int j=0; j<rows; j++){
                    if(v2[j][i]>hold_val){
                        hold_val = v2[j][i];
                        hold_index = j;
                    }
                }
                m.set(0,i,hold_index);
            }
            return m;
        }
        printf("argmax is only implemented for axis = 0");
        exit(-1); 
    }
    float sum()
    {
        float s = 0;
        for(int i=0; i<rows; i++)
        {
            for(int j=0; j<cols; j++)
            {
                s += v2[i][j];
            }
        }
        return s;
    }
};

