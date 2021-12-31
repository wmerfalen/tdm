"use strict";
const { Sequelize, DataTypes } = require("sequelize");
let ral = {};
ral.seq = null;
ral.sync = async function (){
	if(!ral.seq){
		throw new Exception('cannot sync if ral.seq is not set!');
	}
	ral.seq.
};
ral.boot = async function (sequelize) {
  ral.seq = sequelize;
  const Orm = sequelize.define(
    "rifle_attribute_limits",
    {
      id: {
        type: DataTypes.INTEGER,
        allowNull: false,
        primaryKey: true,
        autoIncrement: true,
      },
      ral_type: {
        type: DataTypes.STRING(16),
        allowNull: false,
      },
      ral_attribute: {
        type: DataTypes.STRING(128),
        allowNull: false,
      },
      ral_low: {
        type: DataTypes.INTEGER,
        allowNull: false,
      },
      ral_high: {
        type: DataTypes.INTEGER,
        allowNull: false,
      },
      ral_overpowered: {
        type: DataTypes.INTEGER,
        allowNull: false,
      },
      ral_start_level: {
        type: DataTypes.INTEGER,
        allowNull: false,
      },
      ral_end_level: {
        type: DataTypes.INTEGER,
        allowNull: false,
      },
      created_at: {
        type: DataTypes.STRING,
        allowNull: false,
      },
    },
    {
      // Other model options go here
      tableName: "rifle_attribute_limits",
    }
  );
};

module.exports = ral;
