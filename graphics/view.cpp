#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

#include "view.hpp"
#include "sprite_sheet.hpp"

void View::AddObject(int id, GraphicBody& obj, enum EntityType t){
	
	obj.sprite_sheet_ = sprite_sheet_base_[static_cast<int>(t)];
	assert(obj.sprite_sheet_);

	obj.sprite_.setTexture(obj.sprite_sheet_->GetTexture());

	obj.Update();

	objects_[id] = &obj;
}


bool View::InitSpriteSheetBase(){

	using two_strings = std::pair<std::string, std::string>;

	std::vector<two_strings> files = 
	{	
		{"img/Warrior.png", "img_info/Warrior.txt"}, 
		{"img/Chest.png", "img_info/Chest.txt"}
	};

	sprite_sheet_base_.resize(files.size());

	for(uint32_t i = 0; i < files.size(); i++){
		sprite_sheet_base_[i] = new SpriteSheet(files[i].first, files[i].second);
		assert(sprite_sheet_base_[i]);
	}

	return true;
}


void View::Draw(){

	GraphicBody* obj = nullptr;

	for(auto& item: objects_){
		obj = item.second;
		assert(obj);

		if(obj->is_drawn_)			
			window_->draw(obj->sprite_);
		
		obj->Update();
	}
}


bool View::WindowCreate(){

	window_ = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Best Game", sf::Style::Default);
	assert(window_);
	return true;
}


void View::SetWindowTitle(const std::string new_title){

	window_->setTitle(new_title);
}


void View::BeginDraw(){

	window_->clear();
}


void View::EndDraw(){

	window_->display();
}


sf::RenderWindow& View::GetWindow(){

	return *window_;
}

View::~View(){
	for(auto& item: sprite_sheet_base_){
		delete item;
	}

	delete window_;
}

/*
void View::RunGame(Game& game, sf::RenderWindow& window) {
    game.Init();

    while (window.isOpen()) {
        Control::Input(game, window);

        window.clear();
        Draw(window);
        window.display();

        Update();
    }
}
*/
