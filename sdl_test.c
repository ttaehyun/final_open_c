#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

void logSDLError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, SDL_GetError());
}

void logTTFError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, TTF_GetError());
}

typedef struct {
    TTF_Font* font24;
    TTF_Font* font48;
} Fonts;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    SDL_Color textColor;
    const char* text;
    SDL_Texture* texture;
    TTF_Font* font;
} Button;

SDL_Texture* renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        logTTFError("TTF_RenderText_Solid");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        logSDLError("SDL_CreateTextureFromSurface");
    }
    return texture;
}

bool isMouseOverButton(Button button, int x, int y) {
    return x >= button.rect.x && x <= button.rect.x + button.rect.w &&
           y >= button.rect.y && y <= button.rect.y + button.rect.h;
}

void renderButton(SDL_Renderer *renderer, Button button) {
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
    SDL_RenderFillRect(renderer, &button.rect);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(button.texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstRect = {button.rect.x + (button.rect.w - texW) / 2, button.rect.y + (button.rect.h - texH) / 2, texW, texH};
    SDL_RenderCopy(renderer, button.texture, NULL, &dstRect);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError("SDL_Init");
        return 1;
    }

    if (TTF_Init() == -1) {
        logTTFError("TTF_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Buttons", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        logSDLError("SDL_CreateWindow");
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        logSDLError("SDL_CreateRenderer");
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Fonts fonts;
    fonts.font24 = TTF_OpenFont("font/MangoDdobak.ttf", 24);
    if (fonts.font24 == NULL) {
        logTTFError("TTF_OpenFont (24)");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    fonts.font48 = TTF_OpenFont("/path/to/font.ttf", 48);
    if (fonts.font48 == NULL) {
        logTTFError("TTF_OpenFont (48)");
        TTF_CloseFont(fonts.font24);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color buttonColor = {0, 0, 255, 255};
    SDL_Color buttonColorHover = {0, 0, 200, 255};

    Button prevButton = {{50, 400, 150, 50}, buttonColor, textColor, "Previous", NULL, fonts.font24};
    Button nextButton = {{440, 400, 150, 50}, buttonColor, textColor, "Next", NULL, fonts.font48};

    prevButton.texture = renderText(renderer, prevButton.font, prevButton.text, textColor);
    nextButton.texture = renderText(renderer, nextButton.font, nextButton.text, textColor);

    if (prevButton.texture == NULL || nextButton.texture == NULL) {
        SDL_DestroyTexture(prevButton.texture);
        SDL_DestroyTexture(nextButton.texture);
        TTF_CloseFont(fonts.font24);
        TTF_CloseFont(fonts.font48);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(prevButton, x, y)) {
                    printf("Previous button clicked\n");
                } else if (isMouseOverButton(nextButton, x, y)) {
                    printf("Next button clicked\n");
                }
            }
        }

        int x, y;
        SDL_GetMouseState(&x, &y);
        if (isMouseOverButton(prevButton, x, y)) {
            prevButton.color = buttonColorHover;
        } else {
            prevButton.color = buttonColor;
        }

        if (isMouseOverButton(nextButton, x, y)) {
            nextButton.color = buttonColorHover;
        } else {
            nextButton.color = buttonColor;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderButton(renderer, prevButton);
        renderButton(renderer, nextButton);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(prevButton.texture);
    SDL_DestroyTexture(nextButton.texture);
    TTF_CloseFont(fonts.font24);
    TTF_CloseFont(fonts.font48);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
