#include <iostream>
#include <sstream>
#include <unistd.h>
using namespace std ;

#include <emscripten/emscripten.h>
#include <SDL2/SDL.h>

#include <math.h>

#define SCREEN_WIDTH  500
#define SCREEN_HEIGHT 500

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int quit=0 ;
int mypause=100;
double f1 = 5.0 ;
double f2 = 7.0 ;

void drawText( SDL_Renderer *renderer, const char *pszMsg ) ;

void drawTextJS( SDL_Renderer *renderer, string msg ) 
{
	string jscript = string("document.getElementById('myoutput').innerHTML = '") + msg + "'" ;
	// printf( "%s\n", jscript.c_str() ) ;
	printf( "%s\n", msg.c_str() ) ;
	emscripten_run_script( jscript.c_str() ) ;
}
void initText() ;

int cvtXLogToPix(double x) {
	return x*SCREEN_WIDTH/2 + SCREEN_WIDTH*1.2/2 ;
}

int cvtYLogToPix(double y) {
	return y*SCREEN_HEIGHT/2 + SCREEN_HEIGHT*1.2/2 ;
}

void render()
{
	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 0x00);
	
	double timefact = (double)clock()/CLOCKS_PER_SEC ; // 0 to 1
	//printf( "%lf", timefact ) ;
	double phase = fmod( timefact*5, 10*2*3.142 ) ;
	
	double xlast ;
	double ylast ;
	for( int i=0 ; i<SCREEN_WIDTH*5 ; i++ ) 
	{
		double t = ((double)i/(SCREEN_WIDTH*5))*3.142*2 ; //+ (double)clock()/CLOCKS_PER_SEC ;
		
		double x = cos(t*f1+phase) ;
		double y = sin(t*f2+phase) ;
		
		// SDL_RenderDrawPoint(renderer, x*SCREEN_WIDTH/2 + SCREEN_WIDTH*1.2/2, y*SCREEN_HEIGHT/2 + SCREEN_HEIGHT*1.2/2 );
		if( i > 0 )
			SDL_RenderDrawLine(renderer, xlast, ylast, cvtXLogToPix(x), cvtYLogToPix(y) );
		
		xlast = cvtXLogToPix(x) ;
		ylast = cvtYLogToPix(y) ;
	}
	ostringstream ostr ;
	ostr << "f1: " << f1 << " f2: " << f2 << "    space: pause, cursor up/dn/left/right: change frequencies" ; 
	const string &msg = ostr.str() ;
	drawText( renderer, msg.c_str() ) ;	
	
    SDL_RenderPresent(renderer);
}

void doRender()
{
 SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                quit = 1;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
				case SDLK_SPACE:
				{
					mypause = !mypause ;
					break;
				}
                case SDLK_UP:
                {
					f2 += 1 ;
                    break;
                }
                case SDLK_DOWN:
                {
					f2 -= 1 ;
                    break;
                }
                case SDLK_LEFT:
                {
					f1 -= 1 ;
                    break;
                }
                case SDLK_RIGHT:
                {
					f1 += 1 ;
                    break;
                }
                }
                break;
            }
            }

        }	
		
	if( mypause == 1 ) {
		render();
		SDL_UpdateWindowSurface(window);	
	} else if( mypause > 1 ) {
		mypause-- ;
		//printf( "Counting down %d\n", mypause ) ;
	}
}

void main_loop()
{
    emscripten_set_main_loop(doRender, -1, 1);
}

int main()
{
	try 
	{
		cout << "Starting Lissajous demo... press space to start." << endl ;
		
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(
			"Lissajous WASM Demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH*1.2, SCREEN_HEIGHT*1.2,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

		//drawText( renderer, "test") ;
		
        //sleep(10) ;
		main_loop();

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	catch(runtime_error &x)
	{
		printf( "runtime error, exiting: %s\n", x.what() ) ;
	}
	catch(...)
	{
		printf( "Ooops, exiting\n" ) ;
	}	
}