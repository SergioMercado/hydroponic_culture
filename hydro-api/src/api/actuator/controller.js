const Mapper = require('./mapper');

const getActuator = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode, from, type } = req.query;

    const actuator = new Mapper({ agentCode, agentId });

    const result = await actuator.findActuator();

    if (from === 'mcu') {
      return res.status(200).send({ [type]: { value: result.value } });
    }

    return res.status(200).send(result);
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

const updateActuatorState = async (req, res) => {
  try {
    const { value, agentCode, agentId } = req.body;

    const actuator = new Mapper({ agentCode, agentId, value });

    await actuator.updateState();

    return res
      .status(200)
      .send({ message: `Changed to ${value ? 'on' : 'off'}` });
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

module.exports = {
  getActuator,
  updateActuatorState
};
