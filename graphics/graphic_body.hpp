#pragma once

#include <SFML/Graphics.hpp>

#include "sprite_sheet.hpp"

class GraphicBody{
	friend class View;

public:
	enum Dir{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

	GraphicBody(const enum EntityType t, const int n_anim, 
				const sf::Vector2f pos, const Dir dir = Dir::UP);

	GraphicBody(const GraphicBody& o) = delete;
	GraphicBody& operator=(const GraphicBody& o) = delete;

	void SetPos(sf::Vector2f pos){ pos_ = pos;}
	void SetDir(Dir dir){ dir_ = dir;}
	void SetAnimN(int n){ anim_n_ = n;}
	void SetIsDrawn(){ is_drawn_ = !is_drawn_;}

private:
	//pointer to associated sprite_sheet
	SpriteSheet* sprite_sheet_;
	//pointer to current sprite
	sf::Sprite sprite_;
	//where to draw; with respect to window
	sf::Vector2f pos_;
	//orientation of sprite
	Dir dir_;
	//draw or not
	bool is_drawn_;
	//animation number
	int anim_n_;
	//sprite number in animation
	int sprite_n_;
private:
	void SetupSprite(const sf::Vector2i& top_left);
	void Update();
};

