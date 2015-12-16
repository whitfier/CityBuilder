//
//  City.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/15/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef City_h
#define City_h

#include "Map.h"

class City {
public:
    City() :    birth_rate(0.00055),
                death_rate(0.00023),
                prop_can_work(0.50),
                population_pool(0),
                population(0),
                employment_pool(0),
                employable(0),
                residential_tax(0.05),
                commercial_tax(0.05),
                industrial_tax(0.05),
                earnings(0),
                funds(0),
                current_time(0.0),
                time_per_day(1.0),
                day(0) {}
    
    City(std::string cityName, int tileSize, std::map<std::string, Tile>& tileAtlas) : City()
    {
        map.tile_size = tileSize;
        load(cityName, tileAtlas);
    }
    
    void load(std::string cityName, std::map<std::string, Tile>& tileAtlas);
    void save(std::string cityName);
    
    void update(float dt);
    void bulldoze(const Tile& tile);
    void shuffle_tiles();
    void tile_changed();
    
    double get_homeless() { return population_pool; }
    double get_unemployed() { return employment_pool; }
    
    Map map;
    
    double population;
    double employable;
    
    double residential_tax;
    double commercial_tax;
    double industrial_tax;
    
    // Running total of city earnings (from tax etc) this month.
    double earnings;
    double funds;
    
    int day;
    
private:
    float current_time;
    float time_per_day;
    
    std::vector<int> shuffled_tiles;
    
    // Number of residents who are not in a residential zone.
    double population_pool;
    
    // Number of residents who are not currently employed but can work.
    double employment_pool;
    
    // Proportion of citizens who can work.
    float prop_can_work;
    
    // Proportion of residents who die/give birth each day.
    // Estimate for death rate = 1 / (life expectancy * 360)
    // Current world values are 0.000055 and 0.000023, respectively
    double birth_rate;
    double death_rate;
    
    double distributePool(double& pool, Tile& tile, double rate);
};

#endif // City_h */
