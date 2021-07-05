#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "defineAni.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};

enum class Group
{
	player,
	enemy,
	ground,
	block,
	projectile,
	projectile2,
	marioprojectile,
	shell,
	item,
	portal,
	warp,
	dead	

};
enum class Collision2D
{
	//should make bottom, left, right as well but i just need top to make ghost platform

	Full,
	Top,
	Bottom,
	None
};

class CGameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isBeingHold = false;
	bool isHoldAble = false;

	DWORD dt; 

	//LPANIMATION_SET animation_set;
	vector<LPANIMATION> animation_set;

public: 
	Group ObjectGroup;
	Collision2D collision;
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void setHoldAble(bool holdAble) { this->isHoldAble = holdAble; }
	void setBeingHold(bool beingHold) { this->isBeingHold = beingHold; }
	bool getHoldAble() { return isHoldAble; }
	bool getBeingHold() { return isBeingHold; }

	//int GetState() { return this->state; }
	virtual void OnOverLap(CGameObject* object) {};
	bool IsOverLapped(CGameObject* object);
	bool CheckOverlapped(float left, float top, float right, float bottom, float left1, float top1, float right1, float bottom1);

	void RenderBoundingBox();

	//void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	void AddAnimation(int aniID);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual void TakeDamage() = 0;

	~CGameObject();
};

