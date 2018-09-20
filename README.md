# CSCI-4230 Toy_DES

In this programm, a simpe DES encrytion and decrytion system was implemented. 
Unlike an official approach of DES, this programm simplified the process
by following the instruction of a DES model that works on 8-bit blocks 
and uses 10-bit key. The number of the encryption round is reduced to 
two as well. The working process of the program is shown below.

## Getting Started

The Program takes in 4 command line arguments. The first argument is the 
input file that you want to encrypt or decrypt. The second argument is 
the output file that stores the encrpyted or decrypted data. The third 
argument is either "--encrypt" or "--decrpyted" command base on what
you want. The fourth argument is the key to encrypt or decrypt the first
file. The key is a 10-bit long binary. Failure to follow the format of
command line argument will result in recieving ERROR messages. Here is 
a sample command line arguments.

    ./DES.exe plain_text.txt encrypted_text.txt --encrypt 0010110010

## Explanation & Arithmetic

The program contains mainly two parts:
The fist part is the sub keys generator. The generator first permutate 
the initial key, then divide it into a left half bitset and a right 
half bitset and finally do two rounds of left shifting and permutation 
to generator two sub keys.

The Second part is the encryption/decryption part. Because of the 
feature of DES. The decryption process is simply a reverse version of 
encryption. The encryption process initially permute the plain text,
then divide it into a left half bitset and a right half bitset. Next,
the left half bitset and right half bitset walk through the F function.
After many operations, the left and right half are combined into a single
bitset. Finally we do an inverse initial permutation on the bitset to
get the cipher text.

## Deployment

C++ with manjaro_linux OS


## Authors

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.


