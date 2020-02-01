const { Model, DataTypes } = require('sequelize');
const sequelize = require('../db');

class Agent extends Model {}

Agent.init(
  {
    id: {
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true,
      autoIncrement: true
    },
    stationId: {
      type: DataTypes.INTEGER,
      allowNull: false,
      foreignKey: true
    },
    code: {
      type: DataTypes.STRING,
      allowNull: false,
      unique: true
    },
    name: {
      type: DataTypes.STRING,
      allowNull: false,
      unique: true
    },
    status: {
      type: DataTypes.BOOLEAN,
      defaultValue: false
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
    modelName: 'agent'
  }
);

module.exports = Agent;
