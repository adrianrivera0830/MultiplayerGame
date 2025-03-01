//
// Created by Cuent on 2/9/2025.
//

#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H


#include <cassert>
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "structs.h"

class TextureRenderer {
public:

    TextureRenderer(SDL_Renderer *renderer, std::string path);

    void LoadTexture(const std::string &path);

    void SetPosition(int x, int y);

    void SetSize(int width, int height);

    void RenderTexture();

    ~TextureRenderer();

    void UnloadTexture();


    SDL_Texture *GetTexture() const { return m_texture; }
    SDL_Rect GetRect() const { return m_rect; }

private:
    SDL_Rect m_rect;
    SDL_Texture *m_texture;

    SDL_Renderer *m_renderer;
};


#endif //TEXTURERENDERER_H
