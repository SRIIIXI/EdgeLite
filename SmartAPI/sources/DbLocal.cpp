#include "DbLocal.h"
#include "StringEx.h"
#include "SQLTemplates.h"
#include <memory.h>
#include <fstream>

DataSet DbLocal::set;

static int SQLCallback(void* NotUsed, int argc, char** argv, char** azColName)
{
	int i;

	Row datarow;
	Row header;

	if (DbLocal::set.size() < 1)
	{
		for (i = 0; i < argc; i++)
		{
			header.push_back(azColName[i]);
		}
	}

	DbLocal::set.push_back(header);

	for (i = 0; i < argc; i++)
	{
		datarow.push_back(argv[i]);
	}

	DbLocal::set.push_back(datarow);

	return 0;
}

DbLocal::DbLocal()
{
	db = nullptr;
	_IsOpen = false;
}

DbLocal::~DbLocal()
{
	if (_IsOpen)
	{
		closeDatabase();
	}
}

bool DbLocal::openDatabase(const std::string& dbfilename, std::string& errmsg)
{
	int rc;

	_DbFilename = dbfilename;

	rc = sqlite3_open(dbfilename.c_str(), &db);

	if (rc)
	{
		return false;
	}

	_IsOpen = true;
	return true;
}

bool DbLocal::closeDatabase()
{
	if (db != nullptr)
	{
		sqlite3_close(db);
	}

	_IsOpen = false;
	return true;
}

bool DbLocal::executeDML(const char* sqlstr, std::string& errmsg)
{
	char* zErrMsg = nullptr;
	int rc;

	rc = sqlite3_exec(db, sqlstr, nullptr, nullptr, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		if (zErrMsg != nullptr)
		{
			errmsg = zErrMsg;
		}
		return false;
	}

	return true;
}

bool DbLocal::executeSQL(const char* sqlstr, std::string& errmsg)
{
	char* zErrMsg = nullptr;
	int rc;

	rc = sqlite3_exec(db, sqlstr, SQLCallback, 0, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		if (zErrMsg != nullptr)
		{
			errmsg = zErrMsg;
		}
		return false;
	}

	return true;
}

bool DbLocal::isOpen()
{
	return _IsOpen;
}

bool DbLocal::createDatabase(std::string str)
{
	std::string errmsg;
	int rc;
	char* zErrMsg = nullptr;

	sqlite3* dbnew;

	std::fstream file;
	file.open(str, fstream::out | fstream::trunc);
	file.close();

	rc = sqlite3_open(str.c_str(), &dbnew);

	if (rc)
	{
		return false;
	}

	rc = sqlite3_exec(dbnew, sql_str_create_site.c_str(), nullptr, nullptr, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		errmsg = zErrMsg;
		return false;
	}

	rc = sqlite3_exec(dbnew, sql_str_create_camera.c_str(), nullptr, nullptr, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		errmsg = zErrMsg;
		return false;
	}

	rc = sqlite3_exec(dbnew, sql_str_create_recording.c_str(), nullptr, nullptr, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		errmsg = zErrMsg;
		return false;
	}

	sqlite3_close(dbnew);

	return true;
}

void DbLocal::serialize(std::string& data)
{
	data.clear();

	for (auto row : set)
	{
		int col = row.size();
		int index = 1;

		for (auto fieldval : row)
		{
			if (index >= col)
			{
				data += fieldval + "\n";
			}
			else
			{
				data += fieldval + ",";
				index++;
			}
		}
	}
}

bool DbLocal::getSites(std::string &sitelist)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string errMesg;

	if (executeSQL(sql_str_select_all_site.c_str(), errMesg))
	{
		serialize(sitelist);
		ret = true;
	}

	set.clear();

	_Mutex.unlock();

	return ret;
}

bool DbLocal::getCamera(std::string& camlist, std::string& camid)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string sqlquery = sql_str_select_all_camera;
	sqlquery += " where camera_id = '";
	sqlquery += camid;
	sqlquery += "'";

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		serialize(camlist);
		ret = true;
	}

	set.clear();

	_Mutex.unlock();

	return ret;
}

bool DbLocal::getCameras(std::string &camlist, std::string &sitename)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string sqlquery;

	if (sitename.length() > 0)
	{
		sqlquery = sql_str_select_filtered_camera;
		strreplace(sqlquery, "__site_id", sitename);
	}
	else
	{
		sqlquery = sql_str_select_all_camera;
	}

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		serialize(camlist);
		ret = true;
	}

	set.clear();

	_Mutex.unlock();

	return ret;
}

