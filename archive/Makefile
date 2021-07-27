
# set to 1 to use SDL1.2
SDL_VER?=2

ifeq ($(SDL_VER),2)
	SDL_CONFIG=sdl2-config
	SDL_LD=-lSDL2 -lSDL2_mixer
else
ifeq ($(SDL_VER),1)
	SDL_CONFIG=sdl-config
	SDL_LD=-lSDL -lSDL_mixer
else
	SDL_CONFIG=$(error "invalid SDL version '$(SDL_VER)'. possible values are '1' and '2'")
endif
endif
PYTHON_LOC=-I/usr/include/python3.9
CFLAGS=-Wall -g -O2 `$(SDL_CONFIG) --cflags`
LDFLAGS=$(SDL_LD)
LDFLAGS+=-lpython3.9
CELESTE_CC=$(CC)

ifneq ($(USE_FIXEDP),)
	OUT=ccleste-fixedp
	CELESTE_OBJ=celeste-fixedp.o
	CFLAGS+=-DCELESTE_P8_FIXEDP
	CELESTE_CC=$(CXX)
else
	OUT=celexplore
	CELESTE_OBJ=celeste.o
	CFLAGS+=-fpermissive
	LDFLAGS+=-lm
endif

ifneq ($(HACKED_BALLOONS),)
	CFLAGS+=-DCELESTE_P8_HACKED_BALLOONS
endif

all: $(OUT)

$(OUT): celexplore.cpp $(CELESTE_OBJ) celeste.h sdl20compat.inc.c sdl12main.c
	$(CC) $(CFLAGS) celexplore.cpp $(CELESTE_OBJ) -o $(OUT) $(LDFLAGS)

$(CELESTE_OBJ): celeste.c celeste.h
	$(CELESTE_CC) $(CFLAGS) -c -o $(CELESTE_OBJ) celeste.c

clean:
	$(RM) ccleste ccleste-fixedp celeste.o celeste-fixedp.o
	make -f Makefile.3ds clean
