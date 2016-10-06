#include <stdarg.h> // va_copy(args)
#include <sys/stat.h> // _stat
#include <sys/types.h> // possibly for UNIX MakeDirectory
#include <direct.h> // _mkdir
#include <ctime> // timestamps
#include <fstream> // file streams
#include <iostream> // cout
#include <string>
#include <boost\filesystem.hpp> // linker,General,AddtnlLibraryDirectories - (C:\Boost\boost_1_60_0\stage\lib\) C/C++,General,AddtnlIncludeDirectories - (C:\Boost\boost_1_60_0;)

#include "logging.h"


using namespace Logs;

namespace boostfs = boost::filesystem;

std::ofstream log_stream;

std::string root_directory;


// system needs better implementation overall - it's a little clunky
Logging::Logging()
{
	
}

void Logging::DefaultLogSettings()
{
	memset(log_settings, 0, sizeof(LogSettings)* Logs::Category::MaxCategoryID);

	// 0, Normal, Med, High	
	log_settings[Error].log_to_file = Normal;
	log_settings[Error].log_to_console = Normal;
	log_settings[General].log_to_file = 0;
	log_settings[General].log_to_console = Normal;
	log_settings[Test].log_to_file = 0;
	log_settings[Test].log_to_console = Med;
}

void Logging::CheckLogFiles()
{
	std::string root_as_string = root_foldername;

	root_directory = FormatLogPath(root_as_string);

	LogDirectory(root_directory + LogDirectories[LOG_DIRS]);
	LogDirectory(root_directory + LogDirectories[ERROR_DIR]);
	LogDirectory(root_directory + LogDirectories[DEBUG_DIR]);

	CheckFileTime(root_directory + LogDirectories[ERROR_DIR], LogFileName[ERROR_LOGS], false);
	CheckFileTime(root_directory + LogDirectories[DEBUG_DIR], LogFileName[DEBUG_LOGS], false);
}

void Logging::CheckFileTime(std::string dest_dir, std::string dest_file, bool write)
{
	const char * mc_filename;

	std::string m_file;
	std::string time_increment;
	std::string s_time_stamp;
	std::string s_int;

	size_t num_sz;

	int i_time_increment;
	int i_time;

	bool exists = false;

	char time_stamp[80];
	TimeStamp(time_stamp, true);

	s_time_stamp = time_stamp;
	time_increment = s_time_stamp.substr(5, 2);
	i_time_increment = stoi(time_increment, &num_sz);

	for (unsigned int i = i_time_increment; i < 12; i--) {
		s_int = std::to_string(i);

		if (strlen(s_int.c_str()) == 1) {
			s_int = "0" + s_int;
			s_time_stamp.replace(5, 2, s_int);
		}
		else {
			s_time_stamp.replace(5, 2, s_int);
		}

		mc_filename = s_time_stamp.c_str();
		i_time = stoi(s_int, &num_sz);
		m_file = StringFormat("%s_%s.txt", dest_file.c_str(), mc_filename);

		if (i_time_increment == i_time) {
			if (write == false) {
				if (LogFile(dest_dir + m_file, 1) == true) {
					exists = true;					
				}
				break;
			}
			else {
				LogFile(dest_dir + m_file, 3);
				exists = true;
				break;
			}
		}		
	}
	if (exists == false) {
		mc_filename = time_stamp;
		m_file = StringFormat("%s_%s.txt", dest_file.c_str(), mc_filename);
		LogFile(dest_dir + m_file, 2);
	}
}

void Logging::LogRecord(Logs::LogLevel log_level, int log_category, std::string message, ...)
{
	const bool log_to_console = log_settings[log_category].log_to_console > 0;
	const bool log_to_file = log_settings[log_category].log_to_file > 0;
	const bool nothing_to_log = !log_to_console && !log_to_file;

	if (nothing_to_log) {
		return;
	}

	va_list args;
	va_start(args, message);
	std::string out_message = vStringFormat(message.c_str(), args);
	va_end(args);

	std::string output_message = MessageFormat(log_category, out_message);

	if (log_to_console) {
		ConsoleMessage(log_level, log_category, output_message);		
	}
	if (log_to_file) {
		LogWrite(log_level, log_category, output_message);
	}
}

std::string Logging::FormatLogPath(std::string dir_stop)
{
	std::string find_string;
	std::string source_line;
	std::string new_string;

	std::size_t line_pos = 0;

	auto cur_working_dir = boostfs::current_path();

	source_line = cur_working_dir.string();
	find_string = dir_stop;

	line_pos = source_line.find(find_string);
	if (line_pos != std::string::npos) {
		new_string = source_line.substr(0, line_pos);
	}
	new_string.append(dir_stop);

	return new_string;
}

