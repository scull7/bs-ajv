open Jest;

describe("string tests", () => {
  let options = Ajv_options.make();
  Ajv_options.allErrors(options, Js.true_);
  Ajv_options.jsonPointers(options, Js.true_);
  Ajv_options.removeAdditional(options, Js.true_);
  let validate = (schema, document) => {
    let validate_ajv =
      switch (Ajv.ajv(options) |> Ajv.compile(schema)) {
      | `Sync(fn) => fn
      | `Async(_) => failwith("unexpected_async_mode")
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
                ("type", string("string")),
                ("maxLength", int(5)),
              ]),
            ),
            (
              "bar",
              object_([
                ("type", string("string")),
                ("minLength", int(1)),
              ]),
            ),
          ]),
        ),
      ])
    );
  test("respected limits should validate", () => {
    let validData =
      Json.Encode.(object_([("foo", string("hello")), ("bar", string("eugenia"))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });
  test("disrespected limits should fail to validate", () => {
    let validData =
      Json.Encode.(object_([("foo", string("greetings")), ("bar", string("eugenia"))]));
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test("disrespected limits should report invalid fields", () => {
    let validData =
      Json.Encode.(object_([("foo", string("hello")), ("bar", string(""))]));
    let handler =
      fun
      | `Valid(_) => [||]
      | `Invalid(err) => {
          let x = Ajv.Error.toDict(err);
          [|Belt_MapString.has(x, "foo"), Belt_MapString.has(x, "bar")|];
        };
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toEqual([|false, true|]);
  });
});
