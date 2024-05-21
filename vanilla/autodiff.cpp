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

class Value
{
    public:
        float data;
        float grad = 0.0;
        std::vector<Value> prev;
        std::string op = "";
        Value(float n, std::vector<Value> children = {})
        {
            data = n;
            prev = children;
        }


        void backward(){}
        

        Value operator + (float rhs)
        {
            op = "+";

            Value rhs_v(rhs);
            Value v(data + rhs, {data, rhs_v});
            return v;
        }
        Value operator + (Value rhs_v)
        {
            op = "+";
            Value v(data + rhs_v.data, {data, rhs_v});
            return v;
        }



        
        void print()
        {
            printf("%f ", data);
        }
        void printop()
        {
            printf("%s. ", op.c_str());
        }
};