"use strict";
module.exports = {
  routes: async function (router) {
    router.get("/api/v1/rifle-attribute-limits", function (req, res, next) {
      res.json({ heh: "you bet" });
    });
  },
};
