# one-time-pad-encryption
The one-time-pad encryption method uses a key given by the user to perform operations on each byte of the data to be encrypted. Usually, the operation to encrypt the data is an XOR of each byte of the data and the key. Unless you know the key, it is impossible to decrypt the data. Of course, the problem with the one-time-pad is sharing the key with whoever you allow to decrypt it. That problem is not solved in this project, but usually a trusted carrier is employed to send the Key.
# Details
The one_time_pad_project.c is the main file of this project. Depending on the mode selected (either ENCRYPT or DECRYPT) the program will read the input file in binary and XOR each byte with the key to encrypt it, or reverse the operation to decrypt it. It will then create a new encrypted or decrypted file of the name input_file_enc or input_file_dec depending on if the mode is ENCRPYT or DECRYPT.
# Program Argument Format
To run this program through the command line type:
ProgramName (one_time_pad unless you change it) key (any integer, 0 will not work) mode (either ENCRYPT or DECRYPT) infile (name of the file to be encrypted)



