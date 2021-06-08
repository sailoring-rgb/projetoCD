/**
@file fsize.c
Definição do estado e das funções que o manipulam
*/

#ifndef __F_SIZE_H__
#define __F_SIZE_H__
#include <stdio.h>

#define FSIZE_DEFAULT_BLOCK_SIZE 65536          // Default block size = 512 KBytes
#define FSIZE_MIN_BLOCK_SIZE 1024                // Min block size = 512 Bytes
#define FSIZE_MAX_BLOCK_SIZE 67108864           // Max block size = 64 MBytes
#define FSIZE_MAX_NUMBER_OF_BLOCKS 4294967296   // Max number of blocks that can be returned = 2^32 blocks
#define FSIZE_MAX_SIZE_FSEEK 2147483648         // Max size permitted in fseek/ftell functions = 2 GBytes
#define FSIZE_ERROR_BLOCK_SIZE -1               // Error: Block size is larger than max value
#define FSIZE_ERROR_NUMBER_OF_BLOCKS -2         // Error: Number of Blocks exceeds max value permitted
#define FSIZE_ERROR_IN_FILE -3                  // Error: Opening or reading file
#define FSIZE_ERROR_IN_FTELL -1L                // Error: When using ftell()

long long fsize(FILE *fp_in, unsigned char *filename, unsigned long *the_block_size, long *size_of_last_block);

/*
Function fsize() to get the size of files and the number of blocks contained
in the file (for a given block size).

The function works for files already opened (and pointed by <fp_in>) and also
for files not already opened (in this case the file is identified by <filename>).

This function works on files of any size and on any operating system and on
any machine architecture. Bear in mind that for files larger than 2 GBytes the
function is very slow. Block size of FSIZE_DEFAULT_BLOCK_SIZE should yield
the best results in terms of execution time but it may be dependent of the
operating system and/or machine architecture/hardware.

The function returns directly the number of blocks contained in the file of
a given block size in <*the_block_size>. It also returns, indirectly, the
actual block size used in <*the_block_size>, which may be different than
the original size in <*the_block_size>. If the original value of <*the_block_size>
is smaller than FSIZE_MIN_BLOCK_SIZE then FSIZE_MIN_BLOCK_SIZE is used.
Also, if the original value of <*the_block_size> is bigger than FSIZE_MAX_BLOCK_SIZE
then FSIZE_MAX_BLOCK_SIZE is used. If the original value of <*the_block_size> is
equal to 0 (zero) then FSIZE_DEFAULT_BLOCK_SIZE is used. Bear in mind that the last
block may have a smaller size and its value is returned in <*size_of_last_block>.

If n_blocks is the returned number of blocks then:
   total_file_size = n_blocks * <*the_block_size> + <*size_of_last_block>

Arguments:
  FILE *fp_in - a file pointer (if NULL, then the file is identified by <filename>)
  unsigned char *filename - if NULL then file must be identified by < *fp>
  unsigned long *the_block_size - this is used to pass the intended size for each block
                                  and to receive the actual block size used
  long *size_of_last_block - this is used to receive the size of the last block

This code is open source and free to use as long as the original author is identified.

(c) 2020, Bruno A.F. Dias - University of Minho, Informatics Department
*/

#endif //F_SIZE_H