
#include <stdio.h>

#include <errno.h>
#include <iostream>	// for MakeDir
#include <string>	// for MakeDir
#include <sys/stat.h>	// for MakeDir

#include <iostream>
#include <fstream>

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


void Utils::MakeDir( std::string path, int mode )
{
	// code swiped from http://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
	size_t pre=0,pos;
	std::string dir;
	int mdret;

	if(path[path.size()-1]!='/'){
	    // force trailing / so we can handle everything in loop
	    path+='/';
	}

	while((pos=path.find_first_of('/',pre))!=std::string::npos){
	    dir=path.substr(0,pos++);
	    pre=pos;
	    if(dir.size()==0) continue; // if leading / first time is 0 length
	    if((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST){
		return; // mdret;
	    }
	}
	//return mdret;
}
