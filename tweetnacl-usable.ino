extern "C" {
#include "tweetnacl.h"
#include "randombytes.h"
}
typedef unsigned char u8;


//TODO
#define crypto_box_curve25519xsalsa20poly1305_tweet_BEFORENMBYTES 32
#define crypto_box_curve25519xsalsa20poly1305_tweet_ZEROBYTES 32
#define crypto_box_BEFORENMBYTES crypto_box_curve25519xsalsa20poly1305_BEFORENMBYTES
#define crypto_box_ZEROBYTES crypto_box_curve25519xsalsa20poly1305_ZEROBYTES

#define PUB_KEY_LEN crypto_box_PUBLICKEYBYTES
#define PRIV_KEY_LEN crypto_box_SECRETKEYBYTES
#define NONCE_LEN crypto_box_NONCEBYTES
#define PADDING_LEN 32

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
  u8 sk[PRIV_KEY_LEN] = {0};
  u8 pk[PUB_KEY_LEN] = {0};
  u8 sk2[PRIV_KEY_LEN] = {0};
  u8 pk2[PUB_KEY_LEN] = {0};
  u8 nonce[NONCE_LEN] = {0};
  char* message = "This is a cross-platform test of crypto_box/crypto_box_open in TweetNaCl.";
  u8 * ciphertext;
  char *decryptedmessage;

  // Randomize nonce
  randombytes(nonce, NONCE_LEN);
  Serial.println(F("Nonce:"));
  hexdump((char*)nonce, NONCE_LEN);

  // Generate public and secret keys
  Serial.print(F("\nGenerating keys... "));
  auto m = millis();
  crypto_box_keypair(pk, sk);
  crypto_box_keypair(pk2, sk2);
  Serial.println(millis() - m);

  Serial.println(F("\nPublic key:"));
  hexdump((char*)pk, PUB_KEY_LEN);
  Serial.println(F("\nSecret key:"));
  hexdump((char*)sk, PRIV_KEY_LEN);
  Serial.println(F("\nPublic key2:"));
  hexdump((char*)pk2, PUB_KEY_LEN);
  Serial.println(F("\nSecret key2:"));
  hexdump((char*)sk2, PRIV_KEY_LEN);

  // Create temporary message with padding
  padded_mlen = strlen(message) + PADDING_LEN;
  padded_message = (char*) malloc(padded_mlen);
  memset(padded_message, 0, PADDING_LEN);
  memcpy(padded_message + PADDING_LEN, message, strlen(message));

  Serial.println(F("\nUnencrypted Message:"));
  hexdump((char*)padded_message, padded_mlen);

  ciphertext = (u8*) malloc(padded_mlen);

  // We have a string so add 1 byte and NULL it so we can print it
  decryptedmessage = (char*) malloc(padded_mlen + 1);
  decryptedmessage[padded_mlen] = '\0';

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

  Serial.write(decryptedmessage + PADDING_LEN, strlen(message));
  free(decryptedmessage);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("end");
  delay(1000);
}
