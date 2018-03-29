const Ajv = require('../../src/Ajv.bs');

const invalidData = { username: 'scull7' };
const validData = { username: 'johndoe123' };

const schema = {
  "$async": true,
  "title": "Message",
  "type": "object",
  "required": [
    "username"
  ],
  "properties": {
    "username": {
      "type": "string",
      /* Our custom keyword! */
      "userExists": false
    }
  },
};

/* Instantiate an Ajv.Options.t. */
const ajvOptions = new Ajv.makeOptions();
/* Request all errors (the alternative fails early, but underreports
 * validation errors)
 */
Ajv.setOptionAllErrors(ajvOptions, true);
/* Request that the dataPath property of native Ajv error objects use JSON
 * Pointers to reference subdocuments which failed to validate!  Even
 * though you will probably only be consuming bs-ajv's errors and not Ajv's
 * own error objects, this is CRITICAL. In a future version, bs-ajv may
 * enforce this option!
 */
Ajv.setOptionJsonPointers(ajvOptions, true);
Ajv.setOptionRemoveAdditional(ajvOptions, true);

/* JSON Schema's own validation keywords do not cover all of our validation
 * needs. To address this, we will create a custom validation keyword. The
 * validation criterion asserted by this custom keyword are defined by a
 * function we supply.  This function can be asynchronous (Promise) or
 * synchronous. 
 *
 * For more documentation on this function, see the Ajv docs:
 * https://github.com/epoberezkin/ajv/blob/master/CUSTOM.md#define-keyword-with-validation-function
 */
function validateUserExists(schema, data) {
  /* Mock user database! */
  const users = [
    { username: 'scull7' }
  ];
  return new Promise((resolve, reject) => {
    /* Mock request latency */
    setTimeout(() => {
      /* Does user exist? */
      const userExists = users.some(({username}) => username == data);
      /* schema === false because that's the property value within our
       * application of our custom keyword in our schema.
       */
      resolve(schema === userExists);
    }, 1000);
  })
}
/*
 * We will define a custom keyword which extends the capabilities of
 * JSON schema.
 */
const userExistsKeyword = Ajv.makeKeyword();
/* Our custom keyword only applies to strings. We can specify that like so.
 * If the type doesn't match, the keyword is assumed to validate.
 */
Ajv.setKeywordType(userExistsKeyword, "string");
/* Our custom keyword's validation implementation is asynchronous. We must
 * configure it as such:
 */
Ajv.setKeywordIsAsync(userExistsKeyword, true);
/* Now let's attach our validator function to our custom keyword's
 * definition
 */
Ajv.setValidator(userExistsKeyword, validateUserExists);

/* Instantiate Ajv.
 *
 * From this object, we can compile any number of schemas into document
 * validation functions, or extend the capabilities of any such schemas. We
 * only need to do this once.
 * 
 * DO NOT DO THIS every time you need to validate a document!
 */
const ajv = Ajv.makeAjv(ajvOptions);
/* Add our custom keyword definition */
Ajv.addKeyword("userExists", userExistsKeyword, ajv);

/* Now we can compile a schema to a document validation function!
 *
 * Just like before, we only need to do this ONCE PER SCHEMA!
 *
 * DO NOT DO THIS every time you need to validate a document!
 */
const validate = Ajv.compileAsync(schema, ajv);

/* Now let's validate some documents! Remember that in this example, our
 * schema includes a custom validation keyword whose implementation
 * executes asynchronously using Promise.
 */
async function example() {
  try {
    console.log('\nthis document is expected to fail validation:');
    let validatedDocument = await validate(invalidData)
  } catch (e) {
    console.error(e);
  }
  try {
    console.log('\nthis document is expected to pass validation:');
    let validatedDocument = await validate(validData)
    console.log(validatedDocument);
  } catch (e) {
    console.error(e);
  }
}

example();
