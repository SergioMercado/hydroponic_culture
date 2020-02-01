const router = require('express').Router();
const sensorController = require('./controller');

router.get('/:agentId', sensorController.getSensors);
router.put('/:agentId', sensorController.updateSensorValues);

module.exports = router;
