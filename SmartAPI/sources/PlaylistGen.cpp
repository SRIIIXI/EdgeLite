#include "PlaylistGen.h"
#include "Logger.h"
#include "StringEx.h"
#include "Base64.h"

PlaylistGen::PlaylistGen(Camera cam)
{
    _Camera = cam;
}

PlaylistGen::~PlaylistGen()
{
	_RequestList.clear();
}

bool PlaylistGen::downloadFilelist()
{
	return getAllRecordingSets();
}

bool PlaylistGen::getAllRecordingSets()
{
	_RequestList.clear();

	char urlbuffer[1024] = { 0 };
	sprintf(urlbuffer, "http://%s:%s/sd/", _Camera.ip_address.c_str(), _Camera.sd_port.c_str());

	if (!_Retreiver.retrieve(urlbuffer, _Camera.user_id, _Camera.password))
	{
		return false;
	}

	return processAllRecordingSets(_Retreiver.getHttpBody());
}

bool PlaylistGen::getDailyRecordingSets(std::string &dirdate)
{
	if (dirdate < _Camera.last_dir_timestamp)
	{
		return true;
	}

	char urlbuffer[1024] = { 0 };
	sprintf(urlbuffer, "http://%s:%s/sd/%s/", _Camera.ip_address.c_str(), _Camera.sd_port.c_str(), dirdate.c_str());

	if (!_Retreiver.retrieve(urlbuffer, _Camera.user_id, _Camera.password))
	{
		return false;
	}

	return processDailyRecordingSets(_Retreiver.getHttpBody(), dirdate);
}

bool PlaylistGen::getRecordingSets(std::string &dirdate, std::string &set)
{
	if (dirdate < _Camera.last_dir_timestamp)
	{
		return true;
	}

	char urlbuffer[1024] = { 0 };
	sprintf(urlbuffer, "http://%s:%s/sd/%s/%s/", _Camera.ip_address.c_str(), _Camera.sd_port.c_str(), dirdate.c_str(), set.c_str());

	if (!_Retreiver.retrieve(urlbuffer, _Camera.user_id, _Camera.password))
	{
		return false;
	}

	return processRecordSets(_Retreiver.getHttpBody(), dirdate);
}

bool PlaylistGen::processAllRecordingSets(std::string &urlContent)
{
	strreplace(urlContent, "&nbsp", "");
	strreplace(urlContent, "</tr>", "");
	strreplace(urlContent, "</td>", "");
	strreplace(urlContent, "[DIRECTORY]", "");
	strreplace(urlContent, "<td>", "");
	strreplace(urlContent, "<tr>", "");
	strreplace(urlContent, "<a href=\"/sd/", "");
	strreplace(urlContent, "</a", "");
	strreplace(urlContent, "</table></body></html>", "");
	strreplace(urlContent, "<table>", "");
	strreplace(urlContent, "<body>", "");
	strreplace(urlContent, "<html>", "");
	strreplace(urlContent, "<th>", "");
	strreplace(urlContent, "</th>", "");
	strreplace(urlContent, "<a>", "");
	strreplace(urlContent, "\">", "");
	strreplace(urlContent, ";", "");
	strreplace(urlContent, ">", "");

	std::size_t pos = 0;

	pos = urlContent.find("--") + 2;

	urlContent = urlContent.erase(0, pos);

	std::list<std::string> dirlines;

	strsplit(urlContent, dirlines, '\n');

	dirlines.pop_front();

	std::list<std::string>::iterator it = dirlines.begin();

	while (it != dirlines.end())
	{
		pos = (*it).find("/");
		(*it)[pos] = 0;

		if (!getDailyRecordingSets(*it))
		{
			return false;
		}

		++it;
	}

	return true;
}

bool PlaylistGen::processDailyRecordingSets(std::string &urlContent, std::string parentdir)
{
	strreplace(urlContent, "&nbsp", "");
	strreplace(urlContent, "</tr>", "");
	strreplace(urlContent, "</td>", "");
	strreplace(urlContent, "[DIRECTORY]", "");
	strreplace(urlContent, "<td>", "");
	strreplace(urlContent, "<tr>", "");
	strreplace(urlContent, "<a href=\"/sd/", "");
	strreplace(urlContent, "</a", "");
	strreplace(urlContent, "</table></body></html>", "");
	strreplace(urlContent, "<table>", "");
	strreplace(urlContent, "<body>", "");
	strreplace(urlContent, "<html>", "");
	strreplace(urlContent, "<th>", "");
	strreplace(urlContent, "</th>", "");
	strreplace(urlContent, "<a>", "");
	strreplace(urlContent, "\">", "");
	strreplace(urlContent, ";", "");
	strreplace(urlContent, ">", "");

	std::size_t pos = 0;

	pos = urlContent.find("--") + 2;
	urlContent = urlContent.erase(0, pos);

	std::list<std::string> reclines;

	strsplit(urlContent, reclines, '\n');

	reclines.pop_front();
	reclines.pop_back();

	std::list<std::string>::iterator it = reclines.begin();

	while (it != reclines.end())
	{
		std::vector<std::string> tokens;
		strsplit(*it, tokens, '/');

		if (!getRecordingSets(tokens[0], tokens[1]))
		{
			return false;
		}

		++it;
	}

	return true;
}

