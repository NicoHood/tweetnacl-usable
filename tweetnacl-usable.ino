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

void setup() {
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
  crypto_box_keypair(pk, sk);
  crypto_box_keypair(pk2, sk2);

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

  // Encrypt message
  Serial.println(F("crypto_box returned:"));
  Serial.println(crypto_box(ciphertext, (u8*)padded_message, padded_mlen, nonce, pk2, sk));

  free(padded_message);

  Serial.println(F("\nCipher text: \n"));
  hexdump((char*)ciphertext, padded_mlen);

  // Decrypt message
  Serial.println(F("crypto_box_open returned:"));
  Serial.println(crypto_box_open((u8*)decryptedmessage, ciphertext, padded_mlen, nonce, pk, sk2));

  // Program crashes here
  Serial.println(F("teeeeest"));
  Serial.flush();

  free(ciphertext);
  Serial.println(F("\nDecrypted text: \n"));
  hexdump((char*)decryptedmessage, padded_mlen);

  //Serial.println(F("\n%s\n", decryptedmessage + PADDING_LEN));
  free(decryptedmessage);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("end");
  delay(1000);
}


/*
Startup
Nonce:
123456789ABCDEF101112131415161718

Public key:
5B2316DC2B5C71366EF575388C844D3142ACA95DBCBFA399E12BC4925F5E31

Secret key:
191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738

Public key2:
8C88CAE8A8DE5ED3F5DBE239BF451699ED1F9853CB80C8165953746A75EE3

Secret key2:
393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758

Unencrypted Message:
000000000000000000000000000000005468697320697320612063726F73732D706C6174666F726D2074657374206F662063727970746F5F626F782F63727970746F5F626F785F6F70656E20696E2054776565744E61436C2E
crypto_box returned:
0

Cipher text: 

0000000000000000D81787682936778DF34F4C9FA612658C167235B248D3D246D9611B8BFE0A2A9FFCFAB264C382356E2D467185F7B9D6692627DBA21D6365713FFA2F8212A23015C23F433C8D7494C9B44FC21B7092EE701020288A254A
crypto_box_open returned:

Soft WDT reset

ctx: cont 
sp: 3ffef0c0 end: 3ffefaf0 offset: 01b0

>>>stack>>>
3ffef270:  3ffef2f0 3ffef2f8 3ffef7c0 4020131f  
3ffef280:  7736fd50 ffffffe3 677e9996 ffffffce  
3ffef290:  47a6265a fffffff8 1b0b1ec0 ffffffb0  
3ffef2a0:  a40c4387 ffffffc7 a3f90a5e ffffffe9  
3ffef2b0:  51701f16 00000002 e8241d4f ffffffb9  
3ffef2c0:  528cdec2 ffffffd0 796faeea ffffffda  
3ffef2d0:  b412233f ffffffce f7a56f97 ffffffde  
3ffef2e0:  a36e0345 fffffff4 e2436dc2 fffffff8  
3ffef2f0:  d5bb3812 fffffffe 7c072b45 ffffffff  
3ffef300:  38883af4 ffffffff a4e322ed fffffffe  
3ffef310:  c72ab928 ffffffff dab1a34d fffffffd  
3ffef320:  7fc55c1d fffffffe 61c3d2a0 ffffffff  
3ffef330:  0ad80fbc 00000000 1e3c4254 fffffffe  
3ffef340:  bcb91644 fffffffe fd55d36e fffffffe  
3ffef350:  a244cd57 fffffffe 1e390c8b ffffffff  
3ffef360:  afed3804 ffffffff cc520ba0 ffffffff  
3ffef370:  eecc6d37 ffffffff 3ffef8c0 3ffef8c0  
3ffef380:  3ffef370 00000078 3ffef840 3ffef6c0  
3ffef390:  3ffef2f8 00000000 00008c56 00000000  
3ffef3a0:  00000084 3ffef990 3ffef8c0 00000000  
3ffef3b0:  00000084 3ffef990 3ffef8c0 40201cdc  
3ffef3c0:  0000235b 00000000 0000dc16 00000000  
3ffef3d0:  00005c2b 00000000 00003671 00000000  
3ffef3e0:  0000f56e 00000000 00003875 00000000  
3ffef3f0:  0000848c 00000000 0000314d 00000000  
3ffef400:  0000ac42 00000000 000005a9 00000000  
3ffef410:  0000cbdb 00000000 000003fa 00000000  
3ffef420:  0000e199 00000000 0000c42b 00000000  
3ffef430:  00005f92 00000000 0000315e 00000000  
3ffef440:  0000cc4a 00000000 0000d2b2 00000000  
3ffef450:  00003578 00000000 0000db4f 00000000  
3ffef460:  0000e9bc 00000000 00000c43 00000000  
3ffef470:  0000a177 00000000 0000eac3 00000000  
3ffef480:  00003893 00000000 00009c10 00000000  
3ffef490:  0000b019 00000000 0000a9b4 00000000  
3ffef4a0:  00004774 00000000 00008c56 00000000  
3ffef4b0:  00005a84 00000000 00009de3 00000000  
3ffef4c0:  00003912 00000000 00000dc8 00000000  
3ffef4d0:  0000b23f 00000000 00008e0e 00000000  
3ffef4e0:  0000033f 00000000 000056aa 00000000  
3ffef4f0:  00005096 00000000 0000f313 00000000  
3ffef500:  0000b9d9 00000000 00001f79 00000000  
3ffef510:  00005eb6 00000000 0000b138 00000000  
3ffef520:  00008555 00000000 00003060 00000000  
3ffef530:  00005a86 00000000 000053e5 00000000  
3ffef540:  000078de 00000000 000097d7 00000000  
3ffef550:  00006982 00000000 00007a73 00000000  
3ffef560:  0000765c 00000000 0000af59 00000000  
3ffef570:  000068c9 00000000 00002128 00000000  
3ffef580:  0000be25 00000000 0000387f 00000000  
3ffef590:  000014bc 00000000 0000fa7e 00000000  
3ffef5a0:  0000117a 00000000 00004971 00000000  
3ffef5b0:  00004e8e 00000000 0000eebd 00000000  
3ffef5c0:  0000c4a2 00000000 0000c65a 00000000  
3ffef5d0:  000066af 00000000 00002585 00000000  
3ffef5e0:  0000144a 00000000 00009f3a 00000000  
3ffef5f0:  0000e410 00000000 0000a8c2 00000000  
3ffef600:  00001d5d 00000000 0000d221 00000000  
3ffef610:  000027aa 00000000 0000c776 00000000  
3ffef620:  00001338 00000000 0000346a 00000000  
3ffef630:  00003869 00000000 00004afc 00000000  
3ffef640:  0000e894 00000000 00007dc7 00000000  
3ffef650:  0000e8ef 00000000 000062fa 00000000  
3ffef660:  0000e842 00000000 00003986 00000000  
3ffef670:  00007a5d 00000000 00007020 00000000  
3ffef680:  0000c15c 00000000 000090e0 00000000  
3ffef690:  00008299 00000000 000081ef 00000000  
3ffef6a0:  0000f532 00000000 0000abf0 00000000  
3ffef6b0:  0000b326 00000000 000068b8 00000000  
3ffef6c0:  00018d8e 00000000 00010142 00000000  
3ffef6d0:  000078f9 00000000 00008776 00000000  
3ffef6e0:  0000feb6 00000000 000104f5 00000000  
3ffef6f0:  00005901 00000000 000122ba 00000000  
3ffef700:  0000f00c 00000000 0000e999 00000000  
3ffef710:  00008f56 00000000 00013d29 00000000  
3ffef720:  0001a593 00000000 00013a72 00000000  
3ffef730:  00006341 00000000 0000d2e9 00000000  
3ffef740:  0000f96a 00000000 00004464 00000000  
3ffef750:  00005d31 00000000 0000f0f5 00000000  
3ffef760:  0000ba0b 00000000 00005adb 00000000  
3ffef770:  0000021f 00000000 00007008 00000000  
3ffef780:  00005bd8 00000000 00002502 00000000  
3ffef790:  000000cf 00000000 000009e9 00000000  
3ffef7a0:  0000a6f2 00000000 000055d0 00000000  
3ffef7b0:  00000a58 00000000 000066ce 00000000  
3ffef7c0:  7736fd50 ffffffe3 677e9996 ffffffce  
3ffef7d0:  47a6265a fffffff8 1b0b1ec0 ffffffb0  
3ffef7e0:  a40c4387 ffffffc7 a3f90a5e ffffffe9  
3ffef7f0:  51701f16 00000002 e8241d4f ffffffb9  
3ffef800:  528cdec2 ffffffd0 796faeea ffffffda  
3ffef810:  b412233f ffffffce f7a56f97 ffffffde  
3ffef820:  00012c23 00000000 00018bc7 00000000  
3ffef830:  0000b9ad 00000000 00013783 00000000  
3ffef840:  0000b114 00000000 0000cb60 00000000  
3ffef850:  ffffbc8d ffffffff 0000c050 00000000  
3ffef860:  ffff5092 ffffffff 0000117c 00000000  
3ffef870:  ffffdb67 ffffffff 00003a0c 00000000  
3ffef880:  ffff1427 ffffffff 0000ae33 00000000  
3ffef890:  00008a9a 00000000 ffff5d1c ffffffff  
3ffef8a0:  ffff79ef ffffffff ffffd6d5 ffffffff  
3ffef8b0:  ffffcb19 ffffffff ffffeb1f ffffffff  
3ffef8c0:  00008ace 00000000 0000570c 00000000  
3ffef8d0:  00003c62 00000000 000015b0 00000000  
3ffef8e0:  0000defc 00000000 000087a0 00000000  
3ffef8f0:  0000f765 00000000 0000d965 00000000  
3ffef900:  00004c41 00000000 0000846b 00000000  
3ffef910:  0000de7d 00000000 0000fb33 00000000  
3ffef920:  000018b4 00000000 000025bb 00000000  
3ffef930:  00000c90 00000000 00006995 00000000  
3ffef940:  3c3b3a38 403f3e3d 44434241 48474645  
3ffef950:  4c4b4a49 504f4e4d 54535251 58575655  
3ffef960:  00000000 0003fdf3 3ffef98d 40203711  
3ffef970:  00022ab1 5f582361 3ffef99e 3fff0c50  
3ffef980:  3fff0bd0 00000000 3ffef9c0 40201e77  
3ffef990:  3ffeea98 0000000a 3ffeea98 40202df5  
3ffef9a0:  3ffeea98 00000001 3ffe85e9 40203510  
3ffef9b0:  00000069 3ffeea98 00000069 40201f01  
3ffef9c0:  3fff0c50 00000019 3ffeea98 40203161  
3ffef9d0:  3ffe85e8 0000003a 3ffeea98 40202df5  
3ffef9e0:  3ffefaa0 00000018 3ffeea98 402035e4  
3ffef9f0:  3fff0c50 3fff0b50 3ffeea98 3fff0bd0  
3ffefa00:  3fff0c50 3fff0b50 3ffeea98 40202a94  
3ffefa10:  3ffefa40 00000000 00000000 00000000  
3ffefa20:  ae8cc808 5ede088a e2dbf5d3 1645bf39  
3ffefa30:  981fed99 c880cb53 74535916 03ee756a  
3ffefa40:  3c3b3a39 403f3e3d 44434241 48474645  
3ffefa50:  4c4b4a49 504f4e4d 54535251 58575655  
3ffefa60:  dc16235b 36715c2b 3875f56e 314d848c  
3ffefa70:  05a9ac42 03facbdb c42be199 315e5f92  
3ffefa80:  1c1b1a19 201f1e1d 24232221 28272625  
3ffefa90:  2c2b2a29 302f2e2d 34333231 38373635  
3ffefaa0:  04030201 08070605 0c0b0a09 100f0e0d  
3ffefab0:  14131211 18171615 00000000 00000000  
3ffefac0:  00000000 00000000 00000000 3ffeeac4  
3ffefad0:  3fffdc20 00000000 3ffeeabc 40203292  
3ffefae0:  00000000 00000000 3ffeead0 40100114  
<<<stack<<<

 ets Jan  8 2013,rst cause:4, boot mode:(1,6)

wdt reset

 ets Jan  8 2013,rst cause:4, boot mode:(1,6)

wdt reset

*/
