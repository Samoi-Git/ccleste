rm celexplore
g++ -Wall -g -O2 `sdl2-config --cflags` `pkg-config --cflags --libs python3` `python3 -m pybind11 --includes` -fpermissive celexplore.cpp celeste.c -o runthis -lSDL2 -lSDL2_mixer -lpython3.8 -lm
