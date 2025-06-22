#include<Windows.h>
#include<windows.h>
#include<winuser.h>

#include <omp.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include<time.h>

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
#define fontLoc "assets/fonts/OPEN_LB.ttf"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BIGENDIAN32(_a_)    __builtin_bswap32 (_a_)
#else
#define BIGENDIAN32(_a_)    (_a_)
#endif


#undef main

//COLORS
#define WHITE      (SDL_Color){255, 255, 255, 255}
#define RED        (SDL_Color){255, 0, 0, 0}
#define GREEN      (SDL_Color){0, 255, 0, 0}
#define BLUE       (SDL_Color){30, 0, 255, 0}
#define BLACK      (SDL_Color){0, 0, 0, 0}
#define UNDE_COL   (SDL_Color){252, 3, 177, 0}
#define BLIGHT_COL (SDL_Color){32, 43, 34, 0}


#include<stdbool.h>

#define LOG(...)     			  fprintf(stdout, __VA_ARGS__)
#define ERROR_BREAK(...)  {fprintf(stderr, __VA_ARGS__); exit(-1);}
#define ASSERT(msg) {fprintf(stderr, "aseert in:\n\tFILE %s\n\tLINE %d\n\tmsg: %s" , __FILE__, __LINE__, msg); exit(-1);}
#define DROP(var) {(void)var;}



#define TRAY_WINAPI 1


#include "tray.h"

#define TRAY_ICON1 "icon.ico"


//END TRY






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

#define DA_SIZE 16
#define da_append(da, item)                                                            \
	do {                                                                                 \
		if ((da)->count >= (da)->capacity) {                                               \
			(da)->capacity = (da)->capacity == 0 ? DA_SIZE : (da)->capacity*2;               \
			(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));         \
			assert((da)->items != NULL && "Realloc fail !!!");                               \
			}                                                                                \
		\
		(da)->items[(da)->count++] = (item);                                               \
		} while (0)


#define da_remove_unordered(da, i)               					   \
	do {                                             					   \
		size_t j = (i);                              					   \
		if(j > (da)->count){ASSERT("Not that amount of elements in da");}  \
		(da)->items[j] = (da)->items[--(da)->count]; 					   \
		} while(0)

#define da_remove_last(da) do{ if((da)->count > 0){ (da)->count--; } }while(0)



typedef struct {
	u64 			count;
	u64 			capacity;
	i32   		startX;
	i32   		startY;
	i32   		width;
	i32   		height;
	SDL_Color color;
	char* items;
	} Str;


typedef struct {
	u64 	count;
	u64 	capacity;
	char** items;
	} Str_Builder;

typedef struct {
	u64 	count;
	u64 	capacity;
	i32* items;
	} Num;

typedef struct {
	u64 	count;
	u64 	capacity;
	u8*   items;
	} State;

typedef struct {
	SDL_Color color;
	char dat[64];
	} Date_Color;

typedef struct {
	u64 				count;
	u64 				capacity;
	u64         index;
	Date_Color* items;
	} Date_Color_DA;


typedef struct {
	i32 x;
	i32 y;
	i32 width;
	i32 height;
	i32 radius;
	SDL_Color colorBackground;
	} Color_Button;


typedef enum {
	Element_Button,
	Element_Color,
	Element_Details,
	Element_Num,
	} Elements_Types;

typedef enum {
	Details_HEX,
	Details_RGB,
	Details_HSV,
	Details_HSL
	} Details_Types;

typedef struct {
	Elements_Types types;
	i32 x;
	i32 y;
	i32 width;
	i32 height;
	i32 radius;
	SDL_Color colorBackground;

	Color_Button rColor;
	void (*callback)(i32 x, i32 y, i32 w, i32 h, SDL_Color c);
	Str  text;
	Details_Types typeD;
	} Element;


typedef struct {
	u64 	count;
	u64 	capacity;
	Element* items;
	} Element_DA;


Element create_element(Elements_Types type, i32 x, i32 y,i32 width, i32 height, i32 radius, SDL_Color colorBackground, void (*callback)(i32 x, i32 y, i32 w, i32 h, SDL_Color c)) {
	Element* element = calloc(1, sizeof(Element));
	element->types = type;
	element->x = x;
	element->y = y;
	element->width = width;
	element->height = height;
	element->radius = radius;
	element->colorBackground = colorBackground;
	element->callback = callback;
	element->text.items = NULL;

	return (*element);

	}

void bind_text(Element *e, const char* text, i32 x, i32 y, i32 width, i32 height, SDL_Color color) {
	u64 len = strlen(text);
	e->text.count = 0;
	for(u64 i = 0; i < len; i++) {
		da_append(&e->text, text[i]);
		}
	da_append(&e->text, '\0');
	e->text.startX = x;
	e->text.startY = y;
	e->text.width = width;
	e->text.height = height;
	e->text.color = color;
	}

