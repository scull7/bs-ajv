open Jest;

describe("json pointer escaping tests", () => {
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
        ("required", array(string, [|"foo/bar~baz"|])),
        (
          "properties",
          object_([
            ("foo/bar~baz", object_([("type", string("number"))])),
          ]),
        ),
      ])
    );
  test(
    "validation errors about field names containing special chars should be converted correctly",
    () => {
      let invalidData =
        Json.Encode.(object_([("foo/bar~baz", string("invalid!"))]));
      let handler = v => {
        let handlerResult =
          switch (v) {
          | `Valid(_) => Js.false_
          | `Invalid(err) =>
            let x = Ajv.Error.toDict(err);
            Js.log(x);
            Belt_MapString.has(x, "foo/bar~baz") ? Js.true_ : Js.false_;
          };
        handlerResult;
      };
      validate(schema, invalidData)
      |> handler
      |> Expect.expect
      |> Expect.toBe(Js.true_);
    },
  );
});
