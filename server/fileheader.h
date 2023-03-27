#ifndef FILEHEADER_H
#define FILEHEADER_H

#include <stdint.h> 

#define HEADER_BUF_SIZE 512
#define FILE_BUF_SIZE 1024

#pragma pack(push, 1)
typedef struct fileheader {
    char buf[HEADER_BUF_SIZE];
} fh;
#pragma pack(pop)

#endif