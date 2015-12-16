//
//  City.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/15/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "City.h"
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>

double City::distributePool(double& pool, Tile& tile, double rate = 0.0) {
    const static int moveRate = 4;
    
    unsigned int max_pop = tile.max_pop_per_level * (tile.tile_variant + 1);
    
    // If there is room in the zone, move up to 4 people from the pool into the zone
    if (pool > 0) {
        int moving = max_pop - tile.population;
        
        if (moving > moveRate)
            moving = moveRate;
        if (pool - moving < 0)
            moving = pool;
        
        pool -= moving;
        tile.population += moving;
    }
    
    // Adjust the tile population for births and deaths
    tile.population += tile.population * rate;
    
    // Move population that cannot be sustained by the tile into the pool
    if (tile.population > max_pop) {
        pool += tile.population - max_pop;
        tile.population = max_pop;
    }
    
    return tile.population;
}

void City::bulldoze(const Tile& tile) {
    // Replace selected tiles on the map with tile and update populations accordingly
    for (int pos = 0; pos < map.width * map.height; ++pos) {
        if (map.selected[pos] == 1) {
            if (map.tiles[pos].tile_type == Tile_type_e::RESIDENTIAL)
                population_pool += map.tiles[pos].population;
            
            else if (map.tiles[pos].tile_type == Tile_type_e::COMMERCIAL)
                employment_pool += map.tiles[pos].population;
            
            else if (map.tiles[pos].tile_type == Tile_type_e::INDUSTRIAL)
                employment_pool += map.tiles[pos].population;
            
            map.tiles[pos] = tile;
        }
    }
}

void City::shuffle_tiles() {
    while (shuffled_tiles.size() < map.tiles.size())
        shuffled_tiles.push_back(0);

    std::iota(shuffled_tiles.begin(), shuffled_tiles.end(), 1);
    std::random_shuffle(shuffled_tiles.begin(), shuffled_tiles.end());
}

void City::tile_changed() {
    map.update_direction(Tile_type_e::ROAD);
    map.find_connected_regions({Tile_type_e::ROAD, Tile_type_e::RESIDENTIAL,
        Tile_type_e::COMMERCIAL, Tile_type_e::INDUSTRIAL}, 0);
}

void City::load(std::string cityName, std::map<std::string, Tile>& tileAtlas) {
    int width = 0;
    int height = 0;
    
    std::ifstream inputFile(cityName + "_cfg.dat", std::ios::in);
    
    std::string line;
    
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, '='))
        {
            std::string value;
            if (std::getline(lineStream, value))
            {
                if (key == "width")                 width           = std::stoi(value);
                else if (key == "height")           height          = std::stoi(value);
                else if (key == "day")              day             = std::stoi(value);
                else if (key == "population_pool")  population_pool = std::stod(value);
                else if (key == "employment_pool")  employment_pool = std::stod(value);
                else if (key == "population")       population      = std::stod(value);
                else if (key == "employable")       employable      = std::stod(value);
                else if (key == "birth_rate")       birth_rate      = std::stod(value);
                else if (key == "death_rate")       death_rate      = std::stod(value);
                else if (key == "residential_tax")  residential_tax = std::stod(value);
                else if (key == "commercial_tax")   commercial_tax  = std::stod(value);
                else if (key == "industrial_tax")   industrial_tax  = std::stod(value);
                else if (key == "funds")            funds           = std::stod(value);
                else if (key == "earnings")         earnings        = std::stod(value);
            }
            else
                std::cerr << "Error, no value for key " << key << std::endl;
        }
    }
    
    inputFile.close();
    
    map.load(cityName + "_map.dat", width, height, tileAtlas);
    tile_changed();
}

void City::save(std::string cityName) {
    std::ofstream outputFile(cityName + "_cfg.dat", std::ios::out);
    
    outputFile << "width="          << map.width        << std::endl;
    outputFile << "height="         << map.height       << std::endl;
    outputFile << "day="            << day              << std::endl;
    outputFile << "population_pool="<< population_pool  << std::endl;
    outputFile << "employment_pool="<< employment_pool  << std::endl;
    outputFile << "population="     << population       << std::endl;
    outputFile << "employable="     << employable       << std::endl;
    outputFile << "birth_rate="     << birth_rate       << std::endl;
    outputFile << "death_rate="     << death_rate       << std::endl;
    outputFile << "residential_tax="<< residential_tax  << std::endl;
    outputFile << "commercial_tax=" << commercial_tax   << std::endl;
    outputFile << "industrial_tax=" << industrial_tax   << std::endl;
    outputFile << "funds="          << funds            << std::endl;
    outputFile << "earnings="       << earnings         << std::endl;
    
    outputFile.close();
    
    map.save(cityName + "_map.dat");
}

