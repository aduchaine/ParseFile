#include <iostream>
#include <string>
#include <map>

#include "parsefunctions.h"
#include "parsedirectory.h"

#include "Logging\logging.h"


std::vector<std::string> output_file_data_vec;
std::vector<std::string> input_file_data_vec;
std::vector<std::string> transfer_string_vec;

std::vector<int> strID_vec;
std::vector<int> transfer_int_vec;
std::vector<int> temp_int_vec;
std::vector<int> array_strID_vec;
std::vector<int> content_strID_vec;

bool end_parse = false;
bool process_required = false;


ParseFunctions::ParseFunctions()
{

}

std::string ParseFunctions::PStr(int str_id, std::string arg1, std::string arg2)
{
	std::string r_str;

	if (str_id == 0) { r_str = "\n\tDummy component! {}"; } // testing

	// for parsing directive_type array dimensions - QMLParse
	// additional directive_types must have the equivalent array labels in the same order
	if (str_id == 1) { r_str = "QMLParse/"; }
	if (str_id == 2) { r_str = "QMLfunction_dirFiles"; }
	if (str_id == 3) { r_str = "QMLfunction_inputFiles"; }
	if (str_id == 4) { r_str = "QMLfunction_stringIDs"; }
	if (str_id == 5) { r_str = "QMLdirective_functions"; }
	if (str_id == 6) { r_str = "QMLsubDir_functions"; }

	// commonly used find strings
	if (str_id == 500) { r_str = "0"; }
	if (str_id == 501) { r_str = "["; }			// often parsing arrays
	if (str_id == 502) { r_str = "]"; }			// often parsing arrays
	if (str_id == 503) { r_str = "{"; }			// code files
	if (str_id == 504) { r_str = "}"; }			// code files
	if (str_id == 505) { r_str = " "; }			// often parsing arrays
	if (str_id == 506) { r_str = ","; }			// often parsing arrays
	if (str_id == 507) { r_str = "."; }			// file extensions
	if (str_id == 508) { r_str = "_"; }
	if (str_id == 509) { r_str = "};"; }		// end of array, class, enum, many things
	if (str_id == 510) { r_str = " \t";	}
	if (str_id == 511) { r_str = "{}"; }		// code files
	if (str_id == 512) { r_str = ";"; }			// code files
	if (str_id == 513) { r_str = "\t"; }
	if (str_id == 514) { r_str = ":"; }
	if (str_id == 515) { r_str = "\""; }
	if (str_id == 516) { r_str = "/\\"; }		// path separators
	if (str_id == 517) { r_str = "case"; }		// code files, for switch statements
	if (str_id == 518) { r_str = "break;"; }	// code files
	if (str_id == 519) { r_str = "\n"; }		// code files
	if (str_id == 520) { r_str = " \t\f\v\n\r"; } // whitespace

	// commonly used insertion strings
	if (str_id == 600) { r_str = "case " + arg1 + ":"; }	// code files, for switch statements
	if (str_id == 601) { r_str = "break; "; }				// code files

	return r_str;
}

void ParseFunctions::MainipulateArrayData()
{
	Log.LogRecord(Logs::High, Logs::General, "array_strID_vec %i", array_strID_vec.size());
	Log.LogRecord(Logs::High, Logs::General, "transfer_int_vec %i", transfer_int_vec.size());

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		std::cout << transfer_int_vec.at(i) << ",";
	}
	std::cout << std::endl;

}

void ParseFunctions::TransferVec(int data_container, int i_data1, bool clear_vec)
{
	if (clear_vec == true) {
		transfer_int_vec.clear();
	}
	else {
		transfer_int_vec.push_back(i_data1);
	}
}

// this needs to check "parse_indir_vec" (the infile directory) for files - or separate the files further down the line
// this function is specific to Parse Directive 1 F1,2
void ParseFunctions::GetNewInputFilenames(std::string file_ext)
{
	std::string in_dir;
	std::string in_filename;
	std::string out_file_line;
	std::size_t found_ext;
	std::size_t found;

	bool is_found;
	int line_num = 0; // this is only for testing atm

	// cycle through all files for possible parse
	for (unsigned int i = 0; i < input_filename_vec.size(); i++) {
		in_filename = input_filename_vec.at(i);
		is_found = false;

		// check these files for target parse file extension
		found_ext = in_filename.find(file_ext);
		if (found_ext != std::string::npos) {

			// parse output file line by line for target files(string)
			for (unsigned int j = 0; j < output_file_data_vec.size(); j++) {
				out_file_line = output_file_data_vec.at(j);
				++line_num;

				// find the filename with the accpetable ext
				found = out_file_line.find(in_filename);
				if (found != std::string::npos) {
					line_num = 0;
					is_found = true;
					Log.LogRecord(Logs::High, Logs::General, " Exists: %s line_num: %i", in_filename, line_num);
					break;
				}
			}
			if (is_found == false) {
				Log.LogRecord(Logs::High, Logs::General, " New file found: %s", in_filename);
				transfer_string_vec.push_back(in_filename);
			}
		}
	}
}

