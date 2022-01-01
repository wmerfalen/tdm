"use strict";
const { Sequelize, DataTypes, Model } = require("sequelize");
const Base = require(__dirname + "/../base");

class RifleAttributeLimits extends Model {}

function make(sequelize) {
  RifleAttributeLimits.init(
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
    },
    {
      sequelize: sequelize,
      modelName: "rifle_attribute_limits",
      createdAt: "created_at",
      updatedAt: "updated_at",
    }
  );
  return new Base(RifleAttributeLimits);
}
module.exports = {
  make,
};
