#include "RedFireShootingPlant.h"
#include "Camera.h"
#include "Game.h"
#include "PlantFireBall.h"
#include "Mario.h"
void RedFireShootingPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RED_FIRE_SHOOTING_PLANT_WIDTH;
	bottom = y + fsheight;
}

void RedFireShootingPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CGameObject::Update(dt);
	this->x += dx;
	this->y += dy;

	if (__Mario->y > this->y)
		this->aim = Aim::down;
	else this->aim = Aim::up;

	if (this->x < __Mario->x)
		this->direction.x = -1;
	else this->direction.x = 1;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->ObjectGroup == Group::projectile || e->obj->ObjectGroup == Group::marioprojectile)
			{
				this->TakeDamage();
			}
		}
	}

	switch (this->state)
	{
	case ShootPlantState::idledown:
	{
		this->vy = 0;
		distance = 0;
		if (GetTickCount64() - hideTime_start > RED_FIRE_SHOOTING_PLANT_HIDE_TIME)
		{
			state = ShootPlantState::slideup;
		}

		break;
	}
	case ShootPlantState::slideup:
	{
		this->vy = -RED_FIRE_SHOOTING_PLANT_SPEED;
		/*distance += abs(vy * dt);
		if (distance> fsheight)
		{
			distance = fsheight;
			this->waitTime_start = GetTickCount();
			state = ShootPlantState::idleup;
		}*/
		if (abs(this->y - this->startY) > fsheight)
		{
			this->y = this->startY - fsheight;
			this->waitTime_start = GetTickCount64();
			state = ShootPlantState::idleup;
		}
		break;
	}
	case ShootPlantState::idleup:
	{
		this->vy = 0;
		if (GetTickCount64() - waitTime_start > RED_FIRE_SHOOTING_PLANT_WAIT_TIME)
		{
			distance = 0;
			DebugOut(L"[INFO] slide down \n");
			state = ShootPlantState::slidedown;
			ShootFire();
		}
		break;
	}
	case ShootPlantState::slidedown:
	{
		this->vy = +RED_FIRE_SHOOTING_PLANT_SPEED;
		/*distance += abs(vy * dt);
		if (distance > fsheight)
		{
			distance = fsheight;
			this->hideTime_start = GetTickCount();
			state = ShootPlantState::idledown;
		}*/
		if (this->y > this->startY)
		{
			this->y = this->startY;
			this->hideTime_start = GetTickCount64();
			state = ShootPlantState::idledown;
		}
		break;
	}
	}
}

void RedFireShootingPlant::Render()
{
	int ani=0;
	if (this->aim == Aim::down)
		ani = ANI_ID_RED_FIRE_SHOOTING_PLANT_DOWN;
	else ani = ANI_ID_RED_FIRE_SHOOTING_PLANT_UP;
	Camera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();

	animation_set[ani]->Render(x - camera->GetCamPosX() + RED_FIRE_SHOOTING_PLANT_WIDTH / 2, y - camera->GetCamPosY() + fsheight / 2, direction, 255);

	RenderBoundingBox();
}

void RedFireShootingPlant::ShootFire()
{
	int aimfire;
	if (this->aim == Aim::down)
		aimfire = -1;
	else aimfire = 1;
	PlantFireBall *fireball = new PlantFireBall(this->x+ FIREBALL_POS, this->y+ FIREBALL_POS, -this->direction.x, (float)aimfire);
	CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(fireball);
}

RedFireShootingPlant::RedFireShootingPlant()
{
	this->aim = Aim::up;
	this->state = ShootPlantState::idledown;
	AddAnimation(ID_ANI_RED_SHOOTING_PLANT_DOWN);
	AddAnimation(ID_ANI_RED_SHOOTING_PLANT_UP);

	this->ObjectGroup = Group::projectile;
	this->collision = Collision2D::Full;
	this->hideTime_start = GetTickCount64();
	this->fsheight = RED_FIRE_SHOOTING_PLANT_HEIGHT;
}

void RedFireShootingPlant::TakeDamage()
{
	CGame::GetInstance()->GetCurrentScene()->DeleteObject(this);
}

void RedFireShootingPlant::OnOverLap(CGameObject* obj)
{
	if (obj->ObjectGroup == Group::marioprojectile)
	{
		this->TakeDamage();
	}
}
