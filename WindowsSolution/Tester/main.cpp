#include <stdio.h>

struct singletone {
	
	static singletone& getInstance() {		
		return s_instance;
	}

	singletone(const singletone &) = delete;
	singletone operator=(const singletone &) = delete;

private:
	singletone() {}
	
	static singletone s_instance;
};

singletone singletone::s_instance;

int main() {
	

}


