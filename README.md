# Celexplore
A work in progress by Samuel Cottrell. Intended to apply [OpenAI's Random Network Distillation](https://openai.com/blog/reinforcement-learning-with-prediction-based-rewards/) to [Celeste Classic](https://mattmakesgames.itch.io/celesteclassic). Code for Celeste Classic forked from [cceleste](https://github.com/lemon32767/ccleste).
# Installation
git clone, cd, and run `pip install .` (Yes, the period is part of the command.)
# Running
`python3 test2.py` runs my current test file.

nessecary prerequisite:
`sudo apt-get install libsdl2-dev libsdl2-mixer-dev`


The game logic is in celeste.c, the CelesteGame class is defined in celexplore.cpp, the pybind11 bindings are in module.cpp, and the (incomplete) gym environment logic is in celex_env.py. This code has yet to be editied for clarity or fully commented. Sorry.