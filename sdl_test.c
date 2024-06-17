#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


int main(int argc, char** argv)
{
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    TTF_Font* pFont;

    int width = 640;
    int height = 480;

    if (TTF_Init() == -1)
    {
        return 0;
    }

    if (SDL_CreateWindowAndRenderer(width, height, 0, &pWindow, &pRenderer) < 0)
    {
        printf("SDL_CreateWindowAndRenderer Error\n");
        return 0;
    }

    SDL_Texture* screenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);

    pFont = TTF_OpenFont("font/MangoDdobak.ttf", 30);

    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(pFont, "TTF Test", textColor);
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);

    int mWidth = textSurface->w;
    int mHeight = textSurface->h;

    // render text
    SDL_Rect renderQuad = { 10, 10, mWidth, mHeight };


    bool running = true;

    int lastTickCount = SDL_GetTicks();
    int curTickCount = SDL_GetTicks();
    int k = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;            
            }
            else if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(pRenderer);
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

        SDL_RenderCopy(pRenderer, mTexture, NULL, &renderQuad);
        SDL_RenderPresent(pRenderer);
    }

    TTF_CloseFont(pFont);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}
