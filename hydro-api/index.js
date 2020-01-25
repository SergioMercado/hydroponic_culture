require('dotenv').config();
const db = require('./src/config/db');
const server = require('./server');

const port = process.env.PORT || 3000;

server.listen(port, async () => {
  console.log(`App running on port ${port}`);
});

db._initSync();
