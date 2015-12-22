#ifndef ARDUINO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweetnacl.h"
#include "randombytes.h"

typedef unsigned char u8;

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
	printf("Nonce: \n");
	hexdump((char*)nonce, crypto_box_NONCEBYTES);

	// Generate public and secret keys
	crypto_box_keypair(pk, sk);
	crypto_box_keypair(pk2, sk2);
	
	printf("\nPublic key: \n");
	hexdump((char*)pk, crypto_box_PUBLICKEYBYTES);
	printf("\nSecret key: \n");
	hexdump((char*)sk, crypto_box_SECRETKEYBYTES);
	printf("\nPublic key2: \n");
	hexdump((char*)pk2, crypto_box_PUBLICKEYBYTES);
	printf("\nSecret key2: \n");
	hexdump((char*)sk2, crypto_box_SECRETKEYBYTES);

	// Create temporary message with zero padding (required by NaCl API)
	padded_mlen = strlen(message) + crypto_box_ZEROBYTES;
	padded_message = (char*) malloc(padded_mlen);
	memset(padded_message, 0, crypto_box_ZEROBYTES);
	memcpy(padded_message + crypto_box_ZEROBYTES, message, strlen(message));

	printf("\nUnencrypted Message: \n");
	hexdump((char*)padded_message, padded_mlen);

	ciphertext = (u8*) malloc(padded_mlen);

	// Encrypt message
	printf("crypto_box returned: %d\n",crypto_box(ciphertext, (u8*)padded_message, padded_mlen, nonce, pk2, sk));

	free(padded_message);
	
	printf("\nCipher text: \n");
	hexdump((char*)ciphertext, padded_mlen);
	
	// We have a string so add 1 byte and NULL it so we can print it
	decryptedmessage = (char*) malloc(padded_mlen + 1);
	decryptedmessage[padded_mlen] = '\0';

	// Decrypt message
	printf("crypto_box_open returned: %d\n", crypto_box_open((u8*)decryptedmessage, ciphertext, padded_mlen, nonce, pk, sk2));
	
	free(ciphertext);
	printf("\nDecrypted text: \n");
	hexdump((char*)decryptedmessage, padded_mlen);

	printf("\n%s\n", decryptedmessage + crypto_box_ZEROBYTES);
	free(decryptedmessage);
	return 0;
}
#endif
