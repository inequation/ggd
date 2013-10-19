#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

// helper struct representing a 3D vector or point
struct Vector3
{
	Vector3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}
	Vector3(float InXYZ) : X(InXYZ), Y(InXYZ), Z(InXYZ) {}

	inline Vector3 operator+(const Vector3& Other) {return Vector3(X + Other.X, Y + Other.Y, Z + Other.Z);}

	inline Vector3& operator+=(const Vector3& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}

	// anonymous union and struct to allow direct access to members
	union
	{
		struct
		{
			float X, Y, Z;
		};
		float V[3];
	};
};

// class to emulate a material for a 3D surface
class Character
{
public:
	// enumeration for game object state
	typedef enum
	{
		ECS_Normal,
		ECS_Ragdoll,
	}
	CharacterState;

	// initialize to normal state, located at (0 0 0), with a random offset to life time, and clear ragdoll position
	Character(int InIndex) : Index(InIndex), State(ECS_Normal), Location(0.f), LifeTime((float)(rand() % 120)), RagdollLocation(0.f) {}
	inline int GetIndex() const {return Index;}
	inline Vector3 GetLocation() const {return Location;}
	
	// moves this object by Delta; separate method to emulate the game logic that usually comes with it - shortcut scene graph reconstruction etc.
	inline void RelativeMove(const Vector3& Delta)
	{
		SetLocation(Location + Delta);
	}

	// setter to emulate the game logic that usually comes with it - scene graph reconstruction etc.
	inline void SetLocation(const Vector3& NewLoc) {
		Location = NewLoc;
	}

	// separate method to emulate game logic
	inline void StartRagdoll()
	{
		State = ECS_Ragdoll;
		SetLocation(RagdollLocation);
		LifeTime = 0.f;
	}

	// separate method to emulate game logic
	inline void StopRagdoll()
	{
		State = ECS_Normal;
		LifeTime = 0.f;
	}

	// tells the object to die; separate method to emulate game logic
	inline void Die() {
		printf("Character #%d dying at: (%3.4f %3.4f %3.4f) ", Index, Location.X, Location.Y, Location.Z);
		// here we only start the ragdoll upon "death"
		//UpdateRagdollPositionsFromCharacter();
		StartRagdoll();
		printf("Dead at: (%3.4f %3.4f %3.4f)\n", Location.X, Location.Y, Location.Z);
	}

	// utility function to bring ragdoll in line with the character's animation
	inline void UpdateRagdollPositionsFromCharacter()
	{
		RagdollLocation = Location;
	}

	void TickPhysics(float DeltaTime);

private:
	int				Index;

	CharacterState	State;
	Vector3			Location;
	float			LifeTime;

	Vector3			RagdollLocation;
};

void Character::TickPhysics(float DeltaTime)
{
	LifeTime += DeltaTime;
	if (State == ECS_Normal)
	{
		// normal state - some simple periodical motion
		SetLocation(Vector3(sinf(LifeTime), cosf(LifeTime), Location.Z));
	}
	else
	{
		// character is in ragdoll - fall down
		RagdollLocation += Vector3(0.f, 0.f, -1.f * DeltaTime);
		// align character with ragdoll position
		SetLocation(RagdollLocation);
		// go "back to life" (stop ragdoll) after 2 secs
		if (LifeTime > 2.f)
			StopRagdoll();
	}
}

int main(int argc, char *argv[])
{
	// create a handful of objects
	static const size_t CharacterCount = 100000;
	vector<Character *> Characters(CharacterCount);
	for (size_t i = 0; i < Characters.size(); ++i)
		Characters[i] = new Character(i);

	// emulate main game loop
	while (true)
	{
		for (auto it = Characters.begin(); it != Characters.end(); it++)
		{
			(*it)->TickPhysics(0.167f);
			// randomly die
			const int Chance = rand() % 100;
			if (Chance < 10)
				(*it)->Die();
		}
	}

	return 0;
}
