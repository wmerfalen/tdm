"use strict";
const pluck = require(__dirname + "/pry");

const req = {};
req.set_body = function (b) {
  req.body = b;
};

/**
 * Body is a namespace under the req parent utility object.
 * It should hold all functionality required to interact with
 * express JS' Request.body property.
 */
req.body = {
  body: null,
  set: (b) => {
    req.body.body = b;
  },
  issues: [],
  pluck: (r, items, throw_on_missing = false) => {
    if (!r && req.body.body) {
      r = req.body.body;
    }
    if (!r && !req.body.body) {
      throw new Error(`Missing request object. Did you call req.body.set?`);
    }
    return pry.pluck(r, items, throw_on_missing);
  },
};

module.exports = req;
