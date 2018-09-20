// File DES.cpp
// This program implements a simple DES encryption and decryption process
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <bitset>

// Key and subKeys
std::bitset<10> key;
std::bitset<8> subKeys[2];

// Initial Permutation and invese Initial Permutation
int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP_inverse[8] = {4, 1, 3, 5, 7, 2, 8, 6};

// Permutation of 10 bits
int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
// 8 permutation of 10 bits
int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};


// Expand permutation
int E[8] = {4, 1, 2, 3, 2, 3, 4, 1};

// P4 permutation
int P[4] = {2, 4, 3, 1};

// A simple data structure to store S-Boxes
int S[2][4][4] = {
  {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
  },
  {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
  }
};

// Bits shifted at each round
int bits_shifted[] = {1, 2}; 


// left shift operation
std::bitset<5> left_shift(std::bitset<5> k, int n) {
  std::bitset<5> tmp = k;
  for (int i=4; i>=0; i--) {
  	if (i-n<0) {
  	  k[i] = tmp[i-n+5];
  	} else {
  	  k[i] = tmp[i-n];
  	}
  }
  return k;
} 

// convert character to bitset
std::bitset<8> char_to_bitset(const char c){
  return std::bitset<8>(c);
}

// generate subKeys
void generate_subKeys() {
  std::bitset<10> original_key;
  std::bitset<5> left;
  std::bitset<5> right;
  std::bitset<8> compressed_key;
  
  // Permutate the initial key
  for (int i=0; i<10; i++) {
  	original_key[9-i] = key[10-P10[i]];
  }

  for (int round=0; round<2; round++) {
  	// Divide the permutated key into LH and RH
  	for (int i=5; i<10; i++) {
  	  left[i-5] = original_key[i];
  	}
  	for (int i=0; i<5; i++) {
  	  right[i] = original_key[i];
  	}

    // Left shift the left and right bitset
  	left = left_shift(left, bits_shifted[round]);
  	right = left_shift(right, bits_shifted[round]);

    // Combine the left and right bitset
  	for (int i=5; i<10; i++) {
  	  original_key[i] = left[i-5];
  	}
  	for (int i=0; i<5; i++) {
  	  original_key[i] = right[i];
  	}

  	// Compress the key 
  	for (int i=0; i<8; i++) {
      compressed_key[7-i] = original_key[10-P8[i]];
  	}

  	// Store the compressed key in subKeys
  	subKeys[round] = compressed_key;
  }
}

// F function
std::bitset<4> F(std::bitset<4> input, std::bitset<8> key) {

  std::bitset<8> expanded_input;

  // Expand operation, expand the input from 4 to 8 bits
  for (int i=0; i<8; i++) {
  	expanded_input[7-i] = input[4-E[i]];
  }

  // XOR operation

  expanded_input = expanded_input ^ key;

  // Sbox operation 

  std::bitset<4> out;

  // In each of two rounds of DES Round Structure,
  // computes the row and column on the S-Box
  int row, col, n;
  row = expanded_input[7]*2 + expanded_input[4];
  col = expanded_input[6]*2 + expanded_input[5];
  n = S[0][row][col];
  std::bitset<2> binary(n);
  out[3] = binary[1];
  out[2] = binary[0];

  row = expanded_input[3]*2 + expanded_input[0];
  col = expanded_input[2]*2 + expanded_input[1];
  n = S[1][row][col];
  std::bitset<2> binary2(n);
  out[1] = binary2[1];
  out[0] = binary2[0];

  std::bitset<4> tmp = out;

  // Final permutation in function F
  for (int i=0; i<4; i++) {
  	out[3-i] = tmp[4-P[i]];
  }
  return out;
}

// Encrypt function
std::bitset<8> encrypt(std::bitset<8> &plain) {
  // Basic biset structures
  std::bitset<8> cipher;
  std::bitset<8> current;
  std::bitset<4> left;
  std::bitset<4> right;
  std::bitset<4> tmp;

  // Initial Permutation
  for (int i=0; i<8; i++) {
  	current[7-i] = plain[8-IP[i]];
  }

  // Find Left half and right half of current bitset
  for (int i=4; i<8; i++) {
  	left[i-4] = current[i];
  }
  for (int i=0; i<4; i++) {
  	right[i] = current[i];
  }

  // DES Round Structure
  for (int round=0; round<2; round++) {
  	tmp = right;
  	right = left ^ F(right, subKeys[round]);
  	left = tmp;
  }

  // Combine the left and right bitset to cipher
  for (int i=0; i<4; i++) {
  	cipher[i] = left[i];
  }
  for (int i=4; i<8; i++) {
  	cipher[i] = right[i-4];
  }

  // Disable the accessability of current bitset
  current = cipher;

  // Inverse initial permutation
  for (int i=0; i<8; i++) {
  	cipher[7-i] = current[8-IP_inverse[i]];
  }

  return cipher;
}

