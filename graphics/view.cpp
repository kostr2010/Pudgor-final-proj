#include <vector>
#include <iostream>
#include <fstream>

#include "view.hpp"
#include "control.hpp"

/************Drawable Object***********/

//by default all sprites are oriented upwardly
//(x,y) - sprite position, n_sprite_sheet - sheet number in base
//n_anim - animation number in set
DrawableObject::DrawableObject(	const int n_sprite_sheet, const int n_anim, 
                    			const sf::Vector2f pos, const Dirs dir /*= Dirs::Up*/):

		sprite_(sprite_sheet_base[n_sprite_sheet].texture_), //connecting with texture
		pos_(pos),
		sprite_sheet_(sprite_sheet_base[n_sprite_sheet]),
		n_anim_(n_anim),
		n_frame_(0),
		dir_(dir){
	
	
	SetupSprite(sprite_sheet_.info_[n_anim_].top_left);
}

void DrawableObject::Draw(sf::RenderWindow&  window){

	window.draw(sprite_);
}

void DrawableObject::Update(){
	//all sprites which belong to the same animation must be in one row
	const std::vector<AnimInfo>& v_info = sprite_sheet_.info_;

	//n_anim_ = 0 - idle animation
	if(++n_frame_ >= v_info[n_anim_].n_sprites){			
		n_frame_ = 0;
		n_anim_ = v_info[n_anim_].is_looped ? n_anim_ : 0;
	}

	int new_x = v_info[n_anim_].top_left.x + n_frame_ * v_info[n_anim_].sz.x;
	int new_y = pos_.y;

	SetupSprite({new_x, new_y});
}

//cuts sprite from texture using top_left_x and top_left_y, 
//sets its position to x_, y_, rotates if necessary 
bool DrawableObject::SetupSprite(const sf::Vector2i& top_left){

	const AnimInfo& info = sprite_sheet_.info_[n_anim_];

	sprite_.setTextureRect(sf::IntRect(top_left.x, top_left.y, info.sz.x, info.sz.y));
	
	sprite_.setOrigin(info.sz.x / 2, info.sz.y / 2);

	sprite_.setPosition(pos_);

	if(dir_ != Dirs::Up){
			float angle = 0.0;

			switch(dir_){
				case Dirs::Right:
					angle = 90.0;
					break;
				case Dirs::Down:
					angle = 180.0;
					break;
				case Dirs::Left:
					angle = 270.0;
					break;
			}
			sprite_.setRotation(angle);
		}

	return true;
}

/*********** View **********/
bool View::Init(){
	InitSpriteBase(2);
}

void View::Draw(sf::RenderWindow& window){	
	for(auto& tile: map_){
		tile.Draw(window);
	}

	for(auto& object: objects_){
		object.Draw(window);
	}
}

void View::Update(){

	for(auto& object: objects_){
		object.Update();
	}
}

//creating filename with macros?
bool View::InitSpriteBase(int n_sheets){
	std::vector<SpriteSheet>& base = DrawableObject::sprite_sheet_base;
	base.resize(n_sheets);

	base[static_cast<int>(EntityType::Warrior)] = 
		SpriteSheet("img/Warrior.png", "img_info/Warrior.txt");
	//and so on
}

void View::RunGame(Game& game, sf::RenderWindow& window){
	game.Init();

	while(window.isOpen()){
		Control::Input(game, window);

		window.clear();	
		Draw(window);
		window.display();
	}
}



