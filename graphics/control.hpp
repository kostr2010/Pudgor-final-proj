#pragma once

#include "game.h"

struct Control{
	enum Button{
		B1,
		B2,
		B3,
		B4
	};

	Control() = delete;
	static void Input(Game& game, sf::RenderWindow& window);
};