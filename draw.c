#include "prototypes.h"

SDL_Color couleurBleue = {0, 0, 255};
SDL_Color couleurRouge = {255, 0, 0};
SDL_Color couleurBlanche = {255, 255, 255};
	
void drawGame(void)
{
	drawImage(getBackground(), 0, 0);
	
	drawMap();
	
	drawPlayer(getPlayer1());
    drawPlayer(getPlayer2());
    
	drawImage(getHealth1(), 2, 50);
	drawOppositeImage(getHealth2(), 1180, 50);
	
	writeTxt("Player 1", 2, 2, 50, couleurRouge);
	writeTxt("Player 2", 1075, 2, 50, couleurBleue);

	
	
    SDL_RenderPresent(getrenderer());
     
    SDL_RenderClear(getrenderer());
    
    SDL_Delay(1);
}


void drawMenu() {
    
    writeTxt("PRESS   ENTER   TO    START", 390, 500, 50, couleurBlanche);
	writeTxt("PLAYER 1", 2, 200, 50, couleurBlanche);
	writeTxt("PLAYER 2", 1050, 200, 50, couleurBlanche);
    writeTxt("AHH PRODUCTION", 480, 680, 50, couleurBlanche);
    SDL_RenderPresent(getrenderer());
    
    SDL_RenderClear(getrenderer());
    
    SDL_Delay(1);
}
 
void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/stories/seconde)
    unsigned int ticks = SDL_GetTicks();
 
    if (frameLimit < ticks)
    {
        return;
    }
 
    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }
 
    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

SDL_Texture *loadImage(char *name) {

    SDL_Surface *loadedImage = NULL;
    SDL_Texture *texture = NULL;

    loadedImage = IMG_Load(name);

    if(loadedImage != NULL) {


        texture = SDL_CreateTextureFromSurface(getrenderer(), loadedImage);

        SDL_FreeSurface(loadedImage);
        loadedImage = NULL;

    } else
        printf("L'image n'a pas pu etre chargée ! Error : %s\n", SDL_GetError());

    return texture;
}

void drawImage(SDL_Texture *image, int x, int y) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(getrenderer(), image, NULL, &dest);
    
}

void drawOppositeImage(SDL_Texture *image, int x, int y) {
    SDL_Rect dest;
    
    dest.x = x;
    dest.y = y;
    
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopyEx(getrenderer(), image, NULL, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
    
}

void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy) {
    SDL_Rect dest;

    dest.x = destx;
    dest.y = desty;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;

    SDL_Rect src;

    src.x = srcx;
    src.y = srcy;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;

    SDL_RenderCopy(getrenderer(), image, &src, &dest);
}


int writeTxt(char *message, signed int x, signed int y, int size, SDL_Color color)
{
    int rslt = 0;

    if(message != NULL)
    {
        TTF_Font *myFont = NULL;
        SDL_Surface* txt = NULL;
        SDL_Texture *texture = NULL;

        myFont = TTF_OpenFont("fonts/arcade.TTF", size);

        if(myFont == NULL)
        {
            rslt = 1;
        }
        else
        {
            txt = TTF_RenderText_Blended(myFont, message, color);
            texture = SDL_CreateTextureFromSurface(getrenderer(), txt);
            drawImage(texture, x, y);
        }

        TTF_CloseFont(myFont);
        SDL_FreeSurface(txt);
    }
    else
    {
        rslt = 1;
    }
    return rslt;
}
