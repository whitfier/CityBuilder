//
//  Texture_manager.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Texture_manager.h"

using std::runtime_error;

void Texture_manager::loadTexture(const std::string& name, const std::string& filename) {
    // Load the texture
    sf::Texture tex;
    tex.loadFromFile(filename);
    
    // Add it to the list of textures
    textures[name] = tex;
}

sf::Texture& Texture_manager::getRef(const std::string& texture)
{
    auto texture_it = textures.find(texture);
    if (texture_it == textures.end())
        throw runtime_error("Texture not found!");
    return texture_it->second;
}
