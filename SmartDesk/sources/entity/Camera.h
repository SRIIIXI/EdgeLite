#ifndef _CAMERA
#define _CAMERA

#include <QtCore>

class Camera
{
public:
    QString	camera_id;
    QString	camera_name;
    QString	site_id;
    QString	ip_address;
    QString	onvif_port;
    QString	sd_port;
    QString	user_id;
    QString	password;
    QString	is_active;
    QString	last_accessed;
    QString	last_file_timestamp;
    QString	last_dir_timestamp;
    QString	download_required;
    QString	slot_1;
    QString	slot_2;
    QString	slot_3;
    QString	slot_4;
};

#endif
