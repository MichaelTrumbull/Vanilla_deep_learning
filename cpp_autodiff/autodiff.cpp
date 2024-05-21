#include <vector>
#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>

const int nin = 2;

typedef enum {
  NONE,
  ADD,
  SUBTRACT,
  MULTIPLY,
  POWER,
  RELU,
  TANH,
}backwardEnum;

class Val
{
public:
    float data;
    float grad = 0;
    int prevLen = 0;
    backwardEnum func = NONE;
    Val *prev [2];
    
    Val(){data = ( (rand()%1000000) * 0.000001 ) - 0.5;}
    Val(float v){data = v;}

    void _backward()
    {
        switch(func)
        {
            case ADD:
                prev[0]->grad = prev[0]->grad + grad;
                prev[0]->_backward();
                prev[1]->grad = prev[1]->grad + grad;
                prev[1]->_backward();
                break;
            case MULTIPLY:
                prev[0]->grad = prev[0]->grad + (prev[1]->data * grad);
                prev[0]->_backward();
                prev[1]->grad = prev[1]->grad + (prev[0]->data * grad);
                prev[1]->_backward();
                break;
            case RELU:
                prev[0]->grad = prev[0]->grad + ( ( data > 0 ) * grad );
                prev[0]->_backward();
                break;
            case TANH:
                prev[0]->grad = prev[0]->grad + ( (1 - (data * data)) * grad );
                prev[0]->_backward();
                break;
            default:
                printf("_backward for this case not implemented\n");
                break;
        }
    }

    Val operator + (Val & rhs) // should i return as a reference
    {
        Val result(this->data + rhs.data);
        result.prev[0] = this;
        result.prev[1] = &rhs;
        result.prevLen = 2;
        result.func = ADD;
        return result;
    }
    Val operator * (Val & rhs)
    {
        Val result(this->data * rhs.data);
        result.prev[0] = this;
        result.prev[1] = &rhs;
        result.prevLen = 2;
        result.func = MULTIPLY;
        return result;
    }
    Val relu()
    {
        Val result(data);
        if (data < 0)
        {
            result.data = 0;
        }
        result.prev[0] = this;
        result.prevLen = 1;
        result.func = RELU;
        return result;
    }
    Val tanh()
    {
        Val result((std::exp(2*data) - 1) / (std::exp(2*data) + 1));
        result.prev[0] = this;
        result.prevLen = 1;
        result.func = TANH;
        return result;
    }

    void p()
    {
        printf("%p  data %f  grad %f",this,data, grad);
        for(int i = 0; i < prevLen; i++)
        {
            printf("  prev[%i] %p", i, prev[i]);
        }
        printf("\n");
    }
    void backward()
    {
        // This BASIC method relies on no circular references. 
        // if the graph gets more complicated then use topo sort instead.
        grad = 1;
        _backward();
    }
};

class Neuron
{
public:
    Val b;
    Val w[nin];
    Val s[nin-1]; // to be used in the final summation
    Neuron()
    {
        /*
        ... making a variable length array on the heap is becoming a headache. "Segmentation fault" is 
        ... thrown when I want to access and element, and when I want to run a destructor and 
        ... "delete [] w;" it enters an inf loop for some reason.
        Val * w = new Val[nin];
        */
    }
    Val operator() (float arr[])
    {
        float total = 0;
        for (int i = 0; i < nin; i++)
        {
            total = total + (w[i].data * arr[i]);
        }
        total = total + b.data;
        Val weights(total);
        weights.prevLen = nin;
        for(int i = 0; i<nin; i++)
        {
            weights.prev[i] = &w[i];
        }
        Val sum = weights + b;
        weights.p();
        Val out = sum.tanh();
        return out;  ////// IM doing this wrong.......
    }
    Val sum()
    {
        // sum up via s[nin-1] and return the result
        return s[nin-1];
    }
    void p()
    {
        for(int i = 0; i < nin; i++)
        {
            w[i].p();
        }
        b.p();
        printf("\n");
    }

};


/* Topological sort in c++.
*  Note: Val *topo[] could instead be a vector so the size doesn't need to be known
*        before hand. BUT vectors can be annoying in how they make copies of things.
*        I forget what the issue was before but different references were created.
*/
// Val *topo[10]; // this could be a vector (or could i just use the set?)
// int topoIndex = 0;
// std::set <Val *> visited;
// void build_topo(Val * v)
// {
//     if (visited.count(v) == 0)// if v not in visited
//     {
//         visited.insert(v);
//         for(int i = 0; i < v->prevLen; i++)
//         {
//             build_topo(v->prev[i]);
//         }
//         topo[topoIndex] = v;
//         topoIndex = topoIndex + 1;
//     }
// }

