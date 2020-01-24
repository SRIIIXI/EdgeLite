#ifndef _IPCAM_STRUCTS
#define _IPCAM_STRUCTS

#include <string>

using namespace std;

typedef struct Site
{
	std::string	site_id;
	std::string ip_address;
	std::string	port;
	std::string	contact_first_name;
	std::string	contact_last_name;
	std::string	contact_phone_no;
	std::string	contact_email_id;
	std::string recording_start;
	std::string recording_end;
}Site;

typedef struct Camera
{
	std::string	camera_id;
	std::string	camera_name;
	std::string	site_id;
	std::string	ip_address;
	std::string	onvif_port;
	std::string	sd_port;
	std::string	user_id;
	std::string	password;
	std::string	is_active;
	std::string	last_accessed;
	std::string	last_file_timestamp;
	std::string	last_dir_timestamp;
	std::string	download_required;
	std::string	slot_1;
	std::string	slot_2;
	std::string	slot_3;
	std::string	slot_4;
}Camera;

typedef struct Recording
{
	std::string	site_id;
	std::string	request_id;
	std::string	camera_id;
	std::string	camera_file_uri;
	std::string	storage_file_uri;
	std::string	request_timestamp;
	std::string	request_status;
	std::string	file_size;
	std::string	file_timestamp;
}Recording;

#endif