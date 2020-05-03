#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

#include "view.hpp"

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
			window_.draw(obj->sprite_);
		
		obj->Update();
	}
}


View::~View(){
	for(auto& item: sprite_sheet_base_){
		delete item;
	}
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

/*
int main(){
	std::cout << "Ok!\n";
}
*/
