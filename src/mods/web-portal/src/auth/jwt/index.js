"use strict";
const jwt = require("jsonwebtoken");

let obj = {};
obj.generate_user_token = function (username) {
  return jwt.sign(username, process.env.TOKEN_SECRET, { expiresIn: "1800s" });
};
obj.boot = async function (sequelize) {};

obj.authenticate_middleware = function (req, res, next) {
  const authHeader = req.headers["authorization"];
  const token = authHeader && authHeader.split(" ")[1];

  if (token == null) return res.sendStatus(401);

  jwt.verify(token, process.env.TOKEN_SECRET, (err, user) => {
    console.log(err);

    if (err) return res.sendStatus(403);

    req.user = user;

    next();
  });
};

module.exports = obj;
