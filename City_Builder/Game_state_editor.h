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
#include "City.h"
#include "GUI.h"
#include <SFML/Graphics.hpp>

enum class Action_state_e { NONE, PANNING, SELECTING };

class Game_state_editor : public Game_state {
public:
    Game_state_editor(std::shared_ptr<Game> game_ptr, bool new_game);
    
    void draw(const float dt) override;
    
    void update(const float dt) override;
    
    void handle_input() override;
    
private:
    Action_state_e action_state;
    sf::Vector2i panning_anchor;
    float zoom_level;
    sf::View game_view;
    sf::View gui_view;
    City city;
	sf::Vector2f selection_start_pos;
	sf::Vector2f selection_end_pos;
    sf::Vector2i selection_start;
    sf::Vector2i selection_end;
    Tile* current_tile;
    std::map<std::string, GUI> gui_system;
};

#endif /* Game_state_editor_h */
