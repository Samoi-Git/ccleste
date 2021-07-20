g++ -Wall -g -O2 `sdl2-config --cflags` `pkg-config --cflags --libs python3` `python3 -m pybind11 --includes` -fpermissive module.cpp celeste.o -o celexplore -lSDL2 -lSDL2_mixer -lpython3.9 -lm
