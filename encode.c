#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input Image file ptr
 * Output width * height * bytes per pixel (3 in our case)
 * Description In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs Src Image file, Secret file and
 * Stego Image file
 * Output FILE pointer for above files
 * Return Value e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo){
    if(strstr(argv[2], ".bmp") == NULL){
        return e_failure;
    }else{
        encInfo->src_image_fname = argv[2];
    }
    if(strstr(argv[3], ".txt") == NULL){
        return e_failure;
    }else{
        encInfo->secret_fname = argv[3];
    }
    if(argv[4] != NULL){
        if(strstr(argv[4], ".bmp") == NULL){
            printf("Error Stego image  must be a .bmp file\n");
            return e_failure;
        }else{
            encInfo->stego_image_fname = argv[4];
        }
    }else{
        encInfo->stego_image_fname = "stego.bmp";
    }
    printf("Enter Magic String : ");
    scanf("%s", encInfo->magic_string);
    return e_success;
}
uint get_file_size(FILE *fptr){
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    return size;
}
Status check_capacity(EncodeInfo *encInfo){
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity < ((16 + 32 + 32 + 32 )+ (encInfo->size_secret_file * 8))){
        return e_failure;
    }
    return e_success;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    char header[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    if(fread(header, 1, 54, fptr_src_image) != 54){
        return e_failure;
    }
    if(fwrite(header, 1, 54, fptr_dest_image) != 54){
        return e_failure;
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer){
    for(int i = 0; i < 8; i++){
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
    return e_success;
}
Status encode_size_to_lsb(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char image_buffer[8];
    for(int i = 0; i < size; i++){
        fread(image_buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        fwrite(image_buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){
    encode_size_to_lsb((char *)magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo){
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    for(int i = 0; i < 32; i++){
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((size >> (31 - i)) & 1);
    }
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){
    const char *extn = strrchr(file_extn, '.');
    if(extn == NULL){
        extn = ".txt";
    }
    encode_secret_file_extn_size(strlen(extn), encInfo);

    encode_size_to_lsb((char *)extn, strlen(extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo){
    char image_buffer[32];
    fread(image_buffer, 1, 32, encInfo->fptr_src_image);
    for(int i = 0; i < 32; i++){
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((file_size >> (31 - i)) & 1);
    }
    fwrite(image_buffer, 1, 32, encInfo->fptr_stego_image);

    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo){
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    char ch;
    for(int i = 0; i < encInfo->size_secret_file; i++){
        fread(&ch, 1, 1, encInfo->fptr_secret);
        char buffer[8];
        fread(buffer, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(ch, buffer);
        fwrite(buffer, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo){
    if(open_files(encInfo) == e_failure){
        printf("ERROR Failed to open files\n");
        return e_failure;
    }else{
        printf("Opened %s\n", encInfo->src_image_fname);
        printf("Opened %s\n", encInfo->secret_fname);
        printf("Opened %s\n", encInfo->stego_image_fname);
        printf("Done\n");
    }

    printf("## Encoding Procedure Started ##\n");
    
    printf("Checking for %s size\n", encInfo->secret_fname);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo->size_secret_file == 0){
        printf("ERROR %s is empty\n", encInfo->secret_fname);
        return e_failure;
    }
    printf("Done. Not Empty\n");

    printf("Checking for %s capacity to handle %s\n", encInfo->src_image_fname, encInfo->secret_fname);
    if(check_capacity(encInfo) == e_failure){
        printf("ERROR %s does not have enough capacity\n", encInfo->src_image_fname);
        return e_failure;
    }
    printf("Done. Found OK\n");

    printf("Copying Image Header\n");
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure){
        printf("ERROR Failed to copy header\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Encoding Magic String Signature\n");
    if(encode_magic_string(encInfo->magic_string, encInfo) == e_failure){
        printf("ERROR Failed to encode Magic String\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Encoding %s File Extenstion\n", encInfo->secret_fname);
    if(encode_secret_file_extn(encInfo->secret_fname, encInfo) == e_failure){
        printf("ERROR Failed to encode file extension\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Encoding %s File Size\n", encInfo->secret_fname);
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure){
        printf("ERROR Failed to encode file size\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Encoding %s File Data\n", encInfo->secret_fname);
    if(encode_secret_file_data(encInfo) == e_failure){
        printf("ERROR Failed to encode file data\n");
        return e_failure;
    }
    printf("Done\n");

    printf("Copying Left Over Data\n");
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure){
        printf("ERROR Failed to copy remaining data\n");
        return e_failure;
    }
    printf("Done\n");

    printf("## Encoding Done Successfully ##\n");
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0){
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}