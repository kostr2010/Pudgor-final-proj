#include <vector>
#include <iostream>
#include <fstream>

#include "view.hpp"


/*******AnimInfo*********/
AnimInfo& AnimInfo::operator=(const AnimInfo& a){
	n_sprites = a.n_sprites;
	top_left_x = a.top_left_x;
	top_left_y = a.top_left_y;
	hor_sz = a.hor_sz;
	vert_sz = a.vert_sz;
	is_looped = a.is_looped;

	return *this;
}

/*******SpriteSheet********/

//loads texture and info about sprites in the texture
SpriteSheet::SpriteSheet(const std::string& file_texture, const std::string& file_info):
		info_(),
		texture_(sf::Texture()){

	if(ReadInfo(info_, file_info) == false){
		std::cout << "Reading anim info error" << std::endl;
	}

	if(texture_.loadFromFile(file_texture) == 0){
		std::cout << "Loading texture error" << std::endl;
	}
}

//example of Object.txt:
//total number of animations
//animation number(starting with 0): n_sprites
//	top_left_x top_left_y; //relative to the top left corner of png
//	hor_sz vert_sz; //size of sprite in pixels 
//	is_looped(0 or 1);
bool SpriteSheet::ReadInfo(std::vector<AnimInfo>& info, const std::string& file){

	int n_sprites = 0; //number of sprites in animation
	int top_left_x = 0; // position of the first sprite in animation
	int top_left_y = 0; // - //-
	int hor_sz = 0; //horisontal size of sprite
	int vert_sz = 0; //vertical size of sprite
	bool is_looped = false;

	int anim_n = 0; //animation number

	int total_n = 0; //total number of animations
	std::ifstream input(file);
	input >> total_n;

	info.resize(total_n);

	for(auto i = 0; i < total_n; i++){
		input >> anim_n;
		input.ignore(1);
		input >> n_sprites;

		input >> top_left_x >> top_left_y;
		input.ignore(1);

		input >> hor_sz >> vert_sz;
		input.ignore(1);

		input >> is_looped;
		input.ignore(1);

		info.at(anim_n) = AnimInfo(n_sprites, top_left_x, top_left_y, 
								hor_sz, vert_sz, is_looped);
	}

	return true;
}

SpriteSheet& SpriteSheet::operator=(SpriteSheet&& s){
	
	std::swap(info_, s.info_);
	std::swap(texture_, s.texture_);

	return *this;
}

/************Drawable Object***********/

//by default all sprites are oriented upwardly
//(x,y) - sprite position, n_sprite_sheet - sheet number in base
//n_anim - animation number in set
DrawableObject::DrawableObject(const int n_sprite_sheet, const int n_anim, 
                    const float x, const float y, const Dirs direction = Dirs::Up):

		sprite_(sprite_sheet_base[n_sprite_sheet].texture_), //connecting with texture
		x_(x),
		y_(y),
		sprite_sheet_(sprite_sheet_base[n_sprite_sheet]),
		n_anim_(n_anim),
		n_frame_(0),
		direction_(direction){
	
	SetupSprite(sprite_sheet_.info_[n_anim_].top_left_x, sprite_sheet_.info_[n_anim_].top_left_y);
}

void DrawableObject::SetPosition(const float new_x, const float new_y){
		
	x_ = new_x;
	y_ = new_y;
}

void DrawableObject::SetDirection(const Dirs new_direction){

	direction_ = new_direction;
}

void DrawableObject::Draw(sf::RenderWindow&  window){

	window.draw(sprite_);
}

void DrawableObject::Update(){

	const AnimInfo& info = sprite_sheet_.info_[n_anim_];

	//n_anim = 0 - static animation
	if(++n_frame_ >= info.n_sprites){			
		n_frame_ = 0;
		n_anim_ = info.is_looped ? n_anim_ : 0;
	}

	float new_x = info.top_left_x + n_frame_ * info.hor_sz;
	float new_y = info.top_left_y + n_frame_ * info.vert_sz;

	SetupSprite(new_x, new_y);
}

//cuts sprite from texture using top_left_x and top_left_y, 
//sets its position to x_, y_, rotates if necessary 
bool DrawableObject::SetupSprite(const int top_left_x, const int top_left_y){

	const AnimInfo& info = sprite_sheet_.info_[n_anim_];

	sprite_.setTextureRect(sf::IntRect(top_left_x, top_left_y, info.hor_sz, info.vert_sz));
	
	sprite_.setOrigin(info.hor_sz / 2, info.vert_sz / 2);

	sprite_.setPosition(x_, y_);

	if(direction_ != Dirs::Up){

			float angle = 0.0;

			switch(direction_){
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
	
	for(auto& object: objects_){
		object.Draw(window);
	}
}

void View::Update(){

	for(auto& object: objects_){
		object.Update();
	}
}

bool View::InitSpriteBase(int n_sheets){
	
	DrawableObject::sprite_sheet_base.resize(n_sheets);
	DrawableObject::sprite_sheet_base[static_cast<int>(EntityType::Warrior)] = 
		SpriteSheet("img/Warrior.png", "img_info/Warrior.txt");
	//and so on
}




