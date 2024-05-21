#include <vector>
#include "autodiff.cpp"

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>


int main()
{
	printf("hello world\n");
	Value v0(4);
	Value v1(2);
	Value v2 = v0 + v1;

	v2.print();
	printf("= ");
	v2.prev[0].print();
	v2.printop();
	v2.prev[1].print();
	


	return 0;
}
