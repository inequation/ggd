#include <cstdio>
#include <vector>

using namespace std;

#if _WIN32 || _WIN64
	// for DebugBreak()
	#include <Windows.h>
#elif __GNUC__
	#define DebugBreak()	volatile asm("int 3")
#else
	#warning How to cause a breakpoint on this platform?
	#define DebugBreak()
#endif

class Material
{
public:
	Material() : Index(Counter++) {}
	inline int GetIndex() const {return Index;}

private:
	int			Index;
	static int	Counter;
};

int Material::Counter = 0;

class Object
{
public:
	Object(int InIndex, Material *InMaterial) : Index(InIndex), ObjMaterial(InMaterial) {}
	inline int GetIndex() const {return Index;}

	inline void SetMaterial(Material *InMaterial)
	{
		if (ObjMaterial == InMaterial)
			return;
		delete ObjMaterial;
		ObjMaterial = InMaterial;
	}

private:
	int			Index;
	Material	*ObjMaterial;
};

int main(int argc, char *argv[])
{
	// create a handful of objects
	static const size_t ObjectCount = 100000;
	vector<Object *> Objects(ObjectCount);
	for (size_t i = 0; i < Objects.size(); ++i)
		Objects[i] = new Object(i, new Material());

	while (true)
	{
		for (auto it = Objects.begin(); it != Objects.end(); it++)
		{
			const int Chance = rand() % 100;
			if (Chance < 2)
				(*it)->SetMaterial(nullptr);
			else if (Chance < 40)
				(*it)->SetMaterial(new Material);
		}
	}

	return 0;
}