bool DbLocal::getRecording(std::string &downloadrequest, std::string &camid)
{
	_Mutex.lock();

	bool ret = false;
	set.clear();
	std::string sqlquery;

	if (camid.length() > 0)
	{
		sqlquery = sql_str_select_filtered_recording;
		strreplace(sqlquery, "__cam_id", camid);
	}
	else
	{
		sqlquery = sql_str_select_all_recording;
	}

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		serialize(downloadrequest);
		ret = true;
	}

	set.clear();

	_Mutex.unlock();

	return ret;
}

bool DbLocal::createSite(std::vector <std::string> &fieldlist)
{
	if (fieldlist.size() < 9)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	std::string errmsg;

	std::string sql = sql_str_insert_site;

	strreplace(sql, "__site_id", fieldlist[0]);
	strreplace(sql, "__ip_address", fieldlist[1]);
	strreplace(sql, "__port", fieldlist[2]);
	strreplace(sql, "__contact_first_name", fieldlist[3]);
	strreplace(sql, "__contact_last_name", fieldlist[4]);
	strreplace(sql, "__contact_phone_no", fieldlist[5]);
	strreplace(sql, "__contact_email_id", fieldlist[6]);
	strreplace(sql, "__recording_start", fieldlist[7]);
	strreplace(sql, "__recording_end", fieldlist[8]);

	ret = executeDML(sql.c_str(), errmsg);		

	_Mutex.unlock();

	return ret;
}


