import cppimport.import_hook
import module
import numpy as np
import time
import random
# decorator to calculate duration
# taken by any function.
def calculate_time(func):
      
    # added arguments inside the inner1,
    # if function takes any arguments,
    # can be added like this.
    def inner1(*args, **kwargs):
  
        # storing time before function execution
        begin = time.time()
          
        x = func(*args, **kwargs)
  
        # storing time after function execution
        end = time.time()
        print("Total time taken in : ", func.__name__, end - begin)
        return x
  
    return inner1
def getScreen(gm):
    data = np.zeros((128,128))
    for x in range(128):
        for y in range(128):
            data[x][y] = gm.getPixel(x,y)
    return
@calculate_time
def getPixelSet(gm):
    data = set()
    for x in range(128):
        for y in range(128):
            data.add(gm.getPixel(x,y))
    return data
def maybe():
    return bool(random.getrandbits(1))
@calculate_time
def gameLoop(game):
    game.nextFrame(maybe(),maybe(),maybe(),maybe(),maybe(),maybe())
    getScreen(game)

if __name__ == '__main__':
    game = module.CelesteGame()
    while game.isRunning():
        gameLoop(game)