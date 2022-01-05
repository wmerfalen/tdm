"use strict";

const req = {};
req.set_body = function (b) {
  req.body = b;
};
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
    let plucked = {};
    for (let key of items) {
      if (typeof r.body[key] === "undefined") {
        if (throw_on_missing) {
          throw new Error(`Missing parameter ${key} in body of request`);
        }
      }
      plucked[key] = r.body[key];
    }
    return plucked;
  },
};

module.exports = req;
