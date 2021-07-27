import logging
from gym.envs.registration import register

logger = logging.getLogger(__name__)

register(
    id="celexplore",
    entry_point="celexplore.envs:celexplore"
)