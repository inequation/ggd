// this is a Windows-only sample due to the MS CRT assertion that allows ignoring

#include <crtdbg.h>

// emulation of some 3rd-party library doing work for us
void DoSomeWork(int Argument)
{
	_ASSERT(Argument);
}

int main(int argc, char *argv[])
{
	// iterate many times - enough for the assert to get annoying
	for (int i = 0; i < 100000; ++i)
		DoSomeWork(i % 10 == 9);
}