void bind_color(Element *e, i32 x, i32 y, i32 width, i32 height, i32 radius, SDL_Color color) {

	e->rColor.x = x;
	e->rColor.y = y;
	e->rColor.width = width;
	e->rColor.height = height;
	e->rColor.radius = radius;
	e->rColor.colorBackground = color;
	}





#define WIDTH  250
#define HEIGHT 70

typedef struct {
	u32 rgb;
	POINT cursor;
	bool   isButtonPressed;
	bool   isMouseClicked;
	u8     whatColorIsSelected;
	bool   isMainWindowInited;  //MAIN
	bool   isWindowInited;      //ROUND
	State  howeringState;
	char   dateStr[64];
	} Program_State;

static Program_State program_state;

//static struct tray tray;




static SDL_Renderer *r;
static SDL_Window   *w;
static TTF_Font* f;

const i32 width   = 350;
const i32 height  = 490;


static SDL_Renderer *rM;
static SDL_Window   *wM;
//time
static struct tm *tm;

static Date_Color_DA dateAndColor;



const SDL_Color elemetColor  =  (SDL_Color) {
	0x30, 0x30, 0x30, 0
	};

const SDL_Color elemetColorOnMouse  =  (SDL_Color) {
	0x40, 0x40, 0x40, 0
	};



const SDL_Color elemetTextColor  =  (SDL_Color) {
	0x72, 0x72, 0x72, 0
	};
const SDL_Color backgroundColor = (SDL_Color) {
	0x20, 0x20, 0x20, 0
	};


typedef struct {
	double r, g, b; // h, s, v as output: r = h (deg), g = s (%), b = v (%)
	} ColorHSV;


//MAPING (r->h, g->s, b->v)
ColorHSV rgb2hsv(SDL_Color c) {
	ColorHSV out, in;
	in.r = c.r / 255.0f;
	in.g = c.g / 255.0f;
	in.b = c.b / 255.0f;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.b = max; // value (v)
	delta = max - min;

	if (delta < 0.00001) {
		out.g = 0.0;     // saturation
		out.r = 0.0;     // hue is undefined, set to 0
		}
	else {
		out.g = (max > 0.0) ? (delta / max) : 0.0; // saturation

		// hue
		if (in.r >= max)
			out.r = (in.g - in.b) / delta;         // between yellow & magenta
		else if (in.g >= max)
			out.r = 2.0 + (in.b - in.r) / delta;   // between cyan & yellow
		else
			out.r = 4.0 + (in.r - in.g) / delta;   // between magenta & cyan

		out.r *= 60.0;                             // convert to degrees
		if (out.r < 0.0)
			out.r += 360.0;
		}

	// Convert to percentages and round
	out.r = (int)(out.r + 0.5) % 360;            // hue in degrees
	out.g = (int)(out.g * 100.0 + 0.5);          // sat %
	out.b = (int)(out.b * 100.0 + 0.5);          // val %

	return out;
	}

ColorHSV rgb2hsl(SDL_Color c) {
	ColorHSV out, in;
	in.r = c.r / 255.0f;
	in.g = c.g / 255.0f;
	in.b = c.b / 255.0f;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.b = (max + min) / 2.0; // Lightness (L)

	delta = max - min;

	if (delta < 0.00001) {
		// Achromatic (gray)
		out.r = 0.0; // Hue
		out.g = 0.0; // Saturation
		}
	else {
		// Saturation
		if (out.b <= 0.5)
			out.g = delta / (max + min);
		else
			out.g = delta / (2.0 - max - min);

		// Hue
		if (in.r >= max)
			out.r = (in.g - in.b) / delta;
		else if (in.g >= max)
			out.r = 2.0 + (in.b - in.r) / delta;
		else
			out.r = 4.0 + (in.r - in.g) / delta;

		out.r *= 60.0;
		if (out.r < 0.0)
			out.r += 360.0;
		}

	// Convert to degrees and percentages
	out.r = (int)(out.r + 0.5) % 360;       // Hue in degrees
	out.g = (int)(out.g * 100.0 + 0.5);     // Saturation in percent
	out.b = (int)(out.b * 100.0 + 0.5);     // Lightness in percent

	return out;
	}