bool DbLocal::createCamera(std::vector <std::string> &fieldlist)
{
	if (fieldlist.size() < 17)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	std::string errmsg;

	std::string sql = sql_str_insert_camera;

	strreplace(sql, "__camera_id", fieldlist[0]);
	strreplace(sql, "__camera_name", fieldlist[1]);
	strreplace(sql, "__site_id", fieldlist[2]);
	strreplace(sql, "__ip_address", fieldlist[3]);
	strreplace(sql, "__onvif_port", fieldlist[4]);
	strreplace(sql, "__sd_port", fieldlist[5]);
	strreplace(sql, "__user_id", fieldlist[6]);
	strreplace(sql, "__password", fieldlist[7]);
	strreplace(sql, "__is_active", fieldlist[8]);
	strreplace(sql, "__last_accessed", fieldlist[9]);
	strreplace(sql, "__last_file_timestamp", fieldlist[10]);
	strreplace(sql, "__last_dir_timestamp", fieldlist[11]);
	strreplace(sql, "__download_required", fieldlist[12]);
	strreplace(sql, "__slot_1", fieldlist[13]);
	strreplace(sql, "__slot_2", fieldlist[14]);
	strreplace(sql, "__slot_3", fieldlist[15]);
	strreplace(sql, "__slot_4", fieldlist[16]);

	ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::createRecording(std::vector <std::string> &fieldlist)
{
	if (fieldlist.size() < 9)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	std::string errmsg;

	std::string sql = sql_str_insert_recording;

	strreplace(sql, "_site_id", fieldlist[0]);
	strreplace(sql, "__request_id", fieldlist[1]);
	strreplace(sql, "__camera_id", fieldlist[2]);
	strreplace(sql, "__camera_file_uri", fieldlist[3]);
	strreplace(sql, "__storage_file_uri", fieldlist[4]);
	strreplace(sql, "__request_timestamp", fieldlist[5]);
	strreplace(sql, "__request_status", fieldlist[6]);
	strreplace(sql, "__file_size", fieldlist[7]);
	strreplace(sql, "__file_timestamp", fieldlist[8]);

	ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::createRecording(Recording& dnl)
{
	std::vector <std::string> fieldlist;

	fieldlist.push_back(dnl.site_id);
	fieldlist.push_back(dnl.request_id);
	fieldlist.push_back(dnl.camera_id);
	fieldlist.push_back(dnl.camera_file_uri);
	fieldlist.push_back(dnl.storage_file_uri);
	fieldlist.push_back(dnl.request_timestamp);
	fieldlist.push_back(dnl.request_status);
	fieldlist.push_back(dnl.file_size);
	fieldlist.push_back(dnl.file_timestamp);

	return createRecording(fieldlist);
}

bool DbLocal::updateSite(std::vector <std::string> &fieldlist, std::string objectid)
{
	if (fieldlist.size() < 9)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	if (fieldlist[0] == objectid)
	{
		std::string errmsg;

		ret = executeDML("delete from site", errmsg);

		std::string sql = sql_str_insert_site;

		strreplace(sql, "__site_id", fieldlist[0]);
		strreplace(sql, "__ip_address", fieldlist[1]);
		strreplace(sql, "__port", fieldlist[2]);
		strreplace(sql, "__contact_first_name", fieldlist[3]);
		strreplace(sql, "__contact_last_name", fieldlist[4]);
		strreplace(sql, "__contact_phone_no", fieldlist[5]);
		strreplace(sql, "__contact_email_id", fieldlist[6]);
		strreplace(sql, "__recording_start", fieldlist[7]);
		strreplace(sql, "__recording_end", fieldlist[8]);

		ret = executeDML(sql.c_str(), errmsg);

		sql = "update camera set site_id = '" + fieldlist[0] + "'";
		ret = executeDML(sql.c_str(), errmsg);

		sql = "update recording set site_id = '" + fieldlist[0] + "'";
		ret = executeDML(sql.c_str(), errmsg);
	}

	_Mutex.unlock();

	return ret;
}

bool DbLocal::updateCamera(std::vector <std::string> &fieldlist, std::string objectid)
{
	if (fieldlist.size() < 17)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	if (fieldlist[0] == objectid)
	{
		std::string errmsg;

		std::string sql = sql_str_update_camera;

		strreplace(sql, "__camera_id", fieldlist[0]);
		strreplace(sql, "__camera_name", fieldlist[1]);
		strreplace(sql, "__site_id", fieldlist[2]);
		strreplace(sql, "__ip_address", fieldlist[3]);
		strreplace(sql, "__onvif_port", fieldlist[4]);
		strreplace(sql, "__sd_port", fieldlist[5]);
		strreplace(sql, "__user_id", fieldlist[6]);
		strreplace(sql, "__password", fieldlist[7]);
		strreplace(sql, "__is_active", fieldlist[8]);
		strreplace(sql, "__last_accessed", fieldlist[9]);
		strreplace(sql, "__last_file_timestamp", fieldlist[10]);
		strreplace(sql, "__last_dir_timestamp", fieldlist[11]);
		strreplace(sql, "__download_required", fieldlist[12]);
		strreplace(sql, "__slot_1", fieldlist[13]);
		strreplace(sql, "__slot_2", fieldlist[14]);
		strreplace(sql, "__slot_3", fieldlist[15]);
		strreplace(sql, "__slot_4", fieldlist[16]);

		ret = executeDML(sql.c_str(), errmsg);
	}

	_Mutex.unlock();

	return ret;

}

bool DbLocal::updateRecording(std::vector <std::string> &fieldlist, std::string objectid)
{
	if (fieldlist.size() < 9)
	{
		return false;
	}

	_Mutex.lock();

	bool ret = false;

	if (fieldlist[0] == objectid)
	{
		std::string errmsg;

		std::string sql = sql_str_update_recording;

		strreplace(sql, "_site_id", fieldlist[0]);
		strreplace(sql, "__request_id", fieldlist[1]);
		strreplace(sql, "__camera_id", fieldlist[2]);
		strreplace(sql, "__camera_file_uri", fieldlist[3]);
		strreplace(sql, "__storage_file_uri", fieldlist[4]);
		strreplace(sql, "__request_timestamp", fieldlist[5]);
		strreplace(sql, "__request_status", fieldlist[6]);
		strreplace(sql, "__file_size", fieldlist[7]);
		strreplace(sql, "__file_timestamp", fieldlist[8]);

		ret = executeDML(sql.c_str(), errmsg);
	}

	_Mutex.unlock();

	return ret;
}

bool DbLocal::deleteSite(std::string &siteid)
{
	_Mutex.lock();

	std::string errmsg;

	std::string sql = sql_str_delete_filtered_site;

	strreplace(sql, "__site_id", siteid);

	bool ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::deleteCamera(std::string &camid)
{
	_Mutex.lock();

	std::string errmsg;

	std::string sql = sql_str_delete_filtered_camera;

	strreplace(sql, "__cam_id", camid);

	bool ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::deleteRecording(std::string &requestid)
{
	_Mutex.lock();

	std::string errmsg;

	std::string sql = sql_str_delete_filtered_recording;

	strreplace(sql, "__request_id", requestid);

	bool ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::getSites(std::list<Site>& sitelist)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string sqlquery = sql_str_select_all_site;

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		set.pop_front();

		for (auto row : set)
		{
			int col = row.size();
			int index = 0;

			Site st;

			for (auto fieldval : row)
			{
				switch (index)
				{
					case 0:
					{
						st.site_id = fieldval;
						break;
					}
					case 1:
					{
						st.ip_address = fieldval;
						break;
					}
					case 2:
					{
						st.port = fieldval;
						break;
					}
					case 3:
					{
						st.contact_first_name = fieldval;
						break;
					}
					case 4:
					{
						st.contact_last_name = fieldval;
						break;
					}
					case 5:
					{
						st.contact_phone_no = fieldval;
						break;
					}
					case 6:
					{
						st.contact_email_id = fieldval;
						break;
					}
					case 7:
					{
						st.recording_start = fieldval;
						break;
					}
					case 8:
					{
						st.recording_end = fieldval;
						break;
					}
					default:
					{
						break;
					}
				}			
				
				index++;

			}

			sitelist.push_back(st);
		}
	}

	set.clear();
	_Mutex.unlock();

	return ret;
}

bool DbLocal::getCameras(std::list<Camera> &camlist, std::string& sitename)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string sqlquery;

	if (sitename.length() > 0)
	{
		sqlquery = sql_str_select_filtered_camera;
		strreplace(sqlquery, "__site_id", sitename);
	}
	else
	{
		sqlquery = sql_str_select_all_camera;
	}

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		set.pop_front();

		for (auto row : set)
		{
			int col = row.size();
			int index = 0;

			Camera cam;

			for (auto fieldval : row)
			{
				switch (index)
				{
					case 0:
					{
						cam.camera_id = fieldval;
						break;
					}
					case 1:
					{
						cam.camera_name = fieldval;
						break;
					}
					case 2:
					{
						cam.site_id = fieldval;
						break;
					}
					case 3:
					{
						cam.ip_address = fieldval;
						break;
					}
					case 4:
					{
						cam.onvif_port = fieldval;
						break;
					}
					case 5:
					{
						cam.sd_port = fieldval;
						break;
					}
					case 6:
					{
						cam.user_id = fieldval;
						break;
					}
					case 7:
					{
						cam.password = fieldval;
						break;
					}
					case 8:
					{
						cam.is_active = fieldval;
						break;
					}
					case 9:
					{
						cam.last_accessed = fieldval;
						break;
					}
					case 10:
					{
						cam.last_file_timestamp = fieldval;
						break;
					}
					case 11:
					{
						cam.last_dir_timestamp = fieldval;
						break;
					}
					case 12:
					{
						cam.download_required = fieldval;
						break;
					}
					case 13:
					{
						cam.slot_1 = fieldval;
						break;
					}
					case 14:
					{
						cam.slot_2 = fieldval;
						break;
					}
					case 15:
					{
						cam.slot_3 = fieldval;
						break;
					}
					case 16:
					{
						cam.slot_4 = fieldval;
						break;
					}
					default:
					{
						break;
					}
				}

				index++;
			}
		
			camlist.push_back(cam);
		}
	}

	set.clear();

	_Mutex.unlock();

	return ret;
}

bool DbLocal::getRecording(std::list<Recording> &downloadrequestlist, std::string& camid)
{
	_Mutex.lock();
	bool ret = false;
	set.clear();

	std::string sqlquery;

	if (camid.length() > 0)
	{
		sqlquery = sql_str_select_filtered_recording;
		strreplace(sqlquery, "__cam_id", camid);
	}
	else
	{
		sqlquery = sql_str_select_all_recording;
	}

	std::string errMesg;

	if (executeSQL(sqlquery.c_str(), errMesg))
	{
		set.pop_front();

		for (auto row : set)
		{
			int col = row.size();
			int index = 0;

			Recording dnl;

			for (auto fieldval : row)
			{
				switch (index)
				{
					case 0:
					{
						dnl.site_id = fieldval;
						break;
					}
					case 1:
					{
						dnl.request_id = fieldval;
						break;
					}
					case 2:
					{
						dnl.camera_id = fieldval;
						break;
					}
					case 3:
					{
						dnl.camera_file_uri = fieldval;
						break;
					}
					case 4:
					{
						dnl.storage_file_uri = fieldval;
						break;
					}
					case 5:
					{
						dnl.request_timestamp = fieldval;
						break;
					}
					case 6:
					{
						dnl.request_status = fieldval;
						break;
					}
					case 7:
					{
						dnl.file_size = fieldval;
						break;
					}
					case 8:
					{
						dnl.file_timestamp = fieldval;
						break;
					}
					default:
					{
						break;
					}						
				}

				index++;
			}
			downloadrequestlist.push_back(dnl);
		}
	}

	set.clear();
	_Mutex.unlock();

	return ret;
}

bool DbLocal::updateLastAccessTime(std::string& cam_id, std::string& last_file_access, std::string& last_dir_access)
{
	_Mutex.lock();

	std::string errmsg;

	std::string sql;

	sql += "update camera set ";
	sql += "last_file_timestamp = '" + last_file_access + "', ";
	sql += "last_dir_timestamp = '" + last_dir_access + "' ";
	sql += "where camera_id ='" + cam_id + "'";

	bool ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}

bool DbLocal::updateRecordingStatus(std::string& request_id, std::string status)
{
	_Mutex.lock();

	std::string errmsg;

	std::string sql;

	sql = "update recording set request_status = '" + status + "'";
	sql += " where request_id ='" + request_id + "'";

	bool ret = executeDML(sql.c_str(), errmsg);

	_Mutex.unlock();

	return ret;
}
