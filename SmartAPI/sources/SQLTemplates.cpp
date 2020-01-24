#include "SQLTemplates.h"

// INSERTS
std::string sql_str_insert_site = "insert into site (site_id, ip_address, port, contact_first_name, contact_last_name, contact_phone_no, contact_email_id, recording_start, recording_end) values('__site_id', '__ip_address', '__port', '__contact_first_name', '__contact_last_name', '__contact_phone_no', '__contact_email_id', '__recording_start', '__recording_end')";

std::string sql_str_insert_camera = "insert into camera (camera_id, camera_name, site_id, ip_address, onvif_port, sd_port, user_id, password, is_active, last_accessed, last_file_timestamp, last_dir_timestamp, download_required, slot_1, slot_2, slot_3, slot_4) values('__camera_id', '__camera_name', '__site_id', '__ip_address', '__onvif_port', '__sd_port', '__user_id', '__password', '__is_active', '__last_accessed', '__last_file_timestamp', '__last_dir_timestamp', '__download_required', '__slot_1', '__slot_2', '__slot_3', '__slot_4')";

std::string sql_str_insert_recording = "insert into recording (site_id, request_id, camera_id, camera_file_uri, storage_file_uri, request_timestamp, request_status, file_size, file_timestamp) values ('_site_id', '__request_id', '__camera_id', '__camera_file_uri', '__storage_file_uri', '__request_timestamp', '__request_status', '__file_size', '__file_timestamp')";

// DELETES
std::string sql_str_delete_all_site = "delete from site";
std::string sql_str_delete_all_camera = "delete from camera";
std::string sql_str_delete_all_recording = "delete from recording";

std::string sql_str_delete_filtered_site = "delete from site where site_id = '__site_id'";
std::string sql_str_delete_filtered_camera = "delete from camera where camera_id = '__cam_id'";
std::string sql_str_delete_filtered_recording = "delete from recording where request_id = '__request_id'";

// UPDATES
std::string sql_str_update_camera = "update_camera set camera_name = '__camera_name', site_id = '__site_id', ip_address = '__ip_address', onvif_port = '__onvif_port', sd_port = '__sd_port', user_id = '__user_id', password = '__password', is_active = '__is_active', last_accessed = '__last_accessed', last_file_timestamp = '__last_file_timestamp', last_dir_timestamp = '__last_dir_timestamp', download_required = '__download_required', slot_1 = '__slot_1', slot_2 = '__slot_2', slot_3 = '__slot_3', slot_4 =  '__slot_4' where camera_id = '__camera_id'";

std::string sql_str_update_recording = "update recording set site_id = '_site_id', camera_id = '__camera_id', camera_file_uri = '__camera_file_uri', storage_file_uri = '__storage_file_uri', request_timestamp = '__request_timestamp', request_status = '__request_status', file_size = '__file_size', file_timestamp = '__file_timestamp' where request_id = '__request_id'";;

// SELECTS
std::string sql_str_select_all_site = "select * from site";
std::string sql_str_select_all_camera = "select * from camera";
std::string sql_str_select_all_recording = "select * from recording";

std::string sql_str_select_filtered_camera = "select * from camera where site_id = '__site_id'";
std::string sql_str_select_filtered_recording = "select * from recording where camera_id = '__cam_id'";

std::string sql_str_create_site = "create table site ( \
	site_id varchar(64) not null, \
	ip_address varchar(15) not null, \
	port varchar(5) not null, \
	contact_first_name varchar(64) not null, \
	contact_last_name varchar(64) not null, \
	contact_phone_no varchar(64) not null, \
	contact_email_id varchar(64) not null, \
	recording_start varchar(14) not null, \
	recording_end varchar(14) not null, \
	primary key(site_id) \
	)";

std::string sql_str_create_camera = "create table camera ( \
	camera_id varchar(64) not null, \
	camera_name varchar(64) not null, \
	site_id varchar(64) not null, \
	ip_address varchar(15) not null, \
	onvif_port varchar(5) not null, \
	sd_port varchar(5) not null, \
	user_id varchar(32) not null, \
	password varchar(32) not null, \
	is_active varchar(1) not null, \
	last_accessed varchar(14) not null, \
	last_file_timestamp varchar(14) not null, \
	last_dir_timestamp varchar(14) not null, \
	download_required varchar(1) not null, \
	slot_1 varchar(14) not null, \
	slot_2 varchar(14) not null, \
	slot_3 varchar(14) not null, \
	slot_4 varchar(14) not null, \
	primary key(camera_id) \
	)";

std::string sql_str_create_recording = "create table recording ( \
	site_id varchar(64) not null, \
	request_id varchar(64) not null, \
	camera_id varchar(64) not null, \
	camera_file_uri varchar(1024) not null, \
	storage_file_uri varchar(1024) not null, \
	request_timestamp varchar(14) not null, \
	request_status char(1) not null, \
	file_size varchar(16) not null, \
	file_timestamp varchar(14) not null, \
	primary key(request_id) \
	)";