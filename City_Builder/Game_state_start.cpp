//
//  Game_state_start.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Game_state_start.h"
#include "Game.h"
#include "Game_state_editor.h"
#include <SFML/Graphics.hpp>

using std::shared_ptr; using std::make_shared;

Game_state_start::Game_state_start(shared_ptr<Game> game_ptr)
: Game_state(game_ptr)
{
    sf::Vector2f pos = sf::Vector2f(game_ptr->window.getSize());
    view.setSize(pos);
    pos *= 0.5f;
    view.setCenter(pos);
    
    gui_system.emplace("menu", GUI(sf::Vector2f(192, 32), 4, false, game_ptr->stylesheets.at("button"),
	{ std::make_pair("Load Game", "load_game"),
		std::make_pair("New Game", "new_game") }));
    
    gui_system.at("menu").setPosition(pos);
    gui_system.at("menu").setOrigin(96, 32 * 1/2);
    gui_system.at("menu").show();
}

void Game_state_start::draw(const float dt) {
    auto game_ptr = get_game_ptr();
    game_ptr->window.setView(view);
    game_ptr->window.clear(sf::Color::Black);
    game_ptr->window.draw(game_ptr->background);
    for (auto gui : gui_system)
        game_ptr->window.draw(gui.second);
}

void Game_state_start::update(const float dt) {
}

void Game_state_start::handle_input() {
    sf::Event event;
    auto game_ptr = get_game_ptr();
    sf::Vector2f mouse_pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), view);

    while(game_ptr->window.pollEvent(event)) {
        switch(event.type) {
            // Close the window
            case sf::Event::Closed:
                game_ptr->window.close();
                break;
            // Resize the window
            case sf::Event::Resized: {
                view.setSize(event.size.width, event.size.height);
                game_ptr->background.setPosition(game_ptr->window.mapPixelToCoords(sf::Vector2i(0, 0)));
                
                sf::Vector2f pos = sf::Vector2f(event.size.width, event.size.height);
                pos *= 0.5f;
                pos = game_ptr->window.mapPixelToCoords(sf::Vector2i(pos), view);
                this->gui_system.at("menu").setPosition(pos);
                
                game_ptr->background.setScale(float(event.size.width) / float(game_ptr->background.getTexture()->getSize().x),
                                              float(event.size.height) / float(game_ptr->background.getTexture()->getSize().y));
                break;
            }
            // Highlight menu items.
            case sf::Event::MouseMoved:
                gui_system.at("menu").highlight(gui_system.at("menu").get_entry(mouse_pos));
                break;
            // Click on menu items.
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left) {
                    std::string msg = gui_system.at("menu").activate(mouse_pos);
					if (msg == "load_game")
						load_game(false);
					else if (msg == "new_game")
						load_game(true);
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    game_ptr->window.close();
                break;
            default:
                break;
        }
    }
}

void Game_state_start::load_game(bool new_game) {
    auto game_ptr = get_game_ptr();
    game_ptr->push_state(make_shared<Game_state_editor>(game_ptr, new_game));
}
