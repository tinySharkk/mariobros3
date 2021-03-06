#include "PlayerStateEndGame.h"
#include "Mario.h"
PlayerStateEndGame::PlayerStateEndGame()
{
	this->startEndScene = GetTickCount64();
	__Mario->direction.x = 1.0f;
}

void PlayerStateEndGame::Update()
{
	__Mario->vx = MARIO_MAX_WALKING_SPEED ;
	if (GetTickCount64() - startEndScene > WAIT_END_TIME)
	{
		CGame::GetInstance()->GetCurrentScene()->isEndScene = true;
		__Mario->isEndScene = true;
	}
}

void PlayerStateEndGame::SetAnimation()
{
	__Mario->ani = MARIO_ANI_WALK;
}

void PlayerStateEndGame::OnKeyDown(int KeyCode)
{
}
