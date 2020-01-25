const express = require('express');
const bodyParser = require('body-parser');
const morgan = require('morgan');
const cors = require('cors');
const app = express();
const redirectToHTTPS = require('express-http-to-https').redirectToHTTPS;

const routes = require('./routes')();

// middlewares
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cors());
app.use(morgan('dev'));

//Redirect to HTTPS
app.use(redirectToHTTPS([/localhost:(\d{4})/], [], 301));

// routes
app.use('/', routes);

module.exports = app;
