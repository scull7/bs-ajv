type t;
type options;
type schema = Js.Json.t;
type format;
type keyword('a) = AjvKeyword.t('a);
type errors;
type validator = (
  Js.Json.t /* data */,
  Js.Json.t /* dataPath */,
  Js.Json.t /* parentData */,
  Js.Json.t /* parentDataProperty */,
  Js.Json.t /* rootData */
) => Js.Json.t;

[@bs.module][@bs.new] external ajv : AjvOptions.t('a) => t = "ajv";
[@bs.send.pipe : t] external compile : schema => validator = "";

[@bs.get] external getErrors : validator => errors = "errors";

let compile = (schema, t) => {
  let v = t |> compile(schema);
  let null = Js.Json.null;
  let isAsync =
    schema
    |> Json.Decode.(optional(field("$async", boolean)))
    |> Js.Option.getWithDefault(Js.false_)
    |> Js.to_bool;

  let run = json => v(json, null, null, null, null);

  let classify = json =>
    switch(json |> Js.Json.classify) {
    | Js.Json.JSONObject(_) => `Valid(json)
    | Js.Json.JSONTrue => `Valid(json)
    | Js.Json.JSONFalse => `Invalid(getErrors(v))
    | _ => failwith("invalid_validation_result")
    };

  let sync = json => json |> run |> classify;

  let async = json => Js.Promise.(
    resolve(json)
    |> then_(json => json |> run |> resolve)
    |> then_(res => res |> classify |> resolve)
  );

  isAsync ? `Async(async) : `Sync(sync)
};


/* @TODO: compileAsync */

[@bs.send.pipe : t] external validate :
  ([@bs.unwrap][
  | `Schema(schema)
  | `Key(string)
  | `Ref(string)
  ]) =>
  Js.Json.t =>
  Js.boolean = "";

[@bs.send.pipe : t] external addSchema :
  ([@bs.unwrap][
  | `Schema(schema)
  | `SchemaArray(array(schema))
  | `SchemaKey(string)
  ]) => t = "";

[@bs.send.pipe : t] external addMetaSchema :
  ([@bs.unwrap][
  | `Schema(schema)
  | `SchemaArray(array(schema))
  | `SchemaKey(string)
  ]) => t = "";

[@bs.send.pipe : t] external validateSchema : schema => bool = "";

[@bs.send.pipe : t] external getSchema : string => validator = "";

[@bs.send.pipe : t] external removeSchema :
  ([@bs.unwrap][
  | `Schema(schema)
  | `SchemaKey(string)
  | `SchemaRef(string)
  | `SchemaPattern(Js.Re.t)
  ]) => t = "";

[@bs.send.pipe : t] external addFormat :
  string =>
  ([@bs.unwrap][
  | `String(string)
  | `Pattern(Js.Re.t)
  | `Validator(('a => bool))
  | `Definition(format)
  ]) => t = "";

[@bs.send.pipe : t] external addKeyword : string => keyword('a) => t = "";

[@bs.send.pipe : t] external getKeyword : string => Js.Json.t = "";

[@bs.send.pipe : t] external removeKeyword : string => t = "";

[@bs.send.pipe : t] external errorsText : array(errors) => string = "";

let getKeyword = (name, ajv) =>
  switch (ajv |> getKeyword(name) |> Js.Json.classify) {
  | Js.Json.JSONObject(o) => `Keyword(o)
  | Js.Json.JSONTrue => `Exists
  | Js.Json.JSONFalse => `Unknown
  | _ => failwith("unexpected_keyword_response")
  };
