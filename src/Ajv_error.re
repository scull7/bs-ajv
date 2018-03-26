type t = {
  key: string,
  message: string,
};

module RawValidationError = {
  module RawParams = {
    type t =
      | MissingProperty(string)
      | LimitError(int)
      | TypeError(string)
      | MultipleOfError(int);
    let fromJson = (keyword, json) =>
      switch (keyword) {
      | "required" =>
        MissingProperty(Json.Decode.(field("missingProperty", string, json)))
      | "type" => TypeError(Json.Decode.(field("type", string, json)))
      | "minimum" => LimitError(Json.Decode.(field("limit", int, json)))
      | "maximum" => LimitError(Json.Decode.(field("limit", int, json)))
      | "exclusiveMaximum" =>
        LimitError(Json.Decode.(field("limit", int, json)))
      | "exclusiveMinimum" =>
        LimitError(Json.Decode.(field("limit", int, json)))
      | "multipleOf" =>
        MultipleOfError(Json.Decode.(field("multipleOf", int, json)))
      | _ => failwith({j|Unknown keyword: $keyword|j})
      };
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
  let fromJson = json => {
    let keyword = Json.Decode.field("keyword", Json.Decode.string, json);
    Json.Decode.{
      keyword,
      dataPath: json |> field("dataPath", string),
      schemaPath: json |> field("schemaPath", string),
      params: json |> field("params", RawParams.fromJson(keyword)),
      message: json |> field("message", string),
      schema: json |> optional(field("schema", string)),
      parentSchema: json |> optional(field("parentSchema", string)),
    };
  };
  let toError = ({keyword, dataPath, message, params, _}) =>
    switch (keyword, params) {
    | ("required", MissingProperty(key)) => {key, message}
    | ("type", TypeError(_))
    | ("minimum", LimitError(_))
    | ("maximum", LimitError(_))
    | ("exclusiveMinimum", LimitError(_))
    | ("exclusiveMaximum", LimitError(_))
    | ("multipleOf", MultipleOfError(_)) => {
        key: String.sub(dataPath, 1, String.length(dataPath) - 1),
        message,
      }
    | _ => failwith({j|Unknown keyword: $keyword|j})
    };
};

let toDict = json =>
  Json.Decode.list(RawValidationError.fromJson, json)
  |> Belt_List.map(_, RawValidationError.toError)
  |> Belt_List.reduce(_, Belt_MapString.empty, (m, e) =>
       Belt_MapString.set(m, e.key, e.message)
     );

let logDict = dict =>
  Belt_MapString.forEach(
    dict,
    (k, v) => {
      Js.log("kv");
      Js.log(k);
      Js.log(v);
      ();
    },
  );
