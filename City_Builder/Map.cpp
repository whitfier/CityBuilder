//
//  Map.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Map.h"
#include <fstream>
#include <iostream>

// Load map from disk
void Map::load(const std::string& filename, unsigned int width_, unsigned int height_,
               std::map<std::string, Tile>& tile_atlas)
{
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::in | std::ios::binary);
    
    width = width_;
    height = height_;
    
    for (int pos = 0; pos < width * height; ++pos) {
        resources.push_back(255);
        selected.push_back(0);

        Tile_type_e tile_type;
        inputFile.read((char*)&tile_type, sizeof(int));
        
        switch(tile_type) {
            default:
            case Tile_type_e::VOID:
            case Tile_type_e::GRASS:
                tiles.push_back(tile_atlas.at("grass"));
                break;
            case Tile_type_e::FOREST:
                tiles.push_back(tile_atlas.at("forest"));
                break;
            case Tile_type_e::WATER:
                tiles.push_back(tile_atlas.at("water"));
                break;
            case Tile_type_e::RESIDENTIAL:
                tiles.push_back(tile_atlas.at("residential"));
                break;
            case Tile_type_e::COMMERCIAL:
                tiles.push_back(tile_atlas.at("commercial"));
                break;
            case Tile_type_e::INDUSTRIAL:
                tiles.push_back(tile_atlas.at("industrial"));
                break;
            case Tile_type_e::ROAD:
                tiles.push_back(tile_atlas.at("road"));
                break;
        }
        
        Tile& tile = tiles.back();
        inputFile.read((char*)&tile.tile_variant, sizeof(int));
        inputFile.read((char*)&tile.regions, sizeof(int)*1);
        inputFile.read((char*)&tile.population, sizeof(double));
        inputFile.read((char*)&tile.stored_goods, sizeof(float));
    }
    
    inputFile.close();
}

void Map::save(const std::string& filename) {
    std::ofstream outputFile;
    outputFile.open(filename, std::ios::out | std::ios::binary);
    
    for (auto tile : tiles) {
        outputFile.write((char*)&tile.tile_type, sizeof(int));
        outputFile.write((char*)&tile.tile_variant, sizeof(int));
        outputFile.write((char*)&tile.regions, sizeof(int)*1);
        outputFile.write((char*)&tile.population, sizeof(double));
        outputFile.write((char*)&tile.stored_goods, sizeof(float));
    }
    
    outputFile.close();
    
    return;
}

void Map::draw(sf::RenderWindow& window, float dt) {
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Set the position of the tile in the 2d world
            sf::Vector2f pos;
            pos.x = (x - y) * tile_size + width * tile_size;
            pos.y = (x + y) * tile_size * 0.5;
            tiles[y * width + x].sprite.setPosition(pos);
            
            // Change the color if the tile is selected
            if(selected[y * width + x])
                tiles[y * width + x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
            else
                tiles[y * width + x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));
            
            // Draw the tile
            tiles[y * width + x].draw(window, dt);
        }
    }
}

void Map::update_direction(Tile_type_e tile_type) {
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int pos = y * width + x;
            
            if (tiles[pos].tile_type != tile_type)
                continue;
            
            bool adjacentTiles[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
            
            // Check for adjacent tiles of the same type
            if (x > 0 && y > 0)
                adjacentTiles[0][0] = (tiles[(y-1)*width+(x-1)].tile_type == tile_type);
            if (y > 0)
                adjacentTiles[0][1] = (tiles[(y-1)*width+(x  )].tile_type == tile_type);
            if (x < width-1 && y > 0)
                adjacentTiles[0][2] = (tiles[(y-1)*width+(x+1)].tile_type == tile_type);
            if (x > 0)
                adjacentTiles[1][0] = (tiles[(y  )*width+(x-1)].tile_type == tile_type);
            if (x < width-1)
                adjacentTiles[1][2] = (tiles[(y  )*width+(x+1)].tile_type == tile_type);
            if (x > 0 && y < height-1)
                adjacentTiles[2][0] = (tiles[(y+1)*width+(x-1)].tile_type == tile_type);
            if (y < height-1)
                adjacentTiles[2][1] = (tiles[(y+1)*width+(x  )].tile_type == tile_type);
            if (x < width-1 && y < height-1)
                adjacentTiles[2][2] = (tiles[(y+1)*width+(x+1)].tile_type == tile_type);
            
            // Change the tile variant depending on the tile position
            if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
                tiles[pos].tile_variant = 2;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
                tiles[pos].tile_variant = 7;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
                tiles[pos].tile_variant = 8;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
                tiles[pos].tile_variant = 9;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
                tiles[pos].tile_variant = 10;
            else if (adjacentTiles[1][0] && adjacentTiles[1][2])
                tiles[pos].tile_variant = 0;
            else if (adjacentTiles[0][1] && adjacentTiles[2][1])
                tiles[pos].tile_variant = 1;
            else if (adjacentTiles[2][1] && adjacentTiles[1][0])
                tiles[pos].tile_variant = 3;
            else if (adjacentTiles[0][1] && adjacentTiles[1][2])
                tiles[pos].tile_variant = 4;
            else if (adjacentTiles[1][0] && adjacentTiles[0][1])
                tiles[pos].tile_variant = 5;
            else if (adjacentTiles[2][1] && adjacentTiles[1][2])
                tiles[pos].tile_variant = 6;
            else if (adjacentTiles[1][0])
                tiles[pos].tile_variant = 0;
            else if (adjacentTiles[1][2])
                tiles[pos].tile_variant = 0;
            else if (adjacentTiles[0][1])
                tiles[pos].tile_variant = 1;
            else if (adjacentTiles[2][1])
                tiles[pos].tile_variant = 1;
        }
    }
}

