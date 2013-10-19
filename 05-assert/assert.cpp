// Noise filtering sample "assert"
// Emulates an annoying, non-disablable assertion in a part of code we have no
// control over.
// Illustrated techniques: in-memory CPU opcode edition

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
