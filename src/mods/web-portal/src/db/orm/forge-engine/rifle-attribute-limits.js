"use strict";
const { Sequelize, DataTypes, Model } = require("sequelize");

class RifleAttributeLimits extends Model {
  constructor(sequelize_object) {
    super();
    this.sequelize = sequelize_object;
    this.orm = null;
    this.verbose = true; //false;
    this.make();
  }
  debug(msg) {
    if (this.verbose) {
      console.info(`RifleAttributeLimits: ${msg}`);
    }
  }
  async fresh() {
    if (!this.sequelize) {
      throw new Exception("Please call boot first");
    }
    this.debug("Running fresh.");
    this.make();
    await this.drop();
    await this.sync();
  }

  make() {
    this.debug("Running make()");
    this.orm = this.sequelize.define(
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
    this.debug("orm object created for ral");
  }
}

module.exports = RifleAttributeLimits;
