const objects = [{ "forge-engine": ["rifle-attribute-limits"] }];
module.exports = {
  boot: async function (sequelize) {
    let orm_objects = {};
    for (let obj of objects) {
      for (let key in obj) {
        for (let i = 0; i < obj[key].length; i++) {
          orm_objects[obj[key][i]] = await require(__dirname +
            "/orm/" +
            key +
            "/" +
            obj[key][i]).make(sequelize);
        }
      }
    }
    //console.info({ orm_objects });
    return orm_objects;
  },
};
