//
//  GUI.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/15/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "GUI.h"

sf::Vector2f GUI::get_size() {
    return sf::Vector2f(dimensions.x, dimensions.y * entries.size());
}

int GUI::get_entry(const sf::Vector2f mouse_pos) {
    // If there are no entries then outside the menu.
    if(entries.size() == 0)
        return -1;
    if(!visible)
        return -1;
    
    for(int i = 0; i < entries.size(); ++i) {
        // Translate point to use the entry's local coordinates.
        sf::Vector2f point = mouse_pos;
        point += entries[i].shape.getOrigin();
        point -= entries[i].shape.getPosition();
        
        if(point.x < 0 || point.x > entries[i].shape.getScale().x * dimensions.x)
            continue;
        if(point.y < 0 || point.y > entries[i].shape.getScale().y * dimensions.y)
            continue;
        return i;
    }
    
    return -1;
}

void GUI::set_entry_text(int entry, std::string text) {
    if(entry >= entries.size() || entry < 0)
        return;
    entries[entry].text.setString(text);
}

void GUI::set_dimensions(sf::Vector2f dimensions_) {
    dimensions = dimensions_;
    
    for(auto& entry : entries) {
        entry.shape.setSize(dimensions);
        entry.text.setCharacterSize(dimensions.y - style.border_size - padding);
    }
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(!visible)
        return;
    
    // Draw each entry of the menu.
    for(auto entry : entries) {
        target.draw(entry.shape);
        target.draw(entry.text);
    }
}

void GUI::show() {
    sf::Vector2f offset(0.0f, 0.0f);
    
    visible = true;
    
    // Draw each entry of the menu.
    for(auto& entry : entries) {
        // Set the origin of the entry.
        sf::Vector2f origin = getOrigin();
        origin -= offset;
        entry.shape.setOrigin(origin);
        entry.text.setOrigin(origin);
        
        // Compute the position of the entry.
        entry.shape.setPosition(getPosition());
        entry.text.setPosition(getPosition());
        
        if(horizontal)
            offset.x += dimensions.x;
        else
            offset.y += dimensions.y;
    }    
}

void GUI::hide() {
    visible = false;
}

// Highlights an entry of the menu.
void GUI::highlight(const int entry) {
    for(int i = 0; i < entries.size(); ++i) {
        if(i == entry) {
            entries[i].shape.setFillColor(style.body_highlight_col);
            entries[i].shape.setOutlineColor(style.border_highlight_col);
            entries[i].text.setColor(style.text_highlight_col);
        }
        else {
            entries[i].shape.setFillColor(style.body_col);
            entries[i].shape.setOutlineColor(style.border_col);
            entries[i].text.setColor(style.text_col);
        }
    }
}

// Return the message bound to the entry.
std::string GUI::activate(const int entry) {
    if(entry == -1)
        return "null";
    return entries[entry].message;
}

std::string GUI::activate(sf::Vector2f mousePos) {
    int entry = get_entry(mousePos);
    return activate(entry);
}
