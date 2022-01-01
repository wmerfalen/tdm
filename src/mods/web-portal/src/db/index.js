const objects = [{ "forge-engine": ["rifle-attribute-limits"] }];
module.exports = {
  boot: async function (sequelize) {
    for (let obj of objects) {
      for (let key in obj) {
        for (let i = 0; i < obj[key].length; i++) {
          let orm = require(__dirname + "/orm/" + key + "/" + obj[key][i]);
          const obj = new orm(sequelize);
        }
      }
    }
  },
};
