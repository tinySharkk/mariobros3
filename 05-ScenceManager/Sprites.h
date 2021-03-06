#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left; 
	int top;
	int right;
	int bottom;
	D3DXVECTOR2 direction;

	float centerX;
	float centerY;

	LPDIRECT3DTEXTURE9 texture;
public: 
	CSprite(int id, int left, int top, int right, int bottom, float cx, float cy, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, D3DXVECTOR2 direction = D3DXVECTOR2(1.0f, 1.0f) , int alpha = 255);
	int GetSpriteWidth();
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, float cx, float cy, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);

	static CSprites * GetInstance();
};



