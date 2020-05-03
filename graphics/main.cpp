#include <SFML/Graphics.hpp>
#include <iostream>
#include "view.hpp"
#include "sprite_sheet.hpp"
#include <cassert>

 #include <chrono>
 #include <thread>

int main(){

	View my_view;

	GraphicBody b(0, {250, 250});
	my_view.AddObject(1, b, Rock);


	for(auto i = 1; i < 30; i++){
		my_view.BeginDraw();	
		my_view.Draw();
		my_view.EndDraw();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

/*	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Best Game", sf::Style::Default);
	window.setFramerateLimit(25);

	View my_view;
	my_view.Init();

	Game my_game;
	my_view.RunGame(my_game, window);
*/	

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "ok!\n";
	return 0;
}
/*
//window class - a convenient way to work with game window
class Window{
public:
	Window(); //default constructor
	//parametric constructor
	Window(const std::string& new_title,const sf::Vector2u& new_size);
	~Window(); //default destructor

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.

	void Update(); //Upddates window

	bool IsDone() const; //checks if window should be closed
	sf::Vector2u GetWindowSize() const; //returns size of the window

	const sf::RenderWindow& GetWindow() const; //returns a reference to the window
	void Draw(const sf::Drawable& drawable); //draws an object

	void Setup(	const std::string& new_title,  //changes window parameters
				const sf::Vector2u& new_size);
private:
	void Destroy();
	sf::RenderWindow window_;
	sf::Vector2u window_size_;
	std::string window_title_;
	bool is_done_;
};

//Implementations of Window:: methods
void Window::Setup(	const std::string& new_title, 
					const sf::Vector2u& new_size){
	window_title_ = new_title;
	window_size_ = new_size;
	is_done_ = false;
	auto style = sf::Style::Default;
	window_.create({window_size_.x, window_size_.y, 32}, window_title_, style);
	window_.setFramerateLimit(60); //60fps
}

Window::Window(){
	Setup("Window", sf::Vector2u(640, 480));
}

Window::Window(	const std::string& new_title, 
				const sf::Vector2u& new_size){
	Setup(new_title, new_size);
}

Window::~Window(){
	window_.close();
}

void Window::Update(){
	sf::Event event;
	while(window_.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			is_done_ = true;
		} else if(event.type == sf::Event::KeyPressed &&
					event.key.code == sf::Keyboard::Escape){
			is_done_ = true;
		}
	}
}

sf::Vector2u Window::GetWindowSize() const{
	return window_size_;
}

void Window::BeginDraw(){
	window_.clear(sf::Color::Black);
}

void Window::EndDraw(){
	window_.display();
}

void Window::Draw(const sf::Drawable& to_draw){
	window_.draw(to_draw);
}

bool Window::IsDone() const{
	return is_done_;
}

const sf::RenderWindow& Window::GetWindow() const{
	return window_;
}

//Tile - part of the WorldMap
class Tile{
public:
	Tile(){
		rectangle_ = sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		colors_.resize(2);
		rectangle_.setFillColor(sf::Color::White);
		colors_[1] = sf::Color::Red;
		is_pressed_ = false;
		is_highlighted_ = false;
	}
	~Tile(){;}

	void SetColor(const sf::Color& color){
		colors_[0] = color;
		Update();
	}

	void Update(){
		rectangle_.setFillColor(colors_[(int)is_highlighted_]);
	}

	void Press(){ 
		is_pressed_ = !is_pressed_;
	}

	void Highlight(bool new_val = false){
		is_highlighted_ = new_val;
	}

	bool IsHighlighted() const{
		return is_highlighted_;
	}
	void SetPosition(const sf::Vector2f& new_pos){
		rectangle_.setPosition(new_pos);
	}

	const sf::RectangleShape& GetDrawableTile() const{
		return rectangle_;
	}
	
private:
	bool is_pressed_;
	bool is_highlighted_;
	std::vector<sf::Color> colors_;
	sf::RectangleShape rectangle_;
};

class Game{
public:
	Game();
	~Game(){;}	
	void SetTilesDefault();

	void HandleInput();
	void Update();
	void Render();
	void TilesUpdate();
	Window& GetWindow();
	bool Init();

private:
	Window window_;
	std::vector<Tile> Tiles;
};

Game::Game(){
	window_.Setup("Chess board!", sf::Vector2u(WIDTH,HEIGHT));
	Tiles.resize(N_ROWS * N_COLUMNS, Tile());
}

void Game::TilesUpdate(){
	sf::Vector2i mouse = sf::Mouse::getPosition(window_.GetWindow());

	if(0 < mouse.x && mouse.x < WIDTH && 0 < mouse.y && mouse.y < HEIGHT){
		int cell_to_highlight = mouse.x / (int)TILE_SIZE + 
					(mouse.y / (int)TILE_SIZE) * N_COLUMNS;

		Tiles[cell_to_highlight].Highlight(true);
		Tiles[cell_to_highlight].Update();
	}
}

void Game::Update(){
	window_.Update(); //Update window events
	TilesUpdate();
}

void Game::Render(){
	window_.BeginDraw(); //window.clear
	
	//drawing
	for(auto& item: Tiles){		
		if(item.IsHighlighted() == true){
			window_.Draw(item.GetDrawableTile());
			item.Highlight(false);
			item.Update();
		}
		else{
			window_.Draw(item.GetDrawableTile());
		}
	}

	window_.EndDraw(); //window display
}


void Game::HandleInput(){
	;
}

Window& Game::GetWindow(){
	return window_;
}
int main(){
	Game game;
	game.Init();

	while(!game.GetWindow().IsDone()){
		//main loop of our game
		game.HandleInput();
		game.Update();
		game.Render();
	}

	return 0;
}
*/

