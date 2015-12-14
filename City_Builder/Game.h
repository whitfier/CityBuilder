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
#include <stack>
#include <SFML/Graphics.hpp>

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
    
    sf::RenderWindow window;
    std::stack<std::shared_ptr<Game_state>> states;
    Texture_manager texture_mgr;
    sf::Sprite background;
    
private:
    void load_textures();
};

#endif /* Game_h */
