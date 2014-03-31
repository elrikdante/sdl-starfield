/*
	SDL Video Demo
	demonstrates SDL's video working by moving dots around a screen
	written by Sayuri L. Kitsune (2012)
*/

/* Defines */
#define NUM_DOTS 1024

/* Includes */
#include <time.h>
#include <SDL.h>
#include <stdlib.h>

/* Types */
typedef struct
{
	int red,green; /* The shade of color in red,green. blue is always 0 since we're using it as the background */
	float vx,vy; /* Speed the particle is moving */
	float x,y; /* Current position of particle */
}dot;

/* Globals */
SDL_Surface *demo_screen;
float demo_time_measure = 0.0f;
float demo_time_step = 0.0f;
dot demo_dots[NUM_DOTS];

/* Returns a random floating point number between 0.0f and 1.0f */
float demo_roll()
{
	float r;
	r = (float)(rand()%RAND_MAX); /* 0 ~ whatever RAND_MAX is */
	r /= (float)(RAND_MAX-1); /* one less than RAND_MAX makes it possible for 1.0f to happen */
	return r;
}

/* Initialize dots */
void demo_init()
{
	int i;
	for(i = 0;i < NUM_DOTS;i++)
	{
		demo_dots[i].red = rand()%255;
		demo_dots[i].green = rand()%255;
		demo_dots[i].vx = demo_roll()*16.0f-8.0f;
		demo_dots[i].vy = demo_roll()*16.0f-8.0f;
		demo_dots[i].x = demo_roll()*320.0f;
		demo_dots[i].y = demo_roll()*240.0f;
	}
}

/* Handle dots */
void demo_handle()
{
	int i;
	for(i = 0;i < NUM_DOTS;i++)
	{
		/* Move */
		demo_dots[i].x += demo_dots[i].vx*demo_time_step;
		demo_dots[i].y += demo_dots[i].vy*demo_time_step;
		/* Hit walls? */
		if(demo_dots[i].x < 0.0f || demo_dots[i].x >= 320.0f)
		{
			/* Undo move (demo_time_step is still the same value it was before and is valid for the current frame) */
			demo_dots[i].x -= demo_dots[i].vx*demo_time_step;
			/* Reverse */
			demo_dots[i].vx = -demo_dots[i].vx;
		}
		if(demo_dots[i].y < 0.0f || demo_dots[i].y >= 240.0f)
		{
			/* Undo move (demo_time_step is still the same value it was before and is valid for the current frame) */
			demo_dots[i].y -= demo_dots[i].vy*demo_time_step;
			/* Reverse */
			demo_dots[i].vy = -demo_dots[i].vy;
		}
	}
}

/* Draw dots */
void demo_draw()
{
	int i,bpp,rank,x,y;
	Uint32 *pixel;
	/* Lock surface */
	SDL_LockSurface(demo_screen);
	rank = demo_screen->pitch/sizeof(Uint32);
	pixel = (Uint32*)demo_screen->pixels;
	/* Draw all dots */
	for(i = 0;i < NUM_DOTS;i++)
	{
		/* Rasterize position as integer */
		x = (int)demo_dots[i].x;
		y = (int)demo_dots[i].y;
		/* Set pixel */
		pixel[x+y*rank] = SDL_MapRGBA(demo_screen->format,demo_dots[i].red,demo_dots[i].green,0,255);
	}
	/* Unlock surface */
	SDL_UnlockSurface(demo_screen);
}

/* Convert from timespec to float */
float demo_convert_time(struct timespec *ts)
{
	float accu;
	/* Combine the value into floating number */
	accu = (float)ts->tv_sec; /* Seconds that have gone by */
	accu *= 1000000000.0f; /* One second is 1000x1000x1000 nanoseconds, s -> ms, us, ns */
	accu += (float)ts->tv_nsec; /* Nanoseconds that have gone by */
	/* Bring it back into the millisecond range but keep the nanosecond resolution */
	accu /= 1000000.0f;
	return accu;
}

/* Start time */
void demo_start_time()
{
	struct timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ts);
	demo_time_measure = demo_convert_time(&ts);
}

/* End time */
void demo_end_time()
{
	struct timespec ts;
	float delta;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ts);
	delta = demo_convert_time(&ts)-demo_time_measure; /* Find the distance in time */
	demo_time_step = delta/(1000.0f/16.0f); /* Weird formula, equals 1.0f at 16 frames a second */
}

/* Main */
int main(int argn,char **argv)
{
	SDL_Event ev;
	int active;
	/* Initialize SDL */
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
	/* Open main window */
	demo_screen = SDL_SetVideoMode(320,240,0,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(!demo_screen)
		fprintf(stderr,"Could not set video mode: %s\n",SDL_GetError());
	/* Initialize game */
	demo_init();
	/* Main loop */
	active = 1;
	while(active)
	{
		/* Handle events */
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
				active = 0; /* End */
		}
		/* Start time */
		demo_start_time();
		/* Handle game */
		demo_handle();
		/* Clear screen */
		SDL_FillRect(demo_screen,NULL,SDL_MapRGBA(demo_screen->format,0,0,255,255));
		/* Draw game */
		demo_draw();
		/* Show screen */
		SDL_Flip(demo_screen);
		/* End time */
		demo_end_time();
	}
	/* Exit */
	SDL_Quit();
	return 0;
}