void Logging::ConsoleMessage(int log_level, int log_category, const std::string & message)
{
	if (log_settings[log_category].log_to_console == 0) {
		return;
	}

	if (log_settings[log_category].log_to_console < log_level) {
		return;
	}

#ifdef _WIN32 // #ifdef _WINDOWS	
	std::cout << message << std::endl;	
#else
	std::cout << Logging::GetLinuxConsoleColorFromCategory(log_category) << message << LC_RESET << std::endl;
#endif
}

// crash system from eqemu (crash.h/.cpp) would be helpful to implement
// simple is no endl and no timestamp
void Logging::LogWrite(int log_level, int log_category, const std::string & message)
{
	//	if (log_category == Logs::Crash) {
	//		char time_stamp[80];
	//		TimeStamp(time_stamp);
	//		std::ofstream crash_log;
	//		MakeDirectory("logs/crashes");
	//		crash_log.open(StringFormat("logs/crashes/crash_%s_%i.log", platform_file_name.c_str(), getpid()), std::ios_base::app | std::ios_base::out);
	//		crash_log << time_stamp << " " << message << "\n";
	//		crash_log.close();
	//	}

	if (log_settings[log_category].log_to_file == 0)
		return;

	if (log_settings[log_category].log_to_file < log_level)
		return;

	char time_stamp[80];
	TimeStamp(time_stamp, false);

	if (log_category == Error) {		
		CheckFileTime(root_directory + LogDirectories[ERROR_DIR], LogFileName[ERROR_LOGS], true);
	}
	else {
		CheckFileTime(root_directory + LogDirectories[DEBUG_DIR], LogFileName[DEBUG_LOGS], true);
	}

	if (log_stream) {
		log_stream << time_stamp << " " << message << std::endl;
	}
}

std::string Logging::MessageFormat(int log_category, const std::string &in_message)
{
	std::string category_string;
	if (log_category > 0 && CategoryName[log_category])
		category_string = StringFormat("[%s] ", CategoryName[log_category]);
	return StringFormat("%s%s", category_string.c_str(), in_message.c_str());
}

const std::string Logging::StringFormat(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	std::string output = vStringFormat(format, args);
	va_end(args);
	return output;
}

const std::string Logging::vStringFormat(const char * format, va_list args)
{
	std::string output;
	va_list tmpargs;

	va_copy(tmpargs, args);
	int characters_used = vsnprintf(nullptr, 0, format, tmpargs);
	va_end(tmpargs);

	if (characters_used > 0) {
		output.resize(characters_used + 1);

		va_copy(tmpargs, args);
		characters_used = vsnprintf(&output[0], output.capacity(), format, tmpargs);
		va_end(tmpargs);

		output.resize(characters_used);

		if (characters_used < 0)
			output.clear();
	}
	return output;
}

void Logging::TimeStamp(char * time_stamp, bool filename)
{
	time_t raw_time;
	struct tm * time_info;
	time(&raw_time);
	time_info = localtime(&raw_time);

	if (filename == true) {
		strftime(time_stamp, 80, "%Y-%m", time_info);
	}
	else {
		strftime(time_stamp, 80, "[%m-%d-%Y :: %H:%M:%S]", time_info);
	}
}

void Logging::LogDirectory(const std::string &directory_name)
{
	struct _stat st;
#if defined _WIN32
	//struct _stat st;
	if (_stat(directory_name.c_str(), &st) == 0) { // exists
		std::cout << "\n\t\tERROR: directory already exists" << std::endl;
		return;
	}
	_mkdir(directory_name.c_str());
	//std::cout << "  Directory created:\n    " << directory_name  << std::endl;
#else
	mode_t nMode = 0733; // UNIX style permissions
	if (_stat(directory_name.c_str(), &st) == 0) { // exists
		std::cout << "\n\t\tERROR: directory already exists" << std::endl;
		return;
	}
	mkdir(sPath.c_str(), nMode);
#endif
}

bool Logging::LogFile(std::string tar_directory, int open_status)
{	
	if (log_stream.is_open()) {
		log_stream.close();
	}
	bool valid = false;

	if (open_status == 2) {
		if (std::ifstream(tar_directory)) {
			//std::cout << "\n\t\tFile already exists!" << std::endl;
			return valid;
		}
	}
	else if (!(std::ifstream(tar_directory))) {
		//std::cout << "\n\t\tFile does not exist!" << std::endl;
		return valid;
	}

	switch (open_status) {
	case 1: // READ
		log_stream.open(tar_directory, std::ios_base::out | std::fstream::in);
		valid = true;
		break;
	case 2: // CREATE
		log_stream.open(tar_directory, std::ios_base::out | std::ios_base::app);
		valid = true;
		break;
	case 3: // APPEND
		log_stream.open(tar_directory, std::ios_base::app | std::ios_base::out);
		valid = true;
		break;
	case 4: // TRUNCATE
		log_stream.open(tar_directory, std::ios::trunc);
		valid = true;
		break;
	default: // READ/WRITE?
		log_stream.open(tar_directory, std::ios_base::in | std::ios_base::out);
		valid = true;
		break;
	}
	return valid;
}
