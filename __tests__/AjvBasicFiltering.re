open Jest;

describe("data filtering (removeAdditional option)", () => {
  let options = AjvOptions.make();
  AjvOptions.removeAdditional(options, Js.true_);

  /* https://www.npmjs.com/package/ajv#filtering-data */
  test("basic remove additional test", () => {
    let schema = Json.Encode.(object_([
      ("additionalProperties", bool(false)),
      ("properties", object_([
        ("foo", object_([
          ("type", string("number")),
        ])),
        ("bar", object_([
          ("baz", object_([
            ("type", string("string")),
          ])),
        ])),
      ])),
    ]));

    let validData = Json.Encode.(object_([
      ("foo", int(0)),
      ("additional1", int(1)),
      ("bar", object_([
        ("baz", string("abc")),
        ("additional2", int(2)),
      ])),
    ]));

    let validate =
      switch(Ajv.ajv(options) |> Ajv.compile(schema)) {
      | `Sync(fn) => fn
      | `Async(_) => failwith("unexpected_async_mode")
      };

    let handler = fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;

    validate(validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });
});
