//
//  Animation_handler.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Animation_handler.h"

void Animation_handler::add_animation(Animation& anim) {
    animations.push_back(anim);
}

void Animation_handler::update(const float dt) {
    if(current_animation >= animations.size() || current_animation < 0)
        return;
    
    float duration = animations[current_animation].duration;
    
    // If the animation has progessed to a new frame, change to the next frame
    int new_frame = static_cast<int>((time + dt) / duration);
    int old_frame = static_cast<int>(time / duration);
    if (new_frame > old_frame) {
        // Adjust for looping
        new_frame %= animations[current_animation].getLength();
        
        // Set the sprite to the new frame
        sf::IntRect rect = frame_size;
        rect.left = rect.width * new_frame;
        rect.top = rect.height * current_animation;
        bounds = rect;
    }
    
    // Increment the time elapsed
    time += dt;
    
    // Adjust for looping
    if(time > duration * animations[current_animation].getLength())
        time = 0.0f;
}

void Animation_handler::change_animation(unsigned int animID) {
    // Don't change the animation if it is currently active
    // or the new animation does not exist
    if(current_animation == animID || animID >= animations.size())
        return;
    
    // Set the current animation
    current_animation = animID;
    
    // Update the animation bounds
    sf::IntRect rect = frame_size;
    rect.top = rect.height * animID;
    bounds = rect;
    time = 0.0f;
}
