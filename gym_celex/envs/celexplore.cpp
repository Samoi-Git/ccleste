#include "sdl12main.c"
#include <random>
#include <iostream>
class CelesteGame {
	public:
	CelesteGame() {
		SDL_CHECK(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) == 0);
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
		SDL_GameControllerAddMappingsFromRW(SDL_RWFromFile("gamecontrollerdb.txt", "rb"), 1);
		int videoflag = SDL_SWSURFACE | SDL_HWPALETTE;
		SDL_CHECK(screen = SDL_SetVideoMode(PICO8_W*scale, PICO8_H*scale, 32, videoflag));
		SDL_WM_SetCaption("Celeste", NULL);
		int mixflag = MIX_INIT_OGG;
		if (Mix_Init(mixflag) != mixflag) {
			ErrLog("Mix_Init: %s\n", Mix_GetError());
		}
		if (Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 1024) < 0) {
			ErrLog("Mix_Init: %s\n", Mix_GetError());
		}
		ResetPalette();
		SDL_ShowCursor(0);


		printf("game state size %gkb\n", Celeste_P8_get_state_size()/1024.);

		printf("now loading...\n");

		{
			const unsigned char loading_bmp[] = {
				0x42,0x4d,0xca,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x00,
				0x00,0x00,0x6c,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x09,0x00,
				0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x48,0x00,
				0x00,0x00,0x23,0x2e,0x00,0x00,0x23,0x2e,0x00,0x00,0x02,0x00,
				0x00,0x00,0x02,0x00,0x00,0x00,0x42,0x47,0x52,0x73,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x00,0x00,0x00,
				0x00,0x00,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,
				0x00,0x00,0x66,0x3e,0xf1,0x24,0xf0,0x00,0x00,0x00,0x49,0x44,
				0x92,0x24,0x90,0x00,0x00,0x00,0x49,0x3c,0x92,0x24,0x90,0x00,
				0x00,0x00,0x49,0x04,0x92,0x24,0x90,0x00,0x00,0x00,0x46,0x38,
				0xf0,0x3c,0xf0,0x00,0x00,0x00,0x40,0x00,0x12,0x00,0x00,0x00,
				0x00,0x00,0xc0,0x00,0x10,0x00,0x00,0x00,0x00,0x00
			};
			const unsigned int loading_bmp_len = 202;
			SDL_RWops* rw = SDL_RWFromConstMem(loading_bmp, loading_bmp_len);
			SDL_Surface* loading = SDL_LoadBMP_RW(rw, 1);
			if (!loading) goto skip_load;

			SDL_Rect rc = {60, 60};
			SDL_BlitSurface(loading,NULL,screen,&rc);
			
			SDL_FreeSurface(loading);
		} skip_load:

		LoadData();

		int pico8emu(CELESTE_P8_CALLBACK_TYPE call, ...);
		Celeste_P8_set_call_func(pico8emu);

		//for reset
		initial_game_state = SDL_malloc(Celeste_P8_get_state_size());
		if (initial_game_state) Celeste_P8_save_state(initial_game_state);

		if (TAS) {
			// a consistent seed for tas playback
			Celeste_P8_set_rndseed(8);
		} else {
			Celeste_P8_set_rndseed((unsigned)(time(NULL) + SDL_GetTicks()));
		}
		Celeste_P8_init();

		printf("ready\n");
		{
			FILE* start_fullscreen_f = fopen("ccleste-start-fullscreen.txt", "r");
			const char* start_fullscreen_v = getenv("CCLESTE_START_FULLSCREEN");
			if (start_fullscreen_f || (start_fullscreen_v && *start_fullscreen_v)) {
				SDL_WM_ToggleFullScreen(screen);
			}
			if (start_fullscreen_f) fclose(start_fullscreen_f);
		}

		

	}
	void nextFrame(bool up, bool down, bool left, bool right, bool dash, bool jump) {
	
		const Uint8* kbstate = SDL_GetKeyState(NULL);
			
		static int reset_input_timer = 0;
		//hold F9 (select+start+y) to reset
		if (initial_game_state != NULL && kbstate[SDLK_F9]) {
			reset_input_timer++;
			if (reset_input_timer >= 30) {
				reset_input_timer=0;
				//reset
				OSDset("reset");
				paused = 0;
				Celeste_P8_load_state(initial_game_state);
				Mix_HaltChannel(-1);
				Mix_HaltMusic();
				Celeste_P8_init();
			}
		} else reset_input_timer = 0;

		Uint16 prev_buttons_state = buttons_state;
		buttons_state = 0;


		SDL_GameControllerUpdate();
		ReadGamepadInput(&buttons_state);

		if (!((prev_buttons_state >> PSEUDO_BTN_PAUSE) & 1)
		&& (buttons_state >> PSEUDO_BTN_PAUSE) & 1) {
			goto toggle_pause;
		}

		if (!((prev_buttons_state >> PSEUDO_BTN_EXIT) & 1)
		&& (buttons_state >> PSEUDO_BTN_EXIT) & 1) {
			goto press_exit;
		}

		if (!((prev_buttons_state >> PSEUDO_BTN_SAVE_STATE) & 1)
		&& (buttons_state >> PSEUDO_BTN_SAVE_STATE) & 1) {
			goto save_state;
		}

		if (!((prev_buttons_state >> PSEUDO_BTN_LOAD_STATE) & 1)
		&& (buttons_state >> PSEUDO_BTN_LOAD_STATE) & 1) {
			goto load_state;
		}

		SDL_Event ev;
		while (SDL_PollEvent(&ev)) switch (ev.type) {
			case SDL_QUIT: running = 0; break;
			case SDL_KEYDOWN: {
				if (ev.key.repeat) break; //no key repeat
				if (ev.key.keysym.sym == SDLK_ESCAPE) { //do pause
					toggle_pause:
					if (paused) Mix_Resume(-1), Mix_ResumeMusic(); else Mix_Pause(-1), Mix_PauseMusic();
					paused = !paused;
					break;
				} else if (ev.key.keysym.sym == SDLK_DELETE) { //exit
					press_exit:
					running = 0;
					break;
				} else if (ev.key.keysym.sym == SDLK_F11 && !(kbstate[SDLK_LSHIFT] || kbstate[SDLK_ESCAPE])) {
					if (SDL_WM_ToggleFullScreen(screen)) { //this doesn't work on windows..
						OSDset("toggle fullscreen");
					}
					screen = SDL_GetVideoSurface();
					break;
				} else if (0 && ev.key.keysym.sym == SDLK_5) {
					Celeste_P8__DEBUG();
					break;
				} else if (ev.key.keysym.sym == SDLK_s && kbstate[SDLK_LSHIFT]) { //save state
					save_state:
					game_state = game_state ? game_state : SDL_malloc(Celeste_P8_get_state_size());
					if (game_state) {
						OSDset("save state");
						Celeste_P8_save_state(game_state);
						game_state_music = current_music;
					}
					break;
				} else if (ev.key.keysym.sym == SDLK_d && kbstate[SDLK_LSHIFT]) { //load state
					load_state:
					if (game_state) {
						OSDset("load state");
						if (paused) paused = 0, Mix_Resume(-1), Mix_ResumeMusic();
						Celeste_P8_load_state(game_state);
						if (current_music != game_state_music) {
							Mix_HaltMusic();
							current_music = game_state_music;
							if (game_state_music) Mix_PlayMusic(game_state_music, -1);
						}
					}
					break;
				} else if ( //toggle screenshake (e / L+R)
						ev.key.keysym.sym == SDLK_e) {
					enable_screenshake = !enable_screenshake;
					OSDset("screenshake: %s", enable_screenshake ? "on" : "off");
				} break;
			}
		}

		
		if (left)  buttons_state |= (1<<0);
		if (right) buttons_state |= (1<<1);
		if (up)    buttons_state |= (1<<2);
		if (down)  buttons_state |= (1<<3);
		if (jump) buttons_state |= (1<<4);
		if (dash) buttons_state |= (1<<5);
		

		if (paused) {
			const int x0 = PICO8_W/2-3*4, y0 = 8;

			p8_rectfill(x0-1,y0-1, 6*4+x0+1,6+y0+1, 6);
			p8_rectfill(x0,y0, 6*4+x0,6+y0, 0);
			p8_print("paused", x0+1, y0+1, 7);
		} else {
			Celeste_P8_update();
			Celeste_P8_draw();
		}
		OSDdraw();

		/*for (int i = 0 ; i < 16;i++) {
			SDL_Rect rc = {i*8*scale, 0, 8*scale, 4*scale};
			SDL_FillRect(screen, &rc, i);
		}*/



	}
	
	int getPixel(int x, int y) {
		return getpixelint(x,y);
		//heck
	}

	int getLevel() {
		return level_index();
	}

	void renderScreen() {
		SDL_Flip(screen);
	}


	~CelesteGame() {
		if (game_state) SDL_free(game_state);
		if (initial_game_state) SDL_free(initial_game_state);

		SDL_FreeSurface(gfx);
		SDL_FreeSurface(font);
		for (int i = 0; i < (sizeof snd)/(sizeof *snd); i++) {
			if (snd[i]) Mix_FreeChunk(snd[i]);
		}
		for (int i = 0; i < (sizeof mus)/(sizeof *mus); i++) {
			if (mus[i]) Mix_FreeMusic(mus[i]);
		}

		Mix_CloseAudio();
		Mix_Quit();
		SDL_Quit();
	}
	bool isRunning(void) {
		return running;
	}



};
int test(void) {
	CelesteGame game = CelesteGame();
	srand(time(NULL));
	while (game.isRunning()) {
		game.nextFrame(bool(rand()%2),bool(rand()%2),bool(rand()%2),bool(rand()%2),bool(rand()%2),bool(rand()%2));
		SDL_Delay(16);
		
	}
	game.~CelesteGame();
	return 0;	
}
int main(void) {
	return test();
}
