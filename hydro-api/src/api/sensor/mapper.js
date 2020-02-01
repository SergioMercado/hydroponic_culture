const Storage = require('./storage');

class Sensor {
  constructor(params) {
    this.agentCode = params.agentCode;
    this.agentId = params.agentId;
  }

  async findSensors() {
    const { agentCode, agentId } = this;
    return await Storage.findAll({ agentCode, agentId });
  }
}

module.exports = Sensor;
