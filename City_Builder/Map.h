//
//  Map.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

class Map {
public:
    Map() : tile_size(8), width(0), height(0), num_selected(0) {num_regions[0] = 1;}
    
    // Construct map from file
    Map(const std::string& filename, unsigned int width, unsigned int height,
        std::map<std::string, Tile>& tile_atlas) : tile_size(8), num_selected(0)
    {
        load(filename, width, height, tile_atlas);
    }
    
    // Load map from disk
    void load(const std::string& filename, unsigned int width, unsigned int height,
              std::map<std::string, Tile>& tile_atlas);
    
    // Save map to disk
    void save(const std::string& filename);
    
    // Draw the map
    void draw(sf::RenderWindow& window, float dt);
    
    // Checks if one position in the map is connected to another by only traversing tiles in the whitelist
    void find_connected_regions(std::vector<Tile_type_e> whitelist, int type);
    
    // Update the direction of directional tiles so that they face the correct way.
    // Used to orient roads, pylons, rivers etc
    void update_direction(Tile_type_e tile_type);
    
    // Select the tiles within the bounds
    void select(sf::Vector2i start, sf::Vector2i end, std::vector<Tile_type_e> blacklist);
    
    // Deselect all tiles
    void clear_selected();
    
    std::vector<Tile> tiles;
    unsigned int width;
    unsigned int height;
    unsigned int tile_size;
    
    // 0 = Deselected, 1 = Selected, 2 = Invalid
    std::vector<char> selected;
    unsigned int num_selected;

private:
    std::vector<int> resources;
    
    
    unsigned int numSelected;
    
    unsigned int num_regions[1];
    
    void depthfirstsearch(std::vector<Tile_type_e>& whitelist,
                          sf::Vector2i pos, int label, int type);
};

#endif /* Map_h */
