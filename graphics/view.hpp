
#include <SFML/Graphics.hpp>

#define TILE_SIZE 120.0 //size of tile in pixels

//for each sprite sheet
enum class EntityType{
	Warrior,
	Chest
};

//paramaters of animaiton in sprite sheet
struct AnimInfo{
	explicit AnimInfo(int p1, int p2, int p3, int p4, int p5, bool p6):
			n_sprites(p1),
			top_left_x(p2),
			top_left_y(p3),
			hor_sz(p4),
			vert_sz(p5),
			is_looped(p6){
	}

	explicit AnimInfo(const AnimInfo& a):
			n_sprites(a.n_sprites),
			top_left_x(a.top_left_x),
			top_left_y(a.top_left_y),
			hor_sz(a.hor_sz),
			vert_sz(a.vert_sz),
			is_looped(a.is_looped){
	}

	AnimInfo& operator=(const AnimInfo& a);

	~AnimInfo(){};
	
	int n_sprites; //number of sprites in animation
	int top_left_x; // position of the first sprite in animation
	int top_left_y; // - //-
	int hor_sz; //horisontal size of sprite
	int vert_sz; //vertical size of sprite
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
	bool ReadInfo(std::vector<AnimInfo>& info, const std::string& file);
private:
	sf::Texture texture_;//png with many animations
	std::vector<AnimInfo> info_; //n_anim <-> info[n]
};

class DrawableObject{
	friend class View;
public:
	//by default all sprites are oriented upwardly
	//(x,y) - sprite position, n_sprite_sheet - sheet number in base
	//n_anim - animation number in sheet

	DrawableObject(	const int n_sprite_sheet, const int n_anim, 
 	                const float x, const float y, const Dirs direction = Dirs::Up);

	DrawableObject(const DrawableObject& o) = delete;
	DrawableObject& operator=(const DrawableObject& o) = delete;

	void SetPosition(const float new_x, const float new_y);

	void SetDirection(const Dirs new_direction);

	void Draw(sf::RenderWindow&  window);

	void Update();

private:
	static std::vector<SpriteSheet> sprite_sheet_base;

	sf::Sprite sprite_;
   	const SpriteSheet& sprite_sheet_;

	float x_;
	float y_;
  	Dirs direction_;

	int n_anim_;
	int n_frame_;

	//cutting sprite from texture using top_left_x and top_left_y, 
	//sets its position to x_, y_, rotates if necessary 
	bool SetupSprite(const int top_left_x, const int top_left_y);
};

class View{
public:	

	bool Init();
	void Draw(sf::RenderWindow& window);
	void Update();
	
private:
	bool InitSpriteBase(int n_sheets);
	std::vector<DrawableObject> objects_;
};