void ParseFunctions::GetDelimitedData(std::string string_line, std::string delimiter, int element_type)
{
	/*
	element_type:
	1 = int
	2 = std::string
	*/

	std::string string_data;
	std::string find_char;

	size_t line_pos1 = 0;
	size_t line_pos2 = 0;
	size_t string_data_sz;

	int char_pos = 0;
	int frag_pos = 0;
	int i_data = 0;

	Log.LogRecord(Logs::High, Logs::General, "GetDelimitedData string_line: %s", string_line.c_str());

	for (unsigned int i = 0; i < string_line.length(); ++i) {
		find_char = string_line.at(i);
		++char_pos;
		++frag_pos;
		if (find_char == delimiter) {
			string_data = string_line.substr(frag_pos - char_pos, char_pos - 1);

			if (element_type == 1) {
				i_data = stoi(string_data, &string_data_sz);
				transfer_int_vec.push_back(i_data);
			}
			else if (element_type == 2) {
				transfer_string_vec.push_back(string_data);
			}
			Log.LogRecord(Logs::High, Logs::General, "  string_data: %s", string_data.c_str());
			char_pos = 0;
		}
	}
	line_pos1 = string_line.find_last_not_of(delimiter);
	line_pos2 = string_line.find_last_of(delimiter);

	string_data = string_line.substr(line_pos2 + 1, line_pos1 - line_pos2 + 1);

	if (element_type == 1) {
		i_data = stoi(string_data, &string_data_sz);
		transfer_int_vec.push_back(i_data);
	}
	else if (element_type == 2) {
		transfer_string_vec.push_back(string_data);
	}
	Log.LogRecord(Logs::High, Logs::General, "  string_data: %s", string_data.c_str());
}

// this can be expanded - it currently assumes the filename is at the end of the string
// expansion can include parsing for extensions and putting the string together from there to see if it makes sense
// can likely replace other methods where this type of function is needed - atm 1 usage
std::string ParseFunctions::ExtractFilenameFromString(std::string string_line)
{
	std::string find_string;
	std::string filename;

	std::size_t line_pos1;
	std::size_t line_pos2;

	line_pos2 = string_line.find_last_of(ALPHA_CHAR);
	if (line_pos2 != std::string::npos) {
		find_string = PStr(516);

		line_pos1 = string_line.find_last_of(find_string);
		if (line_pos1 != std::string::npos) {
			filename = string_line.substr(line_pos1 + 1, line_pos2 - line_pos1);
		}
	}
	return filename;
}

std::string ParseFunctions::GetStringBetweenChars(std::string string_line, std::string find_string1, std::string find_string2, bool find_last)
{
	std::string find_frag;

	int char_pos = 0;
	int frag_pos = 0;

	std::size_t line_pos;

	if (find_last == true) {
		line_pos = string_line.find_last_of(find_string1);
		if (line_pos != std::string::npos) {
			frag_pos = line_pos;

			line_pos = string_line.find_last_of(find_string2);
			if (line_pos != std::string::npos) {
				char_pos = line_pos;
				find_frag = string_line.substr(frag_pos + 1, char_pos - frag_pos - 1);
			}
		}
	}
	else {
		line_pos = string_line.find(find_string1);
		if (line_pos != std::string::npos) {
			frag_pos = line_pos;

			line_pos = string_line.find(find_string2);
			if (line_pos != std::string::npos) {
				char_pos = line_pos;
				find_frag = string_line.substr(frag_pos + 1, char_pos - frag_pos - 1);
			}
		}
	}
	Log.LogRecord(Logs::High, Logs::General, " line-frag:(%s)%s", string_line, find_frag);
	return find_frag;
}