void Map::depthfirstsearch(std::vector<Tile_type_e>& whitelist,
                           sf::Vector2i pos, int label, int type = 0)
{
    if (pos.x < 0 || pos.x >= width)
        return;
    if (pos.y < 0 || pos.y >= height)
        return;
    if (tiles[pos.y*width+pos.x].regions[type] != 0)
        return;
    
    bool found = false;
    for (auto type : whitelist) {
        if (type == tiles[pos.y*width+pos.x].tile_type) {
            found = true;
            break;
        }
    }
    
    if (!found)
        return;
    
    tiles[pos.y*width+pos.x].regions[type] = label;
    
    depthfirstsearch(whitelist, pos + sf::Vector2i(-1,  0), label, type);
    depthfirstsearch(whitelist, pos + sf::Vector2i(0 ,  1), label, type);
    depthfirstsearch(whitelist, pos + sf::Vector2i(1 ,  0), label, type);
    depthfirstsearch(whitelist, pos + sf::Vector2i(0 , -1), label, type);
    
    return;
}

void Map::find_connected_regions(std::vector<Tile_type_e> whitelist, int type = 0) {
    int regions = 1;
    
    for (auto& tile : tiles)
        tile.regions[type] = 0;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool found = false;
            
            for (auto type : whitelist) {
                if (type == tiles[y * width + x].tile_type) {
                    found = true;
                    break;
                }
            }
            
            if (tiles[y * width + x].regions[type] == 0 && found)
                depthfirstsearch(whitelist, sf::Vector2i(x, y), regions++, type);
        }
    }
    num_regions[type] = regions;
}

void Map::clear_selected() {
    for(auto& tile : selected)
        tile = 0;
    numSelected = 0;
}

void Map::select(sf::Vector2i start, sf::Vector2i end, std::vector<Tile_type_e> blacklist) {
    // Swap coordinates if necessary
    if(end.y < start.y)
        std::swap(start.y, end.y);
    if(end.x < start.x)
        std::swap(start.x, end.x);
    
    // Clamp in range
    if(end.x >= width)
        end.x = width - 1;
    else if(end.x < 0)
        end.x = 0;
    
    if(end.y >= height)
        end.y = height - 1;
    else if(end.y < 0)
        end.y = 0;
    
    if(start.x >= width)
        start.x = width - 1;
    else if(start.x < 0)
        start.x = 0;
    
    if (start.y >= height)
        start.y = height - 1;
    else if(start.y < 0)
        start.y = 0;
    
    for(int y = start.y; y <= end.y; ++y) {
        for(int x = start.x; x <= end.x; ++x) {
            // Check if the tile type is in the blacklist.
            // If it is, mark it as invalid, otherwise select it
            selected[y * width + x] = 1;
            ++numSelected;
            for(auto type : blacklist) {
                if(tiles[y * width + x].tile_type == type) {
                    selected[y * width + x] = 2;
                    --numSelected;
                    break;
                }
            }
        }
    }
}

Tile* Map::select(sf::Vector2i pos) {
	if (pos.x >= width)
		pos.x = width - 1;
	else if (pos.x < 0)
		pos.x = 0;

	if (pos.y >= height)
		pos.y = height - 1;
	else if (pos.y < 0)
		pos.y = 0;
	return &tiles[pos.y * width + pos.x];
}

void Map::highlight_tile(sf::Vector2i pos) {
	std::cout << pos.x << " " << pos.y << std::endl;
	if (pos.x >= width
		|| (pos.x < 0)
		|| (pos.y >= height)
		|| (pos.y < 0))
		return;
	selected[pos.y * width + pos.x] = 1;
	tiles[pos.y * width + pos.x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
}
