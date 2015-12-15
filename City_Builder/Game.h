//
//  Game.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "Texture_manager.h"
#include "Tile.h"
#include "GUI.h"
#include <SFML/Graphics.hpp>
#include <stack>
#include <map>

class Game_state;

class Game {
public:
    Game();
    ~Game();
    
    // state operations
    void push_state(std::shared_ptr<Game_state> state);
    void pop_state();
    void change_state(std::shared_ptr<Game_state> state);
    std::shared_ptr<Game_state> peek_state();
    
    // main game loop
    void game_loop();
    
    // Public variables (EW!)
    sf::RenderWindow window;
    std::stack<std::shared_ptr<Game_state>> states;
    Texture_manager texture_mgr;
    sf::Sprite background;
    const static int tile_size = 8;
    std::map<std::string, Tile> tile_atlas;
    std::map<std::string, GUI_style> stylesheets;
    std::map<std::string, sf::Font> fonts;
    
private:
    void load_textures();
    void load_tiles();
    void load_style_sheets();
    void load_fonts();
};

#endif /* Game_h */
