open Jest;

describe("$data reference", () => {
  let options = AjvOptions.make();
  AjvOptions.setData(options, Js.true_);

  /* https://www.npmjs.com/package/ajv#data-reference */
  test("smaller/larger example", () => {
    let schema = Json.Encode.(object_([
      ("properties", object_([
        ("smaller", object_([
          ("type", string("number")),
          ("maximum", object_([
            ("$data", string("1/larger"))
          ]))
        ])),
        ("larger", object_([
          ("type", string("number"))
        ]))
      ]))
    ]));

    let validData = Json.Encode.(object_([
      ("smaller", int(5)),
      ("larger", int(7)),
    ]));

    Ajv.ajv(options)
    |> Ajv.validate(`Schema(schema), validData)
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });

  test("example with self reference", () => {
    let schema = Json.Encode.(object_([
      ("additionalProperties", object_([
        ("type", string("string")),
        ("format", object_([
          ("$data", string("0#"))
        ]))
      ]))
    ]));

    let validData = Json.Encode.(object_([
      ("date-time", string("1963-06-19T08:30:06.283185Z")),
      ("email", string("joe.bloggs@example.com")),
    ]));

    Ajv.ajv(options)
    |> Ajv.validate(`Schema(schema), validData)
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  })
});
