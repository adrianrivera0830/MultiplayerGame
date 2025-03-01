//
// Created by Cuent on 2/28/2025.
//

#ifndef STRUCTS_H
#define STRUCTS_H

struct position2d {
    int x,y;
};

struct velocity2d {
    float x,y;
};

struct size2d {
    int x,y;
};

struct GameState {
    position2d playerPos;
    velocity2d playerVel;


};
#endif //STRUCTS_H
