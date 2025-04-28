#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<math.h>
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



#define WIDTH  250
#define HEIGHT 70

static SDL_Renderer *r;
static SDL_Window   *w;
static TTF_Font* f;


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
	static bool prevAState = false;  //SAVE BUFFER a


	bool currentAState = ((GetAsyncKeyState('A') & 0x8000) != 0) & ((GetAsyncKeyState(' ') & 0x8000) != 0);
	Sleep(1);

	if (currentAState && !prevAState) {
		prevAState = currentAState;
		return true;
		}

	prevAState = currentAState;
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

void filled_circle_quarter(SDL_Renderer *renderer, int x_center, int y_center, int radius, int quadrant) {
    for (int dy = 0; dy <= radius; dy++) {
        f64 y;
        switch (quadrant) {
            case 0: y = y_center - dy; break; // Top-left
            case 1: y = y_center - dy; break; // Top-right
            case 2: y = y_center + dy - 1.1; break; // Bottom-left
            case 3: y = y_center + dy - 1.1; break; // Bottom-right
            default: return;
        }

        f64 dx = sqrt(radius * radius - dy * dy);
        f64 x_start, x_end;

        switch (quadrant) {
            case 0: // Top-left
                x_start = (x_center - dx + 1);
                x_end = x_center;
                break;
            case 1: // Top-right
                x_start = x_center;
                x_end = (x_center + dx - 1);
                break;
            case 2: // Bottom-left
                x_start = (x_center - dx + 1.2);
                x_end = x_center;
                break;
            case 3: // Bottom-right
                x_start = x_center;
                x_end = (x_center + dx - 1.2);
                break;
        }

        SDL_RenderDrawLineF(renderer, x_start, y, x_end, y);
    }
    
    
}

void draw_rounded_rect(SDL_Renderer *renderer, int x, int y, int width, int height, int radius) {
    // Main center rectangle
    SDL_FRect main_rect = {x + radius, y + radius, width - 2 * radius, height - 2 * radius};
    SDL_RenderFillRectF(renderer, &main_rect);

    // Side rectangles
    SDL_FRect top_rect = {x + radius, y, width - 2 * radius, radius};
    SDL_FRect bottom_rect = {x + radius, y + height - radius, width - 2 * radius, radius};
    SDL_FRect left_rect = {x, y + radius, radius, height - 2 * radius};
    SDL_FRect right_rect = {x + width - radius, y + radius, radius, height - 2 * radius};

    SDL_RenderFillRectF(renderer, &top_rect);
    SDL_RenderFillRectF(renderer, &bottom_rect);
    SDL_RenderFillRectF(renderer, &left_rect);
    SDL_RenderFillRectF(renderer, &right_rect);

    // Quarter circles for each corner
    filled_circle_quarter(renderer, x + radius, y + radius, radius, 0); // Top-left
    filled_circle_quarter(renderer, x + width - radius, y + radius, radius, 1); // Top-right
    filled_circle_quarter(renderer, x + radius, y + height - radius, radius, 2); // Bottom-left
    filled_circle_quarter(renderer, x + width - radius, y + height - radius, radius, 3); // Bottom-right
}




void init_window_round(POINT cursor) {

	w   = SDL_CreateShapedWindow("stagod", cursor.x, cursor.y, WIDTH, HEIGHT,  SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP  );
	(void)P_SDL_ERR(w);

	SDL_Surface* shapeSurface = SDL_CreateRGBSurfaceWithFormat(
	                              0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888
	                            );

	// Draw circle on the shape surface
	int centerX = 25;
	int centerY = HEIGHT/2;
	int radius = HEIGHT/2;

	SDL_LockSurface(shapeSurface);
	//FIRST HALFS
	Uint32* pixels = (Uint32*)shapeSurface->pixels;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < 20; ++x) {
				{
				int dx = x - centerX, dy = y - centerY;
				if (dx*dx + dy*dy <= radius*radius) {
					pixels[y * WIDTH + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
					}
				else {
					pixels[y * WIDTH + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 0);
					}
				}
			}
		}
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 20; x < WIDTH - 20; ++x) {
			pixels[y * WIDTH + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
			}
		}
	centerX = WIDTH - 25;
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = WIDTH - 20; x < WIDTH; ++x) {
				{
				int dx = x - centerX, dy = y - centerY;
				if (dx*dx + dy*dy <= radius*radius) {
					pixels[y * WIDTH + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
					}
				else {
					pixels[y * WIDTH + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 0);
					}
				}
			}
		}


	SDL_UnlockSurface(shapeSurface);

	// Apply the shape
	SDL_WindowShapeMode shapeMode;
	shapeMode.mode = ShapeModeDefault;
	if (SDL_SetWindowShape(w, shapeSurface, &shapeMode) != 0) {
		SDL_Log("Shape error: %s", SDL_GetError());
		}
	SDL_FreeSurface(shapeSurface);

	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	SDL_RaiseWindow(w);
	}


void destroy_window() {
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	r = NULL;
	w = NULL;
	}


void render_rgb(u32 rgb, POINT cursor, i32 startX, i32 startY, i32 w_c, i32 h_c) {
	char msg[128];
	snprintf(msg, 128, "R: %u, G: %u B: %u", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
	SDL_Color color = {225, 225, 225, 0};
	Text_Renderer_C(r, f, startX, startY, w_c, h_c, msg, color);
	Sleep(3);
	SDL_SetWindowPosition(w, cursor.x + 10, cursor.y + 10);
	Sleep(1);
	}

void render_color_round_square(u32 rgb){
	SDL_SetRenderDrawColor(r, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), 255);
	draw_rounded_rect(r, 10, 10, 40, 40, 5);	
}


int main(int argc, char *argv[]) {
	DROP(argc);
	DROP(argv);
//INIT SDL2
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());

	f = TTF_OpenFont(fontLoc, 128);
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

				init_window_round(cursor);

				isWindowCreated = true;
				Sleep(3);

				}
			}
		else {
			destroy_window();
			isWindowCreated = false;
			}

		if(isWindowCreated == true) {
			GetCursorPos(&cursor);
			rgb = get_pixel_color(&cursor);
			//disp_colorref(rgb);
			Sleep(3);
			SDL_ERR(SDL_RenderClear(r));

			static SDL_Event event;
			if(SDL_PollEvent(&event)) {
				//Sleep(1);
				}
			render_rgb(rgb, cursor, 120, 40, 120, 20);
			render_color_round_square(rgb);
			//SDL_SetRenderDrawColor(r, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), 255);
			SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
			SDL_RenderPresent(r);
			}

		Sleep(1);
		}
	//*/

	}
