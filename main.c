#include <stdio.h>
#include <SDL/SDL.h>
#define NUM_DOTS 2048
#define SCREEN_WIDTH 	640
#define WINDOW_TITLE "Unicorn Tears: Star Field X"
#define SCREEN_HEIGHT	480

struct dot
{
	int red,green,blue,speed;
	float vx, vy;
	float x,y;	
};

struct dot demo_dots[NUM_DOTS];

SDL_Surface *main_screen;

void init_dots() {
	int i = 0;
	for (i;i < NUM_DOTS; i++) {
	  demo_dots[i].red = 0;
	  demo_dots[i].green = rand() % 255;
	  demo_dots[i].blue = rand() % 255;
	  demo_dots[i].x = rand() % SCREEN_WIDTH;
	  demo_dots[i].y = rand() % SCREEN_HEIGHT;
	  demo_dots[i].speed = 1 + rand() % 5;
	}
}

void move_dots() {
	int i = 0;
	for (i; i < NUM_DOTS; i ++) {
		demo_dots[i].x = ((int)(demo_dots[i].x + demo_dots[i].speed) % 640);
	}
}

void draw_screen() {
	int i=0,bpp,rank,x,y;
	Uint32 *pixel;
	rank = main_screen->pitch/sizeof(Uint32);
	pixel = (Uint32*)main_screen->pixels;

	SDL_LockSurface(main_screen);
	for(i=0; i < NUM_DOTS ; i++) {
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
	int gFrames = 0;

	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Could not initialise %s\n",SDL_GetError());
	}

	main_screen = SDL_SetVideoMode(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0,
		SDL_HWSURFACE|SDL_DOUBLEBUF
	);

	if (!main_screen) {
		fprintf(stderr, "Could not initialise %s\n", SDL_GetError());
	}

	SDL_WM_SetCaption( WINDOW_TITLE, 0 );

	printf("SDL On Line\n");
	init_dots();	
	active = 1;

	while(active) {
		SDL_FillRect(main_screen,NULL,SDL_MapRGBA(main_screen->format,0,0,0,255));
		draw_screen();
		SDL_Flip(main_screen);
		move_dots();
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				active = 0;
			}
		}
	}

	SDL_Quit();
	return 0;
}
