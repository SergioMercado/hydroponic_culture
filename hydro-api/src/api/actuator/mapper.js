const Storage = require('./storage');

class Actuator {
  constructor(params) {
    this.agentCode = params.agentCode;
    this.agentId = params.agentId;
    this.value = params.value;
  }

  async findActuator() {
    const { agentCode, agentId } = this;
    return await Storage.findOne({ agentCode, agentId });
  }

  async updateState() {
    const { agentId, agentCode, value } = this;
    return await Storage.updateState({ agentId, agentCode, value });
  }
}

module.exports = Actuator;
