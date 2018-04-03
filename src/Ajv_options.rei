module Format: {
  type t =
    | Fast
    | Full
    | Ignore;
  let toJson: t => Js.Json.t;
};

module UnknownFormatHandling: {
  type t =
    | Exception
    | Ignore(array(string))
    | Warn;
  let toJson: t => Js.Json.t;
};

module SchemaId: {
  type t =
    | Draft4
    | Draft6
    | Draft7
    | Any;
  let toJson: t => Js.Json.t;
};

module RefSetting: {
  type t =
    | Exception
    | Fail
    | Ignore;
  let toJson: t => Js.Json.t;
};

type t('a) = Js.t('a);

let make: unit => t({..});

[@bs.set] external setData : (t('a), Js.boolean) => unit = "$data";

[@bs.set] external allErrors : (t('a), Js.boolean) => unit = "";

[@bs.set] external verbose : (t('a), Js.boolean) => unit = "";

[@bs.set] external comment : (t('a), Js.boolean) => unit = "$comment";

[@bs.set] external jsonPointers : (t('a), Js.boolean) => unit = "";

[@bs.set] external uniqueItems : (t('a), Js.boolean) => unit = "";

[@bs.set] external unicode : (t('a), Js.boolean) => unit = "";

let format: (t('a), Format.t) => unit;

let unknownFormats: (t('a), UnknownFormatHandling.t) => unit;

let schemaId: (t('a), SchemaId.t) => unit;

let missingRefs: (t('a), RefSetting.t) => unit;

let extendRefs: (t('a), RefSetting.t) => unit;

[@bs.set] external removeAdditional : (t('a), Js.boolean) => unit = "";

[@bs.set] external useDefaults : (t('a), Js.boolean) => unit = "";

[@bs.set] external coerceTypes : (t('a), Js.boolean) => unit = "";

[@bs.set] external meta : (t('a), Js.boolean) => unit = "";

[@bs.set] external validateSchema : (t('a), Js.boolean) => unit = "";

[@bs.set] external addUsedSchema : (t('a), Js.boolean) => unit = "";

[@bs.set] external inlineRefs : (t('a), Js.boolean) => unit = "";

[@bs.set] external passContext : (t('a), Js.boolean) => unit = "";

[@bs.set] external ownProperties : (t('a), Js.boolean) => unit = "";

[@bs.set] external multipleOfPrecision : (t('a), Js.boolean) => unit = "";

[@bs.set] external messages : (t('a), Js.boolean) => unit = "";

[@bs.set] external sourceCode : (t('a), Js.boolean) => unit = "";
[@bs.set] external setData : (t('a), Js.boolean) => unit = "$data";

[@bs.set] external allErrors : (t('a), Js.boolean) => unit = "";

[@bs.set] external verbose : (t('a), Js.boolean) => unit = "";

[@bs.set] external comment : (t('a), Js.boolean) => unit = "$comment";

[@bs.set] external jsonPointers : (t('a), Js.boolean) => unit = "";

[@bs.set] external uniqueItems : (t('a), Js.boolean) => unit = "";

[@bs.set] external unicode : (t('a), Js.boolean) => unit = "";

let format: (t('a), Format.t) => unit;

let unknownFormats: (t('a), UnknownFormatHandling.t) => unit;

let schemaId: (t('a), SchemaId.t) => unit;

let missingRefs: (t('a), RefSetting.t) => unit;

let extendRefs: (t('a), RefSetting.t) => unit;

[@bs.set] external removeAdditional : (t('a), Js.boolean) => unit = "";

[@bs.set] external useDefaults : (t('a), Js.boolean) => unit = "";

[@bs.set] external coerceTypes : (t('a), Js.boolean) => unit = "";

[@bs.set] external meta : (t('a), Js.boolean) => unit = "";

[@bs.set] external validateSchema : (t('a), Js.boolean) => unit = "";

[@bs.set] external addUsedSchema : (t('a), Js.boolean) => unit = "";

[@bs.set] external inlineRefs : (t('a), Js.boolean) => unit = "";

[@bs.set] external passContext : (t('a), Js.boolean) => unit = "";

[@bs.set] external ownProperties : (t('a), Js.boolean) => unit = "";

[@bs.set] external multipleOfPrecision : (t('a), Js.boolean) => unit = "";

[@bs.set] external messages : (t('a), Js.boolean) => unit = "";

[@bs.set] external sourceCode : (t('a), Js.boolean) => unit = "";