std::string ParseFunctions::ReplaceStringBetweenChars(std::string string_line, std::string find_string1, std::string find_string2, std::string replace_string, bool replace_last)
{
	std::string new_string;

	int char_pos = 0;
	int frag_pos = 0;

	std::size_t line_pos;
	
	new_string = string_line;

	if (replace_last == true) {
		line_pos = string_line.find_last_of(find_string1);
		if (line_pos != std::string::npos) {
			frag_pos = line_pos;

			line_pos = string_line.find_last_of(find_string2);
			if (line_pos != std::string::npos) {
				char_pos = line_pos;
				new_string.replace(frag_pos + 1, char_pos - frag_pos - 1, replace_string);
			}
		}
	}
	else {
		line_pos = string_line.find(find_string1);
		if (line_pos != std::string::npos) {
			frag_pos = line_pos;

			line_pos = string_line.find(find_string2);
			if (line_pos != std::string::npos) {
				char_pos = line_pos;
				new_string.replace(frag_pos + 1, char_pos - frag_pos - 1, replace_string);
			}
		}
	}
	Log.LogRecord(Logs::High, Logs::General, " replace_string: %s", replace_string);
	return new_string;
}

// each incremental operator subtracts 1 from count for accurate return value
// returns actual line number not something with an offset
// if first_line == false, the second string string to find is contingent upon finding the first string
int ParseFunctions::GetLineNumberAtString(bool output_data, unsigned int begin_pos, unsigned int end_pos, std::string first_string, std::string second_string, bool first_line_only)
{
	std::string source_line;
	int num_line1 = 0;
	int num_line2 = 1;
	int line_number = 0;
	size_t line_pos = 0;
	bool process = false;	

	if (output_data == true) {
		if (end_pos == 0) {
			end_pos = output_file_data_vec.size();
		}

		for (unsigned int i = begin_pos; i < output_file_data_vec.size(); ++i) {
			if (i >= end_pos) {
				break;
			}
			source_line = output_file_data_vec.at(i);
			++num_line1;

			line_pos = source_line.find(first_string);
			if (line_pos != std::string::npos) {
				if (first_line_only == false) {					

					for (unsigned int j = i + 1; j < output_file_data_vec.size(); ++j) {
						if (j >= end_pos) {
							break;
						}
						source_line = output_file_data_vec.at(j);
						++num_line2;

						line_pos = source_line.find(second_string);
						if (line_pos != std::string::npos) {
							process = true;
							break;
						}
					}
				}
				else {
					process = true;
				}
				break;
			}
		}
	}
	if (output_data == false) {
		if (end_pos == 0) {
			end_pos = input_file_data_vec.size();
		}

		for (unsigned int i = begin_pos; i < input_file_data_vec.size(); ++i) {
			if (i >= end_pos) {
				break;
			}
			source_line = input_file_data_vec.at(i);
			++num_line1;			

			line_pos = source_line.find(first_string);
			if (line_pos != std::string::npos) {
				if (first_line_only == false) {

					for (unsigned int j = i + 1; j < input_file_data_vec.size(); ++j) {
						if (j >= end_pos) {
							break;
						}
						source_line = input_file_data_vec.at(j);
						++num_line2;
						
						line_pos = source_line.find(second_string);
						if (line_pos != std::string::npos) {
							process = true;
							Log.LogRecord(Logs::High, Logs::General, "j %i end_line %s", j, source_line.c_str());
							break;
						}
					}
				}
				else {
					process = true;
				}
				break;
			}
		}
	}
	if (process == false) {
		Log.LogRecord(Logs::Med, Logs::General, "Could not find string.");		
		return line_number;
	}
	if (first_line_only == true) {
		line_number = num_line1 - 1 + begin_pos;
	}
	else if (begin_pos == 0) {
		line_number = num_line2 - 1 + num_line1 - 1;
	}
	else {
		line_number = num_line2 - 1 + begin_pos;
	}
	return line_number;
}

int ParseFunctions::GetSwitchCaseCt(bool output_data, unsigned int num_line1, unsigned int num_line2)
{
	std::string find_case;
	std::string source_line;

	size_t line_pos = 0;
	int case_count = 0;

	find_case = PStr(517); // "case"

	if (output_data == true) {
		for (unsigned int i = num_line1; i < output_file_data_vec.size(); ++i) {
			source_line = output_file_data_vec.at(i);
			if (i >= num_line2) {
				break;
			}

			line_pos = source_line.find(find_case);
			if (line_pos != std::string::npos) {
				++case_count;
			}
		}
	}
	return case_count;
}

