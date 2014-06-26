#include "zip.h"
#include <zlib.h>
#include "common.h"

unsigned int s_uEncryptedPvrKeyParts[4] = {0,0,0,0};
unsigned int s_uEncryptionKey[1024];
bool s_bEncryptionKeyIsValid = false;

static void ccDecodeEncodedPvr(unsigned int *data, int len)
{
    const int enclen = 1024;
    const int securelen = 512;
    const int distance = 64;
    
    // create long key
    if(!s_bEncryptionKeyIsValid)
    {
        unsigned int y, p, e;
        unsigned int rounds = 6;
        unsigned int sum = 0;
        unsigned int z = s_uEncryptionKey[enclen-1];
        
        do
        {
#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (s_uEncryptedPvrKeyParts[(p&3)^e] ^ z)))
            
            sum += DELTA;
            e = (sum >> 2) & 3;
            
            for (p = 0; p < enclen - 1; p++)
            {
                y = s_uEncryptionKey[p + 1];
                z = s_uEncryptionKey[p] += MX;
            }
            
            y = s_uEncryptionKey[0];
            z = s_uEncryptionKey[enclen - 1] += MX;
            
        } while (--rounds);
        
        s_bEncryptionKeyIsValid = true;
    }
    
    int b = 0;
    int i = 0;
    
    // encrypt first part completely
    for(; i < len && i < securelen; i++)
    {
        data[i] ^= s_uEncryptionKey[b++];
        
        if(b >= enclen)
        {
            b = 0;
        }
    }
    
    // encrypt second section partially
    for(; i < len; i += distance)
    {
        data[i] ^= s_uEncryptionKey[b++];
        
        if(b >= enclen)
        {
            b = 0;
        }
    }
}

int ccInflateCCZFile(char *compressed, unsigned long fileLen, unsigned char **out) {
    if(NULL == compressed || 0 == fileLen)
    {
        NSLog(@"cocos2d: Error loading CCZ compressed file");
        return -1;
    }
    
    struct CCZHeader *header = (struct CCZHeader*) compressed;
    
    // verify header
    if( header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && header->sig[3] == '!' )
    {
        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST( header->version );
        if( version > 2 )
        {
            NSLog(@"cocos2d: Unsupported CCZ header format");
            return -1;
        }
        
        // verify compression format
        if( CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB )
        {
            NSLog(@"cocos2d: CCZ Unsupported compression method");
            return -1;
        }
    }
    else if( header->sig[0] == 'C' && header->sig[1] == 'C' && header->sig[2] == 'Z' && header->sig[3] == 'p' )
    {
        // encrypted ccz file
        header = (struct CCZHeader*) compressed;
        
        // verify header version
        unsigned int version = CC_SWAP_INT16_BIG_TO_HOST( header->version );
        if( version > 0 )
        {
            NSLog(@"cocos2d: Unsupported CCZ header format");
            return -1;
        }
        
        // verify compression format
        if( CC_SWAP_INT16_BIG_TO_HOST(header->compression_type) != CCZ_COMPRESSION_ZLIB )
        {
            NSLog(@"cocos2d: CCZ Unsupported compression method");
            return -1;
        }
        
        // decrypt
        unsigned int* ints = (unsigned int*)(compressed+12);
        int enclen = (fileLen-12)/4;
        
        ccDecodeEncodedPvr(ints, enclen);
    }
    else
    {
        NSLog(@"cocos2d: Invalid CCZ file");
        return -1;
    }
    
    unsigned int len = CC_SWAP_INT32_BIG_TO_HOST( header->len );
    
    *out = (unsigned char*)malloc( len );
    if(! *out )
    {
        NSLog(@"cocos2d: CCZ: Failed to allocate memory for texture");
        return -1;
    }
    
    unsigned long destlen = len;
    unsigned long source = (unsigned long) compressed + sizeof(*header);
    int ret = uncompress(*out, &destlen, (Bytef*)source, fileLen - sizeof(*header) );
    
    if( ret != Z_OK )
    {
        NSLog(@"cocos2d: CCZ: Failed to uncompress data");
        free( *out );
        *out = NULL;
        return -1;
    }
    
    return len;
}

int ccInflateGZipFile(const char *path, unsigned char **out)
{
    int len;
    unsigned int offset = 0;
    
    gzFile inFile = gzopen(path, "rb");
    if( inFile == NULL ) {
        NSLog(@"cocos2d: ZipUtils: error open gzip file: %s", path);
        return -1;
    }
    
    /* 512k initial decompress buffer */
    unsigned int bufferSize = 512 * 1024;
    unsigned int totalBufferSize = bufferSize;
    
    *out = (unsigned char*)malloc( bufferSize );
    if( ! out )
    {
        NSLog(@"cocos2d: ZipUtils: out of memory");
        return -1;
    }
    
    for (;;) {
        len = gzread(inFile, *out + offset, bufferSize);
        if (len < 0)
        {
            NSLog(@"cocos2d: ZipUtils: error in gzread");
            free( *out );
            *out = NULL;
            return -1;
        }
        if (len == 0)
        {
            break;
        }
        
        offset += len;
        
        // finish reading the file
        if( (unsigned int)len < bufferSize )
        {
            break;
        }
        
        bufferSize *= BUFFER_INC_FACTOR;
        totalBufferSize += bufferSize;
        unsigned char *tmp = (unsigned char*)realloc(*out, totalBufferSize );
        
        if( ! tmp )
        {
            NSLog(@"cocos2d: ZipUtils: out of memory");
            free( *out );
            *out = NULL;
            return -1;
        }
        
        *out = tmp;
    }
    
    if (gzclose(inFile) != Z_OK)
    {
        NSLog(@"cocos2d: ZipUtils: gzclose failed");
    }
    
    return offset;
}