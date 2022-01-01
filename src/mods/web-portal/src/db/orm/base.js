"use strict";
const { Sequelize, DataTypes, Model, Op } = require("sequelize");

class Base {
  constructor(orm) {
    this.orm = orm;
  }
  async create(properties) {
    const obj = this.orm.build(properties);
    return obj.save();
  }

  async findOne(properties = null) {
    this.orm;
  }

  async findById(id) {
    return this.orm.findByPk(id);
  }
  model() {
    return this.orm;
  }
}

module.exports = Base;
