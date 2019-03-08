#include <iostream>
using namespace std ;

#include <SDL2/SDL_ttf.h>

TTF_Font* Sans = nullptr ;
void initText() ;

void drawText( SDL_Renderer *renderer, const char *pszText )
{
	if( Sans == nullptr ) {
		initText() ;
	}
	
	// printf( "error: %s\n", TTF_GetError() ) ;
	SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface *pMessageSurface = TTF_RenderText_Solid(Sans, pszText, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	// printf( "surfaceMessage: %p\n", surfaceMessage ) ;

	SDL_Texture *pMessageTexture = SDL_CreateTextureFromSurface(renderer, pMessageSurface); //now you can convert it into a texture
	//printf( "Message: %p\n", Message ) ;		
	
	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 0;  //controls the rect's x coordinate 
	Message_rect.y = 0; // controls the rect's y coordinte
	Message_rect.w = pMessageSurface->w; // controls the width of the rect
	Message_rect.h = pMessageSurface->h; // controls the height of the rect	

	//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

	//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

	SDL_RenderCopy(renderer, pMessageTexture, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

	//Don't forget too free your surface and texture	
	SDL_FreeSurface(pMessageSurface);	
    SDL_DestroyTexture(pMessageTexture);	
}

void initText()
{
	if( TTF_Init() != 0 ) {
		throw runtime_error( "TTF_init failed") ;
	} else {
		printf( "TTF_Init ok\n") ;
		
		const char *pszFont = "LiberationSansBold.ttf" ;
		Sans = TTF_OpenFont(pszFont, 14); //this opens a font style and sets a size
		if( Sans == nullptr ) {
			printf( "%s not found\n", pszFont ) ;
			throw runtime_error( "Font not found") ;
		}
		printf( "Sans: %p\n", Sans ) ;		
	}
}