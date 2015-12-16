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
    city = City("city", game_ptr->tile_size, game_ptr->tile_atlas);
    city.shuffle_tiles();
    
    // Center the camera on the map
    sf::Vector2f center(city.map.width, city.map.height * 0.5);
    center *= float(city.map.tile_size);
    game_view.setCenter(center);
    
    /* Create gui elements. */
    gui_system.emplace("rightClickMenu", GUI(sf::Vector2f(196, 16), 2, false, game_ptr->stylesheets.at("button"),
    {std::make_pair("Flatten $" + game_ptr->tile_atlas["grass"].get_cost(), "grass"),
    std::make_pair("Forest $" + game_ptr->tile_atlas["forest"].get_cost(), "forest" ),
    std::make_pair("Residential Zone $" + game_ptr->tile_atlas["residential"].get_cost(), "residential"),
    std::make_pair("Commercial Zone $" + game_ptr->tile_atlas["commercial"].get_cost(), "commercial"),
    std::make_pair("Industrial Zone $" + game_ptr->tile_atlas["industrial"].get_cost(), "industrial"),
    std::make_pair("Road $" + game_ptr->tile_atlas["road"].get_cost(), "road")}));
    
    gui_system.emplace("selectionCostText", GUI(sf::Vector2f(196, 16), 0, false, game_ptr->stylesheets.at("text"),
                                                     { std::make_pair("", "") }));
    
    gui_system.emplace("infoBar", GUI(sf::Vector2f(game_ptr->window.getSize().x / 5 , 16), 2, true, game_ptr->stylesheets.at("button"),
                                           {
                                               std::make_pair("time",          "time"),
                                               std::make_pair("funds",         "funds"),
                                               std::make_pair("population",    "population"),
                                               std::make_pair("employment",    "employment"),
                                               std::make_pair("current tile",  "tile")
                                           }));
    gui_system.at("infoBar").setPosition(sf::Vector2f(0, game_ptr->window.getSize().y - 16));
    gui_system.at("infoBar").show();
}

void Game_state_editor::draw(const float dt) {
    auto game_ptr = get_game_ptr();
    game_ptr->window.clear(sf::Color::Black);
    game_ptr->window.setView(gui_view);
    game_ptr->window.draw(game_ptr->background);
    game_ptr->window.setView(game_view);
    city.map.draw(game_ptr->window, dt);
    game_ptr->window.setView(gui_view);
    for(auto gui : gui_system)
        game_ptr->window.draw(gui.second);

}

void Game_state_editor::update(const float dt)
{
    city.update(dt);
    
    /* Update the info bar at the bottom of the screen */
    gui_system.at("infoBar").set_entry_text(0, "Day: " + std::to_string(city.day));
    gui_system.at("infoBar").set_entry_text(1, "$" + std::to_string(long(city.funds)));
    gui_system.at("infoBar").set_entry_text(2, std::to_string(long(city.population)) + " (" + std::to_string(long(city.get_homeless())) + ")");
    gui_system.at("infoBar").set_entry_text(3, std::to_string(long(city.employable)) + " (" + std::to_string(long(city.get_unemployed())) + ")");
    gui_system.at("infoBar").set_entry_text(4, tile_type_to_str(current_tile->tile_type));
    
    /* Highlight entries of the right click context menu */
    gui_system.at("rightClickMenu").highlight(gui_system.at("rightClickMenu").get_entry(get_game_ptr()->window.mapPixelToCoords(sf::Mouse::getPosition(get_game_ptr()->window), gui_view)));
}

