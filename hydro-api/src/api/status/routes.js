const router = require('express').Router();
const statusController = require('./controller');

router.get('/status', statusController.status);
router.get('/on', statusController.turnOn);
router.get('/off', statusController.turnOff);

module.exports = router;
