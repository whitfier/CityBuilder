//
//  Game_state_start.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Game_state_start_h
#define Game_state_start_h

#include "Game_state.h"
#include <SFML/Graphics.hpp>

class Game_state_start : public Game_state {
public:
    Game_state_start(std::shared_ptr<Game> game_ptr);
    
    void draw(const float dt) override;
    
    void update(const float dt) override;
    
    void handle_input() override;
    
private:
    sf::View view;
    void load_game();
};

#endif /* Game_state_start_h */
