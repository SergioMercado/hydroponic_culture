const router = require('express').Router();
const sensorController = require('./controller');

router.get('/:agentId', sensorController.getSensors);

module.exports = router;
