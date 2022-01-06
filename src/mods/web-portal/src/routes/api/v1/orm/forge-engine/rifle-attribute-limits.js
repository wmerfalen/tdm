"use strict";
const { DataTypes, Model } = require("sequelize");
const { prune } = require("@mentoc/utils").array;
const RU = require("$/utils/request");
const { body } = require("express-validator");

module.exports = {
  routes: async function (router, { parent_state }) {
    const ral_orm =
      require("$/db/orm/forge-engine/rifle-attribute-limits").make(
        parent_state.sequelize
      );
    router.get("/rifle-attribute-limits", async function (req, res, next) {
      return res.json(await ral_orm.model().findAll());
    });
    router.get(
      "/rifle-attribute-limits/enumerate/ral_type",
      async function (req, res, next) {
        return res.json(ral_orm.validRifleTypes());
      }
    );
    router.get(
      "/rifle-attribute-limits/enumerate/ral_attribute",
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
        const sniper = await ral_orm.create(data);
        return res.json(sniper);
      }
    );
    router.delete("/rifle-attribute-limits/:id", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
    router.put("/rifle-attribute-limits/:id", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
  },
};
