/*
  https://github.com/epoberezkin/ajv/blob/master/CUSTOM.md
*/

type t('a) = Js.t('a);

let make = (): t('a) => Js.Obj.empty();

/*
  `type`
  Optional string or array of strings with data types that the
  keyword applies to.  If not present, the keyword will apply to all types.
*/
[@bs.set] external setType : (t('a), string) => unit = "type";

/*
  `validate`
  The validation function.
*/
[@bs.set] external setValidator :
  (t('a), (Js.Json.t, Js.Json.t, Js.Json.t) => Js.boolean) => unit = "validate";

[@bs.set] external setAsyncValidator :
  (
    t('a),
    ((Js.Json.t, Js.Json.t, Js.Json.t) => Js.Promise.t(Js.boolean))
  ) => unit = "validate";

/*
  _NOTE:_ `compile`, `macro` and `inline` are mutually exclusive, only one
  should be used at a time.  `validate` can be used separately or in
  addition to them to support $data reference.
*/

/*
  `compile`
  The compiling function.
*/
[@bs.set] external setCompiler :
  (
    t('a), (Js.Json.t, Js.Json.t) => (('b) => Js.boolean)
  ) => unit = "compile";

[@bs.set] external setAsyncCompiler :
  (
    t('a), (Js.Json.t, Js.Json.t) => (('b) => Js.Promise.t(Js.boolean))
  ) => unit = "compile";

/*
  `macro`
  The macro function.
*/
[@bs.set] external setMacro :
  (t('a), (Js.Json.t, Js.Json.t) => Js.Json.t) => unit = "macro";

/*
  `inline`
  Compiling function that returns code (as string)
*/
[@bs.set] external setInline :
  (t('a), (Js.Json.t, Js.Json.t, Js.Json.t) => Js.Json.t) => unit = "inline";

/*
  `metaSchema`
  An optional meta-schema for keyword schema.
*/
[@bs.set] external setMetaSchema :
  (t('a),
  [@bs.unwrap][
  | `Ref(string)
  | `Inline(Js.Json.t)
  ]
) => unit = "metaSchema";

/* `modifying` (boolean)
  _MUST_ be passed if the new keyword modifies data.
*/
[@bs.set] external setModifying : (t('a), Js.boolean) => unit = "modifying";

/*
  `valid` (boolean)
  Pre-define the validation result.  The result returned from the Validation
  function will be ignored.  This optoin cannot be used with macro keywords.
*/
[@bs.set] external setValidationResult : (t('a), Js.boolean) => unit = "valid";

/*
  `$data` (boolean)
  An optional boolean value to support $data reference as the value of
  the custom keyword.  The reference will be resolved at validation time.
  If the keyword has `meta-schema` it would be extended to allow $data
  and it will be used to validate the resolved value.  Supporting $data
  reference requires that keyword has a validating function (as the only
  option or in addition to compile, macro or inline functions).
*/
[@bs.set] external setHasDataReference : (t('a), Js.boolean) => unit = "$data";

/*
  `async` (boolean)
  An optional true value if the validation function is asynchronous
  (whether it is compiled or passed in validate property);  in this case
  it should return a promise that resolves with a value (bool).  This
  optoin is ignored in case of `macro` or `inline` keywords.
*/
[@bs.set] external setIsAsync : (t('a), Js.boolean) => unit = "async";

/*
  `errors` (boolean)
  An optional boolean indicating whether keyword returns errors.  If this
  property is not set Ajv will determine if the errors were set in case
  of failed validation.
*/
