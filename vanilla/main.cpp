#include <vector>
#include "autodiff.cpp"

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>

void testneuron()
{
	////   2d neuron example    ////
	// input
	Value x1(2);
	Value x2(0);
	// weights
	Value w1(-3);
	Value w2(1);
	// bias of neuron
	Value b(6.8813735870195432);
	// Activate neuron
	Value x1w1 = x1*w1;
	Value x2w2 = x2*w2;
	Value x1w1x2w2 = x1w1 + x2w2;
	Value n = x1w1x2w2 + b;
	Value o = n.tanh();
	o.backward();
	history(o);
}

void testsum()
{
	Value v1(1);
	Value v2(2);
	Value v3(3);
	Value v4(4);
	Value lst[] = {v1,v2,v3,v4};
	Value v5 = sum(lst, 4);
	printf("\n");
	(v5).print();
	history(v5);
}
void testp()
{
	Value a(2); a.name = "a";
	Value b(1);
	Value c;
	c = (a + b); c.name = "c";
	c.print();

	a.print();
	(*c.prev0).print();
	b.print();
	(*c.prev1).print();

	printf("\n--history--\n");
	history(c);
}
void testnode()
{
	Neuron n(2);
	float arr[] = {1,2};
	printf("arr[0] %p\narr[1] %p\n",&arr[0], &arr[0]);
	Value result = n(arr);

	result.print();
	printf("\n result \n");
	//history(result);

	(*result.prev0).print();
	(*result.prev1).print();
	printf("\n");

	
	
	(*(*result.prev0).prev0).print();
	(*(*result.prev0).prev1).print();
	//(*(*result.prev1).prev0).print(); //null
	//(*(*result.prev1).prev1).print(); //null
}
int main()
{
	//testneuron();
	//testsum();
	testnode();

	return 0;
}
