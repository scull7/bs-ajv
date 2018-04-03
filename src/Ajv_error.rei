type t = { key: string, message: string, };
module RawValidationError:
  {
    module RawParams:
      {
        type t =
            MissingProperty(string)
          | LimitError(int)
          | TypeError(string)
          | MultipleOfError(int)
          | PatternError(string)
          | ContainsError;
        let fromJson: (string, Js.Json.t) => t;
      };
    type t = {
      keyword: string,
      dataPath: string,
      schemaPath: string,
      params: RawParams.t,
      message: string,
      schema: option(string),
      parentSchema: option(string),
    };
    let fromJson: Js.Json.t => t;
    let unescapeJsonPointerRegexp: Js.Re.t;
    let unescapeJsonPointerEscapedSubstr: (string, 'a, 'b) => string;
    let unescapeJsonPointerStr: Js.String.t => Js.String.t;
    let dataPathToFieldNameRegexp: Js.Re.t;
    let dataPathToFieldName: string => Js.String.t;
  };
let toError: RawValidationError.t => t;
let toDict: Js.Json.t => Belt.Map.String.t(string);
let logDict: Belt.Map.String.t('a) => unit;
