#include <stdio.h>

#include <string>

struct MyStruct {
	void foo() {
		printf("MyStruct::foo()\n");
	};
};

struct MyStruct2 : MyStruct {
	void foo() {
		printf("MyStruct2::foo()\n");
	}
};


int main(int argc, char ** argv) {
	MyStruct2 ms2;
	bool b1 = false;
	b1 |= true;

	if(b1)
		ms2.foo();
}