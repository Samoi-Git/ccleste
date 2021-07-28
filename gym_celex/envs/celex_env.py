import celeste as cel
import gym
from gym import error, spaces, utils
from gym.utils import seeding
import logging
logger = logging.getLogger(__name__)
class CelexEnv(gym.Env):
    metadata = {'render.modes':['human']}
    
    def __init__(self):
        self.game = cel.CelesteGame()
        #actions: 8 for dash(any direction), 2 for jumps (left/right), 8 for walk/drift(any direction)
        self.action_space = spaces.Discrete(18)
        self.level = 0
        
    def step(self, action):
        """Takes an int, to register one of 18 actions"""
        reward = self._get_reward()
        end = False
        return 0, reward, end, {}

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

