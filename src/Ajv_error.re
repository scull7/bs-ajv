type t = {
  key: string,
  message: string
};

module RawValidationError = {
  module RawParams = {
    type t =
      | MissingProperty(string)
      | Type(string);
    let fromJson = (keyword, json) => {
      switch keyword {
      | "required" =>
        MissingProperty(Json.Decode.(field("missingProperty", string, json)))
      | "type" => Type(Json.Decode.(field("type", string, json)))
      | _ => failwith({j|Unknown keyword: $keyword|j})
      };
    };
  };
  type t = {
    keyword: string,
    dataPath: string,
    schemaPath: string,
    params: RawParams.t,
    message: string,
    schema: option(string),
    parentSchema: option(string)
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
      parentSchema: json |> optional(field("parentSchema", string))
    };
  };
  let toError = ({dataPath, message, params, _}) =>
    switch params {
    | RawParams.MissingProperty(key) => {key, message}
    | RawParams.Type(_) => {key: dataPath, message}
    };
};

let toDict = (json) => {
  Json.Decode.list(RawValidationError.fromJson, json)
  |> Belt_List.map(_, RawValidationError.toError)
  |> Belt_List.reduce(_, Belt_MapString.empty, (m, e) => Belt_MapString.set(m, e.key, e.message))
  ;
};
