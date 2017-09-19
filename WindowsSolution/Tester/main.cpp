#include <stdio.h>

struct A {
	virtual void foo() { printf("A\n"); }
};

struct B : public A {
	virtual void foo() { printf("B\n"); };
};

int main() {
	B b = B();
	A * b2 = &b;
	//A & a = b;
	
	

	b2->foo();
}


