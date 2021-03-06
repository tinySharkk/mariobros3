#include "PlayerStateAttack.h"
#include "Mario.h"
#include "MarioFireBall.h"
#include "MarioRaccoonTail.h"
#include "GameObject.h"
#include "Scence.h"
#include "Game.h"

PlayerStateAttack::PlayerStateAttack()
{
	__Mario->state = MARIO_STATE_ATTACK;
	this->cantailattack = false;
	switch (__Mario->level)
	{
	case MARIO_LEVEL_FIRE:
	{
		this->attack_time = MARIO_FIRE_ATTACK_TIME;
		if (__Mario->fireball > 0)
		{
			__Mario->fireball -= 1;
			MarioFireBall* fireball1 = new MarioFireBall(__Mario->x + MARIO_BIG_BBOX_WIDTH / 2, __Mario->y, __Mario->direction.x, __Mario->direction.y);
			CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(fireball1);
		}
		break;
	}
	case MARIO_LEVEL_RACCOON:
		this->attack_time = MARIO_RACCOON_ATTACK_TIME;
		this->raccoonAttackDelay = GetTickCount64();
		this->cantailattack = true;
		if (__Mario->direction.x >= 1)
		{
			MarioRaccoonTail* tail = new MarioRaccoonTail(__Mario->x + MARIO_BIG_BBOX_WIDTH / 2 - 50, __Mario->y + 50);
			CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(tail);
		}
		else if (__Mario->direction.x <= -1)
		{
			MarioRaccoonTail* tail = new MarioRaccoonTail(__Mario->x + MARIO_BIG_BBOX_WIDTH / 2, __Mario->y + 50);
			CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(tail);
		}
		break;
	}
	StartAttack();
}

void PlayerStateAttack::Update()
{
	
	if ((GetTickCount64() - attackTimeStart) > (attack_time))
	{
		__Mario->SetState(new PlayerStateIdle);
	}
	if (__Mario->level == MARIO_LEVEL_RACCOON)
	{
		if (GetTickCount64() - raccoonAttackDelay > RACCOON_ATTACK_DELAY && cantailattack)
		{
			cantailattack = false;
			if (__Mario->direction.x >= 1)
			{
				MarioRaccoonTail* tail = new MarioRaccoonTail(__Mario->x + MARIO_BIG_BBOX_WIDTH / 2, __Mario->y + 50);
				CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(tail);
			}
			else if (__Mario->direction.x <= -1)
			{
				MarioRaccoonTail* tail = new MarioRaccoonTail(__Mario->x + MARIO_BIG_BBOX_WIDTH / 2 - 50, __Mario->y + 50);
				CGame::GetInstance()->GetCurrentScene()->AddObjectInGame(tail);
			}
		}
	}
}

void PlayerStateAttack::SetAnimation()
{
	__Mario->ani = MARIO_ANI_SPECIAL;
}

void PlayerStateAttack::OnKeyDown(int KeyCode)
{
}
