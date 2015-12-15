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

using std::shared_ptr; using std::make_pair;

Game::Game() {
    load_textures();
    load_tiles();
    load_fonts();
    load_style_sheets();
    window.create(sf::VideoMode(800, 600), "City Builder");
    window.setFramerateLimit(60);
    background.setTexture(texture_mgr.get_ref("background"));
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
    texture_mgr.load_texture("grass",       "media/grass.png");
    texture_mgr.load_texture("forest",      "media/forest.png");
    texture_mgr.load_texture("water",       "media/water.png");
    texture_mgr.load_texture("residential", "media/residential.png");
    texture_mgr.load_texture("commercial",  "media/commercial.png");
    texture_mgr.load_texture("industrial",  "media/industrial.png");
    texture_mgr.load_texture("road",        "media/road.png");
    texture_mgr.load_texture("background",  "media/background.png");
}

void Game::load_tiles() {
    Animation static_anim(0, 0, 1.0f);
    tile_atlas.insert(make_pair("grass",    Tile(tile_size, 1, texture_mgr.get_ref("grass"),
                                                 {static_anim},
                                                 Tile_type_e::GRASS, 50, 0, 1)));
    
    tile_atlas.insert(make_pair("forest",   Tile(tile_size, 1, texture_mgr.get_ref("forest"),
                                                 {static_anim},
                                                 Tile_type_e::FOREST, 100, 0, 1)));
    
    tile_atlas.insert(make_pair("water",    Tile(tile_size, 1, texture_mgr.get_ref("water"),
                                                 {Animation(0, 3, 0.5f),Animation(0, 3, 0.5f),Animation(0, 3, 0.5f)},
                                                 Tile_type_e::WATER, 0, 0, 1)));
    
    tile_atlas.insert(make_pair("residential",  Tile(tile_size, 2, texture_mgr.get_ref("residential"),
                                                     {static_anim,static_anim,static_anim,static_anim, static_anim, static_anim},
                                                     Tile_type_e::RESIDENTIAL, 300, 50, 6)));
    
    tile_atlas.insert(make_pair("commercial",   Tile(tile_size, 2, texture_mgr.get_ref("commercial"),
                                                     {static_anim, static_anim, static_anim, static_anim},
                                                     Tile_type_e::COMMERCIAL, 300, 50, 4)));
    
    tile_atlas.insert(make_pair("industrial",   Tile(tile_size, 2, texture_mgr.get_ref("industrial"),
                                                     {static_anim, static_anim, static_anim,static_anim},
                                                     Tile_type_e::INDUSTRIAL, 300, 50, 4)));
    
    tile_atlas.insert(make_pair("road",     Tile(tile_size, 1, texture_mgr.get_ref("road"),
                                                 {static_anim, static_anim, static_anim,static_anim, static_anim,
                                                     static_anim,static_anim, static_anim, static_anim,static_anim, static_anim},
                                                 Tile_type_e::ROAD, 100, 0, 1)));
}

void Game::load_fonts() {
    sf::Font font;
    font.loadFromFile("media/font.ttf");
    fonts["main_font"] = font;
}

void Game::load_style_sheets() {
    stylesheets["button"] = GUI_style(&fonts.at("main_font"), 1,
                                      sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,0x94),
                                      sf::Color(0x00,0x00,0x00), sf::Color(0x61,0x61,0x61),
                                      sf::Color(0x94,0x94,0x94), sf::Color(0x00,0x00,0x00));
    stylesheets["text"] = GUI_style(&fonts.at("main_font"), 0,
                                    sf::Color(0x00,0x00,0x00,0x00), sf::Color(0x00,0x00,0x00),
                                    sf::Color(0xff,0xff,0xff), sf::Color(0x00,0x00,0x00,0x00),
                                    sf::Color(0x00,0x00,0x00), sf::Color(0xff,0x00,0x00));
}
