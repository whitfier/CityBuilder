//
//  Animation.h
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#ifndef Animation_h
#define Animation_h

struct Animation {
public:
    Animation(unsigned int start_frame_, unsigned int end_frame_, float duration_)
        : start_frame(start_frame_), end_frame(end_frame_), duration(duration_) {}
    
    unsigned int getLength() {return end_frame - start_frame + 1;}
    
    unsigned int start_frame;
    unsigned int end_frame;
    float duration;
};

#endif /* Animation_h */
