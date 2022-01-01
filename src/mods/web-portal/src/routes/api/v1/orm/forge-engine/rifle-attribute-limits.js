"use strict";

module.exports = {
  routes: async function (router) {
    router.get("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
    router.post("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
    router.delete("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
    router.put("/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
  },
};
