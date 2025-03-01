//
// Created by Cuent on 2/28/2025.
//

#ifndef MALLET_H
#define MALLET_H
#include <SDL_render.h>
#include "TextureRenderer.h"

class Mallet {
private:
    position2d prevPos;
    position2d currentPos;

    velocity2d vel;
    float velMult;

    size2d size;

    TextureRenderer *mallet_texture;


private:

public:
    Mallet(SDL_Renderer *m_renderer, int diameter, float velocityMultiplier);
    void SetPos(int x, int y);
    void Render();

    position2d GetPosition();
    velocity2d GetVelocity();
    size2d GetSize();

};



#endif //MALLET_H
