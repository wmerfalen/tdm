var express = require("express");
var router = express.Router();

const login = require(__dirname + "/admin/");
login.routes(router);

module.exports = router;
