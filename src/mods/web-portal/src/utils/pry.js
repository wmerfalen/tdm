"use strict";

function pluck(obj, items, throw_on_missing = false) {
  if (!obj) {
    return undefined;
  }
  let plucked = {};
  for (let key of items) {
    if (typeof obj[key] === "undefined") {
      if (throw_on_missing) {
        throw new Error(`Missing parameter ${key} in body of request`);
      }
    }
    plucked[key] = obj[key];
  }
  return plucked;
}

module.exports = {
  pluck,
};
