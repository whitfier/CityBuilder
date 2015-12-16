//
//  Tile.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Tile.h"

using std::string;

void Tile::draw(sf::RenderWindow& window, float dt) {
    // Change the sprite to reflect the tile variant
    anim_handler.change_animation(tile_variant);
    
    // Update the animation
    anim_handler.update(dt);
    
    // Update the sprite
    sprite.setTextureRect(anim_handler.bounds);
    
    // Draw the tile
    window.draw(sprite);
}

void Tile::update() {
    /* If the population is at the maximum value for the tile,
     * there is a small chance that the tile will increase its
     * building stage */
    if((tile_type == Tile_type_e::RESIDENTIAL || tile_type == Tile_type_e::COMMERCIAL || tile_type == Tile_type_e::INDUSTRIAL)
       && population == max_pop_per_level * (tile_variant+1)
       && tile_variant < max_levels)
    {
        if((rand() % 10000) < (1000 / (tile_variant + 1)))
            ++tile_variant;
    }
}

string tile_type_to_str(Tile_type_e type)
{
    switch(type) {
        case Tile_type_e::VOID:         return "Void";
        case Tile_type_e::GRASS:        return "Flatten";
        case Tile_type_e::FOREST:       return "Forest";
        case Tile_type_e::WATER:        return "Water";
        case Tile_type_e::ROAD:         return "Road";
        case Tile_type_e::RESIDENTIAL:  return "Residential Zone";
        case Tile_type_e::COMMERCIAL:   return "Commercial Zone";
        case Tile_type_e::INDUSTRIAL:   return "Industrial Zone";
        default:                        return "Unknown tile type!";
    }
}