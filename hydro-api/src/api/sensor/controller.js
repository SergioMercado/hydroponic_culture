const Mapper = require("./mapper");
const Helper = require("./helper");

const getSensors = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode } = req.query;

    const sensor = new Mapper({ agentCode, agentId });

    const result = await sensor.findSensors();

    const sensors = Helper.sendNotification(result);

    return res.status(200).send(sensors);
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

const updateSensorValues = async (req, res) => {
  try {
    const { agentId } = req.params;
    const { agentCode } = req.query;
    const sensors = req.body;

    sensors.map(async ({ code, value, status, variable }) => {
      console.log(
        "TCL: updateSensorValues -> value, status, variable",
        value,
        status,
        code
      );
      const newSensor = new Mapper({ code, value, agentCode, agentId, status });

      if (code === "us1-h1") {
        if (value <= 10) {
          console.log("low");
          await Helper.updatePump({
            sensor: { agentCode, agentId },
            value: false
          });
        } else if (value >= 90) {
          console.log("high");

          await Helper.updatePump({
            sensor: { agentCode, agentId },
            value: false
          });
        }
        console.log("stable");

        await Helper.updatePump({
          sensor: { agentCode, agentId },
          value: true
        });
      }

      newSensor.updateValue().catch(error => console.log(error));
    });

    return res.status(200).send({ message: "sucessfully updated" });
  } catch (error) {
    return res.status(500).send({ message: error.message });
  }
};

module.exports = {
  getSensors,
  updateSensorValues
};
