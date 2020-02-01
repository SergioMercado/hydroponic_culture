const { Sensor, Agent } = require('../../config/models');

const findAll = async ({ agentCode, agentId }) => {
  const sensors = await Sensor.findAll({
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

  return sensors;
};

const updateValue = async ({ agentId, agentCode, code, value, status }) => {
  const sensor = await Sensor.update(
    { code, value, status },
    {
      include: [
        {
          model: Agent,
          required: true,
          where: { code: agentCode }
        }
      ],
      where: { agentId, code }
    }
  );

  return sensor;
};

module.exports = {
  findAll,
  updateValue
};
