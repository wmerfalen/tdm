var express = require("express");
var router = express.Router();

router.get("/yo", function (req, res, next) {
  res.send("heh");
});

/* GET users listing. */
router.get("/foo/:id", function (req, res, next) {
  res.send(`respond with a resource: ${req.param.id}`);
});

module.exports = router;
