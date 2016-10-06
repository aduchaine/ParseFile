#include <iostream>
#include <boost\filesystem.hpp> // linker,General,AddtnlLibraryDirectories - (C:\Boost\boost_1_60_0\stage\lib\) C/C++,General,AddtnlIncludeDirectories - (C:\Boost\boost_1_60_0;)
#include <sys/stat.h> // _stat
#include <sys/types.h> // possibly for UNIX MakeDirectory
#include <direct.h> // _mkdir

#include "parsedirectory.h"
#include "parsefiles.h"

#include "Logging\logging.h"

#include "QMLParse\qmlparse.h"


// all files and directories on record
std::vector<std::string> output_directory_vec;
std::vector<std::string> output_filename_vec;
std::vector<std::string> input_directory_vec;
std::vector<std::string> input_filetype_vec;
std::vector<std::string> input_filename_vec;

namespace boostfs = boost::filesystem; // using namespace boost::filesystem;


ParseDirectory::ParseDirectory()
{

}

// load data should be done based on Directive Type and drawn from the type
void ParseDirectory::LoadData(int directive_type)
{
	LoadOutputDirectories(directive_type);
	LoadOutputFilenames(directive_type);
	LoadInputDirectories(directive_type);
	LoadFileExtensions();
}

void ParseDirectory::LoadOutputDirectories(int directive_type)
{
	switch (directive_type) {
	case LOCAL_PARSE:
		Log.LogRecord(Logs::High, Logs::General, "LOCAL_PARSE: LoadOutputDirectories - nothing here: %i", directive_type);
		break;
	case QT_QML:
		for (unsigned int i = 0; i < QTOutDir::MaxOutputDir; ++i) {
			if (strlen(QTOutputDirectory[i]) > 0) {
				output_directory_vec.push_back(QTOutputDirectory[i]);
				Log.LogRecord(Logs::High, Logs::General, "LoadOutputDir: %s", QTOutputDirectory[i]);
			}
		}
		break;
	default:
		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: directive_type invalid: %i", directive_type);
		break;
	}	
}

void ParseDirectory::LoadOutputFilenames(int directive_type)
{
	switch (directive_type) {
	case LOCAL_PARSE:
		Log.LogRecord(Logs::High, Logs::General, "LOCAL_PARSE: LoadOutputFilenames nothing here: %i", directive_type);
		break;
	case QT_QML:
		for (unsigned int i = 0; i < QTOutFile::MaxOutputFile; ++i) {
			if (strlen(QTOutputFile[i]) > 0) {
				output_filename_vec.push_back(QTOutputFile[i]);
				Log.LogRecord(Logs::High, Logs::General, "LoadOutFile: %s", QTOutputFile[i]);
			}
		}
		break;
	default:
		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: directive_type invalid: %i", directive_type);
		break;
	}
}

void ParseDirectory::LoadInputDirectories(int directive_type)
{
	switch (directive_type) {
	case LOCAL_PARSE:
		for (unsigned int i = 0; i < InputDir::MaxInputDir; ++i) {
			if (strlen(InputDirectory[i]) > 0) {
				input_directory_vec.push_back(InputDirectory[i]);
				Log.LogRecord(Logs::High, Logs::General, "LoadInputDir: %s", InputDirectory[i]);
			}
		}
		break;
	case QT_QML:
		for (unsigned int i = 0; i < QTInDir::MaxQMLInDir; ++i) {
			if (strlen(QTInputDirectory[i]) > 0) {
				input_directory_vec.push_back(QTInputDirectory[i]);
				Log.LogRecord(Logs::High, Logs::General, "LoadInputDir: %s", QTInputDirectory[i]);
			}
		}
		break;
	default:
		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: directive_type invalid: %i", directive_type);	
		break;
	}	
}

void ParseDirectory::LoadFileExtensions()
{
	for (unsigned int i = 0; i < FileExtensions::MaxFileType; ++i) { // filenames saved in the header
		if (strlen(FileType[i]) > 0) {
			input_filetype_vec.push_back(FileType[i]);
			Log.LogRecord(Logs::High, Logs::General, "FileExtensions: %s", FileType[i]);
		}
	}
}

