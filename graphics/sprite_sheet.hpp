#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

//each entity type has its own sprite_sheet
enum EntityType{
	Rock = 0,
	Human,
	Tile,
	Chest,
	Grass
	//....
};

//paramaters of animation in sprite sheet
struct AnimInfo{
	AnimInfo():
			n_sprites(0),
			top_left(0, 0),
			sz(0, 0),
			is_looped(0){
	}

	explicit AnimInfo(int p1, sf::Vector2i p2, sf::Vector2i p3, bool p4):
			n_sprites(p1), 
			top_left(p2),
			sz(p3), 
			is_looped(p4){
	}

	explicit AnimInfo(const AnimInfo& a):
			n_sprites(a.n_sprites), 
			top_left(a.top_left),
			sz(a.sz), 
			is_looped(a.is_looped){
	}

	AnimInfo& operator=(const AnimInfo& a){
		n_sprites = a.n_sprites;
		top_left = a.top_left;
		sz = a.sz;
		is_looped = a.is_looped;

		return *this;
	}

	~AnimInfo() = default;
	
	int n_sprites; 			//number of sprites in animation
	sf::Vector2i top_left; 	//position of the first sprite in animation
	sf::Vector2i sz; 		//size of sprite (sprites are supposed to be square)
	bool is_looped; 		//restart animation after finishing
};

//SpriteSheet is a holder for texture. Each texture is dedicated to a particular entity type
//The texture has all sprites for considering entity type. Each animation has a corresponding AnimInfo structure. 
//It has some details about animation and it's position in the texture.
//
class SpriteSheet{
public:
	//loads texture and info about animations in the texture
	explicit SpriteSheet(const std::string& filename_texture, const std::string& filename_info);

	SpriteSheet(const SpriteSheet& ) = delete;
	SpriteSheet& operator=(const SpriteSheet& ) = delete;
	SpriteSheet& operator=(SpriteSheet&& s);

	~SpriteSheet() = default;
	const AnimInfo* GetInfo(int n);
	const sf::Texture& GetTexture() const{ return *texture_;}

private:
	bool ReadInfo(const std::string& file);
	bool Load(const std::string& file);

private:
	std::unique_ptr<sf::Texture> texture_;	//png with many animations
	std::vector<AnimInfo> info_; 			//n_anim <-> info[n_anim], n_anim = 0, 1, 2, ...
};