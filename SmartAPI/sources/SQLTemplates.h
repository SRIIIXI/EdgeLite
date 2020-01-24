#ifndef _SQL_TEMPLATES
#define _SQL_TEMPLATES

#include <string>

// INSERTS
extern std::string sql_str_insert_site;
extern std::string sql_str_insert_camera;
extern std::string sql_str_insert_recording;

//DELETES
extern std::string sql_str_delete_all_site;
extern std::string sql_str_delete_all_camera;
extern std::string sql_str_delete_all_recording;

extern std::string sql_str_delete_filtered_site;
extern std::string sql_str_delete_filtered_camera;
extern std::string sql_str_delete_filtered_recording;

// UPDATES
extern std::string sql_str_update_camera;
extern std::string sql_str_update_recording;

// SELECTS
extern std::string sql_str_select_all_site;
extern std::string sql_str_select_all_camera;
extern std::string sql_str_select_all_recording;

extern std::string sql_str_select_filtered_site;
extern std::string sql_str_select_filtered_camera;
extern std::string sql_str_select_filtered_recording;

extern std::string sql_str_create_site;
extern std::string sql_str_create_camera;
extern std::string sql_str_create_recording;

#endif
