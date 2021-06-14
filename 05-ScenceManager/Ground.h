#pragma once
#include "GameObject.h"
class Ground : public CGameObject
{
	int groundWidth, groundHeight;
public:
	Ground(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void TakeDamage() {};

};

