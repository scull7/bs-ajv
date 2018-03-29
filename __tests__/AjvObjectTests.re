open Jest;

describe("object tests", () => {
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
        ("type", string("object")),
        ("required", array(string, [|"messages"|])),
        (
          "properties",
          object_([
            (
              "messages",
              object_([
                ("type", string("object")),
                ("additionalProperties", bool(false)),
                ("maxProperties", int(3)),
                ("minProperties", int(1)),
                (
                  "patternProperties",
                  object_([
                    (
                      "Dr\\. .*",
                      object_([
                        ("type", string("string")),
                        ("pattern", string("^.*\\?$")),
                      ]),
                    ),
                    (
                      "Cpt\\. .*",
                      object_([
                        ("type", string("string")),
                        ("pattern", string("^.*!$")),
                      ]),
                    ),
                    (
                      "Lt\\. Cmdr.\\. .*",
                      object_([
                        ("type", string("string")),
                        ("pattern", string("^.*\\.$")),
                      ]),
                    ),
                    (
                      "Act\\. Ens\\. .*",
                      object_([
                        ("type", string("string")),
                        ("pattern", string("^$")),
                      ]),
                    ),
                  ]),
                ),
              ]),
            ),
          ]),
        ),
      ])
    );
  test("valid document should validate", () => {
    let validData =
      Json.Encode.(
        object_([
          (
            "messages",
            object_([
              ("Dr. Beverly Crusher", string("How are you feeling?")),
              (
                "Cpt. Jean-Luc Picard",
                string("No children allowed on the bridge!"),
              ),
              ("Lt. Cmdr. Data", string("I am an android.")),
            ]),
          ),
        ])
      );
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });
  test("invalid document should validate", () => {
    let validData =
      Json.Encode.(
        object_([
          (
            "messages",
            object_([
              ("Dr. Beverly Crusher", string("How are you feeling!")),
              (
                "Cpt. Jean-Luc Picard",
                string("No children allowed on the bridge?"),
              ),
              ("Lt. Cmdr. Data", string("I am an android!")),
              ("Act. Ens. Wesley Crusher", string("Hi, I'm Wesley!")),
            ]),
          ),
        ])
      );
    let handler =
      fun
      | `Valid(_) => Js.true_
      | `Invalid(_) => Js.false_;
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.false_);
  });
  test(
    "additional property wrt patternProperties keyword should report property name wrt parent object",
    () => {
      let validData =
        Json.Encode.(
          object_([
            (
              "messages",
              object_([
                ("Dr. Beverly Crusher", string("How are you feeling!")),
                (
                  "Cpt. Jean-Luc Picard",
                  string("No children allowed on the bridge?"),
                ),
                ("Lt. Cmdr. Data", string("I am an android!")),
                ("Act. Ens. Wesley Crusher", string("Hi, I'm Wesley!")),
              ]),
            ),
          ])
        );
      let handler = v => {
        let handlerResult =
          switch (v) {
          | `Valid(_) => Js.false_
          | `Invalid(err) =>
            let x = Ajv.Error.toDict(err);
            Belt_MapString.has(x, "messages") ? Js.true_ : Js.false_;
          };
        handlerResult;
      };
      validate(schema, validData)
      |> handler
      |> Expect.expect
      |> Expect.toBe(Js.true_);
    },
  );
  test(
    "additional property wrt maxProperties should report parent property name wrt parent object",
    () => {
      let validData =
        Json.Encode.(
          object_([
            (
              "messages",
              object_([
                ("Cpt. Picard", string("Make it so!")),
                ("Cpt. Janeway", string("Are we there yet?!")),
                (
                  "Cpt. Beverly Crusher",
                  string("It's the future I'm a captain now!"),
                ),
                (
                  "Cpt. Wesley Crusher",
                  string(
                    "I'm not a captain I'm some kind of supernatural being!",
                  ),
                ),
              ]),
            ),
          ])
        );
      let handler = v => {
        let handlerResult =
          switch (v) {
          | `Valid(_) => Js.false_
          | `Invalid(err) =>
            let x = Ajv.Error.toDict(err);
            Belt_MapString.has(x, "messages") ? Js.true_ : Js.false_;
          };
        handlerResult;
      };
      validate(schema, validData)
      |> handler
      |> Expect.expect
      |> Expect.toBe(Js.true_);
    },
  );
  test("disrespected minProperties should report invalid field name", () => {
    let validData = Json.Encode.(object_([("messages", object_([]))]));
    let handler = v => {
      let handlerResult =
        switch (v) {
        | `Valid(_) => Js.false_
        | `Invalid(err) =>
          let x = Ajv.Error.toDict(err);
          Belt_MapString.has(x, "messages") ? Js.true_ : Js.false_;
        };
      handlerResult;
    };
    validate(schema, validData)
    |> handler
    |> Expect.expect
    |> Expect.toBe(Js.true_);
  });
});
