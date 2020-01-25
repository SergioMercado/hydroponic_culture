const { Model, DataTypes } = require('sequelize');
const sequelize = require('../db');

class Status extends Model {}

Status.init(
  {
    id: {
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true,
      autoIncrement: true
    },
    status: {
      type: DataTypes.STRING,
      defaultValue: 'off',
      require: true
    }
  },
  { sequelize, modelName: 'status' }
);

module.exports = Status;
