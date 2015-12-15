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
: Game_state(game_ptr), zoom_level(1.0f), action_state(Action_state_e::NONE),
    selection_start(0,0), selection_end(0,0), current_tile(&game_ptr->tile_atlas.at("grass"))
{
    sf::Vector2f pos = sf::Vector2f(game_ptr->window.getSize());
    gui_view.setSize(pos);
    game_view.setSize(pos);
    pos *= 0.5f;
    gui_view.setCenter(pos);
    game_view.setCenter(pos);
    map = Map("city_map.dat", 64, 64, game_ptr->tile_atlas);
    
    // Center the camera on the map
    sf::Vector2f center(map.width, map.height * 0.5);
    center *= float(map.tile_size);
    game_view.setCenter(center);
}

void Game_state_editor::draw(const float dt) {
    auto game_ptr = get_game_ptr();
    game_ptr->window.clear(sf::Color::Black);
    game_ptr->window.setView(gui_view);
    game_ptr->window.draw(game_ptr->background);
    game_ptr->window.setView(game_view);
    map.draw(game_ptr->window, dt);
}

void Game_state_editor::update(const float dt) {}

void Game_state_editor::handle_input() {
    sf::Event event;
    auto game_ptr = get_game_ptr();

    while(game_ptr->window.pollEvent(event)) {
        switch(event.type) {
            // Close the window
            case sf::Event::Closed:
                game_ptr->window.close();
                break;
            // Resize the window
            case sf::Event::Resized:
                game_view.setSize(event.size.width, event.size.height);
                gui_view.setSize(event.size.width, event.size.height);
                game_ptr->background.setPosition(game_ptr->window.mapPixelToCoords(sf::Vector2i(0, 0), gui_view));
                game_ptr->background.setScale(float(event.size.width) / float(game_ptr->background.getTexture()->getSize().x),
                                              float(event.size.height) / float(game_ptr->background.getTexture()->getSize().y));
                break;
            case sf::Event::MouseMoved:
                // Pan the camera
                if(action_state == Action_state_e::PANNING) {
                    sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(game_ptr->window) - panning_anchor);
                    game_view.move(-1.0f * pos * zoom_level);
                    panning_anchor = sf::Mouse::getPosition(game_ptr->window);
                }
                
                // Selecting tiles
                else if (action_state == Action_state_e::SELECTING) {
                    sf::Vector2f pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), game_view);
                    selection_end.x = pos.y / (map.tile_size) + pos.x / (2 * map.tile_size) - map.width * 0.5 - 0.5;
                    selection_end.y = pos.y / (map.tile_size) - pos.x / (2 * map.tile_size) + map.width * 0.5 + 0.5;
                    
                    map.clear_selected();
                    if(current_tile->tile_type == Tile_type_e::GRASS) {
                        map.select(selection_start, selection_end, {current_tile->tile_type, Tile_type_e::WATER});
                    }
                    else {
                        map.select(selection_start, selection_end,
                                   {
                                       current_tile->tile_type, Tile_type_e::FOREST,
                                       Tile_type_e::WATER,      Tile_type_e::ROAD,
                                       Tile_type_e::RESIDENTIAL,Tile_type_e::COMMERCIAL,
                                       Tile_type_e::INDUSTRIAL
                                   });
                    }
                }
                break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Middle) {
                    // Start panning
                    if(action_state != Action_state_e::PANNING) {
                        action_state = Action_state_e::PANNING;
                        panning_anchor = sf::Mouse::getPosition(game_ptr->window);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Left)
                {
                    // Select map tile
                    if(action_state != Action_state_e::SELECTING) {
                        action_state = Action_state_e::SELECTING;
                        sf::Vector2f pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), game_view);
                        selection_start.x = pos.y / (map.tile_size) + pos.x / (2 * map.tile_size) - map.width * 0.5 - 0.5;
                        selection_start.y = pos.y / (map.tile_size) - pos.x / (2 * map.tile_size) + map.width * 0.5 + 0.5;
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Right) {
                    // Stop selecting
                    if (action_state == Action_state_e::SELECTING) {
                        action_state = Action_state_e::NONE;
                        map.clear_selected();
                    }
                }
                break;
            case sf::Event::KeyPressed:
                // Start panning
                if (event.key.code == sf::Keyboard::LAlt) {
                    if(action_state != Action_state_e::PANNING) {
                        action_state = Action_state_e::PANNING;
                        panning_anchor = sf::Mouse::getPosition(game_ptr->window);
                    }
                }
                break;
            case sf::Event::MouseButtonReleased:
                // Stop panning
                if(event.mouseButton.button == sf::Mouse::Middle)
                    action_state = Action_state_e::NONE;
                // Stop selecting
                else if(event.mouseButton.button == sf::Mouse::Left) {
                    if(action_state == Action_state_e::SELECTING) {
                        action_state = Action_state_e::NONE;
                        map.clear_selected();
                    }
                }
                break;
            case sf::Event::KeyReleased:
                // Stop panning
                if(event.key.code == sf::Keyboard::LAlt)
                    action_state = Action_state_e::NONE;
                break;
            case sf::Event::MouseWheelScrolled:
                // Zoom the view in
                if (event.mouseWheelScroll.delta < 0) {
                    game_view.zoom(2.0f);
                    zoom_level *= 2.0f;
                }
                // Zoom the view out
                else if (event.mouseWheelScroll.delta > 0) {
                    game_view.zoom(0.5f);
                    zoom_level *= 0.5f;
                }
                break;
            default:
                break;
        }
    }
}
