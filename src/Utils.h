
#include <iostream>

class Utils{
public:
	Utils();
	~Utils();

public:
	static bool FileExists( std::string path );
	static void FileCopy( std::string src, std::string dest );
};
