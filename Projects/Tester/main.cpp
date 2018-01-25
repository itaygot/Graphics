#include <stdio.h>

#include <string>

int main(int argc, char ** argv) {
	std::string str = "abc";
	str.insert(1, "d", 1);
	printf("%s\n", str.c_str());
}