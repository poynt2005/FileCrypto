var fs = require("fs");
var path = require("path");

var FileCrypto = require("./FileCrypto");

var PUBLICKEY_PATH = path.resolve("./public.key");
var PRIVATEKEY_PATH = path.resolve("./private.key");
var TESTFILE_PATH = path.resolve("./file.zip");
var ENCFILE_PATH = path.resolve("./file.enc.zip");
var DECFILE_PATH = path.resolve("./file.dec.zip");

function encryptFile() {
  /** Fetch test file */
  var nativeDownloader = require("./FileDownloader.node");

  var isTestingFileExists = fs.existsSync(TESTFILE_PATH);

  if (!isTestingFileExists) {
    console.log("Fetching test file...");

    var isDownloaded = nativeDownloader.Downloader(
      "https://www.rarewares.org/files/mp3/lame3.100.1-x64.zip",
      TESTFILE_PATH
    );

    if (!isDownloaded) {
      throw new Error("Download test file failed");
    }
  }

  console.log("Encrypting file...");

  var fileCrypto = new FileCrypto(false);

  if (!fs.existsSync(PUBLICKEY_PATH)) {
    throw new Error("Public key file not Exists!");
  }

  var isKeySet = fileCrypto.SetPublicKeyFromFile(PUBLICKEY_PATH);

  if (!isKeySet) {
    throw new Error("Set public key failed");
  }

  var isEncrypted = fileCrypto.EncryptFile(TESTFILE_PATH, ENCFILE_PATH);

  if (!isEncrypted) {
    throw new Error("Encryped file failed");
  }

  fileCrypto.ReleaseFileEncryptorDecryptor();

  console.log("Encryped file: file.enc.zip");
}

function decryptFile() {
  if (!fs.existsSync(ENCFILE_PATH)) {
    throw new Error("Encryped file not Exists!");
  }

  console.log("Decrypting file...");

  var fileCrypto = new FileCrypto(false);

  if (!fs.existsSync(PRIVATEKEY_PATH)) {
    throw new Error("Private key file not Exists!");
  }

  var isKeySet = fileCrypto.SetPrivateKeyFromFile(PRIVATEKEY_PATH);

  if (!isKeySet) {
    throw new Error("Set private key failed");
  }

  var isDecrypted = fileCrypto.DecryptFile(ENCFILE_PATH, DECFILE_PATH);

  if (!isDecrypted) {
    throw new Error("Decrypted file failed");
  }

  fileCrypto.ReleaseFileEncryptorDecryptor();

  console.log("Decrypted file: file.dec.zip");
}

function generateKey() {
  console.log("Generating RSA keypair file with 1024 key length...");

  var fileCrypto = new FileCrypto(true, 1024);

  isKeyPairSaved = fileCrypto.SaveKeyPair(PUBLICKEY_PATH, PRIVATEKEY_PATH);

  if (!isKeyPairSaved) {
    throw new Error("Save RSA keypair failed");
  }

  fileCrypto.ReleaseFileEncryptorDecryptor();

  console.log("Generated: %s and %s", PUBLICKEY_PATH, PRIVATEKEY_PATH);
}

switch (process.argv[2]) {
  case "encrypt":
    encryptFile();
    break;
  case "decrypt":
    decryptFile();
    break;
  case "generate":
    generateKey();
    break;
  default:
    throw new Error("Undefined behavior");
}
