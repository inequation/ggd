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

// class to emulate a material for a 3D surface
class Material
{
public:
	// an index uniquely identifies a material instance
	Material() : Index(Counter++) {}
	inline int GetIndex() const {return Index;}

private:
	int			Index;

	// used to assign consecutive indices to new material instances
	static int	Counter;
};

int Material::Counter = 0;

// class to emulate a game object/actor/agent
class Object
{
public:
	// an index uniquely identifies an object
	Object(int InIndex, Material *InMaterial) : Index(InIndex), ObjMaterial(nullptr)
	{
		// use the setter on purpose to use the "game logic"
		SetMaterial(InMaterial);
	}

	inline int GetIndex() const {return Index;}

	// setter for material reference - emulates the presence of additional game logic
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
	{
		// create the object
		Objects[i] = new Object(i, new Material());
		// this is the culprit we're looking for: for every 30 objects, have 10 have their material reset
		if ((i / 10) % 3 == 0)
			Objects[i]->SetMaterial(nullptr);
	}

	// emulate main game loop
	while (true)
	{
		for (auto it = Objects.begin(); it != Objects.end(); it++)
		{
			// emulate some gameplay-related state changes, reflected in changing the material
			const int Chance = rand() % 100;
			if (Chance < 40)
				(*it)->SetMaterial(new Material);
		}
	}

	// no need to clean up since the loop above never exits and this program is just for educational purposes :)

	return 0;
}
