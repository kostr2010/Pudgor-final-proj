#include <cstdio>
#include <fstream>
#include <iostream>
#include <cassert>

#include "sprite_sheet.hpp"

//loads texture and info about sprites in the texture
SpriteSheet::SpriteSheet(const std::string& file_texture, const std::string& file_info){	
	//add exceptions 
	if(ReadInfo(file_info) == false){
		assert(!"Reading anim info error\n");
	}

	if(Load(file_texture) == 0){
		assert(!"Loading texture error\n");
	}
}

//Load texture from file
bool SpriteSheet::Load(const std::string& filename){
	
	std::unique_ptr<sf::Texture> my_texture(new sf::Texture()); 

	if(my_texture->loadFromFile(filename)){
		return false;
	}
	texture_ = std::move(my_texture);

	return true;
}

//example of Object.txt:
//  total number of animations
//  animation number: n_sprites	//animation number: 0, 1, 2, ...
//	top_left.x top_right.y 		//relative to the top left corner of png
//	sz.x sz.y 					//size of sprite in pixels 
//	is_looped					//(0 or 1)
bool SpriteSheet::ReadInfo(const std::string& file){

	int anim_n = 0; //animation number
	int total = 0; //total number of animations

	std::ifstream input(file);

	if(!input.is_open()){
		return false;
	}

	input >> total;
	info_.resize(total);

	for(int i = 0; i < total; i++){

		input >> anim_n;
		input.ignore(1);
		//short cut
		AnimInfo& info = info_.at(anim_n);

		input >> info.n_sprites;

		input >> info.top_left.x >> info.top_left.y;

		input >> info.sz.x >> info.sz.y;

		input >> info.is_looped;

	}

	return true;
}


SpriteSheet& SpriteSheet::operator=(SpriteSheet&& s){
	
	std::swap(info_, s.info_);
	std::swap(texture_, s.texture_);

	return *this;
}


const AnimInfo* SpriteSheet::GetInfo(int n){
	
	assert(n >= 0);
	return &info_[n];
}
