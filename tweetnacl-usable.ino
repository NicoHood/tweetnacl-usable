extern "C" {
#include "tweetnacl.h"
#include "randombytes.h"
}
typedef unsigned char u8;

void hexdump(char * data, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    Serial.print((u8)data[i], HEX);
  }
  Serial.println();
}

#ifdef ESP8266
extern "C" {
  bool system_update_cpu_freq(int);
}
#endif

void setup() {
#ifdef ESP8266
  system_update_cpu_freq(160);
#endif

  Serial.begin(115200);
  Serial.println(F("Startup"));

  char * padded_message;
  int padded_mlen;
  u8 sk[crypto_box_SECRETKEYBYTES] = {0};
  u8 pk[crypto_box_PUBLICKEYBYTES] = {0};
  u8 sk2[crypto_box_SECRETKEYBYTES] = {0};
  u8 pk2[crypto_box_PUBLICKEYBYTES] = {0};
  u8 nonce[crypto_box_NONCEBYTES] = {0};
  char* message = "This is a cross-platform test of crypto_box/crypto_box_open in TweetNaCl.";
  u8 * ciphertext;
  char *decryptedmessage;

  // Randomize nonce
  randombytes(nonce, crypto_box_NONCEBYTES);
  Serial.println(F("Nonce:"));
  hexdump((char*)nonce, crypto_box_NONCEBYTES);

  // Generate public and secret keys
  Serial.print(F("\nGenerating keys... "));
  auto m = millis();
  crypto_box_keypair(pk, sk);
  crypto_box_keypair(pk2, sk2);
  Serial.println(millis() - m);

  Serial.println(F("\nPublic key:"));
  hexdump((char*)pk, crypto_box_PUBLICKEYBYTES);
  Serial.println(F("\nSecret key:"));
  hexdump((char*)sk, crypto_box_SECRETKEYBYTES);
  Serial.println(F("\nPublic key2:"));
  hexdump((char*)pk2, crypto_box_PUBLICKEYBYTES);
  Serial.println(F("\nSecret key2:"));
  hexdump((char*)sk2, crypto_box_SECRETKEYBYTES);

  // Create temporary message with padding
  padded_mlen = strlen(message) + crypto_box_ZEROBYTES;
  padded_message = (char*) malloc(padded_mlen);
  memset(padded_message, 0x00, crypto_box_ZEROBYTES);
  memcpy(padded_message + crypto_box_ZEROBYTES, message, strlen(message));

  Serial.println(F("\nUnencrypted Message:"));
  hexdump((char*)padded_message, padded_mlen);

  ciphertext = (u8*) malloc(padded_mlen);

#ifdef ESP8266
  // Call the yield() function to avoid a wdt reset
  yield();
#endif

  // Encrypt message

  Serial.print(F("\nEncrypting message... "));
  m = millis();
  auto ret = crypto_box(ciphertext, (u8*)padded_message, padded_mlen, nonce, pk2, sk);
  Serial.println(millis() - m);

  Serial.println(F("\ncrypto_box returned:"));
  Serial.println(ret);


  free(padded_message);

  Serial.println(F("\nCipher text: \n"));
  hexdump((char*)ciphertext, padded_mlen);

#ifdef ESP8266
  // Call the yield() function to avoid a wdt reset
  yield();
#endif

  // We have a string so add 1 byte and NULL it so we can print it
  decryptedmessage = (char*) malloc(padded_mlen + 1);
  decryptedmessage[padded_mlen] = '\0';

  // Decrypt message
  Serial.print(F("\nDecrypting message... "));
  m = millis();
  ret = crypto_box_open((u8*)decryptedmessage, ciphertext, padded_mlen, nonce, pk, sk2);
  Serial.println(millis() - m);


  Serial.println(F("\ncrypto_box_open returned:"));
  Serial.println(ret);

  free(ciphertext);
  Serial.println(F("\nDecrypted text: \n"));
  hexdump((char*)decryptedmessage, padded_mlen);

  Serial.write(decryptedmessage + crypto_box_ZEROBYTES, strlen(message));
  free(decryptedmessage);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("end");
  delay(1000);
}
