#ifndef __zip__
#define __zip__

#define BUFFER_INC_FACTOR (2)

struct CCZHeader {
	unsigned char   sig[4];             // signature. Should be 'CCZ!' 4 bytes
	unsigned short  compression_type;   // should 0
	unsigned short  version;            // should be 2 (although version type==1 is also supported)
	unsigned int    reserved;           // Reserved for users.
	unsigned int    len;                // size of the uncompressed file
};

enum {
	CCZ_COMPRESSION_ZLIB,               // zlib format.
	CCZ_COMPRESSION_BZIP2,              // bzip2 format (not supported yet)
	CCZ_COMPRESSION_GZIP,               // gzip format (not supported yet)
	CCZ_COMPRESSION_NONE,               // plain (not supported yet)
};

extern int ccInflateCCZFile(char *compressed, unsigned long fileLen, unsigned char **out);
extern int ccInflateGZipFile(const char *path, unsigned char **out);

#endif /* defined(__zip__) */
