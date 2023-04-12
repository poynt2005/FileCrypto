var path = require("path");
var fs = require("fs");

/** Custom Error: RuntimeError */
function RuntimeError(message) {
  this.name = "RuntimeError";
  this.message = message || "Error running the current script";
  this.stack = new Error().stack;
}

var _f = function () {};
_f.prototype = Error.prototype;
RuntimeError.prototype = new _f();
RuntimeError.prototype.constructor = RuntimeError;
/** */

var findRuntimeDll = function () {
  var dlls = ["FileCrypto.node", "FileEncryptorDecryptorAPI.dll"];
  dlls.forEach(function (dll) {
    if (!fs.existsSync(dll)) {
      throw new RuntimeError("dependice DLL: " + dll + " not found");
    }
  });
};

findRuntimeDll();

var nativeBinding = require("./FileCrypto.node");

var privateMap = new WeakMap();

/**
 * Create a File Cryptoer to encrypt/decrypt files using RSA keypair
 * @class
 * @param {boolean} isGenerate - To Generate a keypair or not
 * @param {number} [keyLength] - (Optional*) RSA keypair length, must be divided with no remain by 8
 */
function FileCrypto(isGenerate, keyLength) {
  /**
   * privateVariable is the private variable of class FileCrypto
   * @type {Object}
   */
  var privateVariable = {
    /**
     * handle is the class handler of native binding
     * @type {string}
     */
    handle: null,
    /**
     * isRelease shows that the native binding instance is released
     * @type {boolean}
     */
    isRelease: false,

    /**
     * isGenerate shows that you want to generate keypair
     * @type {boolean}
     */
    isGenerate: false,
  };

  privateVariable.isGenerate = isGenerate || false;

  if (typeof keyLength == "number") {
    privateVariable.handle = nativeBinding.CreateFileEncryptorDecryptor(
      privateVariable.isGenerate,
      keyLength
    );
  } else {
    privateVariable.handle = nativeBinding.CreateFileEncryptorDecryptor(
      privateVariable.isGenerate
    );
  }

  privateMap.set(this, privateVariable);
}

/**
 * Release the FileCrypto native binding Instance
 * @memberof FileCrypto
 * @returns {boolean} - Shows whether instance has been released
 */
FileCrypto.prototype.ReleaseFileEncryptorDecryptor = function () {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  privateVariable.isRelease = nativeBinding.ReleaseFileEncryptorDecryptor(
    privateVariable.handle
  );

  return privateVariable.isRelease;
};

/**
 * Save the RSA keypair to disk, it works when pass "true" to the first parameter of constructor
 * @memberof FileCrypto
 * @param {string} publicKeyPath - Specific the public key save destition path
 * @param {string} privateKeyPath - Specific the private key save destition path
 * @returns {boolean} - the keypair is saved to disk
 */
FileCrypto.prototype.SaveKeyPair = function (publicKeyPath, privateKeyPath) {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  if (!privateVariable.isGenerate) {
    throw new Error("You cannot save keypair without generate it");
  }

  return nativeBinding.SaveKeyPair(
    privateVariable.handle,
    publicKeyPath,
    privateKeyPath
  );
};

/**
 * Set public key from specific key file path
 * @memberof FileCrypto
 * @param {String} publicKeyPath - Specific the public key load source path
 * @returns {boolean} - Indicates whether the key is set
 */
FileCrypto.prototype.SetPublicKeyFromFile = function (publicKeyPath) {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  return nativeBinding.SetPublicKeyFromFile(
    privateVariable.handle,
    publicKeyPath
  );
};

/**
 * Set private key from specific key file path
 * @memberof FileCrypto
 * @param {String} privateKeyPath - Specific the private key load source path
 * @returns - Indicates whether the key is set
 */
FileCrypto.prototype.SetPrivateKeyFromFile = function (privateKeyPath) {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  return nativeBinding.SetPrivateKeyFromFile(
    privateVariable.handle,
    privateKeyPath
  );
};

/**
 * Encrypt the target file by public key
 * It raises error when public key is not set
 * @memberof FileCrypto
 * @param {string} srcFile - The source file path to encrypt
 * @param {string} dstFile - The output file path of encrypted files
 * @returns {boolean} - Indicates the file is encrypted
 */
FileCrypto.prototype.EncryptFile = function (srcFile, dstFile) {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  if (!nativeBinding.HasPublicKey(privateVariable.handle)) {
    throw new Error("Public key is not set");
  }

  return nativeBinding.EncryptFile(privateVariable.handle, srcFile, dstFile);
};

/**
 * Decrypt the target file by private key
 * It raises error when private key is not set
 * @memberof FileCrypto
 * @param {string} srcFile - The source file path to decrypt
 * @param {string} dstFile - The output file path of decrypted files
 * @returns - Indicates the file is decrypted
 */
FileCrypto.prototype.DecryptFile = function (srcFile, dstFile) {
  var privateVariable = privateMap.get(this);

  if (privateVariable.isRelease) {
    throw new Error("Instance has been released");
  }

  if (!nativeBinding.HasPrivateKey(privateVariable.handle)) {
    throw new Error("Private key is not set");
  }

  return nativeBinding.DecryptFile(privateVariable.handle, srcFile, dstFile);
};

module.exports = FileCrypto;
