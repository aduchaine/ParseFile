#ifndef PARSEFUNCTIONS_H
#define PARSEFUNCTIONS_H


#include <vector>


#define ALPHA_CHAR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMERIC_CHAR "0123456789"
#define ALPHANUM_CHAR "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"


enum StringIDSubsets {

	NoSubset = 0,
	QML_PARSE_STRIDS,						// QML_PARSE_STRIDS DirectiveType setup strIDs
	MaxLocalSubsets

};

static const char * PStrSubsets[StringIDSubsets::MaxLocalSubsets] = {

	"",
	"{ 1, 2, 3, 4, 5, 6 }",

};


class ParseFunctions
{
public:
	ParseFunctions();
	
	void MainipulateArrayData();

	// all below can be used by any function which needs it - I think
	std::string PStr(int str_id, std::string arg1 = "", std::string arg2 = "");
	void TransferVec(int data_container, int i_data1, bool clear_vec);
	void ParseForArrayData(int parse_type, int parse_data_type, int array_dimensions, int return_data_type, std::string find_string);
	void ParseIntoArray(int parse_type, int parse_data_type, int array_dimensions, int in_data_type, std::string find_string, int y_dim = -1);
	void GetArrayNumbers(std::string string_line, std::string string_to_find, size_t line_position);
	void ReplaceArrayNumbers(std::string string_line, std::string string_to_find, size_t line_position, int x_dim, int y_dim, int max_char_length);
	void GetDelimitedData(std::string string_line, std::string delimiter, int data_type);
	void GetNewInputFilenames(std::string file_ext); // this searches the file in memory from the previous step for any new/missing filenames and puts new files into memory for PDirective 1 code addition

	// in general, these create more cpu work but reduce coding work and are quite useful
	int ArrayDimFromFileData(bool from_input, bool replace, bool get_xdim, std::string find_string, std::string replace_value);	
	int GetElementCtFromArrayFileData(int data_container, std::string s_marker, int i_marker, std::string s_element, int i_element, bool single_element); // must use transfer_int_vec/temp_int_vec	
	int GetArrayDimFromFileData(int data_container, std::string s_marker, int i_marker, bool get_xdim); // sets XDim/YDim for other uses - markers are what separates a row/column of X or Y values
	std::string ExtractFilenameFromString(std::string string_line);
	std::string GetStringBetweenChars(std::string string_line, std::string find_string1, std::string find_string2, bool find_last);
	std::string ReplaceStringBetweenChars(std::string string_line, std::string find_string1, std::string find_string2, std::string replace_string, bool replace_last);
	int GetLineNumberAtString(bool output_data, unsigned int begin_pos, unsigned int end_pos, std::string first_string, std::string second_string, bool first_line_only);

	void InsertCaseIntoSwitch(bool output_data, int case_value, std::string case_code, int num_line_offset, int num_indent);
	bool CheckCaseValue(std::string source_line, unsigned int case_value);
	int GetSwitchCaseCt(bool output_data, unsigned int num_line1, unsigned int num_line2);
	bool EraseFileData(bool output_data, int erase_type, unsigned int erase_pos, unsigned int find_begin_pos, unsigned int pos_limit, std::string upto_string, int num_line_offset);
	std::string RemoveWhitespace(std::string string_fragment); // not purely superficial	
	std::string ArrayAddSpaces(std::string string_fragment, int x_dim, int array_element_max_length); // purely superfical
	int MaxCharLength(); // expand usage

};

extern std::vector<std::string> output_file_data_vec;
extern std::vector<std::string> input_file_data_vec;
extern std::vector<int> strID_vec;

extern std::vector<std::string> transfer_string_vec;
extern std::vector<int> transfer_int_vec;
extern std::vector<int> temp_int_vec;

extern std::vector<int> array_strID_vec;
extern std::vector<int> content_strID_vec;

extern bool end_parse;
extern bool process_required;

#endif // PARSEFUNCTIONS_H
