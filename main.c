#include <stdio.h>
#include <SDL/SDL.h>
#define NUM_DOTS 1024

struct dot
{
	int red,green,blue;
	float vx, vy;
	float x,y;	
};

struct dot demo_dots[NUM_DOTS];

SDL_Surface *main_screen;

void init_dots() {
	int i = 0;
	for (i;i < NUM_DOTS; i++) {
		demo_dots[i].red = rand() % 255;
		demo_dots[i].green = rand() % 255;
		demo_dots[i].blue = rand() % 255;
		demo_dots[i].x = rand() % 320;
		demo_dots[i].y = rand() % 240;
	}
}

void move_dots() {
}

void draw_screen() {
	int i=0,bpp,rank,x,y;
	Uint32 *pixel;
	SDL_LockSurface(main_screen);
	rank = main_screen->pitch/sizeof(Uint32);
	pixel = (Uint32*)main_screen->pixels;

	for(; i < NUM_DOTS ; i++) {
		x = (int)demo_dots[i].x;
		y = (int)demo_dots[i].y;
		pixel[x+y*rank] = SDL_MapRGBA(
					main_screen->format,
					demo_dots[i].red,
					demo_dots[i].green,
					demo_dots[i].blue,
					255
		);
	}
	SDL_UnlockSurface(main_screen);
}


int main(int argn, char **argv) {
	int active;
	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Could not initialise %s\n",SDL_GetError());
	}

	main_screen = SDL_SetVideoMode(320,240,0,SDL_HWSURFACE|SDL_DOUBLEBUF);

	if (!main_screen) {
		fprintf(stderr, "Could not initialise %s\n", SDL_GetError());
	}
	printf("SDL On Line\n");
	init_dots();	
	active = 1;
	while(active) {
		SDL_FillRect(main_screen,NULL,SDL_MapRGBA(main_screen->format,0,0,255,255));
		move_dots();
		draw_screen();
		SDL_Flip(main_screen);
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				active = 0;
			}
		}
	}

	SDL_Quit();
	return 0;
}
