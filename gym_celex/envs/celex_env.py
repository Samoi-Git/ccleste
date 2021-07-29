import celeste as cel
import gym
from gym import error, spaces, utils
from gym.utils import seeding
import logging
import numpy as np
logger = logging.getLogger(__name__)
class CelexEnv(gym.Env):
    metadata = {'render.modes':['human']}
    
    def __init__(self):
        self.game = cel.CelesteGame()
        #actions: 8 for dash(any direction), 3 for jumps (left/right/neither), 8 for walk/drift(any direction)
        self.action_space = spaces.Discrete(19)
        self.level = 0
        
    def step(self, action):
        """Takes an int, to register one of 18 actions TODO"""
        self.takeAction(action)
        reward = self._get_reward()
        state = self.getScreen()
        end = False
        return state, reward, end, {}

    def getScreen(self):
        data = np.zeros((128,128))
        for x in range(128):
            for y in range(128):
                data[x][y] = self.game.getPixel(x,y)
        return data




    def _get_reward(self):
        currentlevel = self.game.getLevel()
        if self.level != currentlevel:
            self.level=currentlevel
            return 1
        return 0
    def renderScreen(self):
        self.game.renderScreen();
    def reset(self):
        self.game = cel.CelesteGame()
        self.level = 0
    def render(self, mode='human'):
        self.renderScreen();
    def close(self):
        self.game = None

    def takeAction(self,actionnum):
        #actions 0-7 are movement, actions 8-15 are dashes, actions 16-18 are jumps
        if actionnum>15:
            if actionnum==16:
                self.game.nextFrame(jump=True)
            elif actionnum==17:
                self.game.nextFrame(jump=True,left=True)
            elif actionnum==18:
                self.game.nextFrame(jump=True,right=True)
            else:
                raise ValueError("{} is not a valid action".format(actionnum))
            return
        elif actionnum>7:
            daash=True
            actionnum+=-8
        else: daash=False

        if actionnum==0:
            self.game.nextFrame(dash=daash,up=True)
        elif actionnum==1:
            self.game.nextFrame(dash=daash,up=True,right=True)
        elif actionnum==2:
            self.game.nextFrame(dash=daash,right=True)
        elif actionnum==3:
            self.game.nextFrame(dash=daash,right=True,down=True)
        elif actionnum==4:
            self.game.nextFrame(dash=daash,down=True)
        elif actionnum==5:
            self.game.nextFrame(dash=daash,down=True,left=True)
        elif actionnum==6:
            self.game.nextFrame(dash=daash,left=True)
        elif actionnum==7:
            self.game.nextFrame(dash=daash,left=True,up=True)
        else:
            raise ValueError("{} is not a valid action".format)
        return
            