bool ParseFunctions::CheckCaseValue(std::string source_line, unsigned int case_value)
{
	std::string find_case;
	std::string find_colon;
	std::string case_string;

	size_t line_pos1 = 0;
	size_t line_pos2 = 0;
	size_t num_sz = 0;

	unsigned int found_case = 0;

	bool valid = false;

	find_case = PStr(517); // "case"

	line_pos1 = source_line.find(find_case);
	if (line_pos1 != std::string::npos) {
		find_colon = PStr(514); // ":"

		line_pos2 = source_line.find(find_colon);
		if (line_pos2 != std::string::npos) {
			case_string = source_line.substr(line_pos1 + find_case.length(), line_pos2 - (line_pos1 + find_case.length()));

			// number matches what it should be - check the page path-filename
			found_case = stoi(case_string, &num_sz);
		}
	}
	Log.LogRecord(Logs::Med, Logs::General, "found_case: %i case_value: %i source_line: %s", found_case, case_value, source_line.c_str());

	if (found_case == case_value) {
		valid = true;
	}
	return valid;
}

void ParseFunctions::InsertCaseIntoSwitch(bool output_data, int case_value, std::string case_code, int num_line_offset, int num_indent)
{
	std::vector<std::string>::iterator v_it;

	std::string tab_format;
	std::string case_enum;

	for (int i = 0; i < num_indent; ++i) {
		tab_format.append(PStr(513)); // "\t"
	}

	case_enum = std::to_string(case_value);

	if (output_data == true) {
		v_it = output_file_data_vec.begin();

		Log.LogRecord(Logs::Med, Logs::General, "Insert (%s) at pos (%i)", PStr(600, case_enum).c_str(), num_line_offset);

		output_file_data_vec.emplace(v_it + num_line_offset, tab_format + PStr(601)); // "break;"
		output_file_data_vec.emplace(v_it + num_line_offset, case_code);
		output_file_data_vec.emplace(v_it + num_line_offset, tab_format + PStr(600, case_enum)); // ""case " + arg1 + ":" arg1 = string case_value
	}	
}

bool ParseFunctions::EraseFileData(bool output_data, int erase_type, unsigned int erase_pos, unsigned int find_begin_pos, unsigned int pos_limit, std::string upto_string, int num_line_offset)
{
	/*
	erase_type:
		0 = erase # lines of value "pos_limit"
		1 = erase lines up to "upto_string"
		2 = erase lines only if "upto_string" is found before "pos_limit"
	*/

	std::vector<std::string>::iterator v_it;
	std::string source_line;
	size_t line_pos = 0;
	int line_counter = 0;
	bool process = false;

	line_counter = num_line_offset;

	if (output_data == true) {
		// erase lines up to string
		if (erase_type == 0) {
			line_counter = line_counter + pos_limit;
			process = true;
			Log.LogRecord(Logs::Med, Logs::General, "Erase %i lines at pos (%i)", line_counter, erase_pos);
		}
		else if (erase_type == 1) {
			for (unsigned int i = find_begin_pos; i < output_file_data_vec.size(); ++i) {
				source_line = output_file_data_vec.at(i);
				++line_counter;

				Log.LogRecord(Logs::High, Logs::Test, "Source line:%s", source_line.c_str());

				line_pos = source_line.find(upto_string);
				if (line_pos != std::string::npos) {
					process = true;
					Log.LogRecord(Logs::Med, Logs::General, "Erase %i lines up to string (%s)", line_counter, upto_string.c_str());
					break;
				}
			}
		}
		else if (erase_type == 2) {
			for (unsigned int i = find_begin_pos; i < output_file_data_vec.size(); ++i) {
				source_line = output_file_data_vec.at(i);
				if (find_begin_pos >= pos_limit) {
					break;
				}
				++line_counter;

				line_pos = source_line.find(upto_string);
				if (line_pos != std::string::npos) {
					Log.LogRecord(Logs::Med, Logs::General, "Erase %i lines w/ limit up to string (%s)", line_counter, upto_string.c_str());
					process = true;
					break;
				}
			}
		}

		if (process == true) {
			v_it = output_file_data_vec.begin();
			for (int i = 0; i < line_counter; ++i) {
				Log.LogRecord(Logs::High, Logs::General, "Erase-%s", output_file_data_vec.at(erase_pos).c_str());
				output_file_data_vec.erase(v_it + erase_pos);
			}
			Log.LogRecord(Logs::High, Logs::General, "Erase %i lines at pos (%i)", line_counter, erase_pos);
		}
	}
	return process;
}

