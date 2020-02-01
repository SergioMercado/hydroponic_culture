const Storage = require('./storage');

class Sensor {
  constructor(params) {
    this.agentCode = params.agentCode;
    this.agentId = params.agentId;
    this.code = params.code;
    this.value = params.value;
  }

  async findSensors() {
    const { agentCode, agentId } = this;
    return await Storage.findAll({ agentCode, agentId });
  }

  async updateValue() {
    const { agentId, agentCode, code, value } = this;
    return await Storage.updateValue({ agentId, agentCode, code, value });
  }
}

module.exports = Sensor;
