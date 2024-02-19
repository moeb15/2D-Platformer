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
		AnimationCount
	};
}

namespace States {
	enum Type {
		Air,
		Ground,
	};
}