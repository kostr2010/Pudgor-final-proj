#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

//#include "control.hpp"
#include "view.hpp"

GraphicBody::GraphicBody(const enum EntityType t, const int anim_n, 
						 const sf::Vector2f pos, const View::Dir dir /*= View::Dir::UP*/):
		sprite_sheet_(nullptr),
		sprite_(),
		pos_(pos),
		dir_(dir),
		anim_n_(anim_n),
		sprite_n_(0){	
}


void View::AddObject(int id, GraphicBody& obj, enum EntityType t){
	
	obj.sprite_sheet_ = sprite_sheet_base_[static_cast<int>(t)];
	assert(obj.sprite_sheet_);

	obj.sprite_.setTexture(obj.sprite_sheet_->GetTexture());

	obj.Update();

	objects_[id] = &obj;
}


void GraphicBody::Update() {

	assert(sprite_sheet_);
    // all sprites which belong to the same animation must be in one row
	const AnimInfo* info = sprite_sheet_->GetInfo(anim_n_);

    // anim_n_ = 0 - idle animation
    if (++sprite_n_ >= info->n_sprites) {
        sprite_n_ = 0;

		if(!info->is_looped){
        	anim_n_  = 0;
			info = sprite_sheet_->GetInfo(0);
		}
    }
	//coordinates of new_sprite in the texture
    int new_x = info->top_left.x + sprite_n_ * info->sz.x;
    int new_y = info->top_left.y;
	//cut new sprite from texture
    SetupSprite({new_x, new_y});
}

// cuts sprite from texture using top_left_x and top_left_y,
// sets its position to pos_, rotates if necessary
void GraphicBody::SetupSprite(const sf::Vector2i& top_left) {

	assert(sprite_sheet_);
    const AnimInfo* info = sprite_sheet_->GetInfo(anim_n_);

	//cut sprite at <top_left> with size <info->size>
    sprite_.setTextureRect(sf::IntRect(top_left, info->sz));
	//sets the center
    sprite_.setOrigin(info->sz.x / 2, info->sz.y / 2);
	//sets position
    sprite_.setPosition(pos_);
	//sets the orientation
    if (dir_ != View::Dir::UP) {

        float angle = 0.0;
        switch(dir_) {
        	case View::Dir::RIGHT: angle = 90.0;  break;
        	case View::Dir::DOWN:  angle = 180.0; break;
        	case View::Dir::LEFT:  angle = 270.0; break;
        }
        sprite_.setRotation(angle);
    }
}


bool View::InitSpriteSheetBase(){

	using two_strings = std::pair<std::string, std::string>;

	std::vector<two_strings> files = 
	{	
		{"img/Warrior.png", "img_info/Warrior.txt"}, 
		{"img/Chest.png", "img_info/Chest.txt"}
	};

	sprite_sheet_base_.resize(files.size());

	for(int i = 0; i < files.size(); i++){
		sprite_sheet_base_[i] = new SpriteSheet(files[i].first, files[i].second);
		assert(sprite_sheet_base_[i]);
	}
}


void View::Draw(){

	GraphicBody* obj = nullptr;

	for(auto& item: objects_){
		obj = item.second;
		assert(obj);

		if(obj->is_drawn_){			
			obj->sprite_;
			window_.draw(obj->sprite_);
		}
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
