#ifndef DECODE_H
#define DECODE_H

#include "types.h" // e_success, e_failure, uint

typedef struct _DecodeInfo
{
    /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    int extn_size;
    char extn_secret_file[10];
    int size_secret_file;
    /* Magic String */
    char magic_string[50];

} DecodeInfo;

/* Function prototypes */

/* Read and validate decode args */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open files for decoding */
Status open_decode_files(DecodeInfo *decInfo);

/* Do decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a byte from LSB of image buffer */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode size (32 bits) from LSB */
Status decode_size_from_lsb(int *size, char *image_buffer);

#endif