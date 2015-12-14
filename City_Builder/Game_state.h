//
//  Game_state.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Game_state_h
#define Game_state_h

#include <memory>

class Game;

class Game_state {
public:
    Game_state(std::shared_ptr<Game> game_ptr_) : game_ptr(game_ptr_) {}
    
    std::shared_ptr<Game> get_game_ptr() {return game_ptr;}
    
    virtual void draw(const float delta) = 0;
    
    virtual void update(const float delta) = 0;
    
    virtual void handle_input() = 0;
    
private:
    std::shared_ptr<Game> game_ptr;
};

#endif /* Game_state_h */
