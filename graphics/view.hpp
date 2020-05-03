#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include <map>

#include "sprite_sheet.hpp"

#define TILE_SIZE 120.0 //size of tile in pixels

class GraphicBody;

class View{
public:
	enum Dir{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};
	explicit View(/**/);

	View(View& ) = delete;
	View& operator=(View& ) = delete;
	
	~View();

	bool Init();
	bool Finish();

	void Draw();
	void Update();
	void AddObject(int id, GraphicBody& obj, enum EntityType t);
	void RemoveObject();
	sf::RenderWindow& GetWindow();
	const SpriteSheet* GetSpriteSheet(enum EntityType);

//	void RunGame(Game&, sf::RenderWindow&);
private:
	bool InitSpriteSheetBase();

private:
	std::map<int, GraphicBody*> objects_;			//[id]
	std::vector<SpriteSheet*> sprite_sheet_base_; 	//[entity_type]
	sf::RenderWindow window_;
};


class GraphicBody{
	friend class View;

public:
	GraphicBody(const enum EntityType t, const int n_anim, 
				const sf::Vector2f pos, const View::Dir dir = View::Dir::UP);

	GraphicBody(const GraphicBody& o) = delete;
	GraphicBody& operator=(const GraphicBody& o) = delete;

	void SetPos(sf::Vector2f pos){ pos_ = pos;}
	void SetDir(View::Dir dir){ dir_ = dir;}
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
	View::Dir dir_;
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

