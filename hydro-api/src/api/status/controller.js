const fs = require('fs');
const path = require('path');
const { Status } = require('../../config/models');

const status = async (req, res) => {
  try {
    const result = await Status.findOne({ where: { id: 1 } });

    return res.status(200).send(result.status);
  } catch (error) {
    return res.status(500).send(error.message);
  }
};

const turnOn = async (req, res) => {
  try {
    await Status.update({ status: 'on' }, { where: { id: 1 } });

    return res.status(200).send('updated');
  } catch (error) {
    return res.status(500).send(error.message);
  }
};

const turnOff = async (req, res) => {
  try {
    await Status.update({ status: 'off' }, { where: { id: 1 } });

    return res.status(200).send('updated');
  } catch (error) {
    return res.status(500).send(error.message);
  }
};

module.exports = {
  status,
  turnOn,
  turnOff
};