void disp_colorref(COLORREF rgb) {
	printf("rgb = %lu, r: %u, g: %u b: %u\n", rgb, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
	}

u32 get_pixel_color(POINT* cursor) {
	volatile u32 rgb;
	HDC dt = GetDC(NULL);
	if (dt == NULL) ERROR_BREAK("Is not get context");

	rgb = GetPixel(dt, cursor->x, cursor->y);
	//GetDeviceCaps()
	if (rgb == CLR_INVALID) ERROR_BREAK("Invalid");

	ReleaseDC(NULL, dt);
	return rgb;

	}


bool IsKeyPressedExtendedWindow() {
	static bool prevEState = false;  //SAVE BUFFER


	bool currentEState = ((GetAsyncKeyState('S') & 0x8000) != 0) & ((GetAsyncKeyState(' ') & 0x8000) != 0);
	//Sleep(1);

	if (currentEState && !prevEState) {
		prevEState = currentEState;
		return true;
		}

	prevEState = currentEState;
	return false;
	}





bool IsKeyPressedFollow() {
	static bool prevAState = false;  //SAVE BUFFER a


	bool currentAState = ((GetAsyncKeyState('A') & 0x8000) != 0) & ((GetAsyncKeyState(' ') & 0x8000) != 0);
	//Sleep(1);

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
			case 0:
				y = y_center - dy;
				break; // Top-left
			case 1:
				y = y_center - dy;
				break; // Top-right
			case 2:
				y = y_center + dy - 1.1;
				break; // Bottom-left
			case 3:
				y = y_center + dy - 1.1;
				break; // Bottom-right
			default:
				return;
			}

		f64 dx = sqrt(radius * radius - dy * dy);
		f64 x_start = 0.0f, x_end = 0.0f;

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


void draw_horizontal_gradient_box(SDL_Renderer * renderer,
                                  const int x, const int y, const int w, const int h, const float steps,
                                  const SDL_Color c1, const SDL_Color c2, const int fill) {

	/* Acumulator initial position */
	float yt = y;
	float rt = c1.r;
	float gt = c1.g;
	float bt = c1.b;
	float at = c1.a;

	/* Changes in each attribute */
	float ys = h/steps;
	float rs = (c2.r - c1.r)/steps;
	float gs = (c2.g - c1.g)/steps;
	float bs = (c2.b - c1.b)/steps;
	float as = (c2.a - c1.a)/steps;

	for(int i = 0; i < steps ; i++) {
		/* Create an horizontal rectangle sliced by the number of steps */
		SDL_Rect rect = { x, yt, w, ys+1 };

		/* Sets the rectangle color based on iteration */
		SDL_SetRenderDrawColor(renderer, rt, gt, bt, at);

		/* Paint it or coverit*/
		if(fill)
			SDL_RenderFillRect(renderer, &rect);
		else
			SDL_RenderDrawRect(renderer, &rect);

		/* Update colors and positions */
		yt += ys;
		rt += rs;
		gt += gs;
		bt += bs;
		at += as;
		}
	}


void render_color_square(float red, float green, float blue, int colorBarHeight, int colorBarWidth, int colorBarWidthSpace,
                         int startX, int startY) {
	//int topLeft = colorBarWidth + colorBarWidthSpace;
	DROP(colorBarWidthSpace);
	float xDifferenceRed = 255.0 - red;
	float xDifferenceGreen = 255.0 - green;
	float xDifferenceBlue = 255.0 - blue;

	float xRedDelta = xDifferenceRed/colorBarHeight;
	float xGreenDelta = xDifferenceGreen/colorBarHeight;
	float xBlueDelta = xDifferenceBlue/colorBarHeight;

	xRedDelta = xDifferenceRed == 0 ?  0 : xRedDelta;
	xGreenDelta = xDifferenceGreen == 0 ?  0 : xGreenDelta;
	xBlueDelta = xDifferenceBlue == 0 ?  0 : xBlueDelta;

	float yDifferenceRed = 255.0;
	float yDifferenceGreen = 255.0;
	float yDifferenceBlue = 255.0;

	float yRedDelta = yDifferenceRed/colorBarHeight;
	float yGreenDelta = yDifferenceGreen/colorBarHeight;
	float yBlueDelta = yDifferenceBlue/colorBarHeight;

	float currentRed = 255.0;
	float currentGreen = 255.0;
	float currentBlue = 255.0;
	SDL_Surface* sRgb = SDL_CreateRGBSurfaceWithFormat(0,colorBarHeight,colorBarWidth,32, SDL_PIXELFORMAT_RGBA8888);;
	SDL_Rect rect = (SDL_Rect) {
		startX, startY, colorBarHeight, colorBarWidth
		};
	//SDL_ERR(SDL_FillRect(sRgb, &rect, (u32)0xffffff));
	u32 *pixels = (u32*)sRgb->pixels;
	SDL_ERR(SDL_LockSurface(sRgb));
	for(int y = 0; y < colorBarHeight; y++) {
		for(int x = 0; x < colorBarWidth; x++) { //TBD
			//SDL_SetRenderDrawColor(renderer, currentRed, currentGreen, currentBlue, 0);
			//SDL_RenderDrawPoint(renderer, topLeft + x - startX - 50, y + startY);
			pixels[y * colorBarWidth + x] = SDL_MapRGBA(sRgb->format, currentRed, currentBlue, currentGreen, 255);

			currentRed -= xRedDelta;
			currentGreen -= xGreenDelta;
			currentBlue -= xBlueDelta;

			currentRed = currentRed > 255.0 ? 255 : currentRed;
			currentGreen = currentGreen > 255.0 ? 255 : currentGreen;
			currentBlue = currentBlue > 255.0 ? 255 : currentBlue;

			currentRed = currentRed <  0 ? 0 : currentRed;
			currentGreen = currentGreen < 0 ? 0 : currentGreen;
			currentBlue = currentBlue < 0 ? 0 : currentBlue;

			}
		currentRed = 255 - (yRedDelta* (y+1));
		currentGreen = 255 - (yGreenDelta * (y+1));
		currentBlue = 255 - (yBlueDelta * (y+1));
		}
	SDL_UnlockSurface(sRgb);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(r, sRgb);
	SDL_FreeSurface(sRgb);
	SDL_RenderCopy(r, tex, NULL, &rect);
	SDL_DestroyTexture(tex);

	}






void init_window_round_foolow(POINT cursor, i32 width, i32 height) {

	wM   = SDL_CreateShapedWindow("stagod", cursor.x, cursor.y, width, height,  SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP  );
	(void)P_SDL_ERR(wM);

	SDL_Surface* shapeSurface = SDL_CreateRGBSurfaceWithFormat(
	                              0, width, height, 32, SDL_PIXELFORMAT_RGBA8888
	                            );

	// Draw circle on the shape surface
	int centerX = 25;
	int centerY = height / 2;
	int radius = height / 2;

	SDL_LockSurface(shapeSurface);
	//FIRST HALFS
	u32* pixels = (u32*)shapeSurface->pixels;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < 20; ++x) {
				{
				int dx = x - centerX, dy = y - centerY;
				if (dx*dx + dy*dy <= radius*radius) {
					pixels[y * width + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
					}
				else {
					pixels[y * width + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 0);
					}
				}
			}
		}
	for (int y = 0; y < height; ++y) {
		for (int x = 20; x < width - 20; ++x) {
			pixels[y * width + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
			}
		}
	centerX = width - 25;
	for (int y = 0; y < height; ++y) {
		for (int x = width - 20; x < width; ++x) {
				{
				int dx = x - centerX, dy = y - centerY;
				if (dx*dx + dy*dy <= radius*radius) {
					pixels[y * width + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 255);
					}
				else {
					pixels[y * width + x] = SDL_MapRGBA(shapeSurface->format, 0, 0, 0, 0);
					}
				}
			}
		}


	SDL_UnlockSurface(shapeSurface);

	// Apply the shape
	SDL_WindowShapeMode shapeMode;
	shapeMode.mode = ShapeModeDefault;
	if (SDL_SetWindowShape(wM, shapeSurface, &shapeMode) != 0) {
		SDL_Log("Shape error: %s", SDL_GetError());
		}
	SDL_FreeSurface(shapeSurface);

	rM = SDL_CreateRenderer(wM, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
	SDL_RaiseWindow(wM);
	}




void destroy_window() {
	SDL_DestroyRenderer(rM);
	SDL_DestroyWindow(wM);
	//SDL_Quit();
	rM = NULL;
	wM = NULL;
	}


void render_rgb(SDL_Renderer *r1, u32 rgb, i32 startX, i32 startY, i32 w_c, i32 h_c) {
	char msg[128];
	snprintf(msg, 128, "  %3u %3u %3u", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
	SDL_Color color = {225, 225, 225, 0};
	Text_Renderer_C(r1, f, startX, startY, w_c, h_c, msg, color);
	Sleep(3);

	}

void render_hsv(ColorHSV c, i32 startX, i32 startY, i32 w_c, i32 h_c) {
	char msg[128];
	snprintf(msg, 128, "  %3d %3u %3u", (i16)c.r, (u8)c.g, (u8)c.b);
	SDL_Color color = {225, 225, 225, 0};
	Text_Renderer_C(r, f, startX, startY, w_c, h_c, msg, color);
	Sleep(3);

	}



void render_color_round_square(SDL_Renderer *r1, u32 rgb, i32 startX, i32 startY, i32 wi, i32 hi, i32 ra) {
	SDL_SetRenderDrawColor(r1, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), 255);
	draw_rounded_rect(r1, startX, startY, wi, hi, ra);
	}

u32 ColourToUint(i32 R, i32 G, i32 B) {
	return (u32)((B << 16) + (G << 8) + (R << 0));
	}



void render_rgb_hex(SDL_Renderer* re, u32 rgb, i32 startX, i32 startY, i32 w_c, i32 h_c) {
	char msg[128];
	memset(msg, '\0', 128);
	if(rgb == 0) {
		snprintf(msg, 128, "  #000000");
		}
	else {
		SDL_Color temp = (SDL_Color) {
			GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), 0
			};
		//rgb = ColourToUint(temp.r, temp.g, temp.b);
		snprintf(msg, 128, "  #%.2x%.2x%.2x", temp.r, temp.g, temp.b);
		}

	//SDL_Color color = {225, 225, 225, 0};
	Text_Renderer_C(re, f, startX, startY, w_c, h_c, msg, WHITE);
	//Sleep(3);
	//SDL_SetWindowPosition(w, cursor.x + 10, cursor.y + 10);
	}


void render_elements(Element_DA *elements) {

	for(u64 i = 0; i < elements->count; i++) {
		Element e = elements->items[i];
		if(!program_state.howeringState.items[i]) {
			SDL_SetRenderDrawColor(r, e.colorBackground.r, e.colorBackground.g, e.colorBackground.b, e.colorBackground.a);
			}
		else {
			SDL_SetRenderDrawColor(r, e.colorBackground.r + 20, e.colorBackground.g + 20, e.colorBackground.b + 20, 128);
			}

		switch(e.types) {
			case Element_Button: {
					draw_rounded_rect(r, e.x, e.y, e.width, e.height, e.radius);
					break;
					}
			case Element_Color: {
					draw_rounded_rect(r, e.x, e.y, e.width, e.height, e.radius);
					if(e.rColor.x != 0 && e.rColor.y != 0) {
						SDL_SetRenderDrawColor(r, e.rColor.colorBackground.r, e.rColor.colorBackground.g,
						                       e.rColor.colorBackground.b, e.rColor.colorBackground.a);
						draw_rounded_rect(r, e.rColor.x, e.rColor.y, e.rColor.width, e.rColor.height, e.rColor.radius);

						//LOG("Draw ROUNDED\n");
						}
					break;
					}
			//THIS IS STATICLY AND DEPENDS ON ELEMENT POSITON
			case Element_Details: {
					draw_rounded_rect(r, e.x, e.y, e.width, e.height, e.radius);
					switch(e.typeD) {
						case Details_HEX: {
								render_rgb_hex(r, program_state.rgb, e.x - 15, (e.y + e.height - 30), e.width, 15);
								break;
								}
						case Details_RGB: {
								render_rgb(r, program_state.rgb, e.x - 15, (e.y + e.height - 30), e.width, 15);
								break;
								}
						case Details_HSL: {
								SDL_Color c = {GetRValue(program_state.rgb), GetGValue(program_state.rgb), GetBValue(program_state.rgb), 0 };
								ColorHSV  cHSV = rgb2hsl(c);
								render_hsv(cHSV, e.x - 15, (e.y + e.height - 30), e.width, 15);
								break;
								break;
								}
						case Details_HSV: {

								SDL_Color c = {GetRValue(program_state.rgb), GetGValue(program_state.rgb), GetBValue(program_state.rgb), 0 };
								ColorHSV  cHSV = rgb2hsv(c);
								render_hsv(cHSV, e.x - 15, (e.y + e.height - 30), e.width, 15);
								break;
								}

						}

					break;
					}
			default: {
					//ASSERT("Unrechable");
					break;
					}
			}

		//LOG("%d %d %d %d\n", e.x, e.y, e.width, e.height, e.radius);
		if(e.text.count > 0 && e.text.items != NULL) {
			//LOG("%d\n", i);
			Text_Renderer_C(r, f, e.text.startX, e.text.startY, e.text.width, e.text.height, e.text.items, e.text.color);
			}
		}
	}



static inline void render_window_sdl(SDL_Renderer*  renderer, Element_DA *renderingElements) {
	SDL_RenderClear(renderer);
	render_elements(renderingElements);
	static SDL_Event e;
	if( SDL_PollEvent(&e)) {

		}
	SDL_SetRenderDrawColor(r, 0x20, 0x20, 0x20, 0);
	SDL_RenderPresent(renderer);
	}

// Copies ANSI text to the clipboard
void CopyAnsiTextToClipboard(const char* text) {
	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		const size_t len = strlen(text) + 1;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
		if (hGlobal) {
			char* pGlobal = (char*)GlobalLock(hGlobal);
			if (pGlobal) {
				strcpy_s(pGlobal, len, text);
				GlobalUnlock(hGlobal);
				if (!SetClipboardData(CF_TEXT, hGlobal)) {
					GlobalFree(hGlobal); // Free if failed to set
					}
				}
			else {
				GlobalFree(hGlobal); // Free if failed to lock
				}
			}
		CloseClipboard();
		}
	}


