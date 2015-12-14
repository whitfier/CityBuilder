//
//  Game.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Game.h"
#include "Game_state.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using std::shared_ptr;

Game::Game() {
    load_textures();
    window.create(sf::VideoMode(800, 600), "City Builder");
    window.setFramerateLimit(60);
    background.setTexture(texture_mgr.getRef("background"));
}

Game::~Game() {
    while(!states.empty())
        pop_state();
}

void Game::push_state(shared_ptr<Game_state> state) {
    states.push(state);
}

void Game::pop_state() {
    states.pop();
}

void Game::change_state(shared_ptr<Game_state> state) {
    if(!states.empty())
        pop_state();
    push_state(state);
}

shared_ptr<Game_state> Game::peek_state() {
    if(states.empty())
        return nullptr;
    return states.top();
}

void Game::game_loop() {
    sf::Clock clock;
    
    while(window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        
        if(peek_state() == nullptr)
            continue;
        peek_state()->handle_input();
        peek_state()->update(dt);
        window.clear(sf::Color::Black);
        peek_state()->draw(dt);
        window.display();
    }
}

void Game::load_textures() {
    texture_mgr.loadTexture("background", "media/background.png");
}
