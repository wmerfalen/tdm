var express = require('express')
var router = express.Router()

const login = require(__dirname + '/admin/');
login.routes(router);

const ral = require(__dirname + '/forge-engine/rifle-attribute-limits');
ral.routes(router);

module.exports = router