void ParseDirectory::ClearPathData()
{
	output_directory_vec.clear();
	output_filename_vec.clear();
	input_directory_vec.clear();
	input_filetype_vec.clear();
}

// these need some work
std::string ParseDirectory::GetOutputDir(unsigned int out_dir, int function_id)
{
	std::string out_dir_string;
	int vec_pos = 0;

	for (unsigned int i = 0; i < output_directory_vec.size(); ++i) {
		++vec_pos;
		if (vec_pos == out_dir) {
			out_dir_string = output_directory_vec.at(i);
			Log.LogRecord(Logs::High, Logs::General, "Output Directory ID: %s", out_dir_string);
		}
	}
	if (out_dir == 0) {
		Log.LogRecord(Logs::Med, Logs::General, "Parse Directive function (%i) does not require an Output Directory.", function_id);
	}
	else if (out_dir_string.length() < 1) {
		Log.LogRecord(Logs::Normal, Logs::Error, "\t*** Error retrieving Output Directory ID: %i", out_dir);
	}

	Log.LogRecord(Logs::High, Logs::General, "out_dir_string: %s", out_dir_string);
	vec_pos = 0;
	return out_dir_string;
}

std::string ParseDirectory::GetOutputFile(unsigned int out_file, int function_id)
{
	std::string out_file_string;
	int vec_pos = 0;

	for (unsigned int i = 0; i < output_filename_vec.size(); ++i) {
		++vec_pos;
		if (vec_pos == out_file) {
			out_file_string = output_filename_vec.at(i);
			Log.LogRecord(Logs::High, Logs::General, " Output File ID: %s", out_file_string);
		}
	}
	if (out_file == 0) {
		Log.LogRecord(Logs::Med, Logs::General, "Parse Directive function (%i) does not require an Output File.", function_id);
	}
	else if (out_file_string.length() < 1) {
		Log.LogRecord(Logs::Normal, Logs::Error, "\t*** Error retrieving Output Filename ID: %i", out_file);
	}

	Log.LogRecord(Logs::High, Logs::General, "out_file_string: %s", out_file_string);
	vec_pos = 0;
	return out_file_string;
}

std::string ParseDirectory::GetInputDir(unsigned int in_dir, int function_id)
{
	std::string in_dir_string;
	int vec_pos = 0;

	for (unsigned int i = 0; i < input_directory_vec.size(); ++i) {
		++vec_pos;
		if (vec_pos == in_dir) {
			in_dir_string = input_directory_vec.at(i);
		}
	}
	if (in_dir == 0) {
		Log.LogRecord(Logs::Med, Logs::General, "Parse Directive function (%i) does not require an Input Directory.", function_id);
	}
	else if (in_dir_string.length() < 1) {
		Log.LogRecord(Logs::Normal, Logs::Error, "\t*** Error retrieving Input Directory ID: %i", in_dir);
	}

	Log.LogRecord(Logs::High, Logs::General, "in_dir_string: %s", in_dir_string);
	vec_pos = 0;
	return in_dir_string;
}

// the second option has gone through a check using boost::filesystem but conditions may have changed
bool ParseDirectory::CheckIfDirExists(bool check_out_dir, bool dir_unknown, std::string target_path)
{
	bool exists = false;

	if (check_out_dir == true) {
		if (dir_unknown == false) {
			for (unsigned int i = 0; i < output_directory_vec.size(); ++i) {
				if (target_path == output_directory_vec.at(i)) {
					exists = true;
					break;
				}
			}
		}
		else {
			exists = BoostCheckPath(3, target_path, "");
		}
	}
	else {
		if (dir_unknown == false) {
			for (unsigned int i = 0; i < input_directory_vec.size(); ++i) {
				if (target_path == input_directory_vec.at(i)) {
					exists = true;
					break;
				}
			}
		}
		else {
			exists = BoostCheckPath(4, target_path, "");
		}
	}
	return exists;
}


