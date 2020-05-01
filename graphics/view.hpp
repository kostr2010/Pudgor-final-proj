#pragma once

#include <SFML/Graphics.hpp>

#include "game.h"
#include "sprite_sheet.hpp"

#define TILE_SIZE 120.0 //size of tile in pixels

class DrawableObject{
	friend class View;
public:
	//by default all sprites are oriented upwardly
	//(x,y) - sprite position, n_sprite_sheet - sheet number in base
	//n_anim - animation number in sheet

	DrawableObject(	const int n_sprite_sheet, const int n_anim, 
					const sf::Vector2f pos, const Dirs dir = Dirs::Up);

	DrawableObject(const DrawableObject& o) = delete;
	DrawableObject& operator=(const DrawableObject& o) = delete;

	void SetPosition(const sf::Vector2f pos){ pos_ = pos;}

	void SetDirection(const Dirs dir){ dir_ = dir;}

	void Draw(sf::RenderWindow&  window);

	void Update();

private:
	static std::vector<SpriteSheet> sprite_sheet_base;

	sf::Sprite sprite_;
   	const SpriteSheet& sprite_sheet_;

	sf::Vector2f pos_;
  	Dirs dir_;

	int n_anim_;
	int n_frame_;

	//cutting sprite from texture using top_left_x and top_left_y, 
	//sets its position to x_, y_, rotates if necessary 
	bool SetupSprite(const sf::Vector2i& top_left);
};

class Tile{
public:
	void Draw(sf::RenderWindow& window){
		window.draw(sprite_);
	}

	static sf::Texture texture;
	static const int tile_sz = TILE_SIZE;
	
	Tile(const float x, const float y, const int sprite_n):
			x_(x), y_(y){
		
		sprite_.setTexture(texture);
		sprite_.setTextureRect(sf::IntRect(tile_sz * sprite_n, 0, tile_sz, tile_sz));
		sprite_.setPosition(x_, y_); //top left corner;
	} 

	Tile(const Tile& ) = delete;
	Tile& operator=(const Tile& ) = delete;

private:
	sf::Sprite sprite_;
	float x_; //position
	float y_;
};

class View{
public:	
	bool Init();
	void Draw(sf::RenderWindow& window);
	void Update();
	void RunGame(Game&, sf::RenderWindow&);
	
private:
	bool InitSpriteBase(int n_sheets);
	std::vector<DrawableObject> objects_;
	std::vector<Tile> map_;
};


//separate into files
//add vector instead of x, y
//inheritance