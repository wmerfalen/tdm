"use strict";
const { DataTypes, Model } = require("sequelize");
const { prune } = require("@mentoc/utils").array;
const RU = require(__dirname + "/../../../../../utils/request");
const { body } = require("express-validator");

module.exports = {
  routes: async function (router, { orm_dir, parent_state }) {
    const ral_orm = require(orm_dir +
      "/forge-engine/rifle-attribute-limits").make(parent_state.sequelize);
    router.get("/rifle-attribute-limits", async function (req, res, next) {
      return res.json(await ral_orm.model().findAll());
    });
    router.get(
      "/rifle-attribute-limits/valid/ral_type",
      async function (req, res, next) {
        return res.json(ral_orm.validRifleTypes());
      }
    );
    router.get(
      "/rifle-attribute-limits/valid/ral_attribute",
      async function (req, res, next) {
        return res.json(ral_orm.validAttributes());
      }
    );
    router.get("/rifle-attribute-limits/:id", async function (req, res, next) {
      const row = await ral_orm.findById(req.params.id).catch(console.error);
      return res.json(row);
    });
    router.post(
      "/rifle-attribute-limits",
      body("ral_type").custom(
        (value) => ral_orm.validRifleTypes().indexOf(value) !== -1
      ),
      async function (req, res, next) {
        const data = RU.body.pluck(
          req,
          [
            "ral_type",
            "ral_attribute",
            "ral_low",
            "ral_high",
            "ral_overpowered",
            "ral_start_level",
            "ral_end_level",
          ],
          true
        );
        const sniper = await ral_orm.create({
          ral_type: "SNIPER",
          ral_attribute: "effective_firing_range",
          ral_low: 1,
          ral_high: 10,
          ral_overpowered: 30,
          ral_start_level: 1,
          ral_end_level: 10,
        });
        return res.json(sniper);
      }
    );
    router.delete("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
    router.put("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
  },
};
