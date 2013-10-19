// Noise filtering sample "material"
// Emulates a slice of a game engine - creation of some objects, then a
// rudimentary main game loop.
// Illustrated techniques: breakpoint conditions, DebugBreak()

#include <vector>
#include <string>
#include <sstream>
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
	Object(string InName, Material *InMaterial)
		: Name(InName)
		, ObjMaterial(nullptr)
	{
		// use the setter on purpose to use the "game logic"
		SetMaterial(InMaterial);
	}

	inline string GetName() const {return Name;}

	// setter for material reference - emulates the presence of additional
	// game logic
	inline void SetMaterial(Material *InMaterial)
	{
		if (ObjMaterial == InMaterial)
			return;
		delete ObjMaterial;
		ObjMaterial = InMaterial;
	}

private:
	string		Name;
	Material	*ObjMaterial;
};

static inline void InitObject(Object*& ObjPtr, int Index)
{
	// create the object
	stringstream Builder;
	Builder << "Object" << Index;
	ObjPtr = new Object(Builder.str(), new Material());

	// this is the culprit we're looking for: for every 30 objects, have 10
	// have their material reset
	if ((Index / 10) % 3 == 2)
		ObjPtr->SetMaterial(nullptr);
}

int main(int argc, char *argv[])
{
	// create a handful of objects
	static const size_t ObjectCount = 100000;
	vector<Object *> Objects(ObjectCount);
	for (size_t i = 0; i < Objects.size(); ++i)
		InitObject(Objects[i], i);

	// emulate main game loop
	while (true)
	{
		for (auto it = Objects.begin(); it != Objects.end(); it++)
		{
			// emulate some gameplay-related state changes, reflected in
			// changing the material
			const int Chance = rand() % 100;
			if (Chance < 40)
				(*it)->SetMaterial(new Material);
		}
	}

	// no need to clean up since the loop above never exits and this program is
	// just for educational purposes :)

	return 0;
}
