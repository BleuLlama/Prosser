
#include <stdio.h>

#include <errno.h>
#include <iostream>	// for MakeDir
#include <string>	// for MakeDir
#include <sys/stat.h>	// for MakeDir

#if defined _WIN32 || defined(__MINGW32__)
#include "direct.h"
#include <io.h>
#endif

// for directoruylistings
#include <sys/types.h>
#include <sys/dir.h>
#include <vector>

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

#if defined _WIN32 || defined(__MINGW32__)

#define OS_MKDIR( P, M ) \
	_mkdir( P )

#else

#define OS_MKDIR( P, M ) \
	mkdir( P, M )
#endif

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
	    if((mdret=OS_MKDIR(dir.c_str(),mode)) && errno!=EEXIST){
		return; // mdret;
	    }
	}
	//return mdret;
}

bool Utils::DirectoryExists( std::string path )
{
	struct stat st;
	if( stat( path.c_str(), &st ) == 0 ) {
		if( st.st_mode & S_IFDIR != 0 ) {
			return true;
		}
	}
	return false;
}

void Utils::DirectoryListing( std::string directory, 
		  std::vector<std::string>& listing )
{
	listing.clear();
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
    	return; /* No files found */

    do {
    	const std::string file_name = file_data.cFileName;
    	const std::string full_file_name = directory + "/" + file_name;

    	if (file_name[0] == '.')
    		continue;

    	//listing.push_back(full_file_name);
    	listing.push_back(file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
    	const std::string file_name = ent->d_name;
    	const std::string full_file_name = directory + "/" + file_name;

    	if (file_name[0] == '.')
    		continue;

    	if (stat(full_file_name.c_str(), &st) == -1)
    		continue;

    	//listing.push_back(full_file_name);
    	listing.push_back(file_name);
    }
    closedir(dir);
#endif

}
