/*

DONT USE


This class uses vectors to hold objects which copies them each time.
This has no use for the neccessary backpropogation

*/































/*
https://www.youtube.com/watch?v=VMj-3S1tku0

1:09:

*/


#include <vector> // do i include this here?
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
        std::vector<Value> prev; // should this be  a std::set?
        std::string op = "";
        Value( float n)
        {
            data = n;
        }
        Value(float n, std::vector<Value> & children, std::string operation){data = n;prev = children;op=operation;}
        /*Value(float n, Value * first_child, std::string operation)
        {
            data = n;
            prev0 = first_child;
            op=operation;
        }
        Value(float n, Value * first_child, Value * second_child, std::string operation)
        {
            data = n;
            prev0 = first_child; 
            prev1 = second_child;
            op=operation;
        }*/
        /*Value(float n, Value * first_child, std::string operation)
        {
            data = n;
            prev0 = first_child;
            op = operation;
        }
        Value(float n, Value * first_child, Value * second_child, std::string operation)
        {
            data = n;
            prev0 = first_child;
            prev1 = second_child;
            op = operation;
        }*/
        

        void _backward()
        {
            if (op == "+")
            {
                prev[0].grad += 1.0 * grad;
                prev[1].grad += 1.0 * grad;
                //(*prev0).grad += 1.0 * grad;
                //(*prev1).grad += 1.0 * grad;
            }else if (op == "*")
            {
                prev[0].grad += prev[1].data * grad;
                prev[1].grad += prev[0].data * grad;
                //(*prev0).grad += (*prev1).data * grad;
                //(*prev1).grad += (*prev0).data * grad;
            }else if (op == "tanh")
            {
                //printf("running backward tanh setting prev[0] += %f\n", (1 - pow(data,2)) * grad);
                prev[0].grad += (1 - pow(data,2)) * grad;
                //(*prev0).grad += (1 - pow(data,2)) * grad;
                //prev[0].print();
                //printf("New prev[0] = %f\n",prev[0].grad);
                //prev[0].print();
            }
            
            
            else
            {
                // should only show up if we hit the end of a chain
                //printf(" No backward implemented data %f grad %f\n", data, grad);
            }            
        }

        Value operator + (float rhs)
        {
            Value other(rhs);
            std::vector<Value> p = {*this, other}; // there is no object info here. I'm just copying data which is a float...
            Value out(data + other.data, p, "+");
            //Value out(data + other.data, *this, &other, "+");
            return out;
        }
        Value operator + (Value other)
        {
            std::vector<Value> p = {*this, other}; // there is no object info here. I'm just copying data which is a float...
            Value out(data + other.data, p, "+");
            //Value out(data + other.data, *this, &other, "+");
            return out;
        }

        Value operator * (float rhs)
        {
            Value other(rhs);
            std::vector<Value> p = {*this, other};
            Value out(data * other.data, p, "*");
            //Value out(data * other.data, this, &other, "*");
            return out;
        }
        Value operator * (Value other)
        {
            std::vector<Value> p = {*this, other};
            Value out(data * other.data, p, "*");
            //Value out(data * other.data, this, &other, "*");
            return out;
        }

        Value operator - (float rhs)
        {
            Value other(rhs);
            std::vector<Value> p = {*this, other};
            Value out(data - other.data, p, "-");
            //Value out(data - other.data, this, &other, "-");
            return out;
        }
        Value operator - (Value other)
        {
            std::vector<Value> p = {*this, other};
            Value out(data - other.data, p, "-");
            //Value out(data - other.data, this, &other, "-");
            return out;
        }

        Value operator / (float rhs)
        {
            Value other(rhs);
            std::vector<Value> p = {*this, other};
            Value out(data / other.data, p, "/");
            //Value out(data / other.data, this, &other, "/");
            return out;
        }
        Value operator / (Value other)
        {
            std::vector<Value> p = {*this, other};
            Value out(data / other.data, p, "/");
            //Value out(data / other.data, this, &other, "/");
            return out;
        }

        Value tanh()
        {
            std::vector<Value> p = {*this};
            Value out(std::tanh(data), p, "tanh");
            //Value out(std::tanh(data) , this, "tanh");
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
            printf("\nprev size %li\n",prev.size());
            if(prev.size()>0)
            {
                prev[0].backward_iter();
                if(prev.size()>1)
                {
                    prev[1].backward_iter();
                }
            }
       }
       void backward()
       {
            printf("\nprev size %li\n",prev.size());
            grad = 1.0;
            if(prev.size()>0)
            {
                prev[0].backward_iter();
                if(prev.size()>1)
                {
                    prev[1].backward_iter();
                }
            }
       }


        
        void print()
        {
            printf("data: %f | grad: %f | op: %s | p: %p\n", data, grad, op.c_str(), this);
        }
        void printop()
        {
            printf("%s ", op.c_str());
        }
        void printprev()
        {
            //if(prev0 != NULL){(*prev0).print();}
            prev[0].print();
            printf("%s ", op.c_str());
            if(prev.size() > 0)
            {
                prev[1].print();
            }
            //if(prev1 != NULL){(*prev1).print();}
            
        }
        void printgrad()
        {
            printf("%f \n ", grad);
        }
};
int history(Value v) // BUGGY DONT USE
{
    v.print();
    printf("\n");
    v.printprev();
    printf("\n---\n");
    if (!v.prev.empty())
    {
        history(v.prev[0]);
        if (v.prev.size() == 2){history(v.prev[1]);}
    }
    return 1;
}
Value sum(std::vector<Value> & in)
{
    std::vector<Value> out;
    for(int i; i<in.size(); i++)
    {
        if(i==0){out.push_back(in[i]);}
        else
        {
            out.push_back(in[i] + out[i-1]);
        }
        
    }
    return out[in.size() - 1];
}
