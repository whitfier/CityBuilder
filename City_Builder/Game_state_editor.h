//
//  Game_state_editor.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Game_state_editor_h
#define Game_state_editor_h

#include "Game_state.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

enum class Action_state_e { NONE, PANNING, SELECTING };

class Game_state_editor : public Game_state {
public:
    Game_state_editor(std::shared_ptr<Game> game_ptr);
    
    void draw(const float dt) override;
    
    void update(const float dt) override;
    
    void handle_input() override;
    
private:
    Action_state_e action_state;
    sf::Vector2i panning_anchor;
    float zoom_level;
    sf::View game_view;
    sf::View gui_view;
    Map map;
    sf::Vector2i selection_start;
    sf::Vector2i selection_end;
    Tile* current_tile;
};

#endif /* Game_state_editor_h */
