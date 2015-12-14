//
//  Texture_manager.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Texture_manager_h
#define Texture_manager_h

#include <SFML/Graphics.hpp>
#include <map>

class Texture_manager {
public:
    // Add a texture from a file
    void loadTexture(const std::string& name, const std::string& filename);
    
    // Translate an id into a reference
    sf::Texture& getRef(const std::string& texture);
    
private:
    std::map<std::string, sf::Texture> textures;
};

#endif /* Texture_manager_h */
