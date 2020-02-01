const router = require('express').Router();
const actuatorController = require('./controller');

router.get('/:agentId', actuatorController.getActuator);
router.post('/', actuatorController.updateActuatorState);

module.exports = router;
