[![Build Status](https://www.travis-ci.org/scull7/bs-ajv.svg?branch=master)](https://www.travis-ci.org/scull7/bs-ajv)
[![Coverage Status](https://coveralls.io/repos/github/scull7/bs-ajv/badge.svg?branch=master)](https://coveralls.io/github/scull7/bs-ajv?branch=master)

# bs-ajv
BuckleScript bindings to [Ajv] (Another JSON Validator)

## Why?
To be able to use standardized validation schemas across our JavaScript and
BuckleScript based code.

## How do I install?
1. Add the [bs-ajv] package to your project via yarn/npm
  ```sh
  yarn add bs-ajv
  ```

2. Add [bs-ajv] to your `bsconfig.json`
  ```json
  {
    "dependencies": [ "bs-ajv" ]
  }
  ```

3. Enjoy!

## Usage

The interface here is a bit rough, but it is usable and is used within
production.  Here are a few examples pulled from the [Ajv] docs.

### Basic first example

```reason
let schema = Json.Encode.(object_([
  ("properties", object_([
    ("smaller", object_([
      ("type", string("number")),
      ("maximum", object_([
        ("$data", string("1/larger")),
      ])),
    ])),
    ("larger", object_([
      ("type", string("number")),
    ])),
  ])),
]));

let validData = Json.Encode.(object_([
  ("smaller", int(5)),
  ("larger", int(7)),
]));

AjvOptions.make()
|> AjvOptions.setData(_, Js.true_)
|> Ajv.ajv
|> Ajv.validate(`Schema(schema), validData)
/* This will return `Js.true_` */
```

### Example with a self reference
```reason
let schema = Json.Encode.(object_([
  ("additionalProperties", object_([
    ("type", string("string")),
    ("format", object_([ ("$data", string("0#")) ]))
  ])),
]));

let validData = Json.Encode.(object_([
  ("date-time", string("1963-06-19T08:30:06.283185Z")),
  ("email", string("joe.bloggs@example.com")),
]));

AjvOptions.make()
|> AjvOptions.setData(_, Js.true_)
|> Ajv.ajv
|> Ajv.validate(`Schema(schema), validData)
/* This will return `Js.true_` */
```

### Example using schema compilation and additional property filtering
```reason
let schema = Json.Encode.(object_([
  ("additionalProperties", bool(false)),
  ("properties", object_([
    ("foo", object_([
      ("type", string("number")),
    ])),
    ("bar", object_([
      ("baz", object_([
        ("type", string("string")),
      ])),
    ])),
  ])),
]));

let validData = Json.Encode.(object_([
  ("foo", int(0)),
  ("additional1", int(1)),
  ("bar", object_([
    ("baz", string("abc")),
    ("additional2", int(2)),
  ])),
]));

AjvOptions.make()
|> AjvOptions.removeAdditional(_, Js.true_)
|> Ajv.ajv
|> Ajv.compile(schema)
|> (fun
  | `Sync(fn) => fn
  | `Async(_) => failwith("unexpected_async_mode")
  )
|> (v) => validate(validData)
|> (fun
  | `Valid(_) => Js.true_
  | `Invalid(_) => Js.false_
  )
/* This will return `Js.true_`

```
[Ajv]: http://epoberezkin.github.io/ajv/
[bs-ajv]: https://www.npmjs.com/package/bs-ajv