// Decrypt function
std::bitset<8> decrypt(std::bitset<8> &cipher) {
  // Basic biset structures
  std::bitset<8> plain;
  std::bitset<8> current;
  std::bitset<4> left;
  std::bitset<4> right;
  std::bitset<4> tmp;

  // Initial Permutation
  for (int i=0; i<8; i++) {
  	current[7-i] = cipher[8-IP[i]];
  }

  // Find Left half and right half of current bitset
  for (int i=4; i<8; i++) {
  	left[i-4] = current[i];
  }
  for (int i=0; i<4; i++) {
  	right[i] = current[i];
  }


  // DES Round Structure (but in a reverse direction)
  for (int round=0; round<2; round++) {
  	tmp = right;
  	right = left ^ F(right, subKeys[1-round]);
  	left = tmp;
  }

  // Combine the left and right bitset to plain
  for (int i=0; i<4; i++) {
  	plain[i] = left[i];
  }
  for (int i=4; i<8; i++) {
  	plain[i] = right[i-4];
  }

  // Disable the accessability of current bitset
  current = plain;

  // Inverse initial permutation
  for (int i=0; i<8; i++) {
  	plain[7-i] = current[8-IP_inverse[i]];
  }

  return plain;
}

int main(int argc, char * argv[]){
  
  // Checking command line arguments
  if (argc != 5) {
  	std::cerr << "ERROR: Invalid command line arguments!" << std::endl;
  	std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
  	return 1;
  } else {
    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2]);
    std::string s = argv[4];
    if (s.length() != 10) {
      std::cerr << "ERROR: Invalid key length!" << std::endl;
  	  std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
  	  return 1;
    }

    bool valid = true;
    for (unsigned int i=0; i<s.length(); i++) {
      if (!(s[i] == '0' || s[i] == '1')) {
      	valid = false;
      }
    }

    if(!valid) {
      std::cerr << "ERROR: Invalid key!" << std::endl;
  	  std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
  	  return 1;
    }
    
    // Record key
    for (unsigned int i=0; i<10; i++) {
      if (s[i] == '0') {
      	key[9-i] = 0;
      } else {
      	key[9-i] = 1;
      }
    }

    // check if the files are good 
    if (!input_file.good() || !output_file.good()) {
  	  std::cerr << "ERROR: Invalid file input!" << std::endl;
  	  std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
      return 1;
    }

    assert(input_file.good());
    assert(output_file.good());

    // generate subKeys
    generate_subKeys();

    // --encrypt argument 
    if (std::string(argv[3]) == "--encrypt") {      
      unsigned char c;
      while(input_file >> std::noskipws >> c) {
      	std::bitset<8> plain = char_to_bitset(c);
      	std::bitset<8> cipher = encrypt(plain);
      	unsigned long i = cipher.to_ulong();
      	unsigned char c2 = static_cast<unsigned char>(i);
      	output_file << c2;
      }
    } else {
      std::cerr << "ERROR: Invalid command line arguments!" << std::endl;
  	  std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
  	  return 1;
    }

    // --decrypt argument
    if (std::string(argv[3]) == "--decrypt") {
      unsigned char c;
      while(input_file >> std::noskipws >> c) {
        std::bitset<8> cipher = char_to_bitset(c);
        //std::cout << plain;
        std::bitset<8> plain = decrypt(cipher);
        unsigned long i = plain.to_ulong();
        unsigned char c2 = static_cast<unsigned char>(i);
        output_file << c2;
      }
    } else {
      std::cerr << "ERROR: Invalid command line arguments!" << std::endl;
  	  std::cout << "Usage: 1.plain_text_file 2.output_file 3.encrypt or decrypt command 4.key(10 binary numbers)" << std::endl;
      return 1;
    }
  }
  return 0;
}
