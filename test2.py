import gym
env = gym.make('gym_celex:celex-v0')
while True:
    print(env.step(0))
    env.render()