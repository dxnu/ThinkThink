#pragma warning(disable:4996)
#include "cinatra.hpp"
#include "ThinkThinkMessage.h"
#include "Entity.hpp"
#include "EmailSender.h"
#include "okcrypt.h"


using namespace cinatra;
using sql_type = dbng<mysql>;


void start_server();
std::string generate_unique_id();
std::string get_current_time();
void on_get_video_categories(request& req, response& res);
void on_get_videos(request& req, response& res);
void on_register_account(request& req, response& res);
void on_get_verify_code(request& req, response& res);
void on_get_reset_password_verify_code(request& req, response& res);
void on_login(request& req, response& res);
void on_upload_multipart(request& req, response& res);
void on_upload_video(request& req, response& res);
void on_search_videos(request& req, response& res);
void on_get_my_videos(request& req, response& res);
void on_delete_video(request& req, response& res);
void on_change_password(request& req, response& res);
void on_get_comments(request& req, response& res);
void on_upload_comments(request& req, response& res);
void on_insert_category(request& req, response& res);
void on_delete_category(request& req, response& res);
void on_update_category_cover(request& req, response& res);
void on_update_info(request& req, response& res);


int main()
{
	start_server();

	return 0;
}

void start_server()
{
	int max_thread_num = std::thread::hardware_concurrency();
	http_server server(max_thread_num);
	server.listen("0.0.0.0", "80");
	server.set_http_handler<GET>("/get_video_categories", on_get_video_categories);
	server.set_http_handler<GET>("/get_videos", on_get_videos);
	server.set_http_handler<GET>("/register_account", on_register_account);
	server.set_http_handler<GET>("/get_verify_code", on_get_verify_code);
	server.set_http_handler<GET>("/get_reset_password_verify_code", on_get_reset_password_verify_code);
	server.set_http_handler<GET>("/login", on_login);
	server.set_http_handler<GET, POST>("/upload_multipart", on_upload_multipart);
	server.set_http_handler<GET>("/upload_video", on_upload_video);
	server.set_http_handler<GET>("/search_videos", on_search_videos);
	server.set_http_handler<GET>("/get_my_videos", on_get_my_videos);
	server.set_http_handler<GET>("/delete_video", on_delete_video);
	server.set_http_handler<GET>("/change_password", on_change_password);
	server.set_http_handler<GET>("/get_comments", on_get_comments);
	server.set_http_handler<GET>("/upload_comments", on_upload_comments);
	server.set_http_handler<GET>("/insert_category", on_insert_category);
	server.set_http_handler<GET>("/delete_category", on_delete_category);
	server.set_http_handler<GET>("/update_category_cover", on_update_category_cover);
	server.set_http_handler<GET>("/update_info", on_update_info);

	server.run();
}

std::string generate_unique_id()
{
	auto tp = std::chrono::high_resolution_clock::now();
	auto nano = tp.time_since_epoch().count();
	return std::to_string(nano);
}

std::string get_current_time() {
	// 时间转字符串
	std::chrono::system_clock::time_point tp;
	tp = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts = std::ctime(&t);
	ts.resize(ts.size() - 1);
	/////////////////
	char mbstr[100];
	size_t size = std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
	if (size) {
		ts = mbstr;
	}
	return ts;
}

