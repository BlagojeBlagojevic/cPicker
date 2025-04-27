#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
//#include<SDL2/SDL_image.h>
//#include<SDL2/SDl_timer.h>
#include<SDL2/SDL_keyboard.h>
#include<SDL2/SDL_scancode.h>

#include<SDL2/SDL_ttf.h>
//static const char* fontLoc = "assets/fonts/f.ttf";
//#define fontLoc "assets/fonts/f.ttf"
#define fontLoc "assets/fonts/w.ttf"


#undef main

//COLORS
#define WHITE      (SDL_Color){255, 255, 255, 0}
#define RED        (SDL_Color){255, 0, 0, 0}
#define GREEN      (SDL_Color){0, 255, 0, 0}
#define BLUE       (SDL_Color){30, 0, 255, 0}
#define BLACK      (SDL_Color){0, 0, 0, 0}
#define UNDE_COL   (SDL_Color){252, 3, 177, 0}
#define BLIGHT_COL (SDL_Color){32, 43, 34, 0}


#include<stdbool.h>

#define ERROR_BREAK(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}
#define ASSERT(msg) {fprintf(stderr, "aseert in:\n\tFILE %s\n\tLINE %d\n\tmsg: %s" , __FILE__, __LINE__, msg); exit(-1);}
#define DROP(var) {(void)var;}


#include<stdint.h>
typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;
typedef  float    f32;
typedef  double   f64;


static void disp_colorref(COLORREF rgb) {
	printf("rgb = %lu, r: %u, g: %u b: %u\n", rgb, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
	}

u32 get_pixel_color(POINT* cursor) {
	volatile u32 rgb;
	HDC dt = GetDC(NULL);
	if (dt == NULL) ERROR_BREAK("Is not get context");

	rgb = GetPixel(dt, cursor->x, cursor->y);

	if (rgb == CLR_INVALID) ERROR_BREAK("Invalid");

	ReleaseDC(NULL, dt);
	return rgb;

	}




bool IsAKeyPressedOnce() {
	static bool prevAState = false; // Stores the previous state of the 'A' key

	// Get the current state of the 'A' key (high-order bit indicates down)
	bool currentAState = ((GetAsyncKeyState('A') & 0x8000) != 0) & ((GetAsyncKeyState(' ') & 0x8000) != 0);
	Sleep(1);
	// Trigger action only if the key was NOT pressed before but IS pressed now
	if (currentAState && !prevAState) {
		prevAState = currentAState; // Update previous state
		return true;
		}

	prevAState = currentAState; // Update previous state for next call
	return false;
	}


void SDL_ERR(int code) {

	if((code) < 0) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return;
	}

void *P_SDL_ERR(void *ptr) {
	if(ptr == NULL) {
		ERROR_BREAK("SDL_ERROR: %s\n", SDL_GetError());
		}
	return ptr;
	}



void Text_Renderer_C(SDL_Renderer *renderer, TTF_Font *font, i32 startX, i32 startY, i32 w_c, i32 h_c, const char *c, SDL_Color textColor) {
	if(renderer == NULL) {
		ASSERT("renderer is null!!!");
		}
	if(font == NULL) {
		ASSERT("font is null!!!");
		}
	SDL_Surface *textSurface = P_SDL_ERR(TTF_RenderText_Solid(font, c, textColor));
	SDL_Texture *textTexture = P_SDL_ERR(SDL_CreateTextureFromSurface(renderer, textSurface));
	SDL_Rect textRect = {startX, startY, w_c, h_c}; // rectangle where the text is drawn
	SDL_ERR(SDL_RenderCopy(renderer, textTexture, NULL, &textRect));
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	}




#define WIDTH  250
#define HEIGHT 70



int main(int argc, char *argv[]) {
	DROP(argc);
	DROP(argv);
//INIT SDL2
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());
	static SDL_Renderer *r;
	static SDL_Window   *w;
	TTF_Font* f = TTF_OpenFont(fontLoc, 128);
	(void)P_SDL_ERR(f);
	u32 rgb;

	//int counter = 0;
	POINT cursor = {0, 0}; //IS POINTER TO X, Y POINT STRUCT

	bool state = false;
	bool isWindowCreated = false;



///*
	while(1) {


		if(IsAKeyPressedOnce()) {
			//counter++;
			state = !state;
			}
		if(state == true) {
			if(!isWindowCreated) {
				GetCursorPos(&cursor);
				Sleep(1);
				w   = SDL_CreateWindow("stagod", cursor.x, cursor.y, WIDTH, HEIGHT,  SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP  );
				(void)P_SDL_ERR(w);
				r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
				SDL_RaiseWindow(w);
				isWindowCreated = true;
				Sleep(3);
				}
			}
		else {
			//if(w != NULL && r != NULL) 
			{
				SDL_DestroyRenderer(r);
				SDL_DestroyWindow(w);
				SDL_Quit();
				r = NULL;
				w = NULL;
				}

			isWindowCreated = false;
			}

		if(isWindowCreated == true) {
			GetCursorPos(&cursor);
			rgb = get_pixel_color(&cursor);
			//disp_colorref(rgb);
			Sleep(3);
			SDL_ERR(SDL_RenderClear(r));
			char msg[128];
			snprintf(msg, 128, "r: %u, g: %u b: %u", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
			Text_Renderer_C(r, f, 10, 10, 200, 50, msg, WHITE);
			Sleep(3);
			static SDL_Event event;
			if(SDL_PollEvent(&event)) {
				//Sleep(1);
				}
			SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
			SDL_RenderPresent(r);
			}

		Sleep(1);
		}
	//*/

	}