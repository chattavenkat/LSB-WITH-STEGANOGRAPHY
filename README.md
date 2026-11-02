# LSB Image Steganography Using C

## Project Overview

LSB (Least Significant Bit) Image Steganography is a technique used to
hide secret information inside a BMP image without significantly
changing its visual appearance.

This project is developed using the C programming language. It allows
users to encode a secret text file into a BMP image and decode the
hidden information from the stego image.

The project uses the Least Significant Bit (LSB) technique to embed and
extract secret data.

## Features

### Encoding

1.  Read and validate command-line arguments.
2.  Open the source BMP image and secret text file.
3.  Check whether the image has sufficient capacity.
4.  Copy the BMP image header.
5.  Encode the magic string signature.
6.  Encode the secret file extension.
7.  Encode the secret file size.
8.  Encode the secret file data into image pixels.
9.  Copy the remaining image data.
10. Generate the stego image.

### Decoding

1.  Read and validate decoding arguments.
2.  Open the stego BMP image.
3.  Verify the magic string signature.
4.  Decode the secret file extension.
5.  Decode the secret file size.
6.  Extract the hidden secret file data.
7.  Save the decoded file.

## Technologies Used

-   **Programming Language:** C
-   **Concepts:** File Handling, Pointers, Structures, Enumerations
-   **Bitwise Operations:** Bit Manipulation
-   **Image Format:** BMP
-   **Steganography Technique:** Least Significant Bit (LSB)

## Project Structure

``` text
LSB-With-Steganography/
├── main.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.h
├── types.h
├── beautiful.bmp
├── secret.txt
├── stego.bmp
└── README.md
```

## How It Works

### Encoding Process

The secret file is converted into binary data. Each bit is embedded into
the least significant bit of the BMP image data.

``` text
Source BMP Image
       |
       v
Read Image Data
       |
       v
Check Image Capacity
       |
       v
Encode Magic String
       |
       v
Encode File Extension
       |
       v
Encode File Size
       |
       v
Encode Secret Data
       |
       v
Generate Stego Image
```

### Decoding Process

The hidden information is extracted from the least significant bits of
the stego image.

``` text
Stego BMP Image
       |
       v
Read Image Data
       |
       v
Decode Magic String
       |
       v
Decode File Extension
       |
       v
Decode File Size
       |
       v
Extract Secret Data
       |
       v
Generate Output File
```

## Compilation

Compile the source files using GCC:

``` bash
gcc main.c encode.c decode.c -o steganography
```

## Execution

### 1. Encoding

``` bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

-   `-e` -- Encoding operation
-   `beautiful.bmp` -- Source image
-   `secret.txt` -- Secret file
-   `stego.bmp` -- Output stego image

### 2. Decoding

``` bash
./steganography -d stego.bmp output.txt
```

-   `-d` -- Decoding operation
-   `stego.bmp` -- Image containing hidden information
-   `output.txt` -- Extracted secret file

*Note: The program also supports a default output filename if no output
filename is provided.*

## Applications

-   Secure data hiding
-   Confidential file sharing
-   Digital communication
-   Information security
-   Data protection

## Learning Outcomes

-   Understanding LSB-based steganography.
-   Working with BMP image files.
-   Implementing file handling in C.
-   Applying bitwise operations.
-   Using structures and enumerations.
-   Developing encoding and decoding algorithms.

## Future Enhancements

-   Support for multiple file formats.
-   Improved input validation and error handling.
-   Support for larger secret files.
-   Enhanced security through encryption.

## Author

**Chatta Venkata Narasimha**
