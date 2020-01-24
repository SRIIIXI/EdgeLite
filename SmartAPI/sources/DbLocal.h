#ifndef _DB_LOCAL
#define _DB_LOCAL

#include <string>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include "IPCamStructs.h"
#include "sqlite3.h"

typedef std::list<std::string> Row;
typedef std::list<Row> DataSet;

class DbLocal
{
public:
    DbLocal();
    ~DbLocal();

	bool createSite(std::vector<std::string> &fieldlist);
	bool createCamera(std::vector <std::string> &fieldlist);
	bool createRecording(std::vector <std::string> &fieldlist);
	bool createRecording(Recording &dnl);

	bool updateSite(std::vector <std::string> &fieldlist, std::string objectid);
	bool updateCamera(std::vector <std::string> &fieldlist, std::string objectid);
	bool updateRecording(std::vector <std::string> &fieldlist, std::string objectid);

	bool deleteSite(std::string &siteid);
	bool deleteCamera(std::string &camid);
	bool deleteRecording(std::string &requestid);

	bool getSites(std::string &sitelist);
	bool getCameras(std::string &camlist, std::string &sitename);
	bool getRecording(std::string &downloadrequest, std::string &camid);
	bool getCamera(std::string& camlist, std::string& camid);

	bool getSites(std::list<Site>& sitelist);
	bool getCameras(std::list<Camera> &camlist, std::string& sitename);
	bool getRecording(std::list<Recording> &downloadrequestlist, std::string& camid);

	bool updateRecordingStatus(std::string &request_id, std::string status);
	bool updateLastAccessTime(std::string &cam_id, std::string &last_file_access, std::string &last_dir_access);

	bool openDatabase(const std::string& dbfilename, std::string& errmsg);
	bool closeDatabase();
	bool isOpen();
	static bool createDatabase(std::string str);
	static DataSet set;
private:
	bool executeDML(const char* sqlstr, std::string& errmsg);
	bool executeSQL(const char* sqlstr, std::string& errmsg);
	void serialize(std::string &data);
	bool _IsOpen;
	std::string _DbName;
	std::string _Username;
	std::string _Password;
	std::string _DbFilename;
	sqlite3* db;
	std::mutex _Mutex;
};

#endif // DBCONNECTION_H
