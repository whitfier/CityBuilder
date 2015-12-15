//
//  GUI.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/15/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef GUI_h
#define GUI_h

#include <SFML/Graphics.hpp>

struct GUI_style {
    GUI_style() {}
    GUI_style(sf::Font* font_, float borderSize,sf::Color bodyCol,
              sf::Color borderCol, sf::Color textCol,sf::Color bodyHighlightCol,
              sf::Color borderHighlightCol, sf::Color textHighlightCol)
    : body_col(bodyCol), border_col(borderCol), text_col(textCol), border_highlight_col(bodyHighlightCol),
        text_highlight_col(textHighlightCol), font(font_), border_size(borderSize) {}
    
    sf::Color body_col;
    sf::Color body_highlight_col;
    sf::Color border_col;
    sf::Color border_highlight_col;
    sf::Color text_col;
    sf::Color text_highlight_col;
    sf::Font* font;
    
    float border_size;
};

struct GUI_entry {
    GUI_entry(const std::string& message_, sf::RectangleShape shape_, sf::Text text_)
        : message(message_), shape(shape_), text(text_) {}
    
    // Handles appearance of the entry
    sf::RectangleShape shape;
    
    // String returned when the entry is activated
    std::string message;
    
    // Text displayed on the entry
    sf::Text text;
};

class GUI : public sf::Transformable, public sf::Drawable {
public:
    std::vector<GUI_entry> entries;
    
    bool visible;
    
    GUI(sf::Vector2f dimensions_, int padding_, bool horizontal_,
        GUI_style& style_, std::vector<std::pair<std::string, std::string>> entries_)
    : visible(false), horizontal(horizontal_), style(style_), dimensions(dimensions_), padding(padding_)
    {
        // Construct the background shape
        sf::RectangleShape shape;
        shape.setSize(dimensions);
        shape.setFillColor(style.body_col);
        shape.setOutlineThickness(-style.border_size);
        shape.setOutlineColor(style.border_col);
        
        // Construct each gui entry
        for(auto entry : entries_) {
            // Construct the text
            sf::Text text;
            text.setString(entry.first);
            text.setFont(*style.font);
            text.setColor(style.text_col);
            text.setCharacterSize(dimensions.y - style.border_size - padding);
            
            entries.emplace_back(entry.second, shape, text);
        }
    }
    
    sf::Vector2f get_size();
    
    // Return the entry that the mouse is hovering over.
    // Returns -1 if the mouse if outside of the Gui
    int get_entry(const sf::Vector2f mouse_pos);
    
    // Change the text of an entry.
    void set_entry_text(int entry, std::string text);
    
    // Change the entry dimensions.
    void set_dimensions(sf::Vector2f dimensions_);
    
    // Draw the menu.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    void show();
    
    void hide();
    
    // Highlights an entry of the menu.
    void highlight(const int entry);
    
    // Return the message bound to the entry.
    std::string activate(const int entry);
    std::string activate(const sf::Vector2f mousePos);
    
private:
    // If true the menu entries will be horizontally, not vertically, adjacent
    bool horizontal;
    
    GUI_style style;
    
    sf::Vector2f dimensions;
    
    int padding;
};

#endif /* GUI_h */
