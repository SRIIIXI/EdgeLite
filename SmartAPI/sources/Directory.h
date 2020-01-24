#ifndef _DIRECTORY_UTIL
#define _DIRECTORY_UTIL

#include <time.h>
#include <vector>
#include <string>

using namespace std;

class DirEntry
{
public:
	std::string Name;
	std::string FullPath;
	time_t CreationTime;
	time_t LastModifiedTime;
};

extern void dirgetparentdirectory(const std::string& str, std::string& pstr);
extern void dirgetextension(const std::string& str, std::string& ext);
extern void dirgetname(const std::string& fullpath, std::string& str);
extern void dircreatedirectory(const std::string& str);
extern bool dirisdirectory(const std::string& str);
extern bool dirfileexists(const std::string& str);
extern void dirgetdirectorylist(const std::string& dirname, std::vector<DirEntry>& dlist);
extern void dirgetfilelist(const std::string& dirname, std::vector<DirEntry>& dlist, const std::string& extension);
extern void dircurrentdirectory(std::string& str);
#endif

