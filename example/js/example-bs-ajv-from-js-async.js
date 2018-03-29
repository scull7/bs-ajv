const Ajv = require('../../src/Ajv.bs');

const invalidData = { "messages": {
  "Dr. Beverly Crusher": "How are you feeling!",
  "Cpt. Jean-Luc Picard": "No children allowed on the bridge?",
  "Lt. Cmdr. Data": "I am an android!",
}};
const validData = { "messages": {
  "Dr. Beverly Crusher": "How are you feeling?",
  "Cpt. Jean-Luc Picard": "No children allowed on the bridge!",
  "Lt. Cmdr. Data": "I am an android.",
}};

const schema = {
  "$async": true,
  "title": "Message",
  "type": "object",
  "customKeyword": "custom parameter string",
  "properties": {
    "messages": {
      "type": "object",
      "maxProperties": 3,
      "minProperties": 1,
      "additionalProperties": false,
      "patternProperties": {
        "Dr\\. .*": {
          "type": "string",
          "pattern": "^.*\\?$"
        },
        "Cpt\\. .*": {
          "type": "string",
          "pattern": "^.*!$"
        },
        "Lt. Cmdr. .*": {
          "type": "string",
          "pattern": "^.*[\.?]$"
        },
        "Act. Ens. .*": {
          "type": "string",
          "pattern": "^$"
        }
      }
    }
  },
  "required": [
    "messages"
  ]
};

/* Using bs-ajv from Javascript. TODO is this interface too complicated? */
/* Define async validator for custom keyword */
/* TODO what argument is _ ? */
function asyncValidator(schema, data, _) {
  console.log('asyncValidator got schema=', schema);
  console.log('asyncValidator got data=', data);
  /* Mock request latency */
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      resolve(true);
    }, 1000);
  })
}
/* Instantiate an Ajv.Options.t -- just an empty JS object */
const ajvOptions = new Ajv.makeOptions();
Ajv.setOptionAllErrors(ajvOptions, true);
Ajv.setOptionJsonPointers(ajvOptions, true);
Ajv.setOptionRemoveAdditional(ajvOptions, true);
console.log('ajvOptions=', typeof ajvOptions);
/* Instantiate custom validation keyword */
/* TODO does this just create an empty object with Object.prototype prototype? */
const customKeyword = Ajv.makeKeyword();
console.log('customKeyword constructor name=', customKeyword.constructor.name);
console.log('customKeyword prototype is Object.prototype=', customKeyword.__proto__ === Object.prototype);
/* Set asynchronous validator for custom keyword */
Ajv.setKeywordType(customKeyword, "object");
Ajv.setKeywordIsAsync(customKeyword, true);
Ajv.setValidator(customKeyword, asyncValidator);
/* Instantiate validator factory */
/* TODO does this just create an empty object with Object.prototype prototype? */
const ajv = Ajv.makeAjv(ajvOptions);
console.log('ajv=', typeof ajv);
/* Add custom keyword to validator factory */
console.log('customKeyword=', customKeyword);
Ajv.addKeyword("customKeyword", customKeyword, ajv);
/* Instantiate asynchronous validator */
const validator = Ajv.compileAsync(schema, ajv);
console.log('validator=', typeof validator);
/* Evalutae example document against asynchronous validator */
validator(invalidData)
.then(result => {
  console.log('UNEXPECTED RESULT validator result=', result);
}).catch(err => {
  console.error("EXPECTED RESULT validator error=", err);
}).then(() => {
  console.log('async validating...');
  return validator(validData)
}).then(result => {
  console.log('EXPECTED RESULT validator result=', result);
}).catch(err => {
  console.error("UNEXPECTED RESULT validator error=", err);
}).then(() => {
  /* goodbye.wav */
  console.log('normal exit');
});

console.log('async validating...');
