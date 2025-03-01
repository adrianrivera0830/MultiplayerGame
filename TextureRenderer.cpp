//
// Created by Cuent on 2/9/2025.
//

#include "TextureRenderer.h"

TextureRenderer::TextureRenderer(SDL_Renderer *renderer, std::string path): m_renderer(renderer), m_texture(nullptr) {
    m_rect = {0, 0, 0, 0};
    LoadTexture(path.c_str());
}

void TextureRenderer::LoadTexture(const std::string& path) {
    UnloadTexture(); // Asegurar que no haya una textura previa cargada

    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        throw std::runtime_error("Error cargando la imagen: " + path + " - " + std::string(IMG_GetError()));
    }

    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        throw std::runtime_error("No se pudo crear la textura: " + path);
    }

    // Obtener dimensiones automáticamente
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
}


void TextureRenderer::SetPosition(int x, int y) {
    m_rect.x = x;
    m_rect.y = y;
}

void TextureRenderer::SetSize(int width, int height) {
    m_rect.w = width;
    m_rect.h = height;
}

void TextureRenderer::RenderTexture() {
        if (!m_texture) {
            std::cerr << "Intento de renderizar una textura nula.\n";
            return;
        }
    SDL_RenderCopy(m_renderer, m_texture,NULL, &m_rect);
}

TextureRenderer::~TextureRenderer() {
    UnloadTexture();
}

void TextureRenderer::UnloadTexture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}
