#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h> 
#include<math.h>
















//
//
//
// USE THIS:
//    https://www.geeksforgeeks.org/flexible-array-members-structure-c/
//
// A flexible array member is exactly what i'm looking for. Not a VLA (that has issues because we need to know how big it is)
// Instead it leaves it up to us to say how much memory to use. Its perfect
// ** also use function pointers
// https://stackoverflow.com/questions/351733/how-would-one-write-object-oriented-code-in-c
// 
//
//
//
//

//typedef enum {
//  NONE,
//  ADD,
//  SUBTRACT,
//  MULTIPLY,
//  POWER,
//  RELU,
//  TANH,
//}backwardFunction;

typedef struct Value Val; // declared here so I can self refernce in the struct for prev[]
typedef struct Value
{
    float data;
    float grad;
    //backwardFunction _backward;
    void (*_backward)(Val *);
    int prevLen;
    Val * prev[];
}Val;

Val * getVal(float n, int len)
{
    Val * tmp = malloc(sizeof(Val) + sizeof(Val * [len])); // is this the right size?
    tmp->data = n;
    tmp->grad = 0;  //(*tmp).grad = 0;
    tmp->prevLen = len;
    //tmp->_backward = NONE;
    return tmp;
}
Val * getRandVal()
{
    Val * tmp = getVal( ( (rand()%1000000) * 0.000001 ) - 0.5 , 0);
    return tmp;
}



typedef struct Neuron_4
{
    Val w[4];
    Val b;
} N4;



void backward_add(Val * v)
{
    printf("This is backward ADD");
}
Val * add(Val * a, Val * b)
{
    Val * c = malloc(sizeof(Val));
    c->data = a->data + b->data;
    c->grad = 0;
    //c->_backward = ADD; 
    c->_backward = &backward_add;
    c->prevLen = 2;
    c->prev[0] = a; 
    c->prev[1] = b;
    return c;
}

void backward_mult(Val * v)
{
    printf("This is backward MULT");
}
Val * mul(Val * a, Val * b)
{
    Val * c = getVal(a->data * b->data, 2);
    //c->_backward = MULTIPLY; 
    c->_backward = &backward_mult;
    c->prevLen = 2;
    c->prev[0] = a; 
    c->prev[1] = b;
    return c;
}


/*
Val * tanh(Val * a)
{
    Val * c = getVal(69);
    c->_backward = TANH; 
    c->prevLen = 1;
    c->prev[0] = a; 
    return c;
}
*/

void pv(Val * v)
{
    printf("%p\n", v);
    printf("data %f\n",v->data);
    printf("grad %f\n",v->grad);
    //printf("_backward %i\n",v->_backward);
    printf("prev");
    for(int i = 0; i < v->prevLen; i++)
    {
        printf(" %p", v->prev[i]);
    }
    printf("\n\n");
}


int main()
{
    /*
    printf("hello world!");
    Val *a = getVal(1);
    Val *b = getVal(2);
    Val *c = add(a,b);
    int len = 3;
    Val *tmp[3] = {a,b,c};
    Val * d = sum(tmp, len);
    printf("%f", d->data);
    */


    Val * x1 = getVal(2.0, 0);
    //Val * x2 = getVal(0.0, 0);
    Val * w1 = getVal(-3.0, 0);
    //Val * w2 = getVal(1.0, 0);
    //Val * b = getVal(6.8813735870195432, 0);
    Val * x1w1 = mul(x1,w1);
    x1w1->_backward(x1w1);
    //Val * x2w2 = mul(x2,w2);
    //Val * x1w1x2w2 = add(x1w1, x2w2);
    //Val * n = add(x1w1x2w2, b);
    //Val * o = tanh(n);
    //backward(o);
    
    printf("x1\n");
    pv(x1);
    printf("w1\n");
    pv(w1);
    printf("x1w1\n");
    pv(x1w1);

    //Val * tmp = malloc(sizeof(Val));
    //tmp->data = 69;
    //tmp->grad = 420;
    //tmp->_backward = ADD;

    return 0;
}