void IsMouseKey() {

	bool currentEState = ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) ;
	Sleep(1);
	if(currentEState == true) {
		LOG("Left button\n");
		program_state.isMouseClicked = true;
		}
	}



static inline bool isHower(i32 x, i32 y, i32 width, i32 height) {
	return (bool)((y <= program_state.cursor.y && (y + height) >= program_state.cursor.y) &&
	              (x <= program_state.cursor.x && ((x + width) >= program_state.cursor.x)));
	}


void button_presed_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	DROP(c);
	//CHECK IF ELEMENTS IS COLAIDING
	//LOG("(%d %d %d %d) (%d  %d)\n",x, y, width, height, program_state.cursor.x, program_state.cursor.y);
	bool state = isHower(x, y, width, height);
	if(state) {
		LOG("ON ELEMENT\n");
		program_state.isButtonPressed = !program_state.isButtonPressed;

		}

	}

void color_element0_callback(i32 x, i32 y, i32 width, i32 height,  SDL_Color c) {
	bool state = isHower(x, y, width, height);
	if(state) {
		LOG("ON ELEMENT color0\n");
		program_state.whatColorIsSelected  = 0;
		program_state.rgb = ColourToUint(c.r, c.g, c.b);

		}
	}

void color_element1_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	bool state = isHower(x, y, width, height);
	if(state) {
		LOG("ON ELEMENT color1\n");
		program_state.whatColorIsSelected  = 1;
		program_state.rgb = ColourToUint(c.r, c.g, c.b);
		}

	}

