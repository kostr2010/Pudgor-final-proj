#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include <map>

#include "sprite_sheet.hpp"
#include "graphic_body.hpp"

#define TILE_SIZE 120.0 //size of tile in pixels
#define WIDTH 720 		//horisontal size of the window
#define HEIGHT 1280 	//vertical size of the window

class View{
public:
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
	void SetWindowTitle(const std::string new_title);
	void BeginDraw();
	void EndDraw();

	const SpriteSheet* GetSpriteSheet(enum EntityType);

//	void RunGame(Game&, sf::RenderWindow&);
private:
	bool InitSpriteSheetBase();
	bool WindowCreate();

private:
	std::map<int, GraphicBody*> objects_;			//[id]
	std::vector<SpriteSheet*> sprite_sheet_base_; 	//[entity_type]
	sf::RenderWindow* window_;
};