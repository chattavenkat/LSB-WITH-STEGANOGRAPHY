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