// parse type could be defined by Parse Directive or p_func and possibly renamed to be more descriptive
void ParseFunctions::ParseForArrayData(int parse_type, int parse_data_type, int array_dimensions, int return_data_type, std::string find_string)
{
	std::string string_line;
	std::string find_str;

	std::size_t line_pos;

	int ydim = 0;
	int xdim = 0;
	int line_counter = 0;

	bool array_complete = false;

	ydim = ArrayDimFromFileData(true, false, false, find_string, "");

	Log.LogRecord(Logs::High, Logs::General, "ydim %i", ydim);

	if (parse_type == 1) { // parsing infiles - impies a string vector atm
		if (parse_data_type == 1) { // parsing strings in a vector
			for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
				if (array_complete == true) {
					break;
				}

				string_line = input_file_data_vec.at(i);

				line_pos = string_line.find(find_string);
				if (line_pos != std::string::npos) {
					if (array_dimensions == 1) { // parse 1D array

						// array_XDim = xdim; // this is a very important variable to be passed to w/e function is going to use it
						
						if (return_data_type == 1) { // return intergers in a vector
							GetArrayNumbers(string_line, find_string, line_pos);
						}
						array_complete = true;
					}
					else if (array_dimensions == 2) { // parse 2D array						
						line_counter = ydim;
						
						find_str = PStr(503); // "{"
						for (unsigned int j = i + 1; j < input_file_data_vec.size(); ++j) {
							string_line = input_file_data_vec.at(j);
							
							line_pos = string_line.find(find_str);
							if (line_pos != std::string::npos) {								

								if (line_counter > 0) {
									if (return_data_type == 1) { // return intergers in a vector
										GetArrayNumbers(string_line, "", 0);
									}
									--line_counter;
								}
							}
						}
						array_complete = true;
					}
				}
			}
		}
	}
}

