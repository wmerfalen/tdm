var express = require("express");
var router = express.Router();
const ORM_DIR = __dirname + "/../../../../db/orm/";

const includes = ["/forge-engine/rifle-attribute-limits"];

module.exports = function (parent_state) {
  for (let i of includes) {
    const r = require(__dirname + i);
    r.routes(router, { orm_dir: ORM_DIR, parent_state });
  }
  return router;
};
