/*
Name :- Chatta Venkata Narasimha
Batch No :- 26015B
Date :- 07/10/2026
PROJECT :- LSB WITH IMAGE STEGANOGRAPHY 

DESCRIPTION :- 
    The "IMAGE STEGANOGRAPHY" is a C Language based program. This is a technique
used to hide secret information inside a BMP image without changing the visual
quality of the image. This is done by using LSB (Least Significant Bit) encoding.
In this project, a secret text file is hidden inside a .bmp image and a new stego
image is created. The program provides two main operations:
    1. Encoding (-e)
    2. Decoding (-d)
    By selecting -e option, the user can hide data, and by selecting -d option, the
user can extract the hidden data from the stego image.

    The Encoding procedure checks for file validation of .bmp and .txt files,
checks the image capacity using width*height*3, copies the 54-byte BMP header,
encodes magic string signature, secret file extension size and extension, secret
file size and secret file data into the LSB of image bytes, and copies the remaining
image data to create the stego image.

    The Decoding procedure validates the stego image, decodes and verifies the
magic string, decodes file extension and file size, and finally decodes the secret
file data to recreate the original secret file.

    The program uses file handling to handle binary files and uses bitwise
operations for LSB manipulation. The secret data is hidden in such a way that the
human eye cannot identify the difference between original and stego image.

    This project demonstrates important C programming concepts such as FILE HANDLING
in Binary Mode, BITWISE OPERATORS, STRUCTURES, POINTERS, FUNCTIONS, STRINGS,
COMMAND LINE ARGUMENTS, and BMP FILE FORMAT.
*/
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "encode.h"
#include "types.h"

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Usage:\n");
        printf("%s -e <src.bmp> <secret.txt> [stego.bmp]\n", argv[0]);
        printf("%s -d <stego.bmp> [output file]\n", argv[0]);
        return 1;
    }
    OperationType operation = check_operation_type(argv);
    if (operation == e_encode){
        EncodeInfo encInfo;
        printf("Opening required files for encoding\n");
        if (read_and_validate_encode_args(argv, &encInfo) == e_success){
            if (do_encoding(&encInfo) == e_success){
                printf("Encoding completed\n");
            }else{
                printf("ERROR: Encoding Failed\n");
            }
        }else{
            printf("ERROR: Read and validate failed\n");
        }
    }else if (operation == e_decode) {
        DecodeInfo decInfo;
        printf("Checking required files for decoding\n");

        if (read_and_validate_decode_args(argv, &decInfo) == e_success){
            if (do_decoding(&decInfo) == e_success){
                printf("Decoding completed successfully\n");
            }else{
                printf("ERROR: Decoding Failed\n");
                return 1;
            }
        }else{
            printf("ERROR: Read and validate failed\n");
            return 1;
        }
    }else{
        printf("ERROR: Unsupported operation. Use -e for encode and -d for decode\n");
        return 1;
    }

    return 0;
}
OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1], "-e") == 0){
        return e_encode;
    }else if(strcmp(argv[1], "-d") == 0){
        return e_decode;
    }else{
        return e_unsupported;
    }
}
