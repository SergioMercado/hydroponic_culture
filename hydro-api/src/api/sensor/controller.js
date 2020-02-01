const Mapper = require('./mapper');

const getSensors = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode } = req.query;

    const sensor = new Mapper({ agentCode, agentId });

    const result = await sensor.findSensors();

    return res.status(200).send(result);
  } catch (error) {
    console.log('TCL: getSensors -> error', error);
    return res.status(500).send({ message: error.message });
  }
};

module.exports = {
  getSensors
};
