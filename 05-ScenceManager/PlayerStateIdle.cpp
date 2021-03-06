#include "PlayerStateIdle.h"

#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttack.h"
#include "PlayerStateCrouch.h"
#include "KeyHanler.h"
#include "Mario.h"

PlayerStateIdle::PlayerStateIdle()
{
	DebugOut(L"[INFO] idle \n");
	__Mario->state = MARIO_STATE_IDLE;
	__Mario->isEndScene = false;
}

void PlayerStateIdle::Update()
{
	if (KeyHanler::GetInstance()->IsKeyDown(DIK_RIGHT) && KeyHanler::GetInstance()->IsKeyDown(DIK_LEFT))
	{
	}
	else if (KeyHanler::GetInstance()->IsKeyDown(DIK_RIGHT) || KeyHanler::GetInstance()->IsKeyDown(DIK_LEFT))
		__Mario->SetState(new PlayerStateWalk());
	
	if (KeyHanler::GetInstance()->IsKeyDown(DIK_X))
		__Mario->SetState(new PlayerStateJump());

	if (KeyHanler::GetInstance()->IsKeyDown(DIK_DOWN))
		__Mario->SetState(new PlayerStateCrouch());

	if (__Mario->vx != 0)
		__Mario->vx -= MARIO_DRAG * __Mario->direction.x*__Mario->dt;
	if (__Mario->vx * __Mario->direction.x < 0)
		__Mario->vx = 0;

}


void PlayerStateIdle::SetAnimation()
{
	if (__Mario->vx != 0)
	{
		if (__Mario->isHolding)
			__Mario->ani = MARIO_ANI_RUN_HOLD;
		else
		__Mario->ani = MARIO_ANI_WALK;
	}
	else
	{
		if (__Mario->isHolding)
			__Mario->ani = MARIO_ANI_IDLE_HOLD;
		else
		__Mario->ani = MARIO_ANI_IDLE;
	}
}

void PlayerStateIdle::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_S:
		__Mario->SetState(new PlayerStateJump());
		break;
	case DIK_A:
		if (__Mario->level > MARIO_LEVEL_BIG)
			__Mario->SetState(new PlayerStateAttack());
		break;
	}
}
