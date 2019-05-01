import gym
from gym.envs.registration import registry, make, spec


def register(id, *args, **kvargs):
  if id in registry.env_specs:
    return
  else:
    return gym.envs.registration.register(id, *args, **kvargs)


#register(
#	id='AtlasBulletEnv-v0',
#	entry_point='pybullet_envs.gym_locomotion_envs:AtlasBulletEnv',
#	max_episode_steps=1000
#	)


def getList():
  btenvs = ['- ' + spec.id for spec in gym.envs.registry.all() if spec.id.find('Bullet') >= 0]
  return btenvs
