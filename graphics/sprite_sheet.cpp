#include <cstdio>
#include <fstream>
#include <iostream>

#include "sprite_sheet.hpp"

//loads texture and info about sprites in the texture
SpriteSheet::SpriteSheet(const std::string& file_texture, const std::string& file_info):
		info_(),
		texture_(sf::Texture()){

	if(ReadInfo(file_info) == false){
		printf("Reading anim info error\n");
	}

	if(texture_.loadFromFile(file_texture) == 0){
		printf("Loading texture error\n");
	}
}

//example of Object.txt:
//total number of animations
//animation number(starting with 0): n_sprites
//	top_left.x top_right.y; //relative to the top left corner of png
//	sz.x sz.y; //size of sprite in pixels 
//	is_looped(0 or 1);
bool SpriteSheet::ReadInfo(const std::string& file){

	int anim_n = 0; //animation number
	int total = 0; //total number of animations

	std::ifstream input(file);
	if(!input.is_open()){
		return false;
	}

	input >> total;
	info_.resize(total);

	for(auto i = 0; i < total; i++){
		input >> anim_n;
		input.ignore(1);

		AnimInfo& info = info_.at(anim_n);

		input >> info.n_sprites;

		input >> info.top_left.x >> info.top_left.y;
		input.ignore(1);

		input >> info.sz.x >> info.sz.y;
		input.ignore(1);

		input >> info.is_looped;
		input.ignore(1);
	}

	return true;
}

SpriteSheet& SpriteSheet::operator=(SpriteSheet&& s){
	
	std::swap(info_, s.info_);
	std::swap(texture_, s.texture_);

	return *this;
}
