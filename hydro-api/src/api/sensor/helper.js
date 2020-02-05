const actuatorMapper = require("../actuator/mapper");
const sensorMapper = require("./mapper");

const updatePump = async ({ sensor, value }) => {
  const pump = new actuatorMapper({
    agentId: sensor.agentId,
    agentCode: sensor.agentCode
  });

  pump.value = value;

  await pump.updateState();
};

const sendNotification = sensors => {
  const foo = sensors.map(sensor => {
    switch (sensor.variable) {
      case "waterLevel":
        const waterMapper = new sensorMapper({
          agentId: sensor.agentId,
          agentCode: sensor.agent.code,
          code: sensor.code,
          value: sensor.value,
          status: sensor.status,
          variable: sensor.variable
        });

        if (waterMapper.value <= 10) {
          return { ...waterMapper, notification: "high" };
        } else if (waterMapper.value >= 90) {
          return { ...waterMapper, notification: "low" };
        }
        return { ...waterMapper, notification: "stable" };

        break;

      case "temperature":
        const temperatureMapper = new sensorMapper({
          agentId: sensor.agentId,
          agentCode: sensor.agent.code,
          code: sensor.code,
          value: sensor.value,
          status: sensor.status,
          variable: sensor.variable
        });

        if (temperatureMapper.value <= 10) {
          //await temperatureMapper.updateValue();
          return { ...temperatureMapper, notification: "low" };
        } else if (temperatureMapper.value >= 37) {
          return { ...temperatureMapper, notification: "high" };
        }
        return { ...temperatureMapper, notification: "stable" };

        break;

      case "humidity":
        const humidityMapper = new sensorMapper({
          agentId: sensor.agentId,
          agentCode: sensor.agent.code,
          code: sensor.code,
          value: sensor.value,
          status: sensor.status,
          variable: sensor.variable
        });

        if (humidityMapper.value <= 50) {
          //await humidityMapper.updateValue();
          return { ...humidityMapper, notification: "low" };
        } else if (humidityMapper.value >= 80) {
          return { ...humidityMapper, notification: "high" };
        }
        return { ...humidityMapper, notification: "stable" };

        break;
    }
  });

  console.log("TCL: foo", foo);
  return foo;
};

module.exports = {
  sendNotification,
  updatePump
};
