#include <vector>
#include <cassert>

#include "graphic_body.hpp"
#include "view.hpp"

GraphicBody::GraphicBody(const enum EntityType t, const int anim_n,
						 const sf::Vector2f pos, const Dir dir /*= View::Dir::UP*/):
		sprite_sheet_(nullptr),
		sprite_(),
		pos_(pos),
		dir_(dir),
		anim_n_(anim_n),
		sprite_n_(0){	
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
    if (dir_ != Dir::UP) {

        float angle = 0.0;
        switch(dir_) {
        	case Dir::RIGHT: angle = 90.0;  break;
        	case Dir::DOWN:  angle = 180.0; break;
        	case Dir::LEFT:  angle = 270.0; break;
        }
        sprite_.setRotation(angle);
    }
}

