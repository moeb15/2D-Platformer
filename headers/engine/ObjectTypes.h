#pragma once


namespace Entities {
	enum Type {
		Default,
		Player,
		Enemy,
		Tile,
		Floor,
		QuestionBox,
		Bullet
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
		Shoot,
		Quit,
		Select,
		ToggleBox,
		MouseLeft,
		MouseRight,
		MouseMoved,
		Pause,
	};
}

namespace Scenes {
	enum Type {
		Main,
		Game,
		Editor,
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
		AnimationCount
	};
}

namespace States {
	enum Type {
		Air,
		Ground,
	};
}