void color_element2_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	bool state = isHower(x, y, width, height);
	if(state) {
		LOG("ON ELEMENT color2\n");
		program_state.whatColorIsSelected  = 2;
		program_state.rgb = ColourToUint(c.r, c.g, c.b);
		}

	}





void color_element3_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	bool state = (y <= program_state.cursor.y && (y + height) >= program_state.cursor.y) &&
	             (x <= program_state.cursor.x && ((x + width) >= program_state.cursor.x));
	if(state == true) {
		LOG("ON ELEMENT color3\n");
		program_state.whatColorIsSelected  = 3;
		program_state.rgb =  ColourToUint(c.r, c.g, c.b);

		}
	//DROP(whatElement);
	}

static inline void elements_callback(Element_DA* renderingElements, POINT cursor) {
	for(u64 i = 0; i < renderingElements->count; i++) {
		Element e = renderingElements->items[i];
		if(isHower(e.x + cursor.x, e.y + cursor.y, e.width, e.height)) {
			program_state.howeringState.items[i] = true;
			}
		else {
			program_state.howeringState.items[i] = false;
			}
		}
	if(program_state.isMouseClicked) {
		for(u64 i = 0; i < renderingElements->count; i++) {
			Element e = renderingElements->items[i];
			if(e.callback != NULL) {
				//LOG("Callback %d\n", i);//MAYBE RET FOR BREAK
				e.callback(e.x + cursor.x, e.y + cursor.y, e.width, e.height, e.rColor.colorBackground);
				}
			}
		program_state.isMouseClicked = false;
		}
	}


