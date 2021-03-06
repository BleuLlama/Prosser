
#include <iostream>

class Utils{
public:
	Utils();
	~Utils();

public:
	static bool FileExists( std::string path );
	static void FileCopy( std::string src, std::string dest );

public:
	static void MakeDir( std::string path, int mode = 0755 );

public:
	static bool DirectoryExists( std::string path );
	static void DirectoryListing( std::string path, 
			  std::vector<std::string>& listing );
};
