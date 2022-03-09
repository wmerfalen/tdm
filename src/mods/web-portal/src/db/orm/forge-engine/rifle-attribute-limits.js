"use strict";
const { Sequelize, DataTypes, Model } = require("sequelize");
const Base = require("$/db/orm/base");

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
  const obj = new Base(RifleAttributeLimits);
  obj.validRifleTypes = () => [
    "SNIPER",
    "PISTOL",
    "HANDGUN",
    "MACHINE_PISTOL",
    "SUB_MACHINE_GUN",
    "LIGHT_MACHINE_GUN",
    "SHOTGUN",
    "ASSAULT_RIFLE",
  ];
  obj.validAttributes = () => [
    "ammo_max",
    "ammo_type",
    "chance_to_injure",
    "clip_size",
    "cooldown_between_shots",
    "critical_chance",
    "critical_range",
    "base_damage",
    "disorient_amount",
    "headshot_bonus",
    "max_range",
    "range_multiplier",
    "reload_time",
    "rounds_per_minute",
    "muzzle_velocity",
    "effective_firing_range",
    "damage_dice_count",
    "damage_dice_sides",
    "incendiary_damage",
    "explosive_damage",
    "shrapnel_damage",
    "corrosive_damage",
    "cryogenic_damage",
    "radioactive_damage",
    "emp_damage",
    "shock_damage",
    "anti_matter_damage",
    "stat_strength",
    "stat_intelligence",
    "stat_wisdom",
    "stat_dexterity",
    "stat_constitution",
    "stat_electronics",
    "stat_armor",
    "stat_marksmanship",
    "stat_sniping",
    "stat_demolitions",
    "stat_chemistry",
    "stat_weapon_handling",
    "stat_strategy",
    "stat_medical",
  ];
  return obj;
}
module.exports = {
  make,
};
