const Ajv = require('../../src/Ajv.bs');

const invalidData = { "messages": {
  "Dr. Beverly Crusher": "How are you feeling!",
  "Cpt. Jean-Luc Picard": "No children allowed on the bridge?",
  "Lt. Cmdr. Data": "I am an android!",
}};

const schema = {
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
/* Define sync validator for custom keyword */
/* TODO what argument is _ ? */
function syncValidator(schema, data, _) {
  console.log('**** syncValidator got schema=', schema);
  console.log('**** syncValidator got data=', data);
  return true;
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
/* Set synchronous validator for custom keyword */
Ajv.setKeywordType(customKeyword, "object");
Ajv.setKeywordIsAsync(customKeyword, false);
Ajv.setValidator(customKeyword, syncValidator);
/* Instantiate validator factory */
/* TODO does this just create an empty object with Object.prototype prototype? */
const ajv = Ajv.makeAjv(ajvOptions);
console.log('ajv=', typeof ajv);
/* Add custom keyword to validator factory */
console.log('customKeyword=', customKeyword);
Ajv.addKeyword("customKeyword", customKeyword, ajv);
/* Instantiate synchronous validator */
const validator = Ajv.compileSync(schema, ajv);
console.log('validator=', typeof validator);
/* Evalutae example document against synchronous validator */
const result = validator(invalidData);
console.log('result=', result);
/* goodbye.wav */
console.log('normal exit');
