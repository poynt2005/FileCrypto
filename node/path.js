var path = require("path");

var FileEncryptorDecryptorAPILib = path
  .resolve("../FileEncryptorDecryptorAPI.lib")
  .replace(/\\/gm, "\\\\");

module.exports = {
  FileEncryptorDecryptorAPILib: FileEncryptorDecryptorAPILib,
};
