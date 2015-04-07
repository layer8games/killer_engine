/*-------------------------------------------------------------------
A world is what the different states of game will be. Each world re-
presents  

This is not free to use, and cannot be used without the express permission
of KillerWave.

Written by Maxwell Miller
---------------------------------------------------------------------*/
#ifndef WORLD_H
#define WORLD_H
//Killer1 Includes
#include <Atom.h>
#include <GameObject.hpp>

//STL Includes
#include <map>

class World{
private:
	std::map<U32, GameObject*> _worldObjects;

	S32 _nextID;
	S32 _mapWidth;
	S32 _mapHeight;

public:
	World(void) {}

	~World(void) {}

	bool InitWorld(S32 w, S32 h);

	virtual bool v_Update(void);
	virtual bool v_Render(void);

	bool AddObjectToWorld(GameObject* obj);

	//Map Width and Height Accessors
	S32 GetMapWidth(void)  { return _mapWidth; }
	S32 GetMapHeight(void) { return _mapHeight; }
	void SetMapWidth(S32 w)  { _mapWidth = w; }
	void SetMapHeight(S32 h) { _mapHeight = h; }

	
};


#endif