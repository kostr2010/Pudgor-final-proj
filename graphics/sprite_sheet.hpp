#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

//each entity type has its own sprite_sheet
enum class EntityType{
	Warrior,
	Chest
};

//paramaters of animaiton in sprite sheet
struct AnimInfo{
	explicit AnimInfo(int p1, sf::Vector2i p2, sf::Vector2i p3, bool p4):
			n_sprites(p1), top_left(p2),sz(p3), is_looped(p4){
	}

	explicit AnimInfo(const AnimInfo& a):
			n_sprites(a.n_sprites), top_left(a.top_left), sz(a.sz), is_looped(a.is_looped){
	}

	AnimInfo& operator=(const AnimInfo& a){
		n_sprites = a.n_sprites;
		top_left = a.top_left;
		sz = a.sz;
		is_looped = a.is_looped;

		return *this;
	}

	~AnimInfo(){};
	
	int n_sprites; //number of sprites in animation
	sf::Vector2i top_left; // position of the first sprite in animation
	sf::Vector2i sz; //size of sprite;
	bool is_looped; //restart animation after finishing
};

enum class Dirs{ 
	Up, 
	Right,
	Down,
	Left
};

class SpriteSheet{
	friend class DrawableObject;

public:
	//loads texture and info about sprites in the texture
	explicit SpriteSheet(const std::string& file_texture, const std::string& file_info);

	SpriteSheet(const SpriteSheet& ) = delete;
	SpriteSheet& operator=(const SpriteSheet& ) = delete;
	SpriteSheet& operator=(SpriteSheet&& s);

	~SpriteSheet(){};

private:
	bool ReadInfo(const std::string& file);
private:
	sf::Texture texture_;//png with many animations
	std::vector<AnimInfo> info_; //n_anim <-> info[n_anim], n_anim = 0, 1, 
};