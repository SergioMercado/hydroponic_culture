const fs = require('fs');
const path = require('path');
const { Status } = require('../../config/models');

const status = async (req, res) => {
  try {
    return res.status(200).send({ message: 'Hydroponic Cultive API working' });
  } catch (error) {
    return res.status(500).send(error.message);
  }
};

module.exports = {
  status
};
