const { Sequelize } = require('sequelize');
const {
  DB_NAME,
  DB_USER,
  DB_PASSWORD,
  DB_HOST,
  DB_DIALECT,
  DB_PORT
} = process.env;

const sequelize = new Sequelize(DB_NAME, DB_USER, DB_PASSWORD, {
  host: DB_HOST,
  dialect: DB_DIALECT,
  port: DB_PORT,
  pool: {
    max: 4,
    min: 0,
    acquire: 30000,
    idle: 10000
  },
  define: {
    timestamps: true,
    freezeTableName: true
  }
});

sequelize._initSync = function() {
  this.authenticate()
    .then(() => {
      console.log('Database connected');
    })
    .catch(e => {
      console.log('Database not connected');
    });

  this.sync({ force: false })
    .then(() => {
      console.log('Database syncronized');
    })
    .catch(e => {
      console.log('Database not syncronized');
    });
};

module.exports = sequelize;
