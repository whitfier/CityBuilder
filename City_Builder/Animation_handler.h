//
//  Animation_handler.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Animation_handler_h
#define Animation_handler_h

#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Animation_handler {
public:
    Animation_handler() : time(0.0f), current_animation(-1) {}
    
    Animation_handler(const sf::IntRect& frame_size_)
        : frame_size(frame_size_), time(0.0f), current_animation(-1) {}
    
    // Add a new animation
    void add_animation(Animation& anim);
    
    // Update the current frame of animation. dt is the time since
    // the update was last called (i.e. the time for one frame to be executed)
    void update(const float dt);
    
    // Change the animation, resetting t in the process
    void change_animation(unsigned int num);
    
    // Current section of the texture that should be displayed
    sf::IntRect bounds;
    
    // Pixel dimensions of each individual frame
    sf::IntRect frame_size;
    
private:
    std::vector<Animation> animations;
    float time;
    int current_animation;
};

#endif /* Animation_handler_h */
