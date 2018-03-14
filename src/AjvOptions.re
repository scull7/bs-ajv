module Format = {
  type t =
    | Fast
    | Full
    | Ignore
  ;

  let toJson = fun
    | Fast => Js.Json.string("fast")
    | Full => Js.Json.string("full")
    | Ignore => Js.Json.boolean(Js.false_)
  ;
};

module UnknownFormatHandling = {
  type t =
    | Exception /* evaluates to true, the default setting */
    | Ignore(array(string)) /* ignore the formats within the given list */
    | Warn /* Ignore all uknown formats, output a console warning. */
  ;

  let toJson = fun
    | Exception => Js.Json.boolean(Js.true_)
    | Ignore(a) => Js.Json.stringArray(a)
    | Warn => Js.Json.string("ignore")
};

module SchemaId = {
  type t =
    | Draft4
    | Draft6
    | Draft7
    | Any
  ;

  let toJson = fun
    | Draft4 => Js.Json.string("id")
    | Draft6 | Draft7 => Js.Json.string("$id")
    | Any => Js.Json.string("auto")
  ;
};

module RefSetting = {
  type t =
    | Exception
    | Fail
    | Ignore
  ;

  let toJson = fun
    | Exception => Js.Json.boolean(Js.true_)
    | Fail => Js.Json.string("fail")
    | Ignore => Js.Json.string("ignore")
  ;
};

type t('a) = Js.t('a);

let make = (): t('a) => Js.Obj.empty();

/* Validation and Reporting options */
[@bs.set] external setData : (t('a), Js.boolean) => unit = "$data";
[@bs.set] external allErrors : (t('a), Js.boolean) => unit = "";
[@bs.set] external verbose : (t('a), Js.boolean) => unit = "";
[@bs.set] external comment : (t('a), Js.boolean) => unit = "$comment";
[@bs.set] external jsonPointers : (t('a), Js.boolean) => unit = "";
[@bs.set] external uniqueItems : (t('a), Js.boolean) => unit = "";
[@bs.set] external unicode : (t('a), Js.boolean) => unit = "";
/* formats: Js.Json.t,  Use addFormat method. */
[@bs.set] external format : (t('a), Js.Json.t) => unit = "";
[@bs.set] external unknownFormats : (t('a), Js.Json.t) => unit = "";
  /* schemas: Js.Json.t,  Use addSchema method. */
  /* logger: Not sure how to type this yet */

let format = (t, f) => format(t, Format.toJson(f));
let unknownFormats = (t, f) =>
  unknownFormats(t, UnknownFormatHandling.toJson(f));

/* Refernced Schema options */
[@bs.set] external schemaId : (t('a), Js.Json.t) => unit = "";
[@bs.set] external missingRefs : (t('a), Js.Json.t) => unit = "";
[@bs.set] external extendRefs : (t('a), Js.Json.t) => unit = "";

let schemaId = (t, f) => schemaId(t, SchemaId.toJson(f));
let missingRefs = (t, f) => missingRefs(t, RefSetting.toJson(f));
let extendRefs = (t, f) => extendRefs(t, RefSetting.toJson(f));

/* Data Modification options (validated data) */
[@bs.set] external removeAdditional : (t('a), Js.boolean) => unit = "";
[@bs.set] external useDefaults : (t('a), Js.boolean) => unit = "";
[@bs.set] external coerceTypes : (t('a), Js.boolean) => unit = "";

/* Asynchronous Validation options */
/* transpile: Not sure how to type this yet */

/* Advanced Options */
[@bs.set] external meta : (t('a), Js.boolean) => unit = "";
[@bs.set] external validateSchema : (t('a), Js.boolean) => unit = "";
[@bs.set] external addUsedSchema : (t('a), Js.boolean) => unit = "";
[@bs.set] external inlineRefs : (t('a), Js.boolean) => unit = "";
[@bs.set] external passContext : (t('a), Js.boolean) => unit = "";
/* loopRequired: Not sure how to type this yet */
[@bs.set] external ownProperties : (t('a), Js.boolean) => unit = "";
[@bs.set] external multipleOfPrecision : (t('a), Js.boolean) => unit = "";
[@bs.set] external messages : (t('a), Js.boolean) => unit = "";
[@bs.set] external sourceCode : (t('a), Js.boolean) => unit = "";
/* processCode: Not sure how to type this yet */
/* cache: Not sure how to type this yet */
/* serialize: Not sure how to type this yet */
