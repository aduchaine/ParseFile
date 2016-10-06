#ifndef PARSEDIRECTORY_H
#define PARSEDIRECTORY_H


#include <vector>


#define INFILE_SUBSETS_Y_DIM 3		// artificial limit for PrepInputFiles subset inclusion - this number corresponds to the max row where a subset code can be found


enum FileExtensions {

	No_type = 0,
	ALL_TYPES,
	TEXT,
	QML,
	CPP,
	HEADER,
	MaxFileType
};

static const char* FileType[FileExtensions::MaxFileType] = {

	"",
	".",
	".txt",
	".qml",
	".cpp",
	".h",
};

enum InputDir {

	NothingNess = 0,
	PARSE_FILE,
	MaxInputDir
};

static const char* InputDirectory[InputDir::MaxInputDir] = {

	"",	
	"C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/",
};

enum InputFileNames {

	Zero = 0,
	QMLPARSE,
	MaxInputFile
};

static const char* InputFile[InputFileNames::MaxInputFile] = {

	"",
	"qmlparse.h",
};


class ParseDirectory
{
public:
	ParseDirectory();

	void LoadData(int directive_type); // loads native directories/files to use and search in

	void LoadOutputDirectories(int directive_type); // loaded directories; implies known OUT directories where files will be written to or created
	void LoadOutputFilenames(int directive_type); // used for PDirectives where the output file is known - could limit with "file_ext" also but may require separate array column from file_ext because the current column implies in_files
	void LoadInputDirectories(int directive_type); // loaded directories; implies known IN directories where files are counted, filenames are read or fiels to have their contents read
	void LoadFileExtensions(); // used to increase efficiency of searches where a type is known; often set with PDirective
	void ClearPathData();

	std::string GetOutputDir(unsigned int out_dir, int function_id); // used to get a specific OUT directory string based on Parse Directive - function_id is only for logging/notification
	std::string GetOutputFile(unsigned int out_file, int function_id); // used to get a specific OUT file string based on Parse Directive - function_id is only for logging/notification
	std::string GetInputDir(unsigned int in_dir, int function_id); // used to get a specific IN directory string based on Parse Directive - function_id is only for logging/notification

	void GetInputFiles(std::string tar_directory, std::string file_ext); // used to get all files in a specifc directory with a specific extension
	std::string GetOutputPath(unsigned int out_dir, unsigned int out_file, int function_id); // used to get full output path from dir/file codes
	bool CheckIfDirExists(bool check_out_dir, bool dir_unknown, std::string target_path); // "dir_unknown" uses boost "filesystem" library	

	std::string BoostGetFoldername(std::string target_path);
	void BoostMoveFile(std::string origin_dir, std::string origin_file, std::string dest_dir, std::string dest_file, bool create_dest_dir, bool remove_origin);
	bool BoostCheckPath(int check_status, std::string target_folder, std::string target_file);

	std::string FormatCurrentPath(std::string dir_stop); // can format directory strings with dir_stop as the find folder which is omitted from the cur_woring_dir string to do some other action
	void MakeDirectory(const std::string &directory_name);

};

extern std::vector<std::string> parse_outdir_vec;
extern std::vector<std::string> parse_outpaths_vec;
extern std::vector<std::string> parse_indir_vec;

extern std::vector<std::string> output_directory_vec;
extern std::vector<std::string> output_filename_vec;
extern std::vector<std::string> input_directory_vec;
extern std::vector<std::string> input_filename_vec;


#endif // PARSEDIRECTORY_H
