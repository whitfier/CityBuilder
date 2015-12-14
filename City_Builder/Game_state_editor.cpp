//
//  Game_state_editor.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Game_state_editor.h"
#include "Game.h"
#include <SFML/Graphics.hpp>

using std::shared_ptr;

Game_state_editor::Game_state_editor(shared_ptr<Game> game_ptr)
: Game_state(game_ptr)
{
    sf::Vector2f pos = sf::Vector2f(game_ptr->window.getSize());
    gui_view.setSize(pos);
    game_view.setSize(pos);
    pos *= 0.5f;
    gui_view.setCenter(pos);
    game_view.setCenter(pos);
}

void Game_state_editor::draw(const float dt) {
    auto game_ptr = get_game_ptr();
    game_ptr->window.clear(sf::Color::Black);
    game_ptr->window.draw(game_ptr->background);
}

void Game_state_editor::update(const float dt) {}

void Game_state_editor::handle_input() {
    sf::Event event;
    auto game_ptr = get_game_ptr();

    while(game_ptr->window.pollEvent(event)) {
        switch(event.type) {
            /* Close the window */
            case sf::Event::Closed:
                game_ptr->window.close();
                break;
            /* Resize the window */
            case sf::Event::Resized:
                game_view.setSize(event.size.width, event.size.height);
                gui_view.setSize(event.size.width, event.size.height);
                game_ptr->background.setPosition(game_ptr->window.mapPixelToCoords(sf::Vector2i(0, 0), gui_view));
                game_ptr->background.setScale(float(event.size.width) / float(game_ptr->background.getTexture()->getSize().x),
                                              float(event.size.height) / float(game_ptr->background.getTexture()->getSize().y));
                break;
            default:
                break;
        }
    }
}