// if parsing data into an array, we need to use some sort of data conatainer ?? transfer_string_vec helps with replacements
// this does not handle expanding the array_YDim/XDim
void ParseFunctions::ParseIntoArray(int parse_type, int parse_data_type, int array_dimensions, int in_data_type, std::string find_string, int y_dim)
{
	std::vector<std::string>::iterator v_it;

	std::string string_line;
	std::string find_strings;
	std::string dim_string;

	std::size_t line_pos;

	int x_dim = 0;
	int ydim = 0;
	int line_counter = 0;
	int max_string_length = 0;

	bool array_complete = false;

	x_dim = transfer_int_vec.size() / y_dim;

	ydim = ArrayDimFromFileData(false, false, false, find_string, ""); // unaltered ydim
	max_string_length = MaxCharLength(); // expand usage

	if (parse_type == 2) { // parsing outfiles - impies a string vector atm
		if (parse_data_type == 1) { // parsing strings in a vector
			for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
				if (array_complete == true) {
					break;
				}
				string_line = output_file_data_vec.at(i);

				line_pos = string_line.find(find_string);
				if (line_pos != std::string::npos) {
					if (array_dimensions == 1) { // parse 1D array
						find_strings = PStr(503); // "{"

						line_pos = string_line.find(find_strings);
						if (line_pos != std::string::npos) {
							if (in_data_type == 2) { // return intergers in a vector								
								string_line = string_line.substr(line_pos, string_line.length() - line_pos);
								ReplaceArrayNumbers(string_line, "", 0, x_dim, 0, max_string_length);
							}
						}

						dim_string = std::to_string(x_dim);
						ArrayDimFromFileData(false, true, true, find_string, dim_string);

						v_it = output_file_data_vec.begin();
						for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
							if (array_complete == true) {
								break;
							}
							string_line = output_file_data_vec.at(j);

							line_pos = string_line.find(find_strings);
							if (line_pos != std::string::npos) {								
								for (unsigned int k = 0; k < transfer_string_vec.size(); ++k) {
									string_line = string_line.substr(0, line_pos) + transfer_string_vec.at(k);									
									output_file_data_vec.at(j) = string_line;
								}								
								array_complete = true;
							}
						}
					}
					else if (array_dimensions == 2) { // parse 2D array						
						line_counter = y_dim; // use altered array dimensions					

						find_strings = PStr(503); // "{"					
						for (unsigned int j = i + 1; j < output_file_data_vec.size(); ++j) {
							string_line = output_file_data_vec.at(j);

							line_pos = string_line.find(find_strings);
							if (line_pos != std::string::npos) {
								if (line_counter <= y_dim - ydim) {
									string_line = PStr(513) + PStr(511) + PStr(506); // "\t" + "{}" + ","
								}
								if (line_counter > 0) {
									if (in_data_type == 2) { // int to a string file
										ReplaceArrayNumbers(string_line, "", 0, x_dim, abs(line_counter - y_dim), max_string_length);
									}
									--line_counter;									
								}
								if (line_counter <= 0) {
									break;
								}
							}
						}
						// change array dim
						dim_string = std::to_string(y_dim);
						ArrayDimFromFileData(false, true, false, find_string, dim_string);
						dim_string = std::to_string(x_dim);
						ArrayDimFromFileData(false, true, true, find_string, dim_string);						

						// get altered array data ready for output - the function returns a value but it's uneeded for this purpose
						line_counter = 0;
						v_it = output_file_data_vec.begin();
						for (unsigned int j = i + 1; j < output_file_data_vec.size(); ++j) {
							if (array_complete == true) {
								break;
							}
							string_line = output_file_data_vec.at(j);

							line_pos = string_line.find(find_strings);
							if (line_pos != std::string::npos) {								
								for (unsigned int k = 0; k < transfer_string_vec.size(); ++k) {
									++line_counter;
									if (ydim >= line_counter) { // use original ydim here
										output_file_data_vec.at(j + k) = transfer_string_vec.at(k);
									}
									else {
										output_file_data_vec.insert(v_it + j + k, transfer_string_vec.at(k));
									}									
								}
								if (y_dim < ydim) {
									line_counter = ydim - y_dim;
									for (int l = 0; l < line_counter; ++l) {
										output_file_data_vec.erase(v_it + ydim + l);
									}
								}
								array_complete = true;
							}
						}
					}
				}
			}
		}
	}
}

// uses "transfer_string_vec" as the container to transfer data into the .cpp file
void ParseFunctions::ReplaceArrayNumbers(std::string string_line, std::string found_string, size_t line_position, int x_dim, int y_dim, int max_char_length)
{
	std::string find_string;
	std::string find_frag;
	std::string string_data;

	std::size_t line_pos;
	std::size_t array_begin_pos;

	int array_char_size = 0;
	int delimiter_count = 0;
	
	find_frag = string_line.substr(line_position + found_string.length(), string_line.length() - line_position - found_string.length());

	find_string = PStr(503); // "{"	- find the bracket which should be in the 0 position
	array_begin_pos = find_frag.find(find_string);
	if (array_begin_pos != std::string::npos) {
		find_frag = find_frag.substr(array_begin_pos + find_string.length(), find_frag.length() - array_begin_pos - find_string.length());
		find_string = PStr(504); // "}" find the bracket which should be at the end of the string we care about

		line_pos = find_frag.find_last_of(find_string);
		if (line_pos != std::string::npos) {
			find_frag = find_frag.substr(0, line_pos);
			array_char_size = find_frag.length();
			
			for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
				if (i != (y_dim * x_dim + delimiter_count)) {
					continue;
				}
				++delimiter_count;

				if (delimiter_count >= x_dim) {
					string_data.append(std::to_string(transfer_int_vec.at(y_dim * x_dim + delimiter_count - 1)));
					break;
				}
				string_data.append(std::to_string(transfer_int_vec.at(y_dim * x_dim + delimiter_count - 1)) + PStr(506));
			}
			string_data = ArrayAddSpaces(string_data, x_dim, max_char_length);

			Log.LogRecord(Logs::High, Logs::General, "(%i) array_begin_pos:line_pos (%i)", array_begin_pos, line_pos);
		}
	}
	find_frag = string_line;

	find_frag.replace(array_begin_pos + 1, array_char_size, string_data);
	transfer_string_vec.push_back(find_frag);

	Log.LogRecord(Logs::High, Logs::General, " final find_frag-size-Ydim:(%s):%i:", find_frag, find_frag.size(), y_dim);
	Log.LogRecord(Logs::High, Logs::General, "final string_data-size:(%s):%i", string_data, string_data.size());

	line_pos = 0;
	find_frag.clear();	
}

