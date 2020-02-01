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

module.exports = {
  findAll
};