void Game_state_editor::handle_input() {
    sf::Event event;
    auto game_ptr = get_game_ptr();
    
    sf::Vector2f gui_pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), gui_view);
    sf::Vector2f game_pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), game_view);

    while(game_ptr->window.pollEvent(event)) {
        switch(event.type) {
            // Close the window
            case sf::Event::Closed:
                game_ptr->window.close();
                break;
            // Resize the window
            case sf::Event::Resized:
                game_view.setSize(event.size.width, event.size.height);
                game_view.zoom(zoom_level);
                gui_view.setSize(event.size.width, event.size.height);
                gui_system.at("infoBar").set_dimensions(sf::Vector2f(event.size.width / gui_system.at("infoBar").entries.size(), 16));
                gui_system.at("infoBar").setPosition(game_ptr->window.mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), gui_view));
                gui_system.at("infoBar").show();
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
                    selection_end.x = pos.y / (city.map.tile_size)
                        + pos.x / (2 * city.map.tile_size) - city.map.width * 0.5 - 0.5;
                    selection_end.y = pos.y / (city.map.tile_size)
                        - pos.x / (2 * city.map.tile_size) + city.map.width * 0.5 + 0.5;
                    
                    city.map.clear_selected();
                    if(current_tile->tile_type == Tile_type_e::GRASS) {
                        city.map.select(selection_start, selection_end, {current_tile->tile_type, Tile_type_e::WATER});
                    }
                    else {
                        city.map.select(selection_start, selection_end,
                                   {
                                       current_tile->tile_type, Tile_type_e::FOREST,
                                       Tile_type_e::WATER,      Tile_type_e::ROAD,
                                       Tile_type_e::RESIDENTIAL,Tile_type_e::COMMERCIAL,
                                       Tile_type_e::INDUSTRIAL
                                   });
                    }
                    
                    gui_system.at("selectionCostText").set_entry_text(0, "$" +
                        std::to_string(current_tile->cost * city.map.numSelected));
                    if(city.funds <= city.map.numSelected * current_tile->cost)
                        gui_system.at("selectionCostText").highlight(0);
                    else
                        gui_system.at("selectionCostText").highlight(-1);
                    gui_system.at("selectionCostText").setPosition(gui_pos + sf::Vector2f(16, -16));
                    gui_system.at("selectionCostText").show();
                }
                /* Highlight entries of the right click context menu */
                gui_system.at("rightClickMenu").highlight(gui_system.at("rightClickMenu").get_entry(gui_pos));
                break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Middle) {
                    gui_system.at("rightClickMenu").hide();
                    gui_system.at("selectionCostText").hide();
                    // Start panning
                    if(action_state != Action_state_e::PANNING) {
                        action_state = Action_state_e::PANNING;
                        panning_anchor = sf::Mouse::getPosition(game_ptr->window);
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Left)
                {
                    if(gui_system.at("rightClickMenu").visible == true) {
                        // Select a context menu entry.
                        std::string msg = gui_system.at("rightClickMenu").activate(gui_pos);
                        if(msg != "null")
                            current_tile = &game_ptr->tile_atlas.at(msg);
                        gui_system.at("rightClickMenu").hide();
                    }
                    else {
                        // Select map tile
                        if(action_state != Action_state_e::SELECTING) {
                            action_state = Action_state_e::SELECTING;
                            sf::Vector2f pos = game_ptr->window.mapPixelToCoords(sf::Mouse::getPosition(game_ptr->window), game_view);
                            selection_start.x = pos.y / (city.map.tile_size) + pos.x / (2 * city.map.tile_size) - city.map.width * 0.5 - 0.5;
                            selection_start.y = pos.y / (city.map.tile_size) - pos.x / (2 * city.map.tile_size) + city.map.width * 0.5 + 0.5;
                        }
                    }
                    
                }
                else if(event.mouseButton.button == sf::Mouse::Right) {
                    // Stop selecting
                    if (action_state == Action_state_e::SELECTING) {
                        action_state = Action_state_e::NONE;
                        gui_system.at("selectionCostText").hide();
                        city.map.clear_selected();
                    }
                    else {
                        // Open the tile select menu.
                        sf::Vector2f pos = gui_pos;
                        
                        if(pos.x > game_ptr->window.getSize().x - gui_system.at("rightClickMenu").get_size().x)
                            pos -= sf::Vector2f(gui_system.at("rightClickMenu").get_size().x, 0);

                        if(pos.y > game_ptr->window.getSize().y - gui_system.at("rightClickMenu").get_size().y)
                            pos -= sf::Vector2f(0, gui_system.at("rightClickMenu").get_size().y);

                        gui_system.at("rightClickMenu").setPosition(pos);
                        gui_system.at("rightClickMenu").show();
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
                        // Replace tiles if enough funds and a tile is selected
                        if(current_tile != nullptr) {
                            unsigned int cost = current_tile->cost * city.map.numSelected;
                            if(city.funds >= cost) {
                                city.bulldoze(*current_tile);
                                city.funds -= current_tile->cost * city.map.numSelected;
                                city.tile_changed();
                            }
                        }
                        gui_system.at("selectionCostText").hide();
                        action_state = Action_state_e::NONE;
                        city.map.clear_selected();
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
                if (event.mouseWheelScroll.delta < 0 && zoom_level < 1.75f) {
                    float old_zoom = zoom_level;
                    zoom_level += 0.25f;
                    game_view.zoom(zoom_level / old_zoom);
                }
                // Zoom the view out
                else if (event.mouseWheelScroll.delta > 0 && zoom_level > 0.25f) {
                    float old_zoom = zoom_level;
                    zoom_level -= 0.25f;
                    game_view.zoom(zoom_level / old_zoom);
                }
                break;
            default:
                break;
        }
    }
}
