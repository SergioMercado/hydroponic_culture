const Mapper = require('./mapper');

const getSensors = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode } = req.query;

    const sensor = new Mapper({ agentCode, agentId });

    const result = await sensor.findSensors();

    return res.status(200).send(result);
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

const updateSensorValues = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode } = req.query;
    const sensors = req.body;

    sensors.map(async ({ code, value }) => {
      console.log('TCL: updateSensorValues -> { code, value }', {
        code,
        value
      });
      const newSensor = new Mapper({ code, value, agentCode, agentId });

      newSensor.updateValue().catch(error => console.log(error));
    });

    return res.status(200).send({ message: 'sucessfully updated' });
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

module.exports = {
  getSensors,
  updateSensorValues
};
