#ifndef _PLAYLIST_GEN
#define _PLAYLIST_GEN

#include <curl/curl.h>
#include <string>
#include <list>
#include <vector>
#include "IPCamStructs.h"
#include "Retriever.h"
#include "DbLocal.h"

#undef min
#undef max

class PlaylistGen
{
public:
    PlaylistGen(Camera cam);
    ~PlaylistGen();

    bool downloadFilelist();
	bool writeOutPlaylist(std::string &pdir, std::string &rdir, std::string &vdir, DbLocal *dbptr);
    bool updateLastAccessTime(DbLocal* dbptr);

private:
	bool getAllRecordingSets();
	bool getDailyRecordingSets(std::string &dirdate);
	bool getRecordingSets(std::string &dirdate, std::string &set);

	bool processAllRecordingSets(std::string &urlContent);
	bool processDailyRecordingSets(std::string &urlContent, std::string parentdir);
	bool processRecordSets(std::string &urlContent, std::string parentdir);

    void normalizeTimestamp(std::string &str);

    Camera _Camera;
	Retriever _Retreiver;
    std::list<Recording> _RequestList;
};

#endif // STORAGE_INTERFACE_H
