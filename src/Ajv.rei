module Error = Ajv_error;

module Options = Ajv_options;

module Keyword = Ajv_keyword;

type t;

type options;

type schema = Js.Json.t;

type format;

type keyword('a) = Keyword.t('a);

type errors = Js.Json.t;

type validator =
  (
    Js.Json.t /* data */,
    Js.Json.t /* dataPath */,
    Js.Json.t /* parentData */,
    Js.Json.t /* parentDataProperty */,
    Js.Json.t
  ) => /* rootData */
  Js.Json.t;

[@bs.module] [@bs.new] external ajv : Options.t('a) => t = "ajv";

[@bs.send.pipe: t] external compile : schema => validator = "";

[@bs.get] external getErrors : validator => errors = "errors";

let compile:
  (schema, t) =>
  [>
    | `Async(
        Js.Json.t =>
        Js.Promise.t([> | `Invalid(errors) | `Valid(Js.Json.t)]),
      )
    | `Sync(Js.Json.t => [> | `Invalid(errors) | `Valid(Js.Json.t)])
  ];

/* @TODO: compileAsync */
[@bs.send.pipe: t]
external validate :
  (
    [@bs.unwrap] [ | `Schema(schema) | `Key(string) | `Ref(string)],
    Js.Json.t
  ) =>
  Js.boolean =
  "";

[@bs.send.pipe: t]
external addSchema :
  (
  [@bs.unwrap]
  [ | `Schema(schema) | `SchemaArray(array(schema)) | `SchemaKey(string)]
  ) =>
  t =
  "";

[@bs.send.pipe: t]
external addMetaSchema :
  (
  [@bs.unwrap]
  [ | `Schema(schema) | `SchemaArray(array(schema)) | `SchemaKey(string)]
  ) =>
  t =
  "";

[@bs.send.pipe: t] external validateSchema : schema => bool = "";

[@bs.send.pipe: t] external getSchema : string => validator = "";

[@bs.send.pipe: t]
external removeSchema :
  (
  [@bs.unwrap]
  [
    | `Schema(schema)
    | `SchemaKey(string)
    | `SchemaRef(string)
    | `SchemaPattern(Js.Re.t)
  ]
  ) =>
  t =
  "";

[@bs.send.pipe: t]
external addFormat :
  (
    string,
    [@bs.unwrap] [
      | `String(string)
      | `Pattern(Js.Re.t)
      | `Validator('a => bool)
      | `Definition(format)
    ]
  ) =>
  t =
  "";

let getKeyword:
  (string, t) => [> | `Exists | `Keyword(Js_dict.t(Js.Json.t)) | `Unknown];

/* function bindings for javascript consumers */
/* XXX what does this type mean? is this a closed object with no fields??? */
let makeOptions: unit => Ajv_options.t({..});

let makeAjv: Options.t('a) => t;

let compileSync:
  (schema, t, Js.Json.t) => [> | `Invalid(errors) | `Valid(Js.Json.t)];

let compileAsync:
  (schema, t, Js.Json.t) =>
  Js.Promise.t([> | `Invalid(errors) | `Valid(Js.Json.t)]);

let makeKeyword: unit => Ajv_keyword.t({..});

let setValidator:
  (Ajv_keyword.t('a), (Js.Json.t, Js.Json.t, Js.Json.t) => Js.boolean) => unit;

let setAsyncValidator:
  (
    Ajv_keyword.t('a),
    (Js.Json.t, Js.Json.t, Js.Json.t) => Js.Promise.t(Js.boolean)
  ) =>
  unit;

let addKeyword: (string, keyword('a), t) => t;

let setKeywordType: (Ajv_keyword.t('a), string) => unit;

let setKeywordIsAsync: (Ajv_keyword.t('a), Js.boolean) => unit;

let setOptionAllErrors: (Ajv_options.t('a), Js.boolean) => unit;

let setOptionJsonPointers: (Ajv_options.t('a), Js.boolean) => unit;

let setOptionRemoveAdditional: (Ajv_options.t('a), Js.boolean) => unit;