void ParseFunctions::GetArrayNumbers(std::string string_line, std::string found_string, size_t line_position)
{
	std::string find_string;
	std::string find_frag;
	
	std::size_t line_pos;

	find_frag = string_line.substr(line_position + found_string.length(), string_line.length() - line_position - found_string.length());
	find_string = PStr(503); // "{"

	Log.LogRecord(Logs::High, Logs::General, "GetArrayNumbers find_frag (%s) %i", find_frag.c_str(), find_frag.size());

	line_pos = find_frag.find(find_string);
	if (line_pos != std::string::npos) {
		find_frag = find_frag.substr(line_pos + find_string.length(), find_frag.length() - (line_pos + find_string.length()));
		find_string = PStr(504); // "}"

		Log.LogRecord(Logs::High, Logs::General, "find_frag-size: (%s) %i", find_frag.c_str(), find_frag.size());

		line_pos = find_frag.find_last_of(find_string);
		if (line_pos != std::string::npos) {
			find_frag = find_frag.substr(0, line_pos);

			find_frag = RemoveWhitespace(find_frag);
			
			find_string = PStr(506); // ","

			Log.LogRecord(Logs::High, Logs::General, "final find_frag-size: (%s) %i", find_frag.c_str(), find_frag.size());
			
			GetDelimitedData(find_frag, find_string, 1);	
		}
	}	
	Log.LogRecord(Logs::High, Logs::General, "transfer_int_vec.size: %i", transfer_int_vec.size());
	//for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) { std::cout << transfer_int_vec.at(i) << ","; } std::cout << std::endl;
}

// need to prep the dir/file then container
int ParseFunctions::ArrayDimFromFileData(bool from_input, bool replace, bool get_xdim, std::string find_string, std::string replace_value)
{
	std::string string_line;
	std::string find_frag;
	std::string find_strings;

	std::size_t line_pos;
	std::size_t num_sz;

	int ydim = 0;
	int xdim = 0;

	if (from_input == true) {
		for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
			string_line = input_file_data_vec.at(i);

			line_pos = string_line.find(find_string);
			if (line_pos != std::string::npos) {
				if (get_xdim == true) {
					if (replace == true) {
						input_file_data_vec.at(i) = ReplaceStringBetweenChars(string_line, PStr(501), PStr(502), replace_value, true);
					}
					else {
						find_frag = GetStringBetweenChars(string_line, PStr(501), PStr(502), true); // "[", "]"
						
						if (find_frag.length() > 0) {
							xdim = stoi(find_frag, &num_sz);
						}
					}
					break;
				}
				else if (get_xdim == false) {
					if (replace == true) {
						input_file_data_vec.at(i) = ReplaceStringBetweenChars(string_line, PStr(501), PStr(502), replace_value, false);
					}
					else {
						find_frag = GetStringBetweenChars(string_line, PStr(501), PStr(502), false); // "[", "]"
						
						// this is probably not the best solution
						if (find_frag.find_first_not_of(NUMERIC_CHAR) != std::string::npos) {
							ydim = 0;
						}
						else {
							ydim = stoi(find_frag, &num_sz);
						}						
					}
					break;
				}
			}
		}
	}
	else {
		for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
			string_line = output_file_data_vec.at(i);

			line_pos = string_line.find(find_string);
			if (line_pos != std::string::npos) {
				if (get_xdim == true) {
					if (replace == true) {
						output_file_data_vec.at(i) = ReplaceStringBetweenChars(string_line, PStr(501), PStr(502), replace_value, true);
					}
					else {
						find_frag = GetStringBetweenChars(string_line, PStr(501), PStr(502), true); // "[", "]"

						if (find_frag.length() > 0) {
							xdim = stoi(find_frag, &num_sz);
						}
					}
					break;
				}
				else if (get_xdim == false) {
					if (replace == true) {
						output_file_data_vec.at(i) = ReplaceStringBetweenChars(string_line, PStr(501), PStr(502), replace_value, false);
					}
					else {
						find_frag = GetStringBetweenChars(string_line, PStr(501), PStr(502), false); // "[", "]"
						ydim = stoi(find_frag, &num_sz);
					}
					break;
				}
			}
		}
	}

	if (get_xdim == true) {
		return xdim;
	}
	else {
		return ydim;
	}
}

