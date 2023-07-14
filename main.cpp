#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

//To invert the input value, we write a helper function
unsigned char invert(unsigned char value) {
	return ~value;
}

//A shift funtion to shift the input values to the left
unsigned char shift_left(unsigned char value) {
	return ((value << 1) & 0x0F) | ((value >> 3) & 0X01);
}

//A function to do a simple encryption shifting the message to the left then inverting
unsigned char cipher(unsigned char message, unsigned char key) {
	return shift_left(invert(message ^ key));
}

int main() {


	unsigned char message1 = 0X02; //First plain message
	unsigned char message2 = 0X03;// Second plain message
	unsigned char key = 0X06;//The secret key

	//Computing the encrypted messages
	unsigned char encrypted_message1 = cipher(message1, key);
	unsigned char encrypted_message2 = cipher(message2, key);

	int pair_no = 10000;
	int total_ok_pairs = 0; // Total number of pairs satisfying the condition

	//Success of probability
	double threshold = 0;

	//Attack Code
	for (int k = 0; k < 16; k++) {
		unsigned char computed_key = k;
		int different_pairs = 0;
		int ok_pairs = 0;

		//Computing the number of differential pairs that satisfy the above condition
		for (int j = 0; j < pair_no; j++) {
			//Generate two randodm plain messages
			unsigned char aux_message1 = rand() % 16;
			unsigned char aux_message2 = aux_message1 ^ message1;

			//Computing the corresponding ciphertexts using the computed key
			unsigned char aux_encrypted_message1 = cipher(aux_message1, computed_key);
			unsigned char aux_encrypted_message2 = cipher(aux_message2, computed_key);

			//Check the satisfiability of the pairs
			if ((aux_encrypted_message1 ^ aux_encrypted_message2) == (encrypted_message1 ^ encrypted_message2)) {
				ok_pairs++;
			}
			different_pairs++;
		}

		//Probability of correctly guessing the computed key
		double probability = (double)ok_pairs / different_pairs;

		if (probability > threshold) {
			total_ok_pairs++;
			cout << "Computed Key:" << hex << (int)computed_key << ", probability of Success: " << probability << endl;
		}
	}

	//Verification of the computed key is the same as the correct key
	if (total_ok_pairs > 0) {
		cout << "Correct Key: " << hex << (int)key << ".found" << endl;
	}
	else {
		cout << "Correct Key not found." << endl;
	}
	return 0;
}