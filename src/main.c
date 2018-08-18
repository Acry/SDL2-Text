//BEGIN HEAD
//BEGIN DESCRIPTION

/* This is a small SDL2 text API demo
 *
 */

/* DEFINED PROGRESS GOALS
 * 
 * None atm
 * 
 */
//END   DESCRIPTION

//BEGIN INCLUDES
//system headers
#include <math.h>
#include <string.h>
#include <ctype.h>
//local headers
#include "helper.h"
// for kdev
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define WHITE 	255,255,255,255
#define BLACK 	0,0,0,255
#define RED   	255,0,0,255
#define BLUE   	67,91,112,255
#define WW 	550
#define WH 	(WW/16)*12
//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
//END	DATASTRUCTURES

//BEGIN GLOBALS
int ww=WW;
int wh=WH;

//BEGIN VISIBLES

SDL_Surface    *temp_surface	= NULL;

SDL_Texture    *logo		= NULL;
SDL_Rect 	logo_dst;

SDL_Texture    *text1		= NULL;
SDL_Rect 	text1_dst;

SDL_Rect 	TextInputRect;
TTF_Font       *font		= NULL;

char *text;
SDL_Color color={BLUE};
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
void RenderText (void);
void assets_in	(void);
void assets_out	(void);
//END	FUNCTION PROTOTYPES

//END 	HEAD

//BEGIN MAIN FUNCTION
int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT
init();
assets_in();
// SDL_StartTextInput(); - enabled by default
//BEGIN WINDOW
SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,ww,wh);
SDL_SetWindowTitle(Window, "SDL2 Text");
SDL_ShowWindow(Window);
//END WINDOW

if (SDL_IsTextInputActive()){
	SDL_Log("Unicode text input events are enabled.");
	
	// used as a hint for IME and on-screen keyboard placement
	if (SDL_HasScreenKeyboardSupport()){
		SDL_Log("HasScreenKeyboardSupport.");
		SDL_SetTextInputRect(&TextInputRect);
	}
	else
		SDL_Log("HasNoScreenKeyboardSupport.");
}

SDL_Event event;
int running = 1;
//END   INIT

//BEGIN MAIN LOOP
while(running){

	//BEGIN EVENT LOOP
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running =0;
		}
		if(event.type == SDL_MOUSEMOTION){
			;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_RIGHT){
				;
			}
			if(event.button.button == SDL_BUTTON_MIDDLE){
				;
			}
			if(event.button.button==SDL_BUTTON_LEFT){
				if (event.button.clicks == 2)
					SDL_SetClipboardText( text );
			}
		}
		if(event.type ==SDL_TEXTINPUT){
			size_t lenght = strlen(text);
			if (lenght <= 25){
				size_t lenght2=strlen(event.text.text);
				char *temp=calloc(lenght+lenght2+1,1);
				strcat(temp,text);
				strcat(temp,event.text.text);
				lenght2 = strlen(temp);
				char *temp2;
				temp2 = realloc( text, lenght2+1 );
				if  (temp2 == NULL){
					free(text);
					running = 0;
				}
				text = temp2;
				text = strdup(temp);
				free(temp);
				RenderText();
			}
		}
		if(event.type == SDL_KEYDOWN ){
			switch(event.key.keysym.sym ){
				case SDLK_ESCAPE:
					running =0;
					break;
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL)
						SDL_SetClipboardText( text );
					break;
				case SDLK_BACKSPACE:
					if (strlen(text)){
						size_t lenght = strlen(text);
						char *temp = calloc(lenght-1,1 );
						temp = strndup(text, (lenght-1));
						char *temp2;
						if (lenght-1>0){
							// handle UTF8
							if ( strlen(text) >1 && !isprint(temp[lenght-2]) ){
								
								if (lenght-2>0){
									temp2 = realloc( text, lenght-2 );
									if  (temp2 == NULL){
										free(text);
										running = 0;
									}
									text = temp2;
									temp = strndup(text, (lenght-2));
								}else {
									char *temp2;
									temp2 = realloc( text, 1 );
									if  (temp2 == NULL){
										free(text);
										running = 0;
									} 
									text = temp2;
									text[0] = '\0';
									RenderText();
									break;
								}
							}
							lenght = strlen(temp);
							temp2 = realloc( text, lenght );
							if  (temp2 == NULL){
										free(text);
										running=0;
									}
							text = temp2;
							text = strdup(temp);
							free(temp);
						}else {
								char *temp2;
								temp2 = realloc( text, 1 );
								if  (temp2 == NULL){
									free(text);
									running = 0;
								}
								text = temp2;
								text[0] = '\0';
							}
						RenderText();
					}
					break;
				case SDLK_v:
					if (SDL_GetModState() & KMOD_CTRL){
						if (SDL_HasClipboardText()){
							char *temp = SDL_GetClipboardText();
							size_t lenght = strlen(temp);
							SDL_Log("l: %zu",lenght);
							char *temp2;
							temp2 = realloc( text, lenght+1 );
							if  (temp2 == NULL){
								free(text);
								running=0;
							}
							text = temp2;
							text = strdup(temp);
							free(temp);
							RenderText();
						}
					}
					break;
				default:
					break;
			}
		}
	}
	//END   EVENT LOOP
	//BEGIN RENDERING
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	SDL_RenderCopy(Renderer, logo, NULL, &logo_dst);
	SDL_SetRenderDrawColor(Renderer,111,111,111,128);
	SDL_RenderFillRect(Renderer, &TextInputRect);
	SDL_RenderCopy(Renderer, text1, NULL, &text1_dst);
	SDL_RenderPresent(Renderer);
	//END   RENDERING
}
//END   MAIN LOOP

assets_out();
exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
void assets_in(void)
{
	font = TTF_OpenFont("assets/fonts/NimbusSanL-Regu.ttf", 36);
	text = calloc(10,1);
	text = strcpy(text,"Edit Text");
	//BEGIN LOGO
	temp_surface = IMG_Load("assets/gfx/logo.png");
	logo = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(logo, NULL, NULL, &logo_dst.w, &logo_dst.h);
	logo_dst.x = (ww/2)-(logo_dst.w/2);
	logo_dst.y = (wh/2)-(logo_dst.h/2);
	//END 	LOGO
	RenderText();
	SDL_FreeSurface(temp_surface);
}

void RenderText(void)
{
	//BEGIN Blended
	temp_surface = TTF_RenderUTF8_Blended(font,text,color);
	text1 = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	SDL_QueryTexture(text1, NULL, NULL, &text1_dst.w, &text1_dst.h);
	text1_dst.x = 10;
	text1_dst.y = 0;
	//END 	Blended

	TextInputRect.x = 0;
	TextInputRect.y = 0;
	TextInputRect.w = text1_dst.w+20;
	TextInputRect.h = text1_dst.h;
}

void assets_out(void)
{
	free(text);
	TTF_CloseFont(font);
	SDL_DestroyTexture(logo);
	SDL_DestroyTexture(text1);
}

//END   FUNCTIONS
