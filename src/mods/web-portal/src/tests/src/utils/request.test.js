const bod = require(__dirname + "/../../../utils/request").body;

test("RU.body.pluck() plucks various members", () => {
  const req = {
    body: {
      foo: "foo",
      bar: "bar",
      id: 20,
    },
  };
  const plucked = bod.pluck(req, ["foo", "bar", "id"]);
  expect(plucked.foo).toBe(req.body.foo);
  expect(plucked.bar).toBe(req.body.bar);
  expect(plucked.id).toBe(req.body.id);
});

test("RU.body.pluck() throws if user requests it to throw on absent members", () => {
  const req = {
    body: {
      id: 20,
      //name: "john", // will cause throw
    },
  };
  let throw_on_absent = true;
  const t = () => {
    const plucked = bod.pluck(req, ["name"], throw_on_absent);
    return plucked;
  };
  expect(t).toThrow(Error);
  throw_on_absent = false;
  expect(t().name).toBe(undefined);
});
