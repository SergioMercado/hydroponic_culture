const { Actuator, Agent } = require('../../config/models');

const findOne = async ({ agentCode, agentId }) => {
  const actuator = await Actuator.findOne({
    include: [
      {
        attributes: ['code', 'name'],
        model: Agent,
        required: true,
        where: { code: agentCode }
      }
    ],
    where: { agentId }
  });

  return actuator;
};

const updateState = async ({ agentCode, agentId, value }) => {
  const actuator = await Actuator.update(
    { value },
    {
      include: [
        {
          model: Agent,
          required: true,
          where: { code: agentCode }
        }
      ],
      where: { agentId }
    }
  );

  return actuator;
};

module.exports = {
  findOne,
  updateState
};
