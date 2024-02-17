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
	};
}

namespace Scenes {
	enum Type {
		Main,
		Game,
		Editor,
	};
}

namespace Animations {
	enum Type {
		Idle,
		Run,
		Jump,
		Shoot,
		Tile,
		Ground,
		QuestionBox,
		Explosion,
		AnimationCount
	};
}

namespace States {
	enum Type {
		Air,
		Ground,
	};
}