#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"

class TextLabelComponent : public Component
{
    public:
        TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color)
        {
            m_position.x = x;
            m_position.y = y;
            m_text = text;
            m_fontFamily = fontFamily;
            m_color = color;
            SetLabelText(text, fontFamily);
        }

        void SetLabelText(std::string text, std::string fontFamily)
        {
            SDL_Surface* surface = TTF_RenderText_Blended(Game::mp_assetManager->GetFont(fontFamily), text.c_str(), m_color);
            mp_texture = SDL_CreateTextureFromSurface(Game::mp_renderer, surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(mp_texture, NULL, NULL, &m_position.w, &m_position.h);
        }

        void Render() override
        {
            FontManager::Draw(mp_texture, m_position);
        }
    private:
        SDL_Rect m_position;
        std::string m_text;
        std::string m_fontFamily;
        SDL_Color m_color;
        SDL_Texture* mp_texture;
};

#endif
