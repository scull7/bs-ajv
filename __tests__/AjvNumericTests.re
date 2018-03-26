open Jest;

describe("numeric tests", () => {
  let options = Ajv_options.make();
  Ajv_options.allErrors(options, Js.true_);
  Ajv_options.jsonPointers(options, Js.true_);
  Ajv_options.removeAdditional(options, Js.true_);
  let validate = (schema, document) => {
    let validate_ajv =
      switch (Ajv.ajv(options) |> Ajv.compile(schema)) {
      | `Sync(fn) => fn
      | `Async(fn) => failwith("unexpected_async_mode")
      };
    validate_ajv(document);
  };
  let schema =
    Json.Encode.(
      object_([
        ("required", array(string, [|"foo", "bar"|])),
        ("additionalProperties", bool(false)),
        (
          "properties",
          object_([
            (
              "foo",
              object_([
                ("type", string("number")),
                ("minimum", int(0)),
                ("maximum", int(9000)),
              ]),
            ),
            (
              "bar",
              object_([
                ("type", string("number")),
                ("exclusiveMinimum", int(-9000)),
                ("exclusiveMaximum", int(0)),
              ]),
            ),
          ]),
        ),
      ])
    );
  test("respected limits should validate", () => {
    let validData =
      Json.Encode.(object_([("foo", int(0)), ("bar", int(-1))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });
  test("disrespected maxima should fail to validate", () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(9001)), ("bar", int(0))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test("disrespected minima should fail to validate", () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(-1)), ("bar", int(-9001))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test("disrespected exclusive minima should fail to validate", () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(0)), ("bar", int(-9000))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test("disrespected exclusive maxima should fail to validate", () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(0)), ("bar", int(0))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test(
    "disrespected minimum and exclusiveMaximum should report correct fields",
    () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(-1)), ("bar", int(0))]));
    let handler = v => {
      let handlerResult =
        switch (v) {
        | `Valid(_) => [|true, true, true|]
        | `Invalid(err) =>
          let x = Ajv.Error.toDict(err);
          [|Belt_MapString.has(x, "foo"), Belt_MapString.has(x, "bar")|];
        };
      handlerResult;
    };
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toEqual([|true, true|]);
  });
  test(
    "disrespected exclusiveMinimum and maximum should report correct fields",
    () => {
    let invalidData =
      Json.Encode.(object_([("foo", int(9001)), ("bar", int(-9000))]));
    let handler =
      fun
      | `Valid(_) => [||]
      | `Invalid(err) => {
          let x = Ajv.Error.toDict(err);
          [|Belt_MapString.has(x, "foo"), Belt_MapString.has(x, "bar")|];
        };
    validate(schema, invalidData)
    |> handler
    |> Expect.expect
    |> Expect.toEqual([|true, true|]);
  });
});