void hex_element_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	bool state = isHower(x, y, width, height);
	if(state) {
		char text[256];
		if(program_state.rgb == 0) {
			snprintf(text, 128, "  #000000");
			}
		else {
			SDL_Color temp = (SDL_Color) {
				GetRValue(program_state.rgb), GetGValue(program_state.rgb), GetBValue(program_state.rgb), 0
				};
			snprintf(text, 128, "  #%.2x%.2x%.2x", temp.r, temp.g, temp.b);

			}
		CopyAnsiTextToClipboard(text);
		}
	DROP(c);
	}

void rgb_element_callback(i32 x, i32 y, i32 width, i32 height, SDL_Color c) {
	bool state = isHower(x, y, width, height);
	if(state) {
		char text[256];
		snprintf(text, 128, "%u, %u, %u", GetRValue(program_state.rgb), GetGValue(program_state.rgb), GetBValue(program_state.rgb));
		CopyAnsiTextToClipboard(text);
		}
	DROP(c);
	}





void following_window() {
	if(!program_state.isWindowInited) {
		init_window_round_foolow(program_state.cursor, WIDTH, HEIGHT);
		program_state.isWindowInited = true;
		}
	else {
		SDL_SetWindowPosition(wM, program_state.cursor.x + 10, program_state.cursor.y + 10);
		SDL_RenderClear(rM);
		render_rgb_hex(rM, program_state.rgb, 100, 10, 120, 20);
		render_color_round_square(rM, program_state.rgb, 10, 10, 40, 40, 5);
		render_rgb(rM, program_state.rgb, 80, 40, 160, 25);
		static SDL_Event event1;
		if(SDL_PollEvent(&event1)) {
			//Sleep(1);
			}
		//SDL_SetRenderDrawColor(r, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), 255);
		SDL_SetRenderDrawColor(rM, 25, 25, 25, 255);
		SDL_RenderPresent(rM);
		}
	}

void get_time() {
	time_t t = time(NULL);
	tm = localtime(&t);

	memset(program_state.dateStr, '\0', 64);
	strftime(program_state.dateStr, sizeof(program_state.dateStr), "%c", tm);
	program_state.dateStr[8] = '\0';
	program_state.dateStr[2] = '.';
	program_state.dateStr[5] = '.';
	//printf("asd %s asd\n", program_state.dateStr);

	}

//Probobly some file
void create_date_color() {
	get_time();
	Date_Color dc;
	memcpy(dc.dat, program_state.dateStr, sizeof(dc.dat));
	dc.color = WHITE;
	dateAndColor.index = dateAndColor.count;
	da_append(&dateAndColor, dc);

	}


