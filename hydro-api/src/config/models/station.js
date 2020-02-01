const { Model, DataTypes } = require('sequelize');
const sequelize = require('../db');

class Station extends Model {}

Station.init(
  {
    id: {
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true,
      autoIncrement: true,
      unique: true
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false
    },
    code: {
      type: DataTypes.STRING,
      allowNull: false
    },
    createdAt: {
      type: DataTypes.DATE
    },
    updatedAt: {
      type: DataTypes.DATE,
      onUpdate: DataTypes.DATE
    }
  },
  {
    sequelize,
    modelName: 'station'
  }
);

module.exports = Station;
