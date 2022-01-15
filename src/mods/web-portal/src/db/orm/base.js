"use strict";
const { Sequelize, DataTypes, Model, Op } = require("sequelize");

class Base {
  constructor(orm) {
    this.orm = orm;
    this.pk_column = "id";
  }
  setPrimaryKeyColumn(column) {
    this.pk_column = column;
  }
  primaryKeyColumn() {
    return this.pk_column;
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
  async findAll() {
    return this.orm.findAll();
  }
  async count(column = null, alias = "id_count") {
    if (column === null) {
      column = this.primaryKeyColumn();
    }
    return this.model().findAll({
      attributes: [[Sequelize.fn("COUNT", Sequelize.col(column)), alias]],
    });
  }
  model() {
    return this.orm;
  }
}

module.exports = Base;
