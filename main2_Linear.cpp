#include<iostream>
#include<bitset>
#include<random>

using namespace std;

//A simple Cipher that XORes the message with the key
bitset<4> cipher(bitset<4> message, bitset<4> key) {
	bitset<4> encrypted_message = message ^ key;
	return encrypted_message;
}

int main() {
	//Set Parameters to generate random messages and keys
	random_device rand_value;
	default_random_engine generate(rand_value());
	uniform_int_distribution<int> distance(0, 15);//Generates Random numbers between 0 and 15

	bitset<16> messages;// Stores Messages
	bitset<16> encrypted_messages;//Stores encrypted Messages

	//Generate randomly 16 messages, the keys, each of them on 4-bit
	//Compute the corresponding encrypted messages

	for (int i = 0; i < 16; i++) {
		bitset<4> message(distance(generate));
		bitset<4> private_key(distance(generate));
		bitset<4> encrypted_message = cipher(message, private_key);

		messages[i] = message.to_ulong();
		encrypted_messages[i] = encrypted_message.to_ulong();
	}
	//Apply Linear Cryptanalysis to compute the Key
	//Compute the key and Set the maximum bias to 0
	bitset<4> key_compute(0);
	int maximum_bias = 0;

	//All possible keys are computed
	//for each key the bias value is computed using linear approxmation

	for (int k = 0; k < 16; k++) {
		bitset<4> local_key(k);
		int local_bias = 0;

		//Compute the bias value for each pair of message- and encrypted message
		for (int i = 0; i < 16; i++) {
			bitset<4> aux_message(messages[i]);
			bitset<4> aux_encrypted_message(encrypted_messages[i]);

			bitset<4> aux1(aux_message ^ local_key); // XOR the message with Local Key
			bitset<4> aux2(aux_encrypted_message ^ local_key);// XOR the encrypted message with the Local Key

			//Compute the linear approximation
			//And updating the Bias value
			if ((aux1.count() + aux2.count()) % 2 == 0) {
				local_bias++;//The bias value is increased if the Linear approximation occurs
			}
			else {
				local_bias--;//The bias value is decreased if the Linear approximation does not occur
			}
			//Verify the computed key is same as the local (real) key
			if (key_compute == local_key) {
				cout << "The Key was correctly computed: " << key_compute << " / ";
			}
			else {
				cout << "The Key was not correctly computed: " << key_compute << " / ";
			}
			cout << "The Correct Key: " << local_key << endl;
		}
		if (abs(local_bias) > abs(maximum_bias)) {
			maximum_bias = local_bias;
			key_compute = local_key;
		}
	}
	//Output the Key Guess and maximum bias
	cout << "Computed Key: " << key_compute.to_ulong() << endl;
	cout << "Bias Value: " << maximum_bias << endl;

	return 0;
}
