#ifndef PARSEFILES_H
#define PARSEFILES_H


#include <string>
#include <map>
#include <list>


#define MAX_DIRTYPE_FUNCTIONS 29	// max number of functions between all types + 1 - not additive between types
#define	MAX_DIRECTIVES 14			// max number of directives between all types + 1
#define MAX_SUBDIRECTIVES 4			// max number of subdirectives between all types + 1
#define MAX_FUNCTION_CALLS 5		// main PD function calls including subdirectives
#define MAX_SUBFUNCTION_CALLS 11	// max subdirective function calls
#define FUNCTION_Y_DIM 3			// artificial limit of function_to_directory lines - give static # to iterate through and may change as this program changes - this is mainly for convenience
#define STRID_SUBSETS_Y_DIM 3		// artificial limit for stringID subset inclusion - this number corresponds to the max row where a subset code can be found


enum FileOpenStatus {

    NONE = 0,
    READ = 1,
    CREATE = 2,
    APPEND = 3,
    TRUNCATE = 4
};

// for branching into dir/files/function for that type - this is the only place where any identifying info about the type is found
enum DirectiveType {

	LOCAL_PARSE = 0,
	QT_QML,
	MaxDirectiveType
};

// currently, there will only be one item in memory called when PrepareFunction() is called - this is used for a single element search
struct PrepFuncDir_struct
{
	int p_func_id;				// basically the key or ID - function ID
	std::string out_dir;		// out dir for specific function
	std::string in_dir;			// in dir for specific function
	std::string file_ext;		// file ext for specific function
};


class ParseFiles
{
public:
	ParseFiles();

	void MainParseFunction(int directive_type, int parse_directive);

	// prepares directive_types, directives and functions
	
	void PrepareDirectiveType(int directive_type); // this allows reasonably efficient expansion of the parse system - related to preparing parse functions dir/files/strings		
	void PrepDirTypeArrayData(int directive_type); // saves to memory array data which plugs into the parse system

	void PrepParseDirective(int directive_type, int parse_directive); // Parse Directive: determines the parse functions to use - this will handle user input  1-4 so far 
	int GetFunctionID(int parse_function_count);

	void GetParsePaths(int directive_type, int parse_function_count); // compiles all the in/out directories to do the work from the Parse Directive
	void PrepParseOutpath(int directive_type, int parse_function, bool directory);
	void PrepParseIndir(int directive_type, int parse_function);

	void PrepareFunction(int directive_type, int transfer_status, int parse_function_count, int subdirective_count);
	std::string GetFileExt(int directive_type, int parse_function);
	void GetStringIDs(int directive_type, int str_transfer_status, int parse_function); // basis of all parsing - puts all strings for searching, insertion or replacement into memory - they are in order numerically by strID
	void GetSubsetStringIDs(int directive_type, int subset_id);
	void PrepInputFiles(int directive_type, int parse_function);

	int GetSubdirectiveFunctionCt(int directive_type, int parse_subdirective);
	unsigned int GetInputFileCt(int directive_type, int parse_function_count);
	int GetIFTransferStatus(int directive_type, int parse_function);

	void AddToPrepFuncList(int function_id, std::string output_directory, std::string input_directory, std::string file_extension, bool clear); // clears previous
	std::string GetElementFromPrepFuncList(int function_id, int element);
	int GetFuncIDFromList(int find_id);
	std::list<PrepFuncDir_struct> prepfunc_list;

	// ^^^ prepares directive_types, directives and functions ^^^

	void RemoveStringIDs(int strid_pos, int subset_pos);
	void GetOutputData(std::string tar_directory);
	void GetInputData(std::string tar_directory);

	std::string MakeNewFilename(std::string file_ext, std::string filename); // makes a generic filename with "###" suffixes - pass a unique name and possibly a bool setting to override the generic method
	bool FileOpen(std::string tar_directory, int open_status);
	bool CheckIfFileExists(bool check_out_file, bool file_unknown, std::string filename); // "file_unknown" uses boost "filesystem" library
	void FileReWrite(std::string tar_directory, bool clear_mem); // should pass a file open type (truncate, append, etc)

	void DoDirectiveCleanup(bool clear_directive, bool clear_paths); // clears out all vectors associated with a Parse Directive - turn switches into a bitmask

	bool CheckBitFlags(int num_bits, int bit_value, int bit_pos);
	int GetBitPos(int num_bits, int bit_value);
	unsigned int GetBitValue(int bit_pos, bool total_value);

	// simple testing functions
	void ShowDirectiveTypeData(int directive_type);
	void GetOutDirs();
	void GetOutPath();
	void GetInDirs();
	void GetInFiles();
	void GetNewInFiles();

	// unused
	void ParseSubdirectives(int parse_subdirective, int parse_function_count, int subdirective_count);

};

#endif // PARSEFILES_H