void on_get_video_categories(request& req, response& res)
{
	//ResponseMessage response;
	//sql_type mysql;
	//mysql.connect("127.0.0.1", "root", "", "thinkthink");
	//mysql.execute("SET NAMES GB2312");
	//auto categories = mysql.query<VideoCategory>();
	//for (auto& category : categories) {
	//	response.SetMessage("Category", category.category_name.c_str());
	//}


	//res.set_status_and_content(status_type::ok, response.GetData());

	auto category = std::filesystem::u8path(req.get_query_value("category")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto categories = mysql.query<VideoCategory>();

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("ThinkThink");
	root.append_child("ResponseType").text().set("GetVideoCategories");
	for (auto category : categories) {
		pugi::xml_node v = root.append_child("Category");
		v.append_child("Name").text().set(category.category_name.c_str());
		v.append_child("Cover1Source").text().set(category.cover1_source.c_str());
		v.append_child("Cover2Source").text().set(category.cover2_source.c_str());
		v.append_child("Cover3Source").text().set(category.cover3_source.c_str());
	}

	std::stringstream ss;
	doc.save(ss, "");
	std::cout << ss.str() << '\n';

	res.set_status_and_content(status_type::ok, ss.str());
}

void on_get_videos(request& req, response& res)
{
	auto category = std::filesystem::u8path(req.get_query_value("category")).string();

	// res.set_status_and_content(status_type::ok, "中文测试");

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto videos = mysql.query<Video>();

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("ThinkThink");
	root.append_child("ResponseType").text().set("GetVideos");
	for (auto video : videos) {
		// std::cout << video.video_name << " " << video.category << '\n';
		if (video.category == category || category == "ALL") {
			// 根据用户ID获取用户名
			auto result = mysql.query<Account>("WHERE account_id = '" + video.uploader + "'");

			pugi::xml_node v = root.append_child("Video");
			v.append_child("Id").text().set(video.video_id.c_str());
			v.append_child("Name").text().set(video.video_name.c_str());
			v.append_child("Uploader").text().set(result[0].account_name.c_str());
			v.append_child("CoverImage").text().set(video.cover_image.c_str());
			v.append_child("Describe").text().set(video.describe.c_str());
			v.append_child("UploadDate").text().set(video.upload_date.c_str());
			v.append_child("Source").text().set(video.source.c_str());
		}
	}

	std::stringstream ss;
	doc.save(ss, "");
	std::cout << ss.str() << '\n';

	res.set_status_and_content(status_type::ok, ss.str());
}

void on_register_account(request& req, response& res)
{
	auto password = std::filesystem::u8path(req.get_query_value("password")).string();
	Account account;
	account.account_id = std::filesystem::u8path(req.get_query_value("account_id")).string();
	account.account_name = std::filesystem::u8path(req.get_query_value("account_name")).string();
	account.salt = "&abd123";
	account.profile_photo = "default/avatar2.jpg";
	account.password_hash = okcrypt::EncryptMD5(password + account.salt);
	account.mail = std::filesystem::u8path(req.get_query_value("mail")).string();
	account.about_me = std::filesystem::u8path(req.get_query_value("about_me")).string();
	account.signup_date = get_current_time();

	std::cout << "account_id: " << account.account_id << '\n'
		<< "account_name: " << account.account_name << '\n'
		<< "password: " << account.password_hash << '\n'
		<< "mail: " << account.mail << '\n'
		<< "about_me: " << account.about_me << '\n'
		<< "signup_date: " << account.signup_date << '\n';


	ResponseMessage response;
	response.SetMessage("ResponseType", "RegisterAccount");
	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	if (mysql.insert(account) != INT_MIN) {
		response.SetMessage("Status", "OK");
	}
	else {
		response.SetMessage("Status", "ERR");
		response.SetMessage("ErrorMsg", "用户名重复");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_get_verify_code(request& req, response& res)
{
	auto receiver = req.get_query_value("mail");
	auto verificationCode = rand_bytes(6);
	std::string content = "Thank you for registering Thinkthink account. \nYour verification code is: " + verificationCode;

	std::cout << "receiver: " << receiver << "\non_get_verify_code: " << content << '\n';

	EmailSender mail;
	mail.Connect("smtp.qq.com", "25");
	mail.SendEmail("11406488@qq.com", "abrkcnwehhfcbiec", receiver.data(), "ThinkThink", content.c_str());

	ResponseMessage response;
	response.SetMessage("ResponseType", "GetVerificationCode");
	response.SetMessage("VerificationCode", verificationCode.c_str());

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_get_reset_password_verify_code(request& req, response& res)
{
	auto account = req.get_query_value("account_id");
	auto verificationCode = rand_bytes(6);
	std::string content = "You are changeing your password of Thinkthink account. \nYour verification code is: " + verificationCode;

	// 查询邮箱
	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto accounts = mysql.query<Account>("WHERE account_id='" + std::string{ account } + "'");
	if (accounts.size() == 1) {
		auto receiver = accounts[0].mail;
		std::cout << "receiver: " << receiver << "on_get_reset_password_verify_code: " << content << '\n';

		EmailSender mail;
		mail.Connect("smtp.qq.com", "25");
		mail.SendEmail("11406488@qq.com", "abrkcnwehhfcbiec", receiver.data(), "ThinkThink", content.c_str());
	}

	ResponseMessage response;
	response.SetMessage("ResponseType", "GetResetPasswordVerficationCode");
	response.SetMessage("VerificationCode", verificationCode.c_str());

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_login(request& req, response& res)
{
	auto accountId = req.get_query_value("account_id");
	auto password = req.get_query_value("password");

	bool status{ false };

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto account = mysql.query<Account>("WHERE account_id = '" + std::string(accountId) + "'");
	if (account.size() != 0) {
		auto hash = okcrypt::EncryptMD5(std::string(password) + account[0].salt);
		if (account[0].password_hash == hash)
			status = true;
	}

	ResponseMessage response;
	response.SetMessage("ResponseType", "Login");
	if (status) {
		response.SetMessage("LoginStatus", "OK");
		response.SetMessage("LoginName", account[0].account_name.c_str());
		response.SetMessage("LoginAvatar", account[0].profile_photo.c_str());
		response.SetMessage("AccountId", account[0].account_id.c_str());
		response.SetMessage("Mail", account[0].mail.c_str());
		response.SetMessage("Gender", account[0].gender.c_str());
		response.SetMessage("AboutMe", account[0].about_me.c_str());
		response.SetMessage("SignupDate", account[0].signup_date.c_str());
	}
	else {
		response.SetMessage("LoginStatus", "ERR");
		response.SetMessage("LoginMessage", "用户名或密码错误");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_upload_multipart(request& req, response& res)
{
	assert(req.get_content_type() == content_type::multipart);
	auto& files = req.get_upload_files();
	for (auto& file : files) {
		std::cout << file.get_file_path() << " " << file.get_file_size() << '\n';
	}

	res.set_status_and_content(status_type::ok, "multipart finished");
}

void on_upload_video(request& req, response& res)
{
	Video video;
	video.video_id = generate_unique_id();
	video.video_name = std::filesystem::u8path(req.get_query_value("title")).string();
	video.uploader = std::filesystem::u8path(req.get_query_value("uploader")).string();
	video.cover_image = "video/" + std::filesystem::u8path(req.get_query_value("cover")).string();
	video.source = "video/" + std::filesystem::u8path(req.get_query_value("source")).string();
	video.category = std::filesystem::u8path(req.get_query_value("category")).string();
	video.describe = std::filesystem::u8path(req.get_query_value("describe")).string();
	video.upload_date = get_current_time();

	std::cout << "video id: " << video.video_id << '\n'
		<< "video name: " << video.video_name << '\n'
		<< "uploader: " << video.uploader << '\n'
		<< "cover image: " << video.cover_image << '\n'
		<< "source: " << video.source << '\n'
		<< "category: " << video.category << '\n'
		<< "describe: " << video.describe << '\n'
		<< "upload date: " << video.upload_date << '\n';

	ResponseMessage response;
	response.SetMessage("ResponseType", "UploadVideo");
	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");

	// 開始上傳視頻
	if (mysql.insert(video) != INT_MIN) {
		response.SetMessage("Status", "OK");
	}
	else {
		response.SetMessage("Status", "ERR");
		response.SetMessage("ErrorMsg", "上傳失敗");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_search_videos(request& req, response& res)
{
	auto keyword = std::filesystem::u8path(req.get_query_value("keyword")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto videos = mysql.query<Video>("WHERE video_name LIKE '%" + keyword + "%' OR" + " category" + " LIKE '%" + keyword + "%'");

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("ThinkThink");
	root.append_child("ResponseType").text().set("GetVideos");
	for (auto video : videos) {
		// std::cout << video.video_name << " " << video.category << '\n';
		// 根据用户ID获取用户名
		auto result = mysql.query<Account>("WHERE account_id = '" + video.uploader + "'");

		pugi::xml_node v = root.append_child("Video");
		v.append_child("Id").text().set(video.video_id.c_str());
		v.append_child("Name").text().set(video.video_name.c_str());
		v.append_child("Uploader").text().set(result[0].account_name.c_str());
		v.append_child("CoverImage").text().set(video.cover_image.c_str());
		v.append_child("Describe").text().set(video.describe.c_str());
		v.append_child("UploadDate").text().set(video.upload_date.c_str());
		v.append_child("Source").text().set(video.source.c_str());
	}

	std::stringstream ss;
	doc.save(ss, "");
	std::cout << ss.str() << '\n';

	res.set_status_and_content(status_type::ok, ss.str());
}

void on_get_my_videos(request& req, response& res)
{
	auto keyword = std::filesystem::u8path(req.get_query_value("keyword")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto videos = mysql.query<Video>("WHERE uploader='" + keyword + "'");

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("ThinkThink");
	root.append_child("ResponseType").text().set("GetVideos");
	for (auto video : videos) {
		// std::cout << video.video_name << " " << video.category << '\n';
		// 根据用户ID获取用户名
		auto result = mysql.query<Account>("WHERE account_id = '" + video.uploader + "'");

		pugi::xml_node v = root.append_child("Video");
		v.append_child("Id").text().set(video.video_id.c_str());
		v.append_child("Name").text().set(video.video_name.c_str());
		v.append_child("Uploader").text().set(result[0].account_name.c_str());
		v.append_child("CoverImage").text().set(video.cover_image.c_str());
		v.append_child("Describe").text().set(video.describe.c_str());
		v.append_child("UploadDate").text().set(video.upload_date.c_str());
		v.append_child("Source").text().set(video.source.c_str());
	}

	std::stringstream ss;
	doc.save(ss, "");
	std::cout << ss.str() << '\n';

	res.set_status_and_content(status_type::ok, ss.str());
}

void on_delete_video(request& req, response& res)
{
	auto videoId = std::filesystem::u8path(req.get_query_value("video_id")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	bool result = mysql.delete_records<Video>("video_id='" + videoId + "'");
	
	ResponseMessage response;
	response.SetMessage("ResponseType", "DeleteVideo");
	if (result) {
		response.SetMessage("Status", "OK");
	}
	else {
		response.SetMessage("Status", "ERR");
		response.SetMessage("ErrorMsg", "删除失败");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_change_password(request& req, response& res)
{
	auto account = std::filesystem::u8path(req.get_query_value("account_id")).string();
	auto password = std::filesystem::u8path(req.get_query_value("password")).string();


	ResponseMessage response;
	response.SetMessage("ResponseType", "ChangePassword");
	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto accounts = mysql.query<Account>("WHERE account_id='" + account + "'");
	if (accounts.size() == 1) {
		accounts[0].password_hash = okcrypt::EncryptMD5(password + accounts[0].salt);
		auto res = mysql.execute("UPDATE Account SET password_hash='" + accounts[0].password_hash + "' WHERE account_id='" + account + "'");

		if(res != INT_MIN)
			response.SetMessage("Status", "OK");
	}
	else {
		response.SetMessage("Status", "ERR");
		response.SetMessage("ErrorMsg", "更改密码失败");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_get_comments(request& req, response& res)
{
	auto videoId = std::filesystem::u8path(req.get_query_value("video_id")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto comments = mysql.query<VideoComments>("WHERE video_id='" + videoId + "'");

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("ThinkThink");
	root.append_child("ResponseType").text().set("GetComments");
	for (auto comment : comments) {
		// std::cout << video.video_name << " " << video.category << '\n';
		// 根据用户ID获取用户名
		auto accounts = mysql.query<Account>("WHERE account_id = '" + comment.account_id + "'");

		pugi::xml_node v = root.append_child("VideoComments");
		v.append_child("CommentId").text().set(comment.comment_id.c_str());
		v.append_child("VideoId").text().set(comment.video_id.c_str());
		v.append_child("AccountId").text().set(comment.account_id.c_str());
		v.append_child("Content").text().set(comment.content.c_str());
		v.append_child("UserName").text().set(accounts[0].account_name.c_str());
		v.append_child("UserAvatarSource").text().set(accounts[0].profile_photo.c_str());
		v.append_child("Time").text().set(comment.time.c_str());
	}

	std::stringstream ss;
	doc.save(ss, "");
	std::cout << ss.str() << '\n';

	res.set_status_and_content(status_type::ok, ss.str());
}

void on_upload_comments(request& req, response& res)
{
	VideoComments comment;
	comment.comment_id = generate_unique_id();
	comment.account_id = std::filesystem::u8path(req.get_query_value("user_id")).string();
	comment.video_id = std::filesystem::u8path(req.get_query_value("video_id")).string();
	comment.content = std::filesystem::u8path(req.get_query_value("content")).string();
	comment.time = get_current_time();

	ResponseMessage response;
	response.SetMessage("ResponseType", "UploadComments");
	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");


	// 開始上傳視頻
	if (mysql.insert(comment) != INT_MIN) {
		response.SetMessage("Status", "OK");
	}
	else {
		response.SetMessage("Status", "ERR");
		response.SetMessage("ErrorMsg", "发布评论失败");
	}

	res.set_status_and_content(status_type::ok, response.GetData());
}

void on_insert_category(request& req, response& res)
{
	VideoCategory category;
	category.category_name = std::filesystem::u8path(req.get_query_value("category")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");

	mysql.insert(category);
}

void on_delete_category(request& req, response& res)
{
	auto category = std::filesystem::u8path(req.get_query_value("category")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");

	mysql.delete_records<VideoCategory>("category_name='" + category + "'");
}

void on_update_category_cover(request& req, response& res)
{
	auto category = std::filesystem::u8path(req.get_query_value("category")).string();
	auto coverPath = "video/" + std::filesystem::u8path(req.get_query_value("cover")).string();
	auto coverNumber = std::filesystem::u8path(req.get_query_value("number")).string();

	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");

	auto coverSouce = "cover" + coverNumber + "_source";
	std::cout << "UPDATE VideoCategory SET " + coverSouce + " ='" + coverPath + "' WHERE category_name='" + category + "'\n";

	mysql.execute("UPDATE VideoCategory SET " + coverSouce + " ='" + coverPath + "' WHERE category_name='" + category + "'");

}

void on_update_info(request& req, response& res)
{
	auto userId = std::filesystem::u8path(req.get_query_value("id")).string();
	auto userName = std::filesystem::u8path(req.get_query_value("name")).string();
	auto gender = std::filesystem::u8path(req.get_query_value("gender")).string();
	auto aboutMe = std::filesystem::u8path(req.get_query_value("about_me")).string();


	sql_type mysql;
	mysql.connect("127.0.0.1", "root", "", "thinkthink");
	mysql.execute("SET NAMES GB2312");
	auto accounts = mysql.query<Account>("WHERE account_id='" + userId + "'");
	if (accounts.size() == 1) {
		auto res = mysql.execute("UPDATE Account SET account_name='" 
			+ userName + "', gender='" + gender + "', about_me='" + aboutMe + "' WHERE account_id='" + userId + "'");

	}
}