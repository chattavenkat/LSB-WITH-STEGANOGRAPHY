#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h" 
#include "decode.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2], ".bmp") == NULL){
        printf("ERROR: Stego file must be.bmp\n");
        return e_failure;
    }
    decInfo->stego_image_fname = argv[2];
    if (argv[3]!= NULL){
        decInfo->secret_fname = argv[3];
    }else{
        decInfo->secret_fname = NULL;
    }
    printf("Enter Magic String to decode: ");
    scanf("%s", decInfo->magic_string);
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo){
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL){
        printf("ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer){
    *data = 0;
    for (int i = 0; i < 8; i++){
        *data = (*data << 1) | (image_buffer[i] & 1);
    }
    return e_success;
}

Status decode_size_from_lsb(int *size, char *image_buffer){
    *size = 0;
    for (int i = 0; i < 32; i++){
        *size = (*size << 1) | (image_buffer[i] & 1);
    }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo){
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    int magic_len = strlen(decInfo->magic_string);
    char image_buffer[8];
    char magic[20];
    for (int i = 0; i < magic_len; i++){
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&magic[i], image_buffer);
    }
    magic[magic_len] = '\0';
    if(strcmp(magic, decInfo->magic_string)!= 0){
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo){
    char image_buffer[32];
    fread(image_buffer, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(&decInfo->extn_size, image_buffer);
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo){
    char image_buffer[8];
    for (int i = 0; i < decInfo->extn_size; i++){
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&decInfo->extn_secret_file[i], image_buffer);
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo){
    char image_buffer[32];
    fread(image_buffer, 1, 32, decInfo->fptr_stego_image);
    decode_size_from_lsb(&decInfo->size_secret_file, image_buffer);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo){
    char out_file[100];
    if (decInfo->secret_fname == NULL){
        strcpy(out_file, "output");
        strcat(out_file, decInfo->extn_secret_file);
    }else{
        strcpy(out_file, decInfo->secret_fname);
    }

    decInfo->fptr_secret = fopen(out_file, "wb");
    if (decInfo->fptr_secret == NULL){
        perror("fopen");
        return e_failure;
    }
    char image_buffer[8];
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++){
        fread(image_buffer, 1, 8, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&ch, image_buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_secret);
    }
    fclose(decInfo->fptr_secret);
    printf("Decoded to %s\n", out_file);
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo){
    if (open_decode_files(decInfo) == e_failure){
        return e_failure;
    }
    printf("## Decoding Started ##\n");
    printf("Decoding Magic String\n");
    if (decode_magic_string(decInfo) == e_failure){
        printf("Failed\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Decoding Extn Size\n");
    decode_secret_file_extn_size(decInfo);
    printf("Done. Size = %d\n", decInfo->extn_size);

    printf("Decoding Extn\n");
    decode_secret_file_extn(decInfo);
    printf("Done. Extn = %s\n", decInfo->extn_secret_file);

    printf("Decoding File Size\n");
    decode_secret_file_size(decInfo);
    printf("Done. Size = %d\n", decInfo->size_secret_file);

    printf("Decoding File Data\n");
    if (decode_secret_file_data(decInfo) == e_failure){
        printf("Failed\n");
        return e_failure;
    }
    printf("Done\n");

    printf("## Decoding Done Successfully ##\n");
    fclose(decInfo->fptr_stego_image);
    return e_success;
}