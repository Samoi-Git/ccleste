import gym
import time
import numpy as np
env = gym.make('gym_celex:celex-v0')
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
        print("Current FPS : ", 1/(end - begin))
        return x
  
    return inner1
@calculate_time
def loop():
    env.step(0)
    env.render()
while True:
    loop()
    