open Jest;

describe("Ajv Custom Keywords", () => {

  describe("Ajv Docs: Range Keyword", () => {
    let range = AjvKeyword.make();
    AjvKeyword.setType(range, "number");
    AjvKeyword.setCompiler(range, (schema, parentSchema) => {
      let decode = Json.Decode.array(Json.Decode.float);
      let (min, max) = switch (decode(schema)) {
      | [| min, max |] => (min, max)
      | _ => failwith("range only accepts a 2 element array.")
      };

      let parentDecoder = Json.Decode.(field("exclusiveRange", boolean));
      switch (parentDecoder(parentSchema) |> Js.to_bool) {
      | true => ((x) => (x > min && x < max) |> Js.Boolean.to_js_boolean)
      | false => ((x) => (x >= min && x <= max) |> Js.Boolean.to_js_boolean)
      };
    });

    let options = AjvOptions.make();
    let ajv = Ajv.ajv(options) |> Ajv.addKeyword("range", range);

    let schema = Json.Encode.(object_([
      ("range", array(Json.Encode.float, [|2.0, 4.0|])),
      ("exclusiveRange", boolean(Js.true_))
    ]));

    let validate = ajv |> Ajv.compile(schema);
    let handler = fun
      | `Valid => Js.true_
      | `Invalid(_) => Js.false_;

    let genTest = (msg, input, expected) =>
      test(msg, () => {
        validate(Json.Encode.float(input))
        |> handler
        |> Expect.expect
        |> Expect.toBe(expected)
      });

    genTest("2.01 should pass", 2.01, Js.true_);
    genTest("3.99 should pass", 3.99, Js.true_);
    genTest("2.0 should fail", 2.0, Js.false_);
    genTest("4.0 should fail", 4.0, Js.false_);
  });
});