static struct tray tray;
#define POS_X GetSystemMetrics(SM_CXSCREEN) - 400
#define POS_Y GetSystemMetrics(SM_CYSCREEN) - 500


void init_main_window() {
	w  = SDL_CreateWindow("stagod", POS_X, POS_Y, width, height,  SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP  );
	(void)P_SDL_ERR(w);
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
	SDL_RaiseWindow(w);
	program_state.isMainWindowInited = true;
	LOG("MainWindowCreated\n");
	//tray_exit();
	//Sleep(1);
	//tray_init(&tray);

	}

void destroy_main_window() {
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	//SDL_Quit();
	r = NULL;
	w = NULL;
	program_state.isMainWindowInited = false;
	LOG("MainWindowDestroyed\n");
	//Sleep(1);
	}




static void quit_cb(struct tray_menu *item) {
	DROP(item);
	tray_exit();
	exit(1);
	}


static void bring_cpicker(struct tray_menu *item) {
	DROP(item);
	#pragma omp critical
		{

		if(program_state.isMainWindowInited == true) {
			destroy_main_window();
			}
		else {
			init_main_window();
			}
		//Sleep(1);
		tray_update(&tray);
		}
	}



static struct tray tray = {
	.icon = "icon.ico",
	.menu =
	(struct tray_menu[]) {
			{"CPICK", 0, 0, bring_cpicker, NULL, NULL},
			{"-", 0, 0, NULL, NULL, NULL},
			{"Quit", 0, 0, quit_cb, NULL, NULL},
			{NULL, 0, 0, NULL, NULL, NULL}
		},
	//{.text = "Quit", .cb = quit_cb},
	//{.text = NULL}
	};




#define LAST_COLOR dateAndColor.items[dateAndColor.count - 1].color
#define LAST_DATE  dateAndColor.items[dateAndColor.count - 1].dat
#define INDEX_COLOR dateAndColor.items[dateAndColor.index]

#define INDEX_COLOR_DATE  INDEX_COLOR.dat
#define INDEX_COLOR_COLOR INDEX_COLOR.color


