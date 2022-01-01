const objects = [{ "forge-engine": ["rifle-attribute-limits"] }];
module.exports = {
  boot: async function (sequelize) {
    let orm_objects = {};
    for (let obj of objects) {
      for (let key in obj) {
        for (let i = 0; i < obj[key].length; i++) {
          let orm = require(__dirname + "/orm/" + key + "/" + obj[key][i]);
          orm_objects[orm] = new orm(sequelize);
        }
      }
    }
    return orm_objects;
  },
};
