#include <stdio.h>

#ifdef _WIN32
#	include <Windows.h>
#else
#	include <sys/time.h>
#endif

int main() {

	DWORD time_now, last_idle_time = GetTickCount();

	for (int i = 0; i < 10000000; i++);



	time_now = GetTickCount();

	DWORD dt = (time_now - last_idle_time);

	printf("%d\n", dt);

	printf("%f\n", 1.0e-3 * 2.f);
	printf("%d\n", 1 << 7);
	float f = 1.f;
	float g = 5.f;
	f *= -g;
	printf("%f\n", f);
	
}


