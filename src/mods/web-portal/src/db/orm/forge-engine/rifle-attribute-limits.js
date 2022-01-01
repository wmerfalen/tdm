"use strict";
const { Sequelize, DataTypes } = require("sequelize");
let ral = {};
ral.seq = null;
ral.orm = null;
ral.verbose = false;
ral.debug = function (msg) {
  if (ral.verbose) {
    console.info(`rifle-attribute-limits: ${msg}`);
  }
};

ral.sync = async function ({ force = false }) {
  ral.debug("Running ral.sync()");
  if (!ral.seq) {
    throw new Exception("Please call ral.boot first");
  }
  return ral.orm.sync({ force });
};
ral.drop = async function () {
  ral.debug("Running ral.drop()");
  if (!ral.seq) {
    throw new Exception("Please call ral.boot first");
  }
  return ral.orm.drop();
};
ral.make = function () {
  ral.debug("Running ral.make()");
  ral.orm = ral.seq.define(
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
    },
    {
      // Other model options go here
      tableName: "rifle_attribute_limits",
      createdAt: "created_at",
      updatedAt: "updated_at",
    }
  );
  ral.debug("orm object created for ral");
};
ral.fresh = async function () {
  if (!ral.seq) {
    throw new Exception("Please call boot first");
  }
  ral.debug("Running fresh.");
  ral.make();
  await ral.drop();
  await ral.sync();
  ral.debug("Fresh finished. Table dropped and sync'd");
};

ral.boot = async function (sequelize) {
  ral.seq = sequelize;
  ral.make();
};

module.exports = ral;
