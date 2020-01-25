const router = require('express').Router();
const routes = require('./src/api');

module.exports = () => {
  router.use('/', routes.status);

  return router;
};
