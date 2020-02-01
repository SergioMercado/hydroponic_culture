const router = require('express').Router();
const routes = require('./src/api');

module.exports = () => {
  router.use('/', routes.status);
  router.use('/sensor', routes.sensor);
  router.use('/actuator', routes.actuator);

  return router;
};
