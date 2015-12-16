//
//  main.cpp
//  City_Builder
//
//  Created by Ryan Whitfield on 12/14/15.
//  Copyright © 2015 Ryan Whitfield. All rights reserved.
//

#include "Game.h"
#include "Game_state_start.h"

using std::make_shared; using std::shared_ptr;

//#ifdef _MSC_VER
//int WinMain() {
//#else
int main() {
//#endif
    auto game_ptr = make_shared<Game>();
    game_ptr->push_state(make_shared<Game_state_start>(game_ptr));
    game_ptr->game_loop();
    return 0;
}
