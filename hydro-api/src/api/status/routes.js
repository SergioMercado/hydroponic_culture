const router = require('express').Router();
const statusController = require('./controller');

router.get('/', statusController.status);

module.exports = router;
