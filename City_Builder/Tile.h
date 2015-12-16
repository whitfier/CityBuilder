//
//  Tile.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Tile_h
#define Tile_h

#include "animation_handler.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class Tile_type_e { VOID, GRASS, FOREST, WATER, RESIDENTIAL, COMMERCIAL, INDUSTRIAL, ROAD };

std::string tile_type_to_str(Tile_type_e type);

class Tile {
public:
    Tile() {}
    Tile(const unsigned int tile_size, const unsigned int height, sf::Texture& texture,
         const std::vector<Animation>& animations,
         const Tile_type_e tile_type_, const unsigned int cost_, const unsigned int max_pop_per_level_,
         const unsigned int max_levels_)
    : tile_type(tile_type_), tile_variant(0), cost(cost_), population(0), max_pop_per_level(max_pop_per_level_),
        max_levels(max_levels_), production(0), stored_goods(0)
    {
        regions[0] = 0;
        sprite.setOrigin(sf::Vector2f(0.0f, tile_size * (height - 1)));
        sprite.setTexture(texture);
        anim_handler.frame_size = sf::IntRect(0, 0, tile_size * 2, tile_size * height);
        for(auto animation : animations) {
            anim_handler.add_animation(animation);
        }
        anim_handler.update(0.0f);
    }
    
    void draw(sf::RenderWindow& window, float dt);
    
    void update();

    // Return a string containing the display cost of the tile
    std::string get_cost() {return std::to_string(cost);}

    int tile_variant;               // Tile variant allows for different looking versions of the same tile
    unsigned int regions[1];        // Region IDs of the tile, tiles in the same region are connected.
    double population;              // Current residents / employees
    float stored_goods;             // Goods stored
    Tile_type_e tile_type;
    sf::Sprite sprite;
    unsigned int max_pop_per_level; // Maximum population per growth stage / tile variant
    float production;               // Production output per customer/worker per day, either monetary or goods
    unsigned int cost;              // Placement cost of the tile

private:
    Animation_handler anim_handler;
    unsigned int max_levels;        // Maximum number of building levels
};

#endif /* Tile_h */