int main(int argc, char *argv[]) {
	DROP(argc);
	DROP(argv);
//INIT SDL2
	SDL_ERR(SDL_Init(SDL_INIT_VIDEO));
	SDL_ERR(TTF_Init());
	//SetProcessDpiAwarenessContext();
	f = TTF_OpenFont(fontLoc, 128);
	(void)P_SDL_ERR(f);
	//u32 rgb, eRgb;
	SetProcessDPIAware();

	//int counter = 0;
	POINT cursor = {0, 0}; //IS POINTER TO X, Y POINT STRUCT

	GetCursorPos(&cursor);

	//init time

	program_state.cursor = cursor;
	program_state.isButtonPressed = false;
	program_state.whatColorIsSelected = 0;
	program_state.rgb = 0x000000;



	static Element_DA renderingElements = {0};



	Element button = create_element(Element_Button, 20, 20, width - 40, 60, 15,  elemetColor, NULL);
	button.callback = button_presed_callback;
	bind_text(&button, "Select Color", button.width*0.4, button.height*0.7, button.width*0.4, 20, elemetTextColor);
	da_append(&renderingElements, button);
	da_append(&program_state.howeringState, false);

	Element history = create_element(Element_Button, button.x, button.y + button.height + 20,
	                                 button.width / 2 - 3, height - 120, 0, backgroundColor, NULL);
	bind_text(&history, "History", history.x + 10, history.y + 10, history.width / 2 - 20, 20, elemetTextColor);
	da_append(&renderingElements, history);
	da_append(&program_state.howeringState, false);

	Element details = create_element(Element_Button, history.x + history.width + 3, button.y + button.height + 20,
	                                 button.width / 2 - 3, height - 120, 0, backgroundColor, NULL);
	bind_text(&details, "Details", details.x + 10, details.y + 10, details.width / 2 - 20, 20, elemetTextColor);
	da_append(&renderingElements, details);

	Element middleLine = create_element(Element_Button, (history.x + history.width - 1), details.y, 3, details.height, 0, elemetTextColor, NULL);
	da_append(&renderingElements, middleLine);
	da_append(&program_state.howeringState, false);

	static Element colorElements[4];
	//TBD load from file
	//TBD date or hex or somthing
	create_date_color();

	colorElements[0] = create_element(Element_Color, history.text.startX,
	                                  history.text.startY + 35, (history.width - 30), button.height*1.3, button.radius, elemetColor, NULL);

	bind_text(&colorElements[0], LAST_DATE, colorElements[0].x + 7, (colorElements[0].y + colorElements[0].height / 2 + 10),
	          colorElements[0].width / 2, 20, elemetTextColor);

	bind_color(&colorElements[0], colorElements[0].x + 5, colorElements[0].y + 5,
	           colorElements[0].width - 10, colorElements[0].height / 2, 10, LAST_COLOR);


	colorElements[0].callback = color_element0_callback;
	da_append(&renderingElements, colorElements[0]);
	da_append(&program_state.howeringState, false);

	Element* colorPtr[4];
	for(u8 i = 1; i < 4; i++) {
		create_date_color();
		colorElements[i] = create_element(Element_Color, colorElements[0].x, colorElements[i-1].y + colorElements[i-1].height + 3,
		                                  colorElements[0].width,colorElements[0].height, colorElements[0].radius, colorElements[0].colorBackground, NULL);

		bind_text(&colorElements[i], LAST_DATE, colorElements[i].x + 7, (colorElements[i].y + colorElements[i].height / 2 + 10),
		          colorElements[i].width / 2, 20, elemetTextColor);
		bind_color(&colorElements[i], colorElements[i].x + 5, colorElements[i].y + 5,
		           colorElements[i].width - 10, colorElements[i].height / 2, 10, LAST_COLOR);
		}

	colorElements[1].callback = color_element1_callback;
	colorElements[2].callback = color_element2_callback;
	colorElements[3].callback = color_element3_callback;
	for(u8 i = 0; i < 4; i++) {
		da_append(&renderingElements, colorElements[i]);
		da_append(&program_state.howeringState, false);
		}

	for(u8 i = 0; i < 4; i++) {

		i32 count = renderingElements.count - 4 + i;
		colorPtr[i] = &renderingElements.items[count];

		}


	static Element dataElements[4];

	const char* dataEChar[] = {
		"HEX",
		"RGB",
		"HSV",
		"HSL",
		};
	//THEY ARE RENDERD RELATIVLY TO TEXXT Element_Details
	dataElements[0] = create_element(Element_Details, details.text.startX,
	                                 details.text.startY + 35, (details.width - 30), button.height*1.3,
	                                 button.radius, elemetColor, NULL);

	bind_text(&dataElements[0], dataEChar[0], dataElements[0].x + 7, (dataElements[0].y + 10),
	          30, 20, elemetTextColor);
	dataElements[0].typeD = 0;
	dataElements[0].callback = hex_element_callback;
	da_append(&renderingElements, dataElements[0]);
	da_append(&program_state.howeringState, false);

	for(u8 i = 1; i < 4; i++) {
		dataElements[i] = create_element(Element_Details, dataElements[0].x, dataElements[i-1].y + dataElements[i-1].height + 3,
		                                 dataElements[0].width,colorElements[0].height, dataElements[0].radius,
		                                 dataElements[0].colorBackground, NULL);
		bind_text(&dataElements[i], dataEChar[i], dataElements[i].x + 7, (dataElements[i].y + 10),
		          30, 20, elemetTextColor);
		dataElements[i].typeD = i;

		da_append(&program_state.howeringState, false);
		}
	dataElements[1].callback = rgb_element_callback;
	da_append(&renderingElements, dataElements[1]);
	da_append(&renderingElements, dataElements[2]);
	da_append(&renderingElements, dataElements[3]);
//TBD REBIND TO USE LATER
//LET THIS BE POOLING

	//init_main_window();
	program_state.isMainWindowInited = false;
//INIT try
	#pragma omp parallel
		{


		#pragma omp single
			{


			#pragma omp task

				{
				tray_init(&tray);
				while(tray_loop(1) == 0) {
					//tray_update(&tray);
					}
				}





			//Sleep(1000);
//TRY is for exit
			#pragma omp task

				{

				while(1) {
					//tray_update(&tray);

					while(!program_state.isMainWindowInited) {
						Sleep(1);
						}

					//CALBACK RUNNING
					if(program_state.isMainWindowInited) {

						GetCursorPos(&program_state.cursor);
						Sleep(1);
						IsMouseKey();
						#pragma omp critical
							{
							if(program_state.isButtonPressed == true) {
								program_state.rgb = (get_pixel_color(&program_state.cursor));
								//disp_colorref(program_state.rgb);
								u8 sC = program_state.whatColorIsSelected;
								//LOG("%d", sC);
								colorPtr[sC]->rColor.colorBackground = (SDL_Color) {
									GetRValue(program_state.rgb), GetGValue(program_state.rgb), GetBValue(program_state.rgb), 0
									};
								//IF CLICKED SAVE COLOR
								//COPY_TO_CLIPBOARD 
								//CREATE FOLOWING WINDOW
								//TBD NOT CLICK ELEMENT
								following_window();


								if(program_state.isMouseClicked) {
									program_state.isButtonPressed = false;
									destroy_window();
									program_state.isWindowInited = false;
									//destroy second window
									}
								}
							else {
								cursor.x =  POS_X;
								cursor.y =  POS_Y;
								elements_callback(&renderingElements, cursor);

								}
							if(program_state.isMainWindowInited) {
								render_window_sdl(r, &renderingElements);
								}
							}
						Sleep(30);
						}
					}
				}
			}
		}
	//*/
	//DROP(tray_loop(1));
	}
