#include "engine/Datatables.h"
#include "engine/ObjectTypes.h"


std::vector<AnimationData> initializeAnimationData() {
	std::vector<AnimationData> data(Animations::AnimationCount);
	
	data[Animations::Idle].frameCount = 1;
	data[Animations::Idle].duration = 60;

	data[Animations::IdleLeft].frameCount = 1;
	data[Animations::IdleLeft].duration = 60;

	data[Animations::Jump].frameCount = 1;
	data[Animations::Jump].duration = 60;

	data[Animations::JumpLeft].frameCount = 1;
	data[Animations::JumpLeft].duration = 60;

	data[Animations::Run].frameCount = 4;
	data[Animations::Run].duration = 5;

	data[Animations::RunLeft].frameCount = 4;
	data[Animations::RunLeft].duration = 5;

	data[Animations::Shoot].frameCount = 1;
	data[Animations::Shoot].duration = 30;

	data[Animations::ShootLeft].frameCount = 1;
	data[Animations::ShootLeft].duration = 30;

	data[Animations::Blast].frameCount = 4;
	data[Animations::Blast].duration = 4;

	data[Animations::BlastLeft].frameCount = 4;
	data[Animations::BlastLeft].duration = 4;

	data[Animations::QuestionBox].frameCount = 4;
	data[Animations::QuestionBox].duration = 15;

	data[Animations::Ground].frameCount = 1;
	data[Animations::Ground].duration = 1;

	data[Animations::Tile].frameCount = 1;
	data[Animations::Tile].duration = 1;

	data[Animations::Explosion].frameCount = 4;
	data[Animations::Explosion].duration = 1;

	data[Animations::Enemy].frameCount = 1;
	data[Animations::Enemy].duration = 60;

	data[Animations::Gravel].frameCount = 1;
	data[Animations::Gravel].duration = 60;

	data[Animations::DoorEnter].frameCount = 1;
	data[Animations::DoorEnter].duration = 60;

	data[Animations::DoorExit].frameCount = 1;
	data[Animations::DoorExit].duration = 60;

	data[Animations::Climbable].frameCount = 1;
	data[Animations::Climbable].duration = 60;

	data[Animations::Climb].frameCount = 1;
	data[Animations::Climb].duration = 1;

	data[Animations::ClimbLeft].frameCount = 1;
	data[Animations::ClimbLeft].duration = 1;

	data[Animations::AirDash].frameCount = 5;
	data[Animations::AirDash].duration = 4;

	data[Animations::AirDashLeft].frameCount = 5;
	data[Animations::AirDashLeft].duration = 4;

	return data;
}