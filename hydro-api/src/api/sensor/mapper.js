const Storage = require("./storage");

class Sensor {
  constructor(params) {
    this.agentCode = params.agentCode;
    this.agentId = params.agentId;
    this.code = params.code;
    this.value = params.value;
    this.status = params.status;
    this.variable = params.variable;
  }

  async findSensors() {
    const { agentCode, agentId } = this;
    return await Storage.findAll({ agentCode, agentId });
  }

  async updateValue() {
    const { agentId, agentCode, code, value, status } = this;
    return await Storage.updateValue({
      agentId,
      agentCode,
      code,
      value,
      status
    });
  }
}

module.exports = Sensor;