std::string ParseDirectory::BoostGetFoldername(std::string target_path)
{
	std::string foldername;

	boostfs::path directory(target_path);
	boostfs::path folder = directory.remove_filename();	

	if (boostfs::is_directory(target_path) == true) {
		foldername = folder.filename().string();
		Log.LogRecord(Logs::High, Logs::General, "BoostGetFoldername: %s", foldername.c_str());
	}
	else {
		Log.LogRecord(Logs::Normal, Logs::General, "  The following path is not a folder:\n    (%s)", target_path.c_str());
	}
	return foldername;
}

void ParseDirectory::BoostMoveFile(std::string origin_dir, std::string origin_file, std::string dest_dir, std::string dest_file, bool create_dest_dir, bool remove_origin)
{	
	boostfs::path origin(origin_dir + origin_file);
	boostfs::path destination(dest_dir + dest_file);
	
	if (create_dest_dir == true) {
		boostfs::create_directory(dest_dir);
	}

	boostfs::copy_file(origin, destination);

	if (remove_origin == true) {
		boostfs::remove_all(origin);
	}
	Log.LogRecord(Logs::High, Logs::General, "BoostMoveFile (no log)");
}

// setup more check file/dir possiblities - passing a "tar_directory" may be better in some circumstances
// parse_outdir_vec/parse_indir_vec are loaded before processing begins and soon after a PDirective is chosen
// another option is to skip using "parse_outdir_vec/in" in favor of "target_path"
// checks for one file/folder at a time, I think
bool ParseDirectory::BoostCheckPath(int check_status, std::string target_folder, std::string target_file)
{
	/*
	check type
	1 = files existing outdir
	2 = files existing indir
	3 = folders existing outdir
	4 = folders existing indir 
	5 = folders + files existing outdir - existing partial directory
	6 = folders + files existing indir - existing partial directory
	*/

	std::string io_directory;

	bool exists = false;

	// checks for files
	if (check_status == 1) {
		for (unsigned int i = 0; i < parse_outdir_vec.size(); ++i) {
			if (parse_outdir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_outdir_vec.at(i);
			boostfs::path directory(io_directory);
			
			// find files in dir
			for (auto j = boostfs::directory_iterator(directory); j != boostfs::directory_iterator(); j++) {
				if (target_file == j->path().filename().string()) {
					exists = true;
				}
			}
		}
	}
	else if(check_status == 2) {
		for (unsigned int i = 0; i < parse_indir_vec.size(); ++i) {
			if (parse_indir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_indir_vec.at(i);
			boostfs::path directory(io_directory);
		
			for (auto j = boostfs::directory_iterator(directory); j != boostfs::directory_iterator(); j++) {
				if (target_file == j->path().filename().string()) {
					exists = true;					
				}
			}
		}
	}
	// checks for directories/folders
	else if (check_status == 3) {
		for (unsigned int i = 0; i < parse_outdir_vec.size(); ++i) {
			if (parse_outdir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_outdir_vec.at(i) + target_folder;			
			boostfs::path directory(io_directory);
			
			if (boostfs::is_directory(io_directory)) {
				Log.LogRecord(Logs::High, Logs::General, "%s -- BoostCheckPath Exists", io_directory);
				exists = true;
			}						
		}
	}
	else if (check_status == 4) {
		for (unsigned int i = 0; i < parse_indir_vec.size(); ++i) {
			if (parse_indir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_indir_vec.at(i) + target_folder;
			boostfs::path directory(io_directory);
			
			if (boostfs::is_directory(io_directory)) {
				Log.LogRecord(Logs::High, Logs::General, "%s -- BoostCheckPath Exists", io_directory);
				exists = true;
			}						
		}
	}
	// checks for files from existing partial directory, provides remaining directory string
	// this is susceptible to false-positives for the incorrect directory - passing the entire directory could avoid this by not using "parse_outdir_vec/parse_indir_vec"s
	else if (check_status == 5) {
		for (unsigned int i = 0; i < parse_outdir_vec.size(); ++i) {
			if (parse_outdir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_outdir_vec.at(i) + target_folder;
			boostfs::path directory(io_directory);

			if (boostfs::is_directory(io_directory)) {
				for (auto j = boostfs::directory_iterator(directory); j != boostfs::directory_iterator(); j++) {
					if (target_file == j->path().filename().string()) {
						exists = true;
					}
				}
			}
		}
	}
	else if (check_status == 6) {
		for (unsigned int i = 0; i < parse_indir_vec.size(); ++i) {
			if (parse_indir_vec.at(i).length() < 1) {
				continue;
			}
			io_directory = parse_indir_vec.at(i) + target_folder;			

			boostfs::path directory(io_directory);
			
			if (boostfs::is_directory(io_directory)) {
				for (auto j = boostfs::directory_iterator(directory); j != boostfs::directory_iterator(); j++) {
					if (target_file == j->path().filename().string()) {
						exists = true;
					}
				}
			}
		}
	}
	return exists;
}

// input directories should behave variably as well
// there should not be any duplicates if parsing only one directory and no static files
// this is only used to get all files in a specifc directory with a specific extension
void ParseDirectory::GetInputFiles(std::string tar_directory, std::string file_ext)
{
	std::string input_file;
	std::size_t found;
	bool duplicate_file = false;	
	
	for (unsigned int i = 0; i < parse_indir_vec.size(); ++i) {
		if (tar_directory == parse_indir_vec.at(i)) {
			//input_directory = parse_indir_vec.at(i);
			
			boostfs::path directory(tar_directory);

			// find files in dir
			for (auto j = boostfs::directory_iterator(directory); j != boostfs::directory_iterator(); j++) {
				input_file = j->path().filename().string();

				Log.LogRecord(Logs::High, Logs::General, "  PossInputFile: %s size %i", input_file, input_filename_vec.size());

				// check found files for extensions
				found = input_file.find(file_ext);
				if (found != std::string::npos) {

					// found, check program list of files for descrepancies 
					for (unsigned int k = 0; k < input_filename_vec.size(); ++k) {

						if (input_file == input_filename_vec.at(k)) {
							duplicate_file = true;
						}
					}

					// if no duplicate add to program list
					if (duplicate_file == false) {
						input_filename_vec.push_back(input_file);
						Log.LogRecord(Logs::High, Logs::General, "  InputFile: %s size %i", input_file, input_filename_vec.size());
					}
					duplicate_file = false;
				}
			}
		}	
	}
}

std::string ParseDirectory::GetOutputPath(unsigned int out_dir, unsigned int out_file, int function_id)
{
	std::string full_path;
	std::string tar_dir;
	std::string tar_file;	

	tar_dir = GetOutputDir(out_dir, function_id);
	tar_file = GetOutputFile(out_file, function_id);
	full_path = tar_dir + tar_file;	
	
	if (tar_dir.length() < 1 && out_dir > 0 || tar_file.length() < 1 && out_file > 0) {
		Log.LogRecord(Logs::Normal, Logs::Error, "Function (%i) Output Path out of range: (%i:%i)", function_id, out_dir, out_file);
	}
	Log.LogRecord(Logs::High, Logs::General, "Output Path:\n  %s", full_path);
	return full_path;
}

// no current uses
std::string ParseDirectory::FormatCurrentPath(std::string dir_stop)
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

void ParseDirectory::MakeDirectory(const std::string &directory_name)
{
	struct _stat st;
#if defined _WIN32
	//struct _stat st;
	if (_stat(directory_name.c_str(), &st) == 0) { // exists
		std::cout << "\n\t\tERROR: directory already exists" << std::endl;
		return;
	}
	_mkdir(directory_name.c_str());
	std::cout << "  Directory created:\n    " << directory_name << std::endl;
#else
	mode_t nMode = 0733; // UNIX style permissions
	if (_stat(directory_name.c_str(), &st) == 0) { // exists
		std::cout << "\n\t\tERROR: directory already exists" << std::endl;
		return;
	}
	mkdir(sPath.c_str(), nMode);
#endif
}
