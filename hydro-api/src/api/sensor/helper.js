const actuatorMapper = require('../actuator/mapper');
const sensorMapper = require('./mapper');

const updatePump = async ({ sensor, value }) => {
  const pump = new actuatorMapper({
    agentId: sensor.agentId,
    agentCode: sensor.agentCode,
    value
  });

  await pump.updateState();
};

const sendNotification = sensors => {
  return sensors.map(sensor => {
    const newSensor = new sensorMapper({
      agentId: sensor.agentId,
      agentCode: sensor.agent.code,
      code: sensor.code,
      value: sensor.value,
      status: sensor.status,
      variable: sensor.variable
    });

    switch (sensor.variable) {
      case 'waterLevel':
        if (newSensor.value <= 10) {
          return { ...newSensor, notification: 'high' };
        } else if (newSensor.value >= 90) {
          return { ...newSensor, notification: 'low' };
        }
        return { ...newSensor, notification: 'stable' };

        break;

      case 'temperature':
        if (newSensor.value <= 10) {
          return { ...newSensor, notification: 'low' };
        } else if (newSensor.value >= 40) {
          return { ...newSensor, notification: 'high' };
        }
        return { ...newSensor, notification: 'stable' };

        break;

      case 'humidity':
        if (newSensor.value <= 50) {
          return { ...newSensor, notification: 'low' };
        } else if (newSensor.value >= 80) {
          return { ...newSensor, notification: 'high' };
        }
        return { ...newSensor, notification: 'stable' };

        break;
    }
  });
};

module.exports = {
  sendNotification,
  updatePump
};