void City::update(float dt) {
    double pop_total = 0;
    double commercial_revenue = 0;
    double industrial_revenue = 0;
    
    // Update the game time
    current_time += dt;
    if (current_time < time_per_day)
        return;
    ++day;
    current_time = 0.0;
    if (day % 30 == 0) {
        funds += earnings;
        earnings = 0;
    }
    
    // Run first pass of tile updates. Mostly handles pool distribution.
    for (int i = 0; i < map.tiles.size(); ++i) {
        Tile& tile = map.tiles[shuffled_tiles[i]];
        
        if (tile.tile_type == Tile_type_e::RESIDENTIAL) {
            // Redistribute the pool and increase the population total by the tile's population
            distributePool(population_pool, tile, birth_rate - death_rate);
            pop_total += tile.population;
        }
        else if (tile.tile_type == Tile_type_e::COMMERCIAL) {
            // Hire people.
            if (rand() % 100 < 15 * (1.0 - commercial_tax))
                distributePool(employment_pool, tile, 0.0);
        }
        else if (tile.tile_type == Tile_type_e::INDUSTRIAL) {
            // Extract resources from the ground.
            if (map.resources[i] > 0 && rand() % 100 < population) {
                ++tile.production;
                --map.resources[i];
            }
            
            // Hire people.
            if (rand() % 100 < 15 * (1.0-industrial_tax))
                distributePool(employment_pool, tile, 0.0);
        }
        tile.update();
    }
    
    // Run second pass. Mostly handles goods manufacture
    for(int i = 0; i < map.tiles.size(); ++i) {
        Tile& tile = map.tiles[shuffled_tiles[i]];
        
        if(tile.tile_type == Tile_type_e::INDUSTRIAL) {
            int received_resources = 0;
            // Receive resources from smaller and connected zones
            for(auto& tile2 : map.tiles) {
                if(tile2.regions[0] == tile.regions[0] && tile2.tile_type == Tile_type_e::INDUSTRIAL) {
                    if(tile2.production > 0) {
                        ++received_resources;
                        --tile2.production;
                    }
                    if(received_resources >= tile.tile_variant + 1)
                        break;
                }
            }
            // Turn resources into goods
            tile.stored_goods += (received_resources + tile.production) * (tile.tile_variant + 1);
        }
    }
    
    // Run third pass. Mostly handles goods distribution.
    for(int i = 0; i < map.tiles.size(); ++i) {
        Tile& tile = map.tiles[shuffled_tiles[i]];
        
        if(tile.tile_type == Tile_type_e::COMMERCIAL) {
            int receivedGoods = 0;
            double maxCustomers = 0.0;
            
            for(auto& tile2 : map.tiles) {
                if(tile2.regions[0] == tile.regions[0] &&
                   tile2.tile_type == Tile_type_e::INDUSTRIAL &&
                   tile2.stored_goods > 0)
                {
                    while(tile2.stored_goods > 0 && receivedGoods != tile.tile_variant+1) {
                        --tile2.stored_goods;
                        ++receivedGoods;
                        industrial_revenue += 100 * (1.0-industrial_tax);
                    }
                }
                else if(tile2.regions[0] == tile.regions[0] && tile2.tile_type == Tile_type_e::RESIDENTIAL) {
                    maxCustomers += tile2.population;
                }
                if(receivedGoods == tile.tile_variant+1)
                    break;
            }
            
            // Calculate the overall revenue for the tile.
            tile.production = (receivedGoods * 100.0 + rand() % 20) * (1.0 - commercial_tax);
            
            double revenue = tile.production * maxCustomers * tile.population / 100.0;
            commercial_revenue += revenue;
        }
    }
    
    // Adjust population pool for births and deaths
    population_pool += population_pool * (birth_rate - death_rate);
    pop_total += population_pool;
    
    // Adjust the employment pool for the changing population
    float new_workers = (pop_total - population) * prop_can_work;
    new_workers *= new_workers < 0 ? -1 : 1;
    employment_pool += new_workers;
    employable += new_workers;
    if(employment_pool < 0)
        employment_pool = 0;
    if(employable < 0)
        employable = 0;
    
    // Update the city population
    population = pop_total;
    
    // Calculate city income from tax
    earnings = (population - population_pool) * 15 * residential_tax;
    earnings += commercial_revenue * commercial_tax;
    earnings += industrial_revenue * industrial_tax;
}
