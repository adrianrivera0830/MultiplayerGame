//
// Created by Cuent on 2/28/2025.
//

#include "Mallet.h"

Mallet::Mallet(SDL_Renderer *m_renderer, int diameter, float velocityMultiplier) : size(diameter,diameter) {
    const char *path = "../images/handle.png";
    mallet_texture = new TextureRenderer(m_renderer, path);
    mallet_texture->SetPosition(500, 500);

    mallet_texture->SetSize(diameter, diameter);

}

void Mallet::SetPos(int x, int y) {
    mallet_texture->SetPosition(x + (size.x/2),y + + (size.y/2));
}

void Mallet::Render() {
    mallet_texture->RenderTexture();
}

position2d Mallet::GetPosition() {
    return currentPos;
}

velocity2d Mallet::GetVelocity() {
    return  vel;
}

size2d Mallet::GetSize() {
    return size;
}
