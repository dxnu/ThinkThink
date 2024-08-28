#pragma once
#define ORMPP_ENABLE_MYSQL
#include "mysql.hpp"
#include "dbng.hpp"
using namespace ormpp;
#include <string>


struct VideoCategory {
	std::string category_name;
	std::string cover1_source;
	std::string cover2_source;
	std::string cover3_source;
};

REFLECTION(VideoCategory, category_name, cover1_source, cover2_source, cover3_source)

struct Video {
	std::string video_id;
	std::string video_name;
	std::string uploader;
	std::string cover_image;
	std::string source;
	std::string category;
	std::string upload_date;
	std::string describe;
};

REFLECTION(Video, video_id, video_name, uploader, cover_image, source, category, upload_date, describe)

struct Account {
	std::string account_id;
	std::string account_name;
	std::string password_hash;
	std::string salt;
	std::string mail;
	std::string profile_photo;
	std::string gender;
	std::string about_me;
	std::string signup_date;
};

REFLECTION(Account, account_id, account_name, password_hash, salt, mail, profile_photo, gender, about_me, signup_date)

struct VideoComments {
	std::string comment_id;
	std::string video_id;
	std::string account_id;
	std::string content;
	std::string time;
};

REFLECTION(VideoComments, comment_id, video_id, account_id, content, time)