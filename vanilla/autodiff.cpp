
//https://www.youtube.com/watch?v=VMj-3S1tku0

#include <iostream>

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>
#include <cmath>

class Value
{
    public:
        float data;
        float grad = 0.0;
        Value *prev0;
        Value *prev1;
        std::string op;
        std::string name = " ";
        Value(){data = (-1 + (rand() / (float) RAND_MAX) * ( 2 ));}
        Value(float n, std::string operation = "", Value *p0 = NULL, Value *p1 = NULL)
        {
            data = n;
            op=operation;
            prev0 = p0;
            prev1=p1;
        }

        void _backward()
        {
            if (op == "+")
            {
                (*prev0).grad += 1.0 * grad;
                (*prev1).grad += 1.0 * grad;
            }else if (op == "*")
            {
                (*prev0).grad += (*prev1).data * grad;
                (*prev1).grad += (*prev0).data * grad;
            }else if (op == "tanh")
            {
                (*prev0).grad += (1 - pow(data,2)) * grad;
            }
            
            else
            {
                // should only show up if we hit the end of a chain
                //printf(" No backward implemented data %f grad %f\n", data, grad);
            }            
        }
        void operator = (float rhs)
        {
            data = rhs;
        }
        void operator = (Value other)
        {
            data = other.data;
            prev0 = other.prev0;
            prev1 = other.prev1;
        }
        Value operator + (float rhs)
        {
            Value other(rhs);
            Value out(data + other.data, "+", this, &other);
            return out;
        }
        Value operator + (Value &other)
        {
            Value out(data + other.data, "+", this, &other);
            //printf("running +: this = %p, other %p \n",this, other); //for testing 
            return out;
        }

        Value operator * (float rhs)
        {
            Value other(rhs);
            Value out(data * other.data, "*", this, &other);
            return out;
        }
        Value operator * (Value &other)
        {
            Value out(data * other.data, "*", this, &other);
            return out;
        }

        Value operator - (float rhs)
        {
            Value other(rhs);
            Value out(data - other.data, "-", this, &other);
            return out;
        }
        Value operator - (Value &other)
        {
            Value out(data - other.data, "-", this, &other);
            return out;
        }

        Value operator / (float rhs)
        {
            Value other(rhs);
            Value out(data / other.data, "/", this, &other);
            return out;
        }
        Value operator / (Value &other)
        {
            Value out(data / other.data, "/", this, &other);
            return out;
        }

        Value tanh()
        {
            Value out(std::tanh(data), "tanh", this);
            return out;
        }
        /*
        NOTE: In the tutorial a topological sort is used. This orders 
        the prev Value objects so backwards is called in the right
        order. I think I can get by without implementing it? prob not.
        Their method: 
         1 Topological sort into a list of the Value objects.
         2 Go through this list and run ._backward on each object.
        */
       void backward_iter()
       {
            _backward();
            printf("%p _backward() | grad %f\n",this, grad);
            if(prev0){(*prev0).backward_iter();}
            if(prev1){(*prev1).backward_iter();}
       }
       void backward()
       {
            grad = 1.0;
            backward_iter();
       }


        
        void print()
        {
            printf("%s | data: %f | grad: %f | op: %s | p: %p | prev %p %p\n",name.c_str(), data, grad, op.c_str(), this, prev0, prev1);
        }

};
int history(Value &v) // BUGGY DONT USE
{
    v.print();
    printf("\n----------------------\n");
    if(v.prev0){history((*v.prev0));}
    if(v.prev1){history((*v.prev1));}
    return 1;
}
Value sum(Value arr[], int size)
{
    Value *hold=new Value[size];
    hold[0] = arr[0] + 0;
    for(int i; i<size-1; i++)
    {
        hold[i+1] = hold[i] + arr[i+1];
    }
    return hold[size-1];
}





class Neuron
{
    public:
        int nin;
        Value *w;
        Value b;
        Value *out;
        Neuron(int n)
        {
            nin = n;
            w = new Value[n];
            out = new Value[nin+1];
        }
        ~Neuron(){delete [] w;}

        Value operator()(float *arr)
        {
            printf("in neuron ()\n");
            out[0] = arr[0];
            printf("arr[0] %p\n", &arr[0]);
            printf("out[0] %p\n", &out[0]);
            for(int i = 0; i<nin-1; i++)
            {
                printf("arr[%i] %p\n",i+1, &arr[i+1]);
                printf("out[%i] %p\n",i+1, &out[i+1]);
                
                out[i+1] = out[i] + arr[i+1];
                /*
                This issue is I am not creating a new 'out' object each time. So out is sitting in the 
                same place in memory. When I update it, it correctly changes data by adding BUT
                it refers to itself
                */
               /* 
               
               I NEED TO GET RID OF THESE SELF REFERENCING. WHY DOES IT ALWAYS REFERENCE ITSELF IN THIS ALORITHM?????
               
               
               */
            }
            out[nin] = out[nin-1] + b;
            return out[nin];
        }

        void print()
        {
            for(int i=0; i<nin; i++)
            {
                w[i].print();
            }
            b.print();
        }
};