// only one current usage - multiple element searches will require an additional data container
int ParseFunctions::GetElementCtFromArrayFileData(int data_container, std::string s_marker, int i_marker, std::string s_element, int i_element, bool single_element)
{
	int element_ct = 0;
	int max_elements = 0;

	// transfer_int_vec - int
	if (data_container == 1) {
		for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
			if (transfer_int_vec.at(i) == i_marker) {
				element_ct = 0;
			}
			if (single_element == false) {
				for (unsigned int j = 0; j < temp_int_vec.size(); ++j) {
					if (transfer_int_vec.at(i) == temp_int_vec.at(j)) {
						++element_ct;
					}
				}
			}
			else if (transfer_int_vec.at(i) == i_element) {
				++element_ct;
			}

			if (element_ct > max_elements) {
				max_elements = element_ct;
			}
		}
	}
	return max_elements;
}

// need to prep the container
int ParseFunctions::GetArrayDimFromFileData(int data_container, std::string s_marker, int i_marker, bool get_xdim)
{
	int xcount = 0;
	int ycount = 0;

	int ydim = 0;
	int xdim = 0;

	// transfer_int_vec - int
	if (data_container == 1) {
		for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
			if (transfer_int_vec.at(i) == i_marker) {
				++xcount;
				if (ydim < ycount) {
					ydim = ycount;
				}
				ycount = 0;
			}
			++ycount;
		}
		if (xdim < xcount) {
			xdim = xcount;
		}
		if (ydim < ycount) {
			ydim = ycount;
		}
	}

	if (get_xdim == true) {
		return xdim;
	}
	else {
		return ydim;
	}
}

int ParseFunctions::MaxCharLength()
{
	std::string i_string;
	size_t char_length = 0;

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		i_string = std::to_string(transfer_int_vec.at(i));
		if (i_string.length() > char_length) {
			char_length = i_string.length();
			Log.LogRecord(Logs::High, Logs::General, "char_length: %d", char_length);
		}
	}
	return char_length;
}

std::string ParseFunctions::RemoveWhitespace(std::string string_fragment)
{
	std::string find_string;
	std::string string_data;

	size_t line_pos = 0;

	string_data = string_fragment;

	find_string = PStr(520); // " " - space

	line_pos = string_data.find_first_of(find_string);
	while (line_pos != std::string::npos) {
		string_data.erase(string_data.begin() + line_pos);
		line_pos = string_data.find_first_of(find_string);
	}
	Log.LogRecord(Logs::High, Logs::General, "original:%s--=--new_string:%s", string_fragment.c_str(), string_data.c_str());
	return string_data;
}

// purely superficial
std::string ParseFunctions::ArrayAddSpaces(std::string string_fragment, int x_dim, int array_element_max_length)
{
	std::string find_string;
	std::string find_char;
	std::string string_data;

	int char_pos = 0;
	int frag_pos = 0;
	int element_count = 0;

	string_data = string_fragment;

	find_string = PStr(506); // ","

	for (unsigned int i = 0; i < string_data.length(); ++i) { // loop through the delimited string array fragment - it's condensed to only numbers and commas
		find_char = string_data.at(i);
		++char_pos;
		++frag_pos;

		if (find_char == find_string) {
			++element_count;

			if (element_count == 1) {
				while (char_pos < array_element_max_length + 2) {
					string_data.insert(0, PStr(505)); // " "
					++char_pos;
				}
			}
			else {
				char_pos = 0;
				for (unsigned int j = frag_pos; j < string_data.length(); ++j) {
					find_char = string_data.at(j);
					++char_pos;
					if (find_char == find_string) {
						break;
					}
				}
				if (element_count < x_dim) {
					while (char_pos < array_element_max_length + 2) {
						string_data.insert(frag_pos, PStr(505)); // " "
						++char_pos;
					}
				}
				else {
					while (char_pos < array_element_max_length + 1) {
						string_data.insert(frag_pos, PStr(505)); // " "
						++char_pos;
					}
				}
			}
			Log.LogRecord(Logs::High, Logs::General, "(%i-%s) element_count   frag_pos:char_pos %i:%i", element_count, find_char.c_str(), frag_pos, char_pos);
			char_pos = 0;
		}
	}

	string_data.insert(string_data.length(), PStr(505));
	return string_data;
}
