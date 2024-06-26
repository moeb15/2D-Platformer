#pragma once


namespace Entities {
	enum Type {
		Default,
		Player,
		Enemy,
		Tile,
		Floor,
		QuestionBox,
		Bullet,
		Door,
		Climbable,
	};
}

namespace Actions {
	enum Type {
		None,
		Start,
		Stop,
	};

	enum Name {
		NoneType,
		Left,
		Right,
		Up,
		Down,
		Jump,
		Dash,
		AirDash,
		Shoot,
		Quit,
		Select,
		ToggleBox,
		MouseLeft,
		MouseRight,
		MouseMoved,
		Pause,
		Copy,
		Interact
	};
}

namespace Scenes {
	enum Type {
		Main,
		Game,
		Editor,
		LevelSelect,
		CustomLevels,
		Records,
	};
}

namespace Animations {
	enum Type {
		Idle,
		IdleLeft,
		Run,
		RunLeft,
		Jump,
		JumpLeft,
		Shoot,
		ShootLeft,
		Climb,
		ClimbLeft,
		AirDash,
		AirDashLeft,
		Blast,
		BlastLeft,
		Tile,
		Ground,
		QuestionBox,
		Explosion,
		Enemy,
		Gravel,
		DoorExit,
		DoorEnter,
		Climbable,
		AnimationCount
	};
}

namespace States {
	enum Type {
		Air,
		Ground,
		Climb,
		AirDash,
	};
}

namespace AIStates {
	enum Type {
		Follow,
		Idle,
	};
}