bool PlaylistGen::processRecordSets(std::string &urlContent, std::string parentdir)
{
	 strreplace(urlContent, "&nbsp", "");
	 strreplace(urlContent, "</tr>", "");
	 strreplace(urlContent, "</td>", "");
	 strreplace(urlContent, "[DIRECTORY]", "");
	 strreplace(urlContent, "<td>", "");
	 strreplace(urlContent, "<tr>", "");
	 strreplace(urlContent, "<a href=\"/sd/", "");
	 strreplace(urlContent, "</a", "");
	 strreplace(urlContent, "</table></body></html>", "");
	 strreplace(urlContent, "<table>", "");
	 strreplace(urlContent, "<body>", "");
	 strreplace(urlContent, "<html>", "");
	 strreplace(urlContent, "<th>", "");
	 strreplace(urlContent, "</th>", "");
	 strreplace(urlContent, "<a>", "");
	 strreplace(urlContent, ">", "");

	 std::size_t pos = 0;

	 pos = urlContent.find("--") + 2;
	 urlContent = urlContent.erase(0, pos);

	strreplace(urlContent, '"', '/');
	strreplace(urlContent, ';', '/');

	pos = urlContent.find("-//-") + 4;
	urlContent = urlContent.erase(0, pos);

	std::list<std::string> reclines;

	strsplit(urlContent, reclines, '\n');

	reclines.pop_front();

	std::list<std::string>::iterator it = reclines.begin();

	while (it != reclines.end())
	{
		std::vector<std::string> tokens;
		strsplit(*it, tokens, '/');

		Recording dnl;

		Timestamp ts;

		dnl.camera_id = _Camera.camera_id;
		dnl.file_size = tokens[6];
		dnl.file_timestamp = tokens[4] + ":00";
		dnl.camera_file_uri = "http://" + _Camera.user_id + ":" + _Camera.password + "@" + _Camera.ip_address + ":" + _Camera.sd_port + "/sd/" + tokens[0] + "/" + tokens[1] + "/" + tokens[2];
		dnl.storage_file_uri = tokens[2];
		dnl.request_id = _Camera.site_id + _Camera.camera_id + tokens[2]; // Need a guid generator

		if (_Camera.download_required == "y" || _Camera.download_required == "Y")
		{
			dnl.request_status = "Q";
		}
		else
		{
			dnl.request_status = "R";
		}

		dnl.request_timestamp = ts.getDateString("yyyyMMddhhmmss");
		dnl.site_id = _Camera.site_id;

		if (dnl.camera_file_uri.find("999999") != std::string::npos)
		{
			++it;
			continue;
		}

		normalizeTimestamp(dnl.file_timestamp);

		if (dnl.file_timestamp < _Camera.last_file_timestamp)
		{
			++it;
			continue;
		}

		_RequestList.push_back(dnl);

		if (tokens[0] >= _Camera.last_dir_timestamp)
		{
			_Camera.last_dir_timestamp = tokens[0];
		}

		if (dnl.file_timestamp >= _Camera.last_file_timestamp)
		{
			_Camera.last_file_timestamp = dnl.file_timestamp;
		}

		++it;
	}

	return true;
}

bool PlaylistGen::writeOutPlaylist(std::string& pdir, std::string &rdir, std::string &vdir, DbLocal* dbptr)
{
	if (_RequestList.size() < 1)
	{
		return false;
	}

	std::string plylistdir;
	
	plylistdir = pdir;
	plylistdir += "/";
	plylistdir += _Camera.camera_name;
	plylistdir += "/";


	strreplace(plylistdir, "//", "/");

	if (!dirisdirectory(plylistdir.c_str()))
	{
		dircreatedirectory(plylistdir.c_str());
	}

	std::string txtfilename = plylistdir + _Camera.last_dir_timestamp + ".txt";
    std::string tmpfilename = plylistdir + _Camera.last_dir_timestamp + ".tmp";

    FILE* fptmp = fopen(tmpfilename.c_str(), "w");

    if (fptmp == NULL)
	{
		return false;
	}

    fprintf(fptmp, "Camera URL, File Timestamp, File Size, Storage URL\n");

	std::list<Recording>::iterator it = _RequestList.begin();

	while (it != _RequestList.end())
	{
        fprintf(fptmp, "%s, %s, %s, %s/%s/%s\n", (*it).camera_file_uri.c_str(), (*it).file_timestamp.c_str(), (*it).file_size.c_str(), rdir.c_str(), _Camera.camera_name.c_str(), (*it).storage_file_uri.c_str());

        fflush(fptmp);

		dbptr->createRecording(*it);
		
		++it;
	}

    fclose(fptmp);

	int res = 0;
	remove(txtfilename.c_str());
	res = rename(tmpfilename.c_str(), txtfilename.c_str());

	return true;
}

bool PlaylistGen::updateLastAccessTime(DbLocal* dbptr)
{
	return dbptr->updateLastAccessTime(_Camera.camera_id, _Camera.last_file_timestamp, _Camera.last_dir_timestamp);
}

void PlaylistGen::normalizeTimestamp(std::string &str)
{
	int len = str.length();

	for (int ctr = 0; ctr < len; ctr++)
	{
		if (isalpha(str[ctr]))
		{
			str[ctr] = tolower(str[ctr]);
		}
	}

	strreplace(str, "jan", "01");
	strreplace(str, "feb", "02");
	strreplace(str, "mar", "03");
	strreplace(str, "apr", "04");
	strreplace(str, "may", "05");
	strreplace(str, "jun", "06");
	strreplace(str, "jul", "07");
	strreplace(str, "aug", "08");
	strreplace(str, "sep", "09");
	strreplace(str, "oct", "10");
	strreplace(str, "nov", "11");
	strreplace(str, "dec", "12");	
	
	char buffer[32] = { 0 };

	memcpy(&buffer[0], &str.c_str()[6], 4);
	memcpy(&buffer[4], &str.c_str()[3], 2);
	memcpy(&buffer[6], &str.c_str()[0], 2);
	memcpy(&buffer[8], &str.c_str()[11], 2);
	memcpy(&buffer[10], &str.c_str()[14], 2);
	memcpy(&buffer[12], &str.c_str()[17], 2);

	str = buffer;
}
