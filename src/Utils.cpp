
#include <stdio.h>
#include "Utils.h"


Utils::Utils( void )
{
}

Utils::~Utils( void )

{
}

bool Utils::FileExists( std::string strPath )
{
       FILE * fp;
       fp = fopen( strPath.c_str(), "rb" );
       if( fp ) {
               fclose( fp );
               return true;
       }
       return false;
}


void Utils::FileCopy( std::string fromPath, std::string toPath )
{
#define kCopyFileBufSize        ((4096) * 64)

        // HACK: This will break on non-windows, but seems ok for now.
        //fromPath = Utils::SearchReplace( fromPath, "/", "\\" );

        char buf[kCopyFileBufSize];
        FILE * inf = fopen( fromPath.c_str(), "rb" );
        if( !inf ) {
                std::cerr << "Unable to open source " << fromPath << " (to " << toPath << ")" << std::endl;
                return;
        }
        FILE * outf = fopen( toPath.c_str(), "wb" );
        if( !outf ) {
                std::cerr << "Unable to open dest " << toPath << std::endl;
                fclose( inf );
                return;
        };

        size_t n = 0;
        do {
                n = fread( buf, 1, kCopyFileBufSize, inf );
                fwrite( buf, n, 1, outf );
        } while( n != 0 );

        fclose( outf );
        fclose( inf );
}
