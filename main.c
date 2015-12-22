#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"
#include "randombytes.h"

typedef unsigned char u8;


//TODO
#define crypto_box_curve25519xsalsa20poly1305_tweet_BEFORENMBYTES 32
#define crypto_box_curve25519xsalsa20poly1305_tweet_ZEROBYTES 32
#define crypto_box_BEFORENMBYTES crypto_box_curve25519xsalsa20poly1305_BEFORENMBYTES
#define crypto_box_ZEROBYTES crypto_box_curve25519xsalsa20poly1305_ZEROBYTES

#define PUB_KEY_LEN crypto_box_PUBLICKEYBYTES
#define PRIV_KEY_LEN crypto_box_SECRETKEYBYTES
#define NONCE_LEN crypto_box_NONCEBYTES
#define PADDING_LEN 512

void hexdump(char * data, int len) 
{
	int i;
	for (i = 0; i < len; i++) {
		printf("%02X", (u8)data[i]);
	}
	printf("\n");
}

int main() {
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
	printf("Nonce: \n");
	hexdump((char*)nonce, NONCE_LEN);

	// Generate public and secret keys
	crypto_box_keypair(pk, sk);
	crypto_box_keypair(pk2, sk2);
	
	printf("\nPublic key: \n");
	hexdump((char*)pk, PUB_KEY_LEN);
	printf("\nSecret key: \n");
	hexdump((char*)sk, PRIV_KEY_LEN);
	printf("\nPublic key2: \n");
	hexdump((char*)pk2, PUB_KEY_LEN);
	printf("\nSecret key2: \n");
	hexdump((char*)sk2, PRIV_KEY_LEN);

	// Create temporary message with padding
	padded_mlen = strlen(message) + PADDING_LEN;
	padded_message = (char*) malloc(padded_mlen);
	memset(padded_message, 0, PADDING_LEN);
	memcpy(padded_message + PADDING_LEN, message, strlen(message));

	printf("\nUnencrypted Message: \n");
	hexdump((char*)padded_message, padded_mlen);

	ciphertext = (u8*) malloc(padded_mlen);

	// We have a string so add 1 byte and NULL it so we can print it
	decryptedmessage = (char*) malloc(padded_mlen+1);
	decryptedmessage[padded_mlen] = '\0';

	// Encrypt message
	printf("crypto_box returned: %d\n",crypto_box(ciphertext, (u8*)padded_message, padded_mlen, nonce, pk2, sk));

	free(padded_message);
	
	printf("\nCipher text: \n");
	hexdump((char*)ciphertext, padded_mlen);
	
	// Decrypt message
	printf("crypto_box_open returned: %d\n", crypto_box_open((u8*)decryptedmessage, ciphertext, padded_mlen, nonce, pk, sk2));
	
	free(ciphertext);
	printf("\nDecrypted text: \n");
	hexdump((char*)decryptedmessage, padded_mlen);

	printf("\n%s\n", decryptedmessage + PADDING_LEN);
	free(decryptedmessage);
	return 0;
}
