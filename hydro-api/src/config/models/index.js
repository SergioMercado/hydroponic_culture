const Sensor = require('./sensor');
const Station = require('./station');
const Agent = require('./agent');
const Actuator = require('./actuator');

Agent.belongsTo(Station, { foreignKey: 'stationId' });
Sensor.belongsTo(Agent, { foreignKey: 'agentId' });
Actuator.belongsTo(Agent, { foreignKey: 'agentId' });

module.exports = {
  Sensor,
  Station,
  Agent,
  Actuator
};
