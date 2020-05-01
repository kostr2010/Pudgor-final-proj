#include "control.hpp"
#include <SFML/Graphics.hpp>

void Control::Input(Game& game, sf::RenderWindow& window){
	sf::Event event;
	while(window.pollEvent(event)){
		switch(event.type){
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Escape){
					window.close();
				}
				break;
			case sf::Event::MouseButtonPressed:
				//if + button checkers;
				if(event.mouseButton.button == sf::Mouse::Left){
					sf::Vector2i mouse = sf::Mouse::getPosition(window);
					//game.Click(mouse.x, mouse.y);
				}
				break;
		}
	}
}
