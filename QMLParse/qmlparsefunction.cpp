#include <iostream>
#include <string>

#include "qmlparsefunction.h"
#include "qmlparse.h"

#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/Logging/logging.h"

#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/parsedirectory.h"
#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/parsefunctions.h"


// general - these could be moved to the general function eventually
std::map<int, int> transfer_i_int_map;
std::map<int, std::string> transfer_i_string_map;
std::map<std::string, std::string> transfer_s_string_map;

std::vector<std::string> temp_string_vec;

// F 1,2
std::vector<std::string> mod_infile_vec;

// F 5
std::map<int, std::string> pageID_to_path_map;

// F 6,7
std::vector<int> fieldID_to_pageID_vec;
std::vector<int> fieldID_to_num_responses_vec;
std::vector<int> responseID_from_fieldID_vec;
int response_array_Ydim = 0;

// F 7
int page_field_iter = 0;
int page_iter = 0;
int menu_count = 0;
int text_count = 0;
int field_iter = 1;

// F 8,12,18
std::vector<int> componentID_from_pageID_vec;

// F 10,16,20
std::map<int, int> all_qmlcomponents_map;
std::map<int, std::string> all_qmlcompstrings_map;

// F 12,19
int component_array_Ydim = 0;
int component_array_Xdim = 0;


ParseFunctions PFunc;


QMLParseFunction::QMLParseFunction()
{

}

void QMLParseFunction::CPP_CODE_DCH(std::string extension) // 1
{
	if (end_parse == true) {
		return;
	}

	std::vector<std::string>::iterator v_it;
	std::string find_string;
	std::string source_line;
	std::string secondary_source_line;
	std::string filename;
	std::string uc_filename;
	std::string new_string;
	std::size_t found;
	std::size_t found_secondary;
	int file_counter = 0;

	Log.LogRecord(Logs::Normal, Logs::General, "     Updating DataCodes header with new code from filenames.");

	PFunc.GetNewInputFilenames(extension); // compare data in output memory and input file names with file extention, put non-existent filenames into a separate memory container

	if (transfer_string_vec.size() == 0) { // no new files, this will skip the remaining steps
		end_parse = true;
		Log.LogRecord(Logs::Normal, Logs::General, " ...no new files, Parse Directive complete");
		return;
	}

	find_string = QPStr(strID_vec[0]); // "MaxDataCode"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		found = source_line.find(find_string);
		if (found != std::string::npos) {
			for (unsigned int k = 0; k < transfer_string_vec.size(); ++k) {
				filename = transfer_string_vec.at(k);

				found_secondary = filename.find_last_of(QPStr(507)); // "."
				if (found_secondary != std::string::npos) {
					filename.erase(filename.begin() + found_secondary);

					filename.insert(found_secondary, QPStr(508));
					mod_infile_vec.push_back(filename);

					for (std::string::size_type h = 0; h < filename.length(); ++h) {
						filename[h] = toupper(filename[h]);
					}
				}
				new_string = QPStr(strID_vec[1], filename);
				v_it = output_file_data_vec.begin();
				output_file_data_vec.emplace(v_it + i + file_counter, new_string);
				++file_counter;
			}
			file_counter = 0;
			break;
		}
	}
	find_string = QPStr(strID_vec[2]); // "DataFileString"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		found = source_line.find(find_string);
		if (found != std::string::npos) {

			for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
				secondary_source_line = output_file_data_vec.at(j);
				found_secondary = secondary_source_line.find_first_of(QPStr(509)); // "};"

				if (found_secondary != std::string::npos) {
					for (unsigned int k = 0; k < transfer_string_vec.size(); ++k) {
						filename = transfer_string_vec.at(k);
						new_string = QPStr(strID_vec[3], filename);

						v_it = output_file_data_vec.begin();
						output_file_data_vec.emplace(v_it + j + file_counter - 1, new_string);
						++file_counter;
					}
					file_counter = 0;
					break;
				}
			}
		}
	}
	find_string = QPStr(strID_vec[4]); // "class DataCodes"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		found = source_line.find(find_string);
		if (found != std::string::npos) {

			for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
				secondary_source_line = output_file_data_vec.at(j);

				found_secondary = secondary_source_line.find_first_of(QPStr(509)); // "};"
				if (found_secondary != std::string::npos && found_secondary < 2) {

					for (unsigned int k = 0; k < mod_infile_vec.size(); ++k) {
						filename = mod_infile_vec.at(k);

						new_string = QPStr(strID_vec[5], filename);
						v_it = output_file_data_vec.begin();
						output_file_data_vec.emplace(v_it + j + file_counter - 1, new_string);
						++file_counter;
					}
					file_counter = 0;
					break;
				}
			}
		}
	}
	transfer_string_vec.clear();
}

void QMLParseFunction::CPP_CODE_DCSRC() // 2
{
	if (end_parse == true) {
		return;
	}

	std::vector<std::string>::iterator v_it;
	std::string find_string;
	std::string source_line;
	std::string secondary_source_line;
	std::string filename;
	std::string uc_filename;
	std::string new_string;
	std::size_t found;
	std::size_t found_secondary;
	int file_counter = 0;

	Log.LogRecord(Logs::Normal, Logs::General, "     Updating DataCodes source with new code from filenames.");

	find_string = QPStr(strID_vec[0]); // "void DataCodes::PrepareFileData()"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		found = source_line.find(find_string);
		if (found != std::string::npos) {

			for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
				secondary_source_line = output_file_data_vec.at(j);

				found_secondary = secondary_source_line.find_first_of(QPStr(504)); // "}"
				if (found_secondary != std::string::npos && found_secondary < 2) {

					for (unsigned int k = 0; k < mod_infile_vec.size(); ++k) {
						filename = mod_infile_vec.at(k);
						uc_filename = filename;

						for (std::string::size_type h = 0; h < filename.length(); ++h) {
							uc_filename[h] = toupper(uc_filename[h]);
						}
						new_string = QPStr(strID_vec[1], uc_filename, filename);

						v_it = output_file_data_vec.begin();
						output_file_data_vec.emplace(v_it + j + file_counter - 1, new_string);
						++file_counter;
					}
					file_counter = 0;
					break;
				}
			}
		}
	}
	find_string = QPStr(strID_vec[2]); // "void DataCodes::GetFileData(std::string tar_directory)"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		found = source_line.find(find_string);
		if (found != std::string::npos) {

			for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
				secondary_source_line = output_file_data_vec.at(j);

				find_string = QPStr(strID_vec[3]);
				found_secondary = secondary_source_line.find(find_string);
				if (found_secondary != std::string::npos) {

					for (unsigned int k = 0; k < mod_infile_vec.size(); ++k) {
						filename = mod_infile_vec.at(k);
						uc_filename = filename;

						for (std::string::size_type h = 0; h < filename.length(); ++h) {
							uc_filename[h] = toupper(uc_filename[h]);
						}
						new_string = QPStr(strID_vec[4], uc_filename, filename);

						v_it = output_file_data_vec.begin();
						output_file_data_vec.emplace(v_it + j + file_counter - 1, new_string);
						++file_counter;
					}
					file_counter = 0;
					break;
				}
			}
		}
	}
	mod_infile_vec.clear();
}

// new pages .qml don't need page number, user will set the page number manually or otherwise
void QMLParseFunction::QML_CREATE_PAGE(std::string filename, int page_number) // 3
{
	if (end_parse == true) {
		return;
	}

	int compid_iter = 0;		
	int compcode = 0;

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		compcode = strID_vec.at(i);

		if (compcode >= 200 || compcode < 100) {
			continue;
		}
		if (CompHasNoID(compcode) == false) {
			++compid_iter;
		}

		AppendQMLComponents(filename, page_number, compcode, compid_iter);
	}
	compid_iter = 0;
	transfer_int_vec.clear();
	Log.LogRecord(Logs::Normal, Logs::General, "  New .qml: (%s) created", filename.c_str());
}

// alter new page .qml - process to insert(emplace) components one after another - good example for insertions
void QMLParseFunction::QML_ALTER_NEWPAGE() // 4
{
	if (end_parse == true) {
		return;
	}

	std::vector<std::string>::iterator v_it;
	std::string new_string;
	std::string find_string;
	std::string source_line;
	std::string snum_page;
	std::string comp_id;

	std::size_t pg_pos;
	std::size_t num_sz;

	int page_id = 0;
	int line_counter = 0;
	int compid_iter = 0;
	
	// ---- for TEST ---- implement correct GetStrIDs() method and remove this section - that means what? array can be parsed, I don't think this function is needed
	static const int qml_component_TEST[12] = { 111, 110, 151, 112, 112, 150, 152, 110, 113, 110, 114, 150 };
	for (unsigned int i = 0; i < 12; ++i) {
		strID_vec.push_back(qml_component_TEST[i]);
	}

	// line counter is in front to set the position with compid_iter for ID set after insertion
	// compid_iter needs to check other ids in the file	

	find_string = QPStr(strID_vec[0]); // "pageID:"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			snum_page = source_line.substr(pg_pos + find_string.length(), source_line.length() - pg_pos);
		}
	}

	// num_page has a space, using stoi and converting back removes the space 
	page_id = stoi(snum_page, &num_sz);
	snum_page = std::to_string(page_id);

	find_string = QPStr(strID_vec[0]); // "pageID: "
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			strID_vec.erase(strID_vec.begin()); // be aware of find string being erased if needing this for multiple pages

			for (unsigned int j = 1; j < strID_vec.size(); ++j) {
				++line_counter;
				v_it = output_file_data_vec.begin();

				switch (strID_vec.at(j)) {
				case 115: // keyboardprop
					new_string = QPStr(strID_vec[j], snum_page);
					output_file_data_vec.emplace(v_it + i + line_counter, new_string);
					break;
				default: // most generic standard components
					if (CompHasNoID(strID_vec.at(j)) == true) {
						new_string = QPStr(strID_vec[j]);
						output_file_data_vec.emplace(v_it + i + line_counter, new_string);
						break;
					}
					++compid_iter;
					comp_id = std::to_string(compid_iter);
					new_string = QPStr(strID_vec[j], comp_id, snum_page);
					output_file_data_vec.emplace(v_it + i + line_counter, new_string);
					break;
				}
			}
			break;
		}
	}
	line_counter = 0;
	compid_iter = 0;	
}

// get map of pageID to .qml pages
void QMLParseFunction::FILE_MATCH_PG(std::string tar_directory) // 5
{
	if (end_parse == true) {
		return;
	}
	std::map<int, std::string>::iterator m_it;

	std::string find_string;
	std::string source_line;
	std::string snum_page;

	std::size_t pg_pos;
	std::size_t num_sz;
	int page_id = 0;

	Log.LogRecord(Logs::High, Logs::General, "  Matching page ID to filename");	

	find_string = QPStr(strID_vec[0]); // "pageID:"
	for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
		source_line = input_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {

			snum_page = source_line.substr(pg_pos + find_string.length(), source_line.length() - pg_pos - find_string.length());
			page_id = stoi(snum_page, &num_sz);			

			// do a page check here
			m_it = pageID_to_path_map.find(page_id);
			if (m_it != pageID_to_path_map.end()) {
				Log.LogRecord(Logs::Normal, Logs::General, "\nMultiple files exist with the same pageID.\n  Resolve this issue and try again.");
				Log.LogRecord(Logs::Normal, Logs::General, "    pageID: %i\n%s", m_it->first, m_it->second.c_str());
				Log.LogRecord(Logs::Normal, Logs::General, "    pageID: %i\n%s", page_id, tar_directory.c_str());
				Log.LogRecord(Logs::Normal, Logs::General, "\n    Aborting Parse Directive.");				
				end_parse = true;
			}
			else {
				pageID_to_path_map[page_id] = tar_directory;
			}
			Log.LogRecord(Logs::High, Logs::General, "(%i) page - %s", page_id, tar_directory);
		
			break;
		}
	}

	Log.LogRecord(Logs::High, Logs::General, "(%i) page - %s", page_id, tar_directory);
	input_file_data_vec.clear();
}

void QMLParseFunction::CPP_ARRAY_GETLOGICIDS() // 6
{
	if (end_parse == true) {
		return;
	}
	process_required = true;

	Log.LogRecord(Logs::High, Logs::General, "     preparing .qml component IDs... ");

	PFunc.ParseForArrayData(1, 1, 1, 1, QPStr(strID_vec[0])); // "fieldID_to_pageID"
	for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
		fieldID_to_pageID_vec.push_back(transfer_int_vec.at(j));
	}
	if (transfer_int_vec.size() == 0) {
		process_required = false;
	}
	transfer_int_vec.clear();

	PFunc.ParseForArrayData(1, 1, 1, 1, QPStr(strID_vec[1])); // "fieldID_to_num_responses"
	for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
		fieldID_to_num_responses_vec.push_back(transfer_int_vec.at(j));
	}
	if (transfer_int_vec.size() == 0) {
		process_required = false;
	}
	transfer_int_vec.clear();

	PFunc.ParseForArrayData(1, 1, 2, 1, QPStr(strID_vec[2])); // "responseID_from_fieldID"	
	for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
		responseID_from_fieldID_vec.push_back(transfer_int_vec.at(j));
	}
	if (transfer_int_vec.size() == 0) {
		process_required = false;
	}

	response_array_Ydim = PFunc.ArrayDimFromFileData(true, false, false, QPStr(strID_vec[2]), "");
	if (response_array_Ydim == 0) {
		process_required = false;
	}
	input_file_data_vec.clear();
	transfer_int_vec.clear();

	Log.LogRecord(Logs::High, Logs::General, "fieldID_to_pageID_vec elements %i", fieldID_to_pageID_vec.size());
	//for (unsigned int i = 0; i < fieldID_to_pageID_vec.size(); ++i) { std::cout << fieldID_to_pageID_vec.at(i) << ","; } std::cout << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "fieldID_to_num_responses_vec elements %i", fieldID_to_num_responses_vec.size());
	//for (unsigned int i = 0; i < fieldID_to_num_responses_vec.size(); ++i) { std::cout << fieldID_to_num_responses_vec.at(i) << ","; } std::cout << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "responseID_from_fieldID_vec elements %i dimensions %i", responseID_from_fieldID_vec.size(), response_array_Ydim);
	//	for (unsigned int i = 0; i < responseID_from_fieldID_vec.size(); ++i) {
	//		std::cout << responseID_from_fieldID_vec.at(i) << ",";
	//	}
	//	std::cout << std::endl;

}

// update global component ID - textID, menuID, etc
// this will be added to once other data is prepared
// need to account for a lack of spaces between ID name and numeric ID - only search for the characters w/o the space and add the space at the end
// if existing IDs have more digits than what is replacing it, the last digits will remain - string.length or strlen() - this may have been fixed
void QMLParseFunction::QML_PG_UPDATELOGICIDS(int page_number) // 7
{
	if (end_parse == true) {
		return;
	}

	std::vector<std::string>::iterator v_it;

	std::string find_string;
	std::string source_line;
	std::string new_string;

	std::size_t pg_pos;

	int pageID = 0;
	int fieldID = 0;
	int responseID = 0;

	int field_comp_iter = 0;
	int page_comp_iter = 0;
	int line_count = 0;

	int comp_iter = 0;
	int fcomp_mult = 0;

	Log.LogRecord(Logs::High, Logs::General, "new page");

	// these need to be reset if doing this multiple times in a parse
	if (page_number == 0) {
		menu_count = 0;
		text_count = 0;
		field_iter = 1;
		page_field_iter = 0;
	}

	find_string = QPStr(strID_vec[0]); // find "textID:"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			++text_count;

			new_string = std::to_string(text_count);
			v_it = output_file_data_vec.begin();
			output_file_data_vec.erase(output_file_data_vec.begin() + i);
			output_file_data_vec.emplace(v_it + i, QPStr(strID_vec[1], new_string));
		}
	}
	find_string = QPStr(strID_vec[2]); // find "menuID:"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			++menu_count;

			new_string = std::to_string(menu_count);
			v_it = output_file_data_vec.begin();
			output_file_data_vec.erase(output_file_data_vec.begin() + i);
			output_file_data_vec.emplace(v_it + i, QPStr(strID_vec[3], new_string));
		}
	}
	find_string = QPStr(strID_vec[4]); // find "fieldID:"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			Log.LogRecord(Logs::High, Logs::General, "%s", source_line);

			for (unsigned int j = field_iter; j < fieldID_to_pageID_vec.size(); ++j) {
				pageID = fieldID_to_pageID_vec.at(j);
				fieldID = j;
				++comp_iter;

				Log.LogRecord(Logs::High, Logs::General, "sourceline %s field(%i) page(%i)", source_line, fieldID, pageID);
				for (unsigned int k = field_iter; k < fieldID_to_num_responses_vec.size(); ++k) {
					if (comp_iter <= fieldID_to_num_responses_vec.at(k)) {

						new_string = std::to_string(fieldID);
						v_it = output_file_data_vec.begin();
						output_file_data_vec.erase(output_file_data_vec.begin() + i);
						output_file_data_vec.emplace(v_it + i, QPStr(strID_vec[5], new_string));
						if (comp_iter == fieldID_to_num_responses_vec.at(k)) {
							++field_iter;
							comp_iter = 0;
						}
					}
					break;
				}
				break;
			}
		}
	}
	find_string = QPStr(strID_vec[6]); // find "responseID:"
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);
		++line_count; // for testing

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			Log.LogRecord(Logs::High, Logs::General, "%s", source_line);

			if (page_comp_iter == 0) {
				for (unsigned int k = 0; k < fieldID_to_pageID_vec.size(); ++k) {
					if (page_iter == fieldID_to_pageID_vec.at(k)) {
						++page_comp_iter;
					}
				}
				Log.LogRecord(Logs::High, Logs::General, "      page:(%i) components:(%i)", page_iter, page_comp_iter);
			}

			if (field_comp_iter == 0) {
				comp_iter = 0;
			}
			fcomp_mult = responseID_from_fieldID_vec.size() / response_array_Ydim * comp_iter; // comp_iter must be reset here if conditions are met

			if (field_comp_iter == 0) {
				++page_field_iter;
				for (unsigned int k = 0; k < fieldID_to_num_responses_vec.size(); ++k) {
					field_comp_iter = fieldID_to_num_responses_vec.at(page_field_iter);
					break;
				}
				Log.LogRecord(Logs::High, Logs::General, "      field:(%i) components:(%i)", page_field_iter, field_comp_iter);
			}
			--field_comp_iter;

			if (field_comp_iter == 0) {
				--page_comp_iter;
			}

			for (unsigned int j = (page_field_iter + fcomp_mult); j < responseID_from_fieldID_vec.size(); ++j) {
				responseID = responseID_from_fieldID_vec.at(j);
				fieldID = (j % (responseID_from_fieldID_vec.size() / response_array_Ydim));

				if (fieldID != page_field_iter) {
					break;
				}
				else {
					new_string = std::to_string(responseID);
					v_it = output_file_data_vec.begin();
					output_file_data_vec.erase(output_file_data_vec.begin() + i);
					output_file_data_vec.emplace(v_it + i, QPStr(strID_vec[7], new_string));

					Log.LogRecord(Logs::High, Logs::General, "line_ct(%i) page(%i) field(%i) resID(%i)", line_count, page_iter, fieldID, responseID);
					++comp_iter;
					break;
				}
			}
		}
	}
	comp_iter = 0;
	++page_iter;
	//output_file_data_vec.clear(); // breaks the Parse Directive if uncommented only used for testing
}

// 2-step function, get array from file and process/organize data into memory
// this will know page # from outside source array
// page_number is passed is because it has dual-purpose - get a single page or get all
// num_pages will be known by counting compID 100 instances
// unique = true - currently implies the order in which the componentIDs inserted into transfer_int_vec in not important
// doing this for individual pages can be used for having the correct "imports", although this is minor atm
void QMLParseFunction::CPP_ARRAY_GETCOMPCODES(bool unique, int page_number) // 8
{
	if (end_parse == true) {
		return;
	}

	int array_YDim = 0;
	int array_XDim = 0;
	int comp_iter = 0;

	bool element_exists = false;

	componentID_from_pageID_vec.clear(); // can't have anything in

	PFunc.ParseForArrayData(1, 1, 2, 1, QPStr(strID_vec[0])); // "componentID_from_pageID"	
	for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
		componentID_from_pageID_vec.push_back(transfer_int_vec.at(j));
	}

	if (transfer_int_vec.size() == 0) {
		process_required = false;
		return;
	}

	array_XDim = PFunc.ArrayDimFromFileData(true, false, true, QPStr(strID_vec[0]), "");
	array_YDim = PFunc.ArrayDimFromFileData(true, false, false, QPStr(strID_vec[0]), "");

	input_file_data_vec.clear();
	transfer_int_vec.clear();

	if (page_number == -1) {
		if (unique == false) {
			for (int h = 0; h < array_XDim; ++h) {
				for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
					if (i % array_XDim != h) {
						continue;
					}
					++comp_iter;
					transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));

					//std::cout << componentID_from_pageID_vec.at(i) << ",";
					if (comp_iter == array_YDim) {
						comp_iter = 0;
					}
				}
			}
		}
		else if (unique == true) {
			for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
				element_exists = false;

				for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
					if (transfer_int_vec.at(j) == componentID_from_pageID_vec.at(i)) {
						element_exists = true;
						break;
					}
				}

				if (element_exists == false) {
					transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));
				}
			}
		}
	}
	else if (page_number >= 0) { // for individual page #
		if (unique == false) {
			for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
				if (i % array_XDim != page_number) {
					continue;
				}
				++comp_iter;
				transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));

				//std::cout << componentID_from_pageID_vec.at(i) << ",";
				if (comp_iter == array_YDim) {
					comp_iter = 0;
				}
			}
		}
		else if (unique == true) {
			for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
				if (i % array_XDim != page_number) { // this is for cherry picking a specific page's compcodes which could be used for "imports"
					continue;
				}
				element_exists = false;

				for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
					if (transfer_int_vec.at(j) == componentID_from_pageID_vec.at(i)) {
						element_exists = true;
						break;
					}
				}
				if (element_exists == false) {
					transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));
				}
			}
		}
	}

	array_strID_vec.clear();
	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		if (transfer_int_vec.at(i) == -1 || transfer_int_vec.at(i) == 999) {
			continue;
		}
		array_strID_vec.push_back(transfer_int_vec.at(i));
	}
	transfer_int_vec.clear();

	if (array_strID_vec.size() > 0) {
		process_required = true;
	}
	Log.LogRecord(Logs::High, Logs::General, " array_strID_vec elements %i", array_strID_vec.size());
	//for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {		
	//	if (array_strID_vec.at(i) == 100) {
	//		std::cout << std::endl;
	//	}
	//	std::cout << array_strID_vec.at(i) << ",";
	//}
	//std::cout << std::endl;
}

// this makes the assumption the filenames will be the same as the pageID - need to fix this (case 101,102 throughout)
// pageID will be equal to (page_count - 1)
// this creates one page at a time
void QMLParseFunction::QML_PG_CREATE(std::string filename, std::string extension, int page_count) // 9
{
	if (end_parse == true) {
		return;
	}

	int comp_pages = 0;
	int compid_iter = 0;	

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		if (strID_vec.at(i) == 100) { // string ID of new page text ("imports..." - signifies new page)
			++comp_pages;
		}
		if (comp_pages != page_count) {
			continue;
		}

		if (CompHasNoID(strID_vec.at(i)) == false) {
			++compid_iter;
		}

		AppendQMLComponents(filename, page_count - 1, strID_vec.at(i), compid_iter);
	}
	compid_iter = 0;
	Log.LogRecord(Logs::High, Logs::General, "  New .qml created: %s", filename);
}

// parse file to get all compIDs by comparing to mapped comp strings - process/organize data into memory
// the IDs saved correspond to the component strings to save a step if inserting
void QMLParseFunction::QML_PG_GETCOMPCODES() // 10
{
	if (end_parse == true) {
		return;
	}

	std::map<int, std::string>::iterator m_it;

	std::string find_string;
	std::string source_line;
	std::string comp_string;

	std::size_t pg_pos;
	std::size_t space_pos;

	int num_line = 0;

	MapQMLCompIDtoStrID(1, true);

	find_string = QPStr(strID_vec[0]); // find "pageID:"
	for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {	// parse up to "pageID:" to get the compIDs and line # for later	
		source_line = input_file_data_vec.at(i);
		++num_line;

		pg_pos = source_line.find(find_string);
		if (pg_pos != std::string::npos) {
			break;
		}
	}
	for (unsigned int j = 0; j < input_file_data_vec.size(); ++j) { // take care of first three compIDs if they match and
		if (j + 1 == num_line) {
			break;
		}
		source_line = input_file_data_vec.at(j);
		for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) {
			find_string = m_it->second;

			pg_pos = source_line.find(find_string);
			if (pg_pos != std::string::npos) {
				transfer_int_vec.push_back(m_it->first);

				Log.LogRecord(Logs::High, Logs::General, "%i num_line:str %s", j, find_string);
				Log.LogRecord(Logs::High, Logs::General, " pg_pos:(%i) code:(%i) comp_string:(%s) ", pg_pos, m_it->first, comp_string);
				break;
			}
		}
	}
	for (unsigned int i = num_line; i < input_file_data_vec.size(); ++i) {
		find_string = QPStr(511); // find "{}"
		source_line = input_file_data_vec.at(i);

		pg_pos = source_line.find_first_of(find_string);
		if (pg_pos != std::string::npos) {
			find_string = QPStr(510); // find " \t"

			space_pos = source_line.find_first_not_of(find_string);
			if (space_pos != std::string::npos) {
				comp_string = source_line.substr(space_pos, pg_pos - space_pos + 1);

				if (comp_string == QPStr(504)) { // find "}"
					comp_string = source_line.substr(space_pos, source_line.length() - pg_pos);
				}
				Log.LogRecord(Logs::High, Logs::General, "  comp_string %s", comp_string);

				for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) {
					if (comp_string == m_it->second) {
						Log.LogRecord(Logs::High, Logs::General, " pg_pos:(%i) space_pos:(%i) code:(%i) comp_string:(%s) ", pg_pos, space_pos, m_it->first, comp_string);

						transfer_int_vec.push_back(m_it->first);
						break;
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		content_strID_vec.push_back(transfer_int_vec.at(i));

	}
	transfer_int_vec.clear();

	input_file_data_vec.clear();

	Log.LogRecord(Logs::High, Logs::General, "   content_strID_vec elements %i", content_strID_vec.size());

	//	int count = 0;
	//	for (unsigned int i = 0; i < content_strID_vec.size(); ++i) {
	//		if (content_strID_vec.at(i) == 100) {			
	//			std::cout << "-YDim-"<< count << "\n";
	//			count = 0;
	//		}
	//		++count;
	//		std::cout << content_strID_vec.at(i) << ",";		
	//	}
	//	std::cout << "-YDim-" << count << "\n";

}

// alters .qml content from array
// insertion, replacement and deletion of components for both contiguous and non-contiguous changes - uncertain about multiple kinds of changes (insertion and deletion within the same file)
void QMLParseFunction::QML_PG_CHANGE() // 11
{
	if (end_parse == true) {
		return;
	}

	std::map<int, int>::iterator m_it;
	std::map<int, int>::reverse_iterator rm_it;
	std::vector<std::string>::iterator v_it;
	std::vector<int>::iterator cv_it;

	std::string find_string;
	std::string source_line;
	std::string insert_string;

	std::size_t pg_pos;

	int num_line = 0;
	int function_pos = 0;
	int function = 0;
	int comp_pos = 0;
	int line_counter = 0;
	int map_start_position = 0;

	bool replaced = false;

	Log.LogRecord(Logs::High, Logs::General, "   content_strID_vec.size():  %i", content_strID_vec.size());
	//	for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "   array_strID_vec.size():  %i", array_strID_vec.size());
	//	for (unsigned int i = 0; i < array_strID_vec.size(); ++i) { std::cout << array_strID_vec.at(i) << ","; } std::cout << std::endl;

	v_it = output_file_data_vec.begin();
	cv_it = content_strID_vec.begin();

	//for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << " content_strID_vec" << std::endl;

	// insert should not use rm_it
	// cycle through each component until we get to the target component
	for (rm_it = transfer_i_int_map.rbegin(); rm_it != transfer_i_int_map.rend(); ++rm_it) {
		function_pos = rm_it->first;
		function = rm_it->second;
		if (function == 2) {
			for (unsigned int h = num_line; h < array_strID_vec.size(); ++h) { // num_line is not used for this "function" and it's name is a bit similar to it's purpose here					
				m_it = transfer_i_int_map.find(h);
				if (m_it != transfer_i_int_map.end()){
					function_pos = h;
					break;
				}
			}
		}
		replaced = false;
		num_line = 0;

		// we could keep track of the line we are on with line counter (i = line_counter) - for better efficiency
		for (unsigned int i = line_counter; i < output_file_data_vec.size(); ++i) {
			if (replaced == true) {
				break;
			}
			++line_counter;

			for (unsigned int j = comp_pos; j < content_strID_vec.size(); ++j) {
				if (replaced == true) {
					break;
				}
				source_line = output_file_data_vec.at(i);
				find_string = all_qmlcompstrings_map[content_strID_vec.at(j)]; // something isn't right here

				pg_pos = source_line.find(find_string);

				// found a component which does not get replaced - goto next component
				if (pg_pos != std::string::npos && j != function_pos) {
					++comp_pos;
					break;
				}

				// found the component
				else if (pg_pos != std::string::npos && j == function_pos) {
					++comp_pos;

					// search for next component - start the counter on the line of the component
					// get the next component - j or comp_pos
					for (unsigned int k = line_counter - 1; k < output_file_data_vec.size(); ++k) {
						if (replaced == true) { //	std::cout << " break:line k: " << k << std::endl;
							break;
						}
						++num_line;

						source_line = output_file_data_vec.at(k);
						find_string = all_qmlcompstrings_map[content_strID_vec.at(j + 1)]; // bug here with insertions when file is small

						Log.LogRecord(Logs::High, Logs::General, "line ct %i num_line %i \tsource_line:\n%s ", line_counter, num_line, source_line.c_str());					

						pg_pos = source_line.find(find_string);

						// this should take care of components which are the same in contiguous positions (ie. 4 & 5)
						if (pg_pos != std::string::npos && num_line > 1) {

							Log.LogRecord(Logs::High, Logs::General, "find_string: %s", find_string.c_str());								

							if (function == 1) {
								for (int l = 0; l < num_line - 1; ++l) {
									output_file_data_vec.erase(v_it + line_counter - 1);
								}
								output_file_data_vec.emplace(v_it + line_counter - 1, QPStr(array_strID_vec.at(function_pos)));
								content_strID_vec.at(function_pos) = array_strID_vec.at(function_pos);
								replaced = true;
							}
							else if (function == 2) { // works but is kinda funny with what is inserted
								output_file_data_vec.insert(v_it + line_counter - 1, QPStr(array_strID_vec.at(function_pos)));
								content_strID_vec.insert(cv_it + function_pos, array_strID_vec.at(function_pos));
								num_line = function_pos + 1; // if consecutive inserts, increment by one
								replaced = true;
							}
							else if (function == 3) {
								for (int l = 0; l < num_line - 1; ++l) {
									output_file_data_vec.erase(v_it + line_counter - 1);
								}
								content_strID_vec.erase(cv_it + function_pos);
								replaced = true;
							}

							Log.LogRecord(Logs::High, Logs::General, "linect(%i) line(%s) comp(%s) comp_pos(%i)",
								line_counter, output_file_data_vec.at(line_counter).c_str(), all_qmlcompstrings_map[content_strID_vec.at(comp_pos)].c_str(), comp_pos);
																			
							comp_pos = 0;
							line_counter = 0;
							break;
						}
					}
				}
			}
		}
	}
	transfer_i_int_map.clear();

	//for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << " content_strID_vec" << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "\n\n\t-------output_file_data_vec----------\n\n");
	//for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) { std::cout << output_file_data_vec.at(i) << std::endl; }

}

// write file component changes to array - no added pages - rewrites entire array
// because we're writing to an array, the process is much different than writing what the array codes mean to a file
// issue with componentID_from_pageID_vec()
void QMLParseFunction::CPP_ARRAY_CHGCOMPCODES(int page_number) // 12
{
	if (end_parse == true) {
		return;
	}

	std::vector<int>::iterator v_it;

	int asize_variance = 0;
	int comp_iter = 0;
	int XDim_iter = 0;

	Log.LogRecord(Logs::High, Logs::General, " transfer_int_vec elements %i", transfer_int_vec.size());

	if (page_number == -1) { // put strID_vec data into componentID_from_pageID_vec for all pages

		for (unsigned int i = 0; i < strID_vec.size(); ++i) {
			transfer_int_vec.push_back(strID_vec.at(i));
		}

		component_array_Xdim = PFunc.GetArrayDimFromFileData(1, "", 100, true);
		component_array_Ydim = PFunc.GetArrayDimFromFileData(1, "", 100, false);
		component_array_Ydim = component_array_Ydim + 1; // add 1 because we need a terminator

		transfer_int_vec.clear();

		asize_variance = component_array_Ydim * component_array_Xdim - strID_vec.size(); // "array_strID_vec" may be a different vector - testing strID_vec in 5 locations

		if (asize_variance > 0) {
			while (XDim_iter + 1 < component_array_Xdim) {
				for (unsigned int i = 0; i < strID_vec.size(); ++i) {
					if (strID_vec.at(i) == 100) { // string ID of new page text(imports)
						if (i % component_array_Ydim == 0) {
							continue;
						}
						comp_iter = component_array_Ydim - i % component_array_Ydim;

						for (int j = 0; j < comp_iter; ++j) {
							v_it = strID_vec.begin();

							if (j == comp_iter - 1) {
								strID_vec.insert(v_it + i, 999);
							}
							else {
								strID_vec.insert(v_it + i, -1);
							}
						}
						++XDim_iter;
						break;
					}
				}
			}
			comp_iter = component_array_Ydim - strID_vec.size() % component_array_Ydim;

			for (int j = 0; j < comp_iter; ++j) {
				if (j == 0) {
					strID_vec.push_back(999);
				}
				else {
					strID_vec.push_back(-1);
				}
			}
		}
		componentID_from_pageID_vec.clear();
		for (int j = 0; j < component_array_Ydim; ++j) {
			comp_iter = 0;

			for (unsigned int i = 0; i < strID_vec.size(); ++i) {
				if (i % component_array_Ydim != j) {
					continue;
				}
				++comp_iter;

				componentID_from_pageID_vec.push_back(strID_vec.at(i));
				if (comp_iter == component_array_Xdim) {
					break;
				}
			}
		}
	}
	else { // put strID_vec data into componentID_from_pageID_vec for one page
		component_array_Xdim = PFunc.ArrayDimFromFileData(false, false, true, QPStr(24), "");
		component_array_Ydim = PFunc.ArrayDimFromFileData(false, false, false, QPStr(24), "");

		if (component_array_Ydim > PFunc.ArrayDimFromFileData(false, false, false, QPStr(24), "")) {
			Log.LogRecord(Logs::Normal, Logs::Error, "\n Attempting to add to array beyond array parameters.\nYou must synchronize the entire array to make this type of change."
				"\n    Please choose the appropriate Parse Directive.\n Parse Directive aborted.");
		
			end_parse = true;
			return;
		}

		asize_variance = component_array_Ydim - strID_vec.size(); // we need array_Ydim for single page alterations

		if (asize_variance > 0) {
			for (int i = 0; i < asize_variance; ++i) {
				if (i == 0) {
					strID_vec.push_back(999);
				}
				else {
					strID_vec.push_back(-1);
				}
			}
		}

		for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
			if (i % component_array_Xdim != page_number) {
				continue;
			}
			++comp_iter;

			componentID_from_pageID_vec.at(i) = strID_vec.at(comp_iter - 1);
			Log.LogRecord(Logs::High, Logs::General, "%i,%i,%i", componentID_from_pageID_vec.at(i), comp_iter, strID_vec.at(comp_iter - 1));
		}
	}

	for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
		transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));
	}
	PFunc.ParseIntoArray(2, 1, 2, 2, QPStr(24), component_array_Ydim); // QPStr(24) value won't change but how it gets here should

	component_array_Ydim = 0;
	component_array_Xdim = 0;
	componentID_from_pageID_vec.clear();
	transfer_int_vec.clear();
	transfer_string_vec.clear();

	Log.LogRecord(Logs::High, Logs::General, "\ncomponentID_from_pageID_vec elements %i", componentID_from_pageID_vec.size());
	/*
	int count = 0;
	for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
	++count;
	if (count == component_array_Xdim) {
	std::cout << componentID_from_pageID_vec.at(i) << ",";
	std::cout << "\n";
	count = 0;
	continue;
	}
	std::cout << componentID_from_pageID_vec.at(i) << ",";
	}
	std::cout << "\n" << std::endl;
	return;
	*/
}

// get/sync compIDs on individual pages
void QMLParseFunction::QML_PG_GETCOMPIDS(int page_id, bool change_ids, bool process_all) // 13
{
	std::string find_string;
	std::string source_line;
	std::string frag_line;
	std::string compid_char;
	std::string pageid_char;
	std::string replace_id;

	size_t line_pos;
	size_t frag_pos;
	size_t pg_pos;
	size_t num_sz;

	int line_counter = 0;
	int compid_counter = 0;
	int page_count = 0;

	bool next_comp = false;
	bool get_compids = false;

	for (unsigned int i = 0; i < content_strID_vec.size(); ++i) {
		if (process_all == true) {
			if (content_strID_vec.at(i) == 100) {
				if (page_count == page_id) {
					get_compids = true;
				}
				else {
					get_compids = false;
				}
				++page_count;
			}
		}
		else {
			get_compids = true;
		}

		if (get_compids == true) {
			find_string = all_qmlcompstrings_map[content_strID_vec.at(i)];
			next_comp = false;

			for (unsigned int j = line_counter; j < output_file_data_vec.size(); ++j) { // input_file_data_vec
				if (next_comp == true) {
					break;
				}
				source_line = output_file_data_vec.at(j); // input_file_data_vec
				++line_counter;

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					if (CompHasNoID(content_strID_vec.at(i)) == true && content_strID_vec.at(i) != 115) {
						break;
					}
					++compid_counter;

					find_string = QPStr(strID_vec[0]); // "id:"
					for (unsigned int k = j; k < output_file_data_vec.size(); ++k) { // input_file_data_vec
						source_line = output_file_data_vec.at(k); // input_file_data_vec

						line_pos = source_line.find(find_string);
						if (line_pos != std::string::npos) {

							// do something with components only needing a pageID					

							find_string = QPStr(strID_vec[1]); // "pg"
							pg_pos = source_line.find(find_string);
							if (pg_pos != std::string::npos) {

								frag_pos = pg_pos + QPStr(strID_vec[1]).length();
								frag_line = source_line.substr(frag_pos, source_line.length() - frag_pos);
								pageid_char = frag_line.substr(0, 1);

								if (pageid_char.find_first_not_of(NUMERIC_CHAR) != std::string::npos || frag_line.length() == 0) {
									Log.LogRecord(Logs::High, Logs::General, "    No page ID assigned to component.");					
									replace_id = std::to_string(page_id);
									source_line = source_line.insert(frag_pos, replace_id);
								}
								else if (frag_line.length() > 4) { // if length is over 4 it means either 10k+ pages or there is likely a semicolon and there should be stuff after it

									line_pos = frag_line.find_first_of(QPStr(512)); // ";"
									if (line_pos != std::string::npos) {
										pageid_char = frag_line.substr(0, line_pos);

										if (page_id != stoi(pageid_char, &num_sz)) {
											Log.LogRecord(Logs::High, Logs::General, "    Page ID incorrectly assigned to component:  %s proper: %i", pageid_char, page_id);
											replace_id = std::to_string(page_id);
											source_line = source_line.replace(frag_pos, pageid_char.length(), replace_id);
										}
									}
									else {
										Log.LogRecord(Logs::Normal, Logs::Error, "    ERROR finding page ID assigned to component.");
									}
								}
								else {
									pageid_char = frag_line.substr(0, source_line.length() - frag_pos);
									if (pageid_char.length() > 0) {
										if (pageid_char.length() != std::to_string(page_id).length()) {
											Log.LogRecord(Logs::High, Logs::General, "    Page ID incorrectly assigned to component:  %s proper: %i", pageid_char, page_id);
											replace_id = std::to_string(page_id);
											source_line = source_line.replace(frag_pos, pageid_char.length(), replace_id);
										}
										else if (page_id != stoi(pageid_char, &num_sz)) {
											Log.LogRecord(Logs::High, Logs::General, "    Page ID incorrectly assigned to component:  %s proper: %i", pageid_char, page_id);
											replace_id = std::to_string(page_id);
											source_line = source_line.replace(frag_pos, pageid_char.length(), replace_id);
										}
									}
									else {
										Log.LogRecord(Logs::Normal, Logs::Error, "    ERROR finding page ID assigned to component.");
									}
								}

								// differentiate components only needing a pageID
								if (content_strID_vec.at(i) != 115) {
									compid_char = source_line.substr(pg_pos - 1, 1);
									if (compid_char.find_first_not_of(NUMERIC_CHAR) != std::string::npos) {
										Log.LogRecord(Logs::High, Logs::General, "    No component ID assigned to component.");
										replace_id = std::to_string(compid_counter);
										source_line = source_line.insert(pg_pos, replace_id);
									}
									else {
										line_pos = source_line.find_first_of(NUMERIC_CHAR);
										if (line_pos != std::string::npos) {
											compid_char = source_line.substr(line_pos, pg_pos - line_pos);

											if (compid_counter != stoi(compid_char, &num_sz)) {
												Log.LogRecord(Logs::High, Logs::General, "    Component ID incorrectly assigned to component: (%s)(%i)", compid_char, compid_counter);
												replace_id = std::to_string(compid_counter);
												source_line = source_line.replace(line_pos, compid_char.length(), replace_id);
											}
										}
										else {
											Log.LogRecord(Logs::Normal, Logs::Error, "    ERROR finding component ID assigned to component.");
										}
									}
								}

								// differentiate components only needing a pageID - this way is ugly - it's also not used
								// change_ids = true implies analysis of IDs was already done and saving the data is not necessary - the opposite provides the need to save the data to analyze							
								if (change_ids == false) {
									if (content_strID_vec.at(i) != 115) {
										transfer_i_string_map[compid_counter] = compid_char + "," + pageid_char;
									}
								}
								else { // probably output file data, if rewrite is the option
									output_file_data_vec.at(k) = source_line;
									Log.LogRecord(Logs::High, Logs::General, "source_line (%s)", source_line);
									
								}
								next_comp = true;
								break;
							}
						}
					}
				}
			}
		}
	}
	if (change_ids == false) {
		output_file_data_vec.clear();
	}	
	//std::map<int, std::string>::iterator m_it; for (m_it = transfer_i_string_map.begin(); m_it != transfer_i_string_map.end(); ++m_it) { std::cout << m_it->first << ":" << m_it->second << std::endl; }	
}

// used when inserting a page into a set of current ordered pages 
// reorder makes sure everything is currently incremented properly - it uses pageIDtofile map to get the position of the page - if a page was deleted, this would be useful
// I think this automatically rewrites each file even if the pageID is correct, should fix this
// !reorder increments +1 at new_page_id value - basically reserves a spot for the new page to be inserted in the next function
void QMLParseFunction::QML_PG_ORDER(bool reorder, int new_page_id, int cur_page_ct) // 14
{
	std::string find_string;
	std::string source_line;
	std::string frag_line;
	std::string num_page;
	std::string cur_num_page;

	std::size_t line_pos;
	std::size_t in_pos;
	std::size_t num_sz;

	int cur_page_id = 0;

	if (reorder == true) {
		num_page = std::to_string(cur_page_ct);
		num_page = QPStr(505) + num_page; // " " space

		find_string = QPStr(strID_vec[0]); // "pageID:"
		for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
			source_line = output_file_data_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {
				in_pos = line_pos + find_string.length();
				frag_line = source_line.substr(0, in_pos);
				source_line = frag_line + num_page;
				output_file_data_vec.at(i) = source_line;
				Log.LogRecord(Logs::High, Logs::General, "source_line (%s)", source_line);
				break;
			}
		}
	}
	else {
		find_string = QPStr(strID_vec[0]); // "pageID:"
		for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
			source_line = output_file_data_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {
				in_pos = line_pos + find_string.length();

				cur_num_page = source_line.substr(in_pos, source_line.length() - in_pos);
				cur_page_id = stoi(cur_num_page, &num_sz);

				if (cur_page_id < new_page_id) {
					continue;
				}

				cur_page_id = cur_page_id + 1;
				num_page = std::to_string(cur_page_id);
				num_page = QPStr(505) + num_page; // " " space

				frag_line = source_line.substr(0, in_pos);
				source_line = frag_line + num_page;
				output_file_data_vec.at(i) = source_line;
				Log.LogRecord(Logs::High, Logs::General, "source_line (%s)", source_line);
				break;
			}
		}
	}
}

// creates directories after checking
// function ends at subset marker which isn't a flexible way to do it - unless the marker is taken care of immediately after with more directories to create
void QMLParseFunction::APP_SETUP(std::string tar_directory) // 15
{
	if (end_parse == true) {
		return;
	}

	ParseDirectory PDir;

	std::string dir_path;

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		if (strID_vec.at(i) < 0) {
			break;
		}
		dir_path = tar_directory + QPStr(strID_vec.at(i));

		if (PDir.CheckIfDirExists(true, true, QPStr(strID_vec.at(i))) == false) {
			PDir.MakeDirectory(dir_path);
			Log.LogRecord(Logs::Normal, Logs::General, "  ...directory created");
		}
	}
}

// prepares filenames to be checked
// possibly alter GetNewInputFilenames() to work with this function by notating which files do not exist
void QMLParseFunction::FILE_CHECK_DEPEND(bool get_filenames, std::string tar_directory) // 16
{
	if (end_parse == true) {
		return;
	}

	ParseDirectory PDir;

	std::map<std::string, std::string>::iterator m_it;

	std::string source_line;
	std::string find_string;
	std::string new_string;
	std::string element_string;

	size_t line_pos = 0;

	bool element_exists = false;

	if (get_filenames == true) {
		find_string = QPStr(507); // "."

		for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
			source_line = input_file_data_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {
				new_string = source_line.substr(0, line_pos);
				temp_string_vec.push_back(new_string);
				Log.LogRecord(Logs::High, Logs::General, "   new_string: %s", new_string.c_str());
			}
		}
		//for (unsigned int i = 0; i < temp_string_vec.size(); ++i) { std::cout << "temp_string_vec: " << temp_string_vec.at(i) << std::endl; }
	}
	else if (get_filenames == false) {
		find_string = PFunc.PStr(506); // ","

		for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
			source_line = input_file_data_vec.at(i);
			PFunc.GetDelimitedData(source_line, find_string, 2);
		}
		find_string = FileType[QML]; // ".qml"

		for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
			source_line = transfer_string_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {
				element_string.append(transfer_string_vec.at(i) + PFunc.PStr(506));
			}
			else {
				if (new_string.length() > 0) {
					transfer_s_string_map[new_string] = element_string;
				}
				new_string.clear();
				element_string.clear();

				new_string = transfer_string_vec.at(i);
			}
		}
		input_file_data_vec.clear();
		transfer_string_vec.clear();

		MapQMLCompIDtoStrID(0, true);

		for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
			element_exists = false;

			for (m_it = transfer_s_string_map.begin(); m_it != transfer_s_string_map.end(); ++m_it) {
				find_string = m_it->first;
				source_line = all_qmlcompstrings_map[array_strID_vec.at(i)];

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					transfer_string_vec.push_back(find_string + FileType[QML]);

					PFunc.GetDelimitedData(m_it->second, PFunc.PStr(506), 2);
					element_exists = true;
				}
			}

			if (element_exists == true) {
				continue;
			}

			for (unsigned int j = 0; j < temp_string_vec.size(); ++j) {
				find_string = temp_string_vec.at(j);
				source_line = all_qmlcompstrings_map[array_strID_vec.at(i)];

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					transfer_string_vec.push_back(find_string + FileType[QML]);
				}
			}
		}
		temp_string_vec.clear();

		for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
			element_exists = false;

			if (transfer_string_vec.at(i).length() == 0) {
				continue;
			}
			for (unsigned int j = 0; j < temp_string_vec.size(); ++j) {
				if (temp_string_vec.at(j) == transfer_string_vec.at(i)) {
					element_exists = true;
					break;
				}
			}
			if (element_exists == false) {
				temp_string_vec.push_back(transfer_string_vec.at(i));
			}
		}
		transfer_string_vec.clear();

		// check each repo folder vs what exists in temp_string_vec, if found, check app folder for the same file, if not found copy over
		// cycle through files we need
		for (unsigned int i = 0; i < temp_string_vec.size(); ++i) {

			// cycle through QPStr() of the folders
			for (unsigned int j = 0; j < strID_vec.size(); ++j) {
				element_exists = false;
				Log.LogRecord(Logs::High, Logs::General, "%s : %s", QPStr(strID_vec.at(j)).c_str(), temp_string_vec.at(i).c_str());

				// create the possible path and confirm which repo folder it exists in
				if (PDir.BoostCheckPath(6, QPStr(strID_vec.at(j)), temp_string_vec.at(i)) == true) {

					// check the app outpath folder
					if (PDir.BoostCheckPath(5, QPStr(strID_vec.at(j)), temp_string_vec.at(i)) == false) {
						PDir.BoostMoveFile(QTInputDirectory[QML_FILES] + QPStr(strID_vec.at(j)), temp_string_vec.at(i), tar_directory + QPStr(strID_vec.at(j)), temp_string_vec.at(i), false, false);
						break;
					}
				}
			}
		}
		temp_string_vec.clear();
		all_qmlcomponents_map.clear();
		all_qmlcompstrings_map.clear();
	}
	//for (m_it = transfer_s_string_map.begin(); m_it != transfer_s_string_map.end(); ++m_it) { std::cout << "\t" << m_it->first << "\n" << m_it->second << std::endl; }
}

// compares what's in the .cpp array to what's in the .qml file - replacments between .cpp array and .qml file are handled quite a bit different
// currently, the only end brackets handled are...
void QMLParseFunction::COMPARE_COMPCODES(bool use_file_data) // 17
{
	if (end_parse == true) {
		return;
	}

	int num_line = 0;
	int diff_ct = 0;
	int vsize_variance = 0;
	int pos_shift_ct = 0;

	bool diff_found = false;
	bool vsize_nodiff = content_strID_vec.size() == array_strID_vec.size();

	vsize_variance = content_strID_vec.size() - array_strID_vec.size();

	Log.LogRecord(Logs::High, Logs::General, "   strID_vec.size(): %i", strID_vec.size());
	Log.LogRecord(Logs::High, Logs::General, "   content_strID_vec.size(): %i", content_strID_vec.size());
	Log.LogRecord(Logs::High, Logs::General, "   array_strID_vec.size(): %i", array_strID_vec.size());
	Log.LogRecord(Logs::High, Logs::General, "   vsize_variance %i", vsize_variance);

	// much of the c-output in this large section is under developemnt
	if (use_file_data == false) {
		// add components to .qml file
		if (vsize_variance < 0) { // there are more components in the array than the file
			Log.LogRecord(Logs::High, Logs::General, "There are more components in the array than the file");
			for (unsigned int i = 0; i < content_strID_vec.size(); ++i) {
				if (diff_ct > 0) {
					break;
				}
				++num_line;

				// first, find the difference
				for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
					if (j == num_line - 1) {
						if (content_strID_vec.at(i) != array_strID_vec.at(j)) {
							diff_found = true;
							++diff_ct;
							transfer_i_int_map[i] = 2; // insert in file data
							Log.LogRecord(Logs::High, Logs::General, "    difference found");
							Log.LogRecord(Logs::High, Logs::General, "i:(%i) num_line:(%i)", i, num_line - 1); // position of components	
							Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i)", array_strID_vec.at(i), content_strID_vec.at(j));
						}
						break;
					}
				}
				// secondary check - only handles removing components from a page
				if (diff_found == true) {

					// k = num_line -> increase array_vec iter to check if there are more differences
					// we don't need to check everything before the difference - go to the next positions and check those for differences
					for (unsigned int k = num_line; k < array_strID_vec.size(); ++k) {
						if (i + diff_ct == k) {
							++num_line;
							if (content_strID_vec.at(i) != array_strID_vec.at(k)) {
								++diff_ct;
								transfer_i_int_map[k] = 2; // insert in file data
								Log.LogRecord(Logs::High, Logs::General, "    difference found in secondary check");
								Log.LogRecord(Logs::High, Logs::General, "i:(%i) num_line:(%i)", i, num_line - 1); // position of components - (num_line - 1) is for debug comparison only	
								Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i)", array_strID_vec.at(k), content_strID_vec.at(i));
							}
							else {
								diff_found = false;
								Log.LogRecord(Logs::High, Logs::General, "    no difference in secondary check");
								Log.LogRecord(Logs::High, Logs::General, "i:(%i) num_line:(%i)", i, num_line - 1); // position of components - (num_line - 1) is for debug comparison only	
								Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i)", array_strID_vec.at(k), content_strID_vec.at(i));
								break;
							}
						}
					}
				}
			}
		}
		// remove components from a .qml file 
		else if (vsize_variance > 0) { // there are more components in the file than the array
			Log.LogRecord(Logs::High, Logs::General, "There are more components in the file than the array");
			for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
				if (diff_ct > 0) {
					break;
				}
				++num_line;

				// first, find the difference
				for (unsigned int j = 0; j < content_strID_vec.size(); ++j) {
					if (j == num_line - 1) {
						if (array_strID_vec.at(i) != content_strID_vec.at(j)) {
							diff_found = true;
							++diff_ct;
							transfer_i_int_map[i] = 3; // erase in file data
							Log.LogRecord(Logs::High, Logs::General, "    difference found");
							Log.LogRecord(Logs::High, Logs::General, "i:(%i) num_line:(%i)", i, num_line - 1); // position of components
							Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i)", array_strID_vec.at(i), content_strID_vec.at(i));							
						}
						break;
					}
				}
				// secondary check - only handles adding components to a page
				if (diff_found == true) {

					// k = num_line -> increase content_vec iter to check if there are more differences
					// we don't need to check everything before the difference - go to the next positions and check those for differences
					for (unsigned int k = num_line; k < content_strID_vec.size(); ++k) {
						if (i + diff_ct == k) {
							++num_line;
							if (array_strID_vec.at(i) != content_strID_vec.at(k)) {
								++diff_ct;
								transfer_i_int_map[k] = 3; // erase in file data
								Log.LogRecord(Logs::High, Logs::General, "    difference found in secondary check");
								Log.LogRecord(Logs::High, Logs::General, "i:(%i) num_line:(%i)", i, num_line - 1); // position of components - (num_line - 1) is for debug comparison only
								Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i)", array_strID_vec.at(i), content_strID_vec.at(i));								
							}
							else {
								diff_found = false;
								Log.LogRecord(Logs::High, Logs::General, "    no difference in secondary check");
								Log.LogRecord(Logs::High, Logs::General, " i:(%i) num_line:(%i)"); // position of components - (num_line - 1) is for debug comparison only
								Log.LogRecord(Logs::High, Logs::General, " array:(%i) content:(%i) ", array_strID_vec.at(i), content_strID_vec.at(i));			
								break;
							}
						}
					}
				}
			}
		}
		// replace components in a .qml file
		else if (vsize_variance == 0) {
			for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
				++num_line;

				for (unsigned int j = 0; j < content_strID_vec.size(); ++j) {
					if (j == num_line - 1) {
						Log.LogRecord(Logs::High, Logs::General, " j:(%i) num_line - 1 (%i)", j, num_line - 1);
						if (array_strID_vec.at(i) != content_strID_vec.at(j)) {
							++diff_ct;
							transfer_i_int_map[j] = 1; // replace file data
							Log.LogRecord(Logs::High, Logs::General, " j(%i) array:(%i) content:(%i) i(%i) ", j, array_strID_vec.at(j), content_strID_vec.at(i), i);
							Log.LogRecord(Logs::High, Logs::General, "    content (%i) array (%i)", content_strID_vec.at(i), array_strID_vec.at(j));					
						}
						break;
					}
				}
			}
		}
	}
	// changes .cpp page component ID array - handles all cases 
	else if (use_file_data == true) {
		for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
			++num_line;
			for (unsigned int j = 0; j < content_strID_vec.size(); ++j) {
				if (j == num_line - 1) {
					Log.LogRecord(Logs::High, Logs::General, "%i  j:num_line - 1  %i", j, num_line - 1);
					if (array_strID_vec.at(i) != content_strID_vec.at(j)) {
						++diff_ct;
					}
					break;
				}
			}
		}
	}

	CompareCompIDProcessSwitch(diff_found, vsize_nodiff, use_file_data, vsize_variance, diff_ct);

	// new data - these should match
	//	for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << "\n";
	//	for (unsigned int i = 0; i < array_strID_vec.size(); ++i) { std::cout << array_strID_vec.at(i) << ","; } std::cout << "\n";
	//	for (unsigned int i = 0; i < strID_vec.size(); ++i) { std::cout << strID_vec.at(i) << ","; } std::cout << "\n";

}

// these three are very similar to their page counterparts, especially QML_PG_GETCOMPCODES()
// they could be combined with them if the directory situation can be figured out
void QMLParseFunction::CPP_ARRAY_GETMAINCOMPCODES(bool unique) // 18
{
	if (end_parse == true) {
		return;
	}

	bool element_exists = false;

	componentID_from_pageID_vec.clear(); // can't have anything in

	PFunc.ParseForArrayData(1, 1, 1, 1, QPStr(strID_vec[0])); // "componentID_from_main"
	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		componentID_from_pageID_vec.push_back(transfer_int_vec.at(i));
	}
	if (transfer_int_vec.size() == 0) {
		process_required = false;
		return;
	}
	input_file_data_vec.clear();
	transfer_int_vec.clear();

	if (unique == false) {
		for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
			transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));			
			//std::cout << componentID_from_pageID_vec.at(i) << ",";				
		}
	}
	else if (unique == true) {
		for (unsigned int i = 0; i < componentID_from_pageID_vec.size(); ++i) {
			element_exists = false;

			for (unsigned int j = 0; j < transfer_int_vec.size(); ++j) {
				if (transfer_int_vec.at(j) == componentID_from_pageID_vec.at(i)) {
					element_exists = true;
					break;
				}
			}
			if (element_exists == false) {
				transfer_int_vec.push_back(componentID_from_pageID_vec.at(i));
			}
		}
	}

	array_strID_vec.clear();
	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		if (transfer_int_vec.at(i) == -1 || transfer_int_vec.at(i) == 999) {
			continue;
		}
		array_strID_vec.push_back(transfer_int_vec.at(i));
	}

	transfer_int_vec.clear();

	Log.LogRecord(Logs::High, Logs::General, " array_strID_vec elements: %i", array_strID_vec.size());
	//for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {		
	//	std::cout << array_strID_vec.at(i) << ",";
	//}
	//std::cout << std::endl;

}

void QMLParseFunction::CPP_ARRAY_CHGMAINCOMPCODES() // 19
{
	if (end_parse == true) {
		return;
	}

	int comp_iter = 0;

	component_array_Xdim = PFunc.ArrayDimFromFileData(false, false, true, QPStr(25), "");
	component_array_Ydim = 1;

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		transfer_int_vec.push_back(strID_vec.at(i));
	}
	PFunc.ParseIntoArray(2, 1, 1, 2, QPStr(25), component_array_Ydim); // QPStr(24) value won't change but how it gets here should

	component_array_Ydim = 0;
	component_array_Xdim = 0;
	componentID_from_pageID_vec.clear();
	array_strID_vec.clear();
}

// the same compare function can likely be used to compare and possibly the same change file function
// parsing this file will be a bit more difficult due to not all component find_strings containing a begin bracket "{"
void QMLParseFunction::QML_MAIN_GETCOMPCODES() // 20
{
	if (end_parse == true) {
		return;
	}

	std::map<int, std::string>::iterator m_it;

	std::string find_string;
	std::string source_line;
	std::string comp_string;

	std::size_t pg_pos;
	std::size_t space_pos;

	int subset_counter = 0;
	int num_line1 = 0;
	int num_line2 = 0;

	bool begin_file = false;
	bool spec_comp_found = false;

	MapQMLCompIDtoStrID(2, true);

	// there are currently no uses of any component more than once within main.qml
	// try to find other components which do not have end brackets - only three currently exist for main.qml - 262,268,274 with 262 near the beginning and 268 at the end
	// 262,268 are part of all main.qml's, so we only need to verify they exist - 262 should be in the 3rd pos and 268 in the last pos
	// after we parse up to the line# of 262, get a 2nd line # for 274 - parse normally up to that line then insert 274
	// make the 2nd line # + 1 = 1st line # and get theline # for 268 - parse normally up to that line then insert 268
	// verify there are no other components after 268 searching for everything	

	// this could be a separate function when mutliple subsets are used
	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		if (subset_counter > 1) { // pass a strID_vec subset_pos			
			break;
		}
		else if (subset_counter == 1) { // pass a strID_vec subset_pos
			transfer_string_vec.push_back(QPStr(strID_vec.at(i)));
		}

		if (strID_vec.at(i) < 0) {
			++subset_counter;
		}
	}

	for (unsigned int h = 0; h < transfer_string_vec.size(); ++h) {
		if (h == 0) {
			begin_file = true;
		}
		else {
			begin_file = false;
			num_line1 = num_line2;
			num_line2 = 0;
		}
		find_string = transfer_string_vec.at(h);

		for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {	// parse up to "property int pageNum" to get the compcodes and line # for later	
			source_line = input_file_data_vec.at(i);
			++num_line2;

			pg_pos = source_line.find(find_string);
			if (pg_pos != std::string::npos) {
				spec_comp_found = true;
				break;
			}
		}
		if (spec_comp_found == false) {
			num_line2 = num_line1;
			continue;
		}

		if (begin_file == true) {
			for (unsigned int j = 0; j < input_file_data_vec.size(); ++j) { // take care of first three compcodes if they match
				if (j == num_line2) {
					spec_comp_found = false;
					break;
				}

				source_line = input_file_data_vec.at(j);
				for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) {
					find_string = m_it->second;

					pg_pos = source_line.find(find_string);
					if (pg_pos != std::string::npos) {
						transfer_int_vec.push_back(m_it->first);						
						Log.LogRecord(Logs::High, Logs::General, "code: %i num_line %i find_string %s", m_it->first, j, find_string);
						break;
					}
				}
			}
		}
		else {

			for (unsigned int j = num_line1; j < input_file_data_vec.size(); ++j) {
				if (j == num_line2) {
					if (spec_comp_found == true) {
						spec_comp_found = false;
						Log.LogRecord(Logs::High, Logs::General, "num_line2(%i) find_string(%s)", num_line2, transfer_string_vec.at(h));

						for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) {
							if (m_it->second == transfer_string_vec.at(h)) {
								transfer_int_vec.push_back(m_it->first);
								break;
							}
						}
					}
					break;
				}
				find_string = QPStr(511); // find "{}"
				source_line = input_file_data_vec.at(j);

				pg_pos = source_line.find_first_of(find_string);
				if (pg_pos != std::string::npos) {
					find_string = QPStr(510); // find " \t"

					space_pos = source_line.find_first_not_of(find_string);
					if (space_pos != std::string::npos) {
						comp_string = source_line.substr(space_pos, pg_pos - space_pos + 1);

						if (comp_string == QPStr(504)) { // find "}"
							comp_string = source_line.substr(space_pos, source_line.length() - pg_pos);
						}
						Log.LogRecord(Logs::High, Logs::General, " comp_string (%s)", comp_string);

						for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) {
							if (comp_string == m_it->second) {
								Log.LogRecord(Logs::High, Logs::General, "line: %i code: %i comp_string: %s", j, m_it->first, comp_string);

								transfer_int_vec.push_back(m_it->first);
								break;
							}
						}
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		content_strID_vec.push_back(transfer_int_vec.at(i));
	}
	transfer_int_vec.clear();
	transfer_string_vec.clear();
	input_file_data_vec.clear();

	Log.LogRecord(Logs::High, Logs::General, "content_strID_vec elements %i", content_strID_vec.size());
	//for (unsigned int i = 0; i < content_strID_vec.size(); ++i) {		
	//	std::cout << content_strID_vec.at(i) << ",";		
	//}
	//std::cout << std::endl;
}

void QMLParseFunction::QML_MAIN_CHANGE() // 21
{
	if (end_parse == true) {
		return;
	}

	std::map<int, int>::iterator m_it;
	std::map<int, int>::reverse_iterator rm_it;
	std::vector<std::string>::iterator v_it;
	std::vector<int>::iterator cv_it;

	std::string find_string;
	std::string source_line;
	std::string insert_string;

	std::size_t pg_pos;

	int num_line = 0;
	int function_pos = 0;
	int function = 0;
	int comp_pos = 0;
	int line_counter = 0;
	int map_start_position = 0;

	bool replaced = false;

	Log.LogRecord(Logs::High, Logs::General, "   content_strID_vec.size(): %i", content_strID_vec.size());
	//for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "   array_strID_vec.size(): %i", array_strID_vec.size());
	//for (unsigned int i = 0; i < array_strID_vec.size(); ++i) { std::cout << array_strID_vec.at(i) << ","; } std::cout << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "   transfer_i_int_map.size(): %i", transfer_i_int_map.size());
	//for (unsigned int i = 0; i < transfer_i_int_map.size(); ++i) { std::cout << transfer_i_int_map.size() << ","; } std::cout << std::endl;

	// insert should not use rm_it
	// cycle through each component until we get to the target component
	for (rm_it = transfer_i_int_map.rbegin(); rm_it != transfer_i_int_map.rend(); ++rm_it) {
		function_pos = rm_it->first;
		function = rm_it->second;
		if (function == 2) {
			for (unsigned int h = num_line; h < array_strID_vec.size(); ++h) { // num_line is not used for this "function" and it's name is a bit similar to it's purpose here					
				m_it = transfer_i_int_map.find(h);
				if (m_it != transfer_i_int_map.end()){
					function_pos = h;
					break;
				}
			}
		}
		replaced = false;
		num_line = 0;

		// we could keep track of the line we are on with line counter (i = line_counter) - for better efficiency
		for (unsigned int i = line_counter; i < output_file_data_vec.size(); ++i) {
			if (replaced == true) {
				break;
			}
			++line_counter;

			for (unsigned int j = comp_pos; j < content_strID_vec.size(); ++j) {
				if (replaced == true) {
					break;
				}
				source_line = output_file_data_vec.at(i);
				find_string = all_qmlcompstrings_map[content_strID_vec.at(j)]; // something isn't right here

				pg_pos = source_line.find(find_string);

				// found a component which does not get replaced - goto next component
				if (pg_pos != std::string::npos && j != function_pos) {
					++comp_pos;
					break;
				}

				// found the component
				else if (pg_pos != std::string::npos && j == function_pos) {
					++comp_pos;

					// search for next component - start the counter on the line of the component
					// get the next component - j or comp_pos
					for (unsigned int k = line_counter - 1; k < output_file_data_vec.size(); ++k) {
						if (replaced == true) { //	std::cout << " break:line k: " << k << std::endl;
							break;
						}
						++num_line;

						source_line = output_file_data_vec.at(k);
						find_string = all_qmlcompstrings_map[content_strID_vec.at(j + 1)]; // bug here with insertions when file is small

						Log.LogRecord(Logs::High, Logs::General, " %i:%i\tsource_line :\n%s", line_counter, num_line, source_line.c_str());					

						pg_pos = source_line.find(find_string);

						// this should take care of components which are the same in contiguous positions (ie. 4 & 5)
						if (pg_pos != std::string::npos && num_line > 1) {

							Log.LogRecord(Logs::High, Logs::General, "find_string: %s", find_string.c_str());

							v_it = output_file_data_vec.begin();
							cv_it = content_strID_vec.begin();

							if (function == 1) {
								for (int l = 0; l < num_line - 1; ++l) {
									output_file_data_vec.erase(v_it + line_counter - 1);
								}
								output_file_data_vec.emplace(v_it + line_counter - 1, QPStr(array_strID_vec.at(function_pos)));
								content_strID_vec.at(function_pos) = array_strID_vec.at(function_pos);
								replaced = true;
							}
							else if (function == 2) { // works but is kinda funny with what is inserted -- still??
								output_file_data_vec.insert(v_it + line_counter - 1, QPStr(array_strID_vec.at(function_pos)));
								content_strID_vec.insert(cv_it + function_pos, array_strID_vec.at(function_pos));
								num_line = function_pos + 1; // if consecutive inserts, increment by one
								replaced = true;
							}
							else if (function == 3) {
								for (int l = 0; l < num_line - 1; ++l) {
									output_file_data_vec.erase(v_it + line_counter - 1);
								}
								content_strID_vec.erase(cv_it + function_pos);
								replaced = true;
							}

							Log.LogRecord(Logs::High, Logs::General, "line ct (%i) line (%s) comp (%s) comp_pos(%i)", 
								line_counter, output_file_data_vec.at(line_counter).c_str(), all_qmlcompstrings_map[content_strID_vec.at(comp_pos)].c_str(), comp_pos);													

							comp_pos = 0;
							line_counter = 0;
							break;
						}
					}
				}
			}
		}
	}
	transfer_i_int_map.clear();

	//for (unsigned int i = 0; i < content_strID_vec.size(); ++i) { std::cout << content_strID_vec.at(i) << ","; } std::cout << " content_strID_vec" << std::endl;
	Log.LogRecord(Logs::High, Logs::General, "\n\n\t-------output_file_data_vec----------\n\n");
	//for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) { std::cout << output_file_data_vec.at(i) << std::endl; }

}

// checks array for existence of contingent components based on existing components
void QMLParseFunction::CPP_ARRAY_CONTINGENTOPERATIONS(bool search_main_qml, bool main_qml_contingents, bool verify) // 22
{
	std::map<int, std::string>::iterator m_it;
	std::vector<int>::iterator v_it;

	std::string source_line;
	std::string find_string;
	std::string contingents;
	std::string new_string;

	size_t line_pos = 0;
	size_t num_sz = 0;

	int contingent_code = 0;
	int num_page1 = 0;
	int num_page2 = 0;

	bool contingent_exists = false;
	bool clear_vec = false;

	find_string = PFunc.PStr(506); // ","
	for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
		source_line = input_file_data_vec.at(i);

		PFunc.GetDelimitedData(source_line, find_string, 1);

		line_pos = source_line.find(find_string);
		if (line_pos != std::string::npos) {
			contingents = source_line.substr(line_pos + 1, source_line.length() - line_pos + 1);
			transfer_i_string_map[transfer_int_vec.at(0)] = contingents;
			transfer_int_vec.clear();
		}
	}
	transfer_int_vec.clear();

	if (search_main_qml == true) {
		if (verify == true) { // done
			// need to check for codes which begin > 150			

			for (unsigned int i = 0; i < temp_string_vec.size(); ++i) {
				source_line = temp_string_vec.at(i);
				contingent_exists = false;

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					continue;
				}
				else {
					contingent_code = stoi(temp_string_vec.at(i), &num_sz);

					if (contingent_code > 150) {
						for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
							if (array_strID_vec.at(j) == contingent_code) {
								contingent_exists = true;
								break;
							}
						}
						// save for later main writing
						if (contingent_exists == false) {
							transfer_int_vec.push_back(contingent_code);
						}
					}
				}
			}
			// add transfer_int_vec items in the positions they belong
			// write to main array - anything before 180 write after 166, anything after 180 write before 166			
			// insert any additions
			for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
				for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
					v_it = array_strID_vec.begin();

					if (array_strID_vec.at(j) == 166) {
						if (transfer_int_vec.at(i) >= 180) {
							array_strID_vec.insert(v_it + j, transfer_int_vec.at(i));
							break;
						}
						else {
							array_strID_vec.insert(v_it + j + 1, transfer_int_vec.at(i));
							break;
						}
					}
				}
			}
			// remove duplicates
			for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
				contingent_exists = false;
				if (array_strID_vec.at(i) >= 160) {
					for (unsigned int j = i + 1; j < array_strID_vec.size(); ++j) {
						if (array_strID_vec.at(j) == array_strID_vec.at(i)) {
							contingent_exists = true;
							break;
						}
					}
				}
				if (contingent_exists == false) {
					temp_int_vec.push_back(array_strID_vec.at(i));
				}
			}
			transfer_int_vec.clear();
			array_strID_vec.clear();
			for (unsigned int i = 0; i < temp_int_vec.size(); ++i) {
				array_strID_vec.push_back(temp_int_vec.at(i));
				Log.LogRecord(Logs::High, Logs::General, "temp_int_vec %i", temp_int_vec.at(i));
			}
			temp_int_vec.clear();
			Log.LogRecord(Logs::High, Logs::General, "temp_string_vec.size %i", temp_string_vec.size());
			temp_string_vec.clear();
			//clear_vec = true;
		}
		else {
			for (m_it = transfer_i_string_map.begin(); m_it != transfer_i_string_map.end(); ++m_it) {
				if (m_it->first > 150) {
					PFunc.GetDelimitedData(m_it->second, find_string, 1);

					for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
						contingent_code = transfer_int_vec.at(i);
						contingent_exists = false;

						//	find 169 - 183
						//	find 170 - 180, 181, 182
						//	find 173 - 181, 182
						if (main_qml_contingents == true) {
							if (contingent_code < 150) {
								continue;
							}
							for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
								if (array_strID_vec.at(j) == contingent_code) {
									contingent_exists = true;
									break;
								}
							}
							if (contingent_exists == false) {
								new_string = std::to_string(contingent_code);
								transfer_string_vec.push_back(new_string);
								Log.LogRecord(Logs::High, Logs::General, "    Main/MainCont - check main page - compcode primary:main %i:%i", m_it->first, contingent_code);
							}
						}
						//	find 173 - 131, 132
						else if (main_qml_contingents == false) {
							if (contingent_code > 150) {
								continue;
							}

							for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
								if (array_strID_vec.at(j) == m_it->first) {
									contingent_exists = true;
									break;
								}
							}
							if (contingent_exists == true) { // done - this needs to go on each page
								new_string = std::to_string(contingent_code);
								transfer_string_vec.push_back(new_string);
								Log.LogRecord(Logs::High, Logs::General, "    Main/PageCont - check all pages - compcode primary:page %i:%i", m_it->first, contingent_code);
							}
							clear_vec = true;
						}
					}
					transfer_int_vec.clear();
				}
			}
		}
	}
	else if (search_main_qml == false) {
		if (verify == true) {
			// need to check for codes which begin < 150 - codes from previous check

			for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
				source_line = transfer_string_vec.at(i);

				// first check for codes which belong on all pages
				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					continue;
				}
				else {
					contingent_code = stoi(transfer_string_vec.at(i), &num_sz);

					if (contingent_code < 150) {

						for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
							if (array_strID_vec.at(j) == 100) {
								if (contingent_exists == false && num_page1 != 0) {
									new_string = std::to_string(num_page1) + PFunc.PStr(506) + std::to_string(contingent_code);
									transfer_string_vec.push_back(new_string);
								}
								++num_page1;
							}
							contingent_exists = false;

							for (unsigned int k = 0; k < array_strID_vec.size(); ++k) {
								if (array_strID_vec.at(k) == 100) {
									++num_page2;
								}
								if (num_page2 > num_page1) {
									break;
								}
								else if (num_page1 != num_page2) {
									continue;
								}

								if (array_strID_vec.at(k) == contingent_code) {
									contingent_exists = true;
									break;
								}
							}
							num_page2 = 0;
						}
						if (contingent_exists == false) {
							new_string = std::to_string(num_page1) + PFunc.PStr(506) + std::to_string(contingent_code);
							transfer_string_vec.push_back(new_string);
						}
						num_page1 = 0;
					}
				}
			}
			for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
				source_line = transfer_string_vec.at(i);
				contingent_exists = false;

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					for (unsigned int j = i + 1; j < transfer_string_vec.size(); ++j) {
						if (transfer_string_vec.at(i) == transfer_string_vec.at(j)) {
							contingent_exists = true;
							break;
						}
					}
					// save for later page writing
					if (contingent_exists == false) {
						temp_string_vec.push_back(transfer_string_vec.at(i));
						Log.LogRecord(Logs::High, Logs::General, "     save for later page writing compcode %s", transfer_string_vec.at(i).c_str());
					}
				}
			}
			// determine where to put the contingent components
			// all but one are currently signal handling, 1 is a component - three different locations
			// 130-138 go after 119; 139 goes after 120; < 130 can go anywhere but we'll say before 103

			// check the page number in the string and match it up with the page_num for array_strID_vec - start with arraystrid
			for (unsigned int i = 0; i < temp_string_vec.size(); ++i) {
				source_line = temp_string_vec.at(i);

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					new_string = source_line.substr(0, line_pos);
					num_page2 = stoi(new_string, &num_sz);
					for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
						if (array_strID_vec.at(j) == 100) {
							++num_page1;
						}
						v_it = array_strID_vec.begin();

						if (num_page2 == num_page1) {
							contingents = source_line.substr(line_pos + 1, source_line.length() - line_pos + 1);
							contingent_code = stoi(contingents, &num_sz);
							if (contingent_code < 130) {
								if (array_strID_vec.at(j) == 103) {
									array_strID_vec.insert(v_it + j, contingent_code);
									break;
								}
							}
							else if (contingent_code >= 139) {
								if (array_strID_vec.at(j) == 120) {
									array_strID_vec.insert(v_it + j + 1, contingent_code);
									break;
								}
							}
							else {
								if (array_strID_vec.at(j) == 119) {
									array_strID_vec.insert(v_it + j + 1, contingent_code);
									break;
								}
							}
						}
					}
					num_page1 = 0;
				}
			}
			temp_string_vec.clear();
			for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
				source_line = transfer_string_vec.at(i);

				line_pos = source_line.find(find_string);
				if (line_pos != std::string::npos) {
					continue;
				}
				else {
					temp_string_vec.push_back(transfer_string_vec.at(i));
				}
			}
			transfer_string_vec.clear();
			//clear_vec = true;
		}
		else {
			for (m_it = transfer_i_string_map.begin(); m_it != transfer_i_string_map.end(); ++m_it) {
				if (m_it->first < 150) {
					PFunc.GetDelimitedData(m_it->second, find_string, 1);

					for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
						contingent_code = transfer_int_vec.at(i);
						contingent_exists = false;

						//	122, 174
						if (main_qml_contingents == true) {
							if (contingent_code < 150) {
								continue;
							}
							for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
								if (array_strID_vec.at(j) == m_it->first) {
									contingent_exists = true;
									break;
								}
							}
							if (contingent_exists == true) { // done - this needs to go on the main page
								// the main comp code will determine where to put it and since we know it goes on main page we do not need the primary comp code
								new_string = std::to_string(contingent_code);
								transfer_string_vec.push_back(new_string);
								Log.LogRecord(Logs::High, Logs::General, "    Page/MainCont - check main page - compcode primary:contingent %i:%i", m_it->first, contingent_code);
							}
						}
						//	112, 122, 134
						//	113, 135, 136, 137
						//	114, 122, 135, 136, 137					
						//	117, 133
						//	118, 130, 131, 132
						else if (main_qml_contingents == false) {
							if (contingent_code > 150) {
								continue;
							}
							for (unsigned int j = 0; j < array_strID_vec.size(); ++j) {
								if (array_strID_vec.at(j) == 100) {
									++num_page1;
								}
								contingent_exists = false;

								if (array_strID_vec.at(j) == m_it->first) { // match primary code in the page

									for (unsigned int k = 0; k < array_strID_vec.size(); ++k) { // search the same page for contingents
										if (array_strID_vec.at(k) == 100) {
											++num_page2;
										}
										if (num_page2 > num_page1) {
											break;
										}
										else if (num_page1 != num_page2) {
											continue;
										}

										if (array_strID_vec.at(k) == contingent_code) {
											contingent_exists = true;
											break;
										}
									}
									if (contingent_exists == false) { // done - these go on the page if the primary is on the page
										// the page comp code will determine where to put it
										new_string = std::to_string(num_page1) + PFunc.PStr(506) + std::to_string(contingent_code);
										transfer_string_vec.push_back(new_string);
										Log.LogRecord(Logs::High, Logs::General, "    page #: %i Page/PageCont - compcode primary:pagecont %i:%i", num_page1, m_it->first, contingent_code);
									}
									num_page2 = 0;
								}
							}
							num_page1 = 0;
						}
					}
					transfer_int_vec.clear();
				}
			}
		}
	}
	if (clear_vec == true) {
		array_strID_vec.clear();
	}
}

void QMLParseFunction::QML_MAIN_CONTINGENTS()
{
	std::map<int, std::string>::iterator m_it;
	std::map<int, std::string>::reverse_iterator rm_it;
	std::vector<std::string>::iterator v_it;

	std::string source_line;
	std::string file_string;
	std::string new_string;

	std::string find_string1;
	std::string find_string2;

	size_t line_pos1 = 0;
	size_t line_pos2 = 0;

	unsigned int begin_line = 0;
	unsigned int end_line = 0;
	unsigned int num_cases = 0;
	unsigned int case_count = 0;

	int line_counter = 0;
	int found_case = 0;

	bool process = false;

	// get begin/end line numbers for target search
	begin_line = PFunc.GetLineNumberAtString(true, 0, 0, QPStr(strID_vec.at(0)), "", true); // "switch(page_num) {"
	end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, QPStr(strID_vec.at(1)), "", true); // "switch(page_num) {", "return page_string;"

	if (begin_line == 0 || end_line == 0) {
		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: could not find switch to check page inclusion.");
		end_parse = true;
		return;
	}

	num_cases = PFunc.GetSwitchCaseCt(true, begin_line, end_line);

	// take care of existing cases by checking and rewriting where necessary
	for (unsigned int i = begin_line; i < output_file_data_vec.size(); ++i) {
		source_line = output_file_data_vec.at(i);
		if (num_cases <= case_count) {
			break;
		}
		++line_counter;

		Log.LogRecord(Logs::High, Logs::Test, "i:%i source_line:%s ", i, source_line.c_str());

		find_string1 = PFunc.PStr(517); // "case"

		line_pos1 = source_line.find(find_string1);
		if (line_pos1 != std::string::npos) {
	
			// number matches what it should be - check the page path-filename
			if (PFunc.CheckCaseValue(source_line, case_count) == true) {
				Log.LogRecord(Logs::High, Logs::Test, "case match - case_count %i", case_count);

				find_string1 = QPStr(strID_vec.at(2)); // "Pages/"
				for (unsigned int j = i + 1; j < output_file_data_vec.size(); ++j) {
					source_line = output_file_data_vec.at(j);

					line_pos1 = source_line.find(find_string1);
					if (line_pos1 != std::string::npos) {
						file_string = source_line.substr(line_pos1 + find_string1.length(), source_line.length() - (line_pos1 + find_string1.length()));
						find_string2 = PFunc.PStr(515); // "\""

						line_pos2 = file_string.find(find_string2);
						if (line_pos2 != std::string::npos) {
							file_string = file_string.substr(0, line_pos2);

							for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
								if (m_it->first == case_count) { // found_case
									new_string = PFunc.ExtractFilenameFromString(m_it->second);

									if (file_string != new_string) {
										Log.LogRecord(Logs::High, Logs::Test, "filename string does not match the actual filename: %s", file_string.c_str());

										if (PFunc.EraseFileData(true, 2, i, i + 1, end_line, PFunc.PStr(517), 0) == true) {
											process = true;
											Log.LogRecord(Logs::High, Logs::Test, "rewrite");
										}
										if (process == false) {
											if (PFunc.EraseFileData(true, 1, i, i + 1, 0, PFunc.PStr(504), 0) == true) {
												process = true;
												Log.LogRecord(Logs::High, Logs::Test, "find alternative string and rewrite uptostring");
											}
										}
										for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
											if (m_it->first == case_count) {
												new_string = PFunc.ExtractFilenameFromString(m_it->second);

												v_it = output_file_data_vec.begin();
												if (process == true) {
													Log.LogRecord(Logs::High, Logs::Test, "insert string");
													output_file_data_vec.emplace(v_it + i, QPStr(strID_vec.at(3), std::to_string(case_count), new_string));
												}
											}
										}
										++case_count;
										line_counter = 0;
										break;
									}
									else {
										++case_count;
										line_counter = 0;
										Log.LogRecord(Logs::High, Logs::Test, " MATCH! no changes needed: %s", file_string.c_str());
										break;
									}
								}
							}
							break;
						}
					}
				}
			}
			else if (case_count + 1 > pageID_to_path_map.size()) { // line_counter = 0 old					
				Log.LogRecord(Logs::High, Logs::Test, "there are more cases than pages - cases:(%i) pages:(%i)", case_count + 1, pageID_to_path_map.size());

				if (PFunc.EraseFileData(true, 1, i, i + 1, 0, PFunc.PStr(504), 0) == true) {
					case_count = num_cases;
				}
			}
			else {
				Log.LogRecord(Logs::High, Logs::Test, "case number does not match the pageID: %i", case_count);

				if (PFunc.EraseFileData(true, 2, i, i + 1, end_line, PFunc.PStr(517), 0) == true) {
					process = true;
					Log.LogRecord(Logs::High, Logs::Test, "rewrite");
				}

				if (process == false) {
					process = PFunc.EraseFileData(true, 1, i, i + 1, 0, PFunc.PStr(504), 0);
					Log.LogRecord(Logs::High, Logs::Test, "find alternative string and rewrite uptostring");
				}

				for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
					if (m_it->first == case_count) {
						new_string = PFunc.ExtractFilenameFromString(m_it->second);

						v_it = output_file_data_vec.begin();
						if (process == true) {
							output_file_data_vec.emplace(v_it + i, QPStr(strID_vec.at(3), std::to_string(case_count), new_string));
						}
					}
				}
				++case_count;
				line_counter = 0;
			}
		}
	}	
	line_counter = 0;

	// do extra cases
	if (case_count < pageID_to_path_map.size()) {
		num_cases = pageID_to_path_map.size() - case_count;
		Log.LogRecord(Logs::High, Logs::Test, "add %i more cases", num_cases);

		find_string1 = PFunc.PStr(517); // "case"
		for (unsigned int i = begin_line; i < output_file_data_vec.size(); ++i) {
			if (num_cases <= 0) {
				break;
			}
			source_line = output_file_data_vec.at(i);

			line_pos1 = source_line.find(find_string1);
			if (line_pos1 != std::string::npos) {

				find_string1 = PFunc.PStr(504); // "}"
				for (unsigned int k = i; k < output_file_data_vec.size(); ++k) {
					if (num_cases <= 0) {
						break;
					}
					source_line = output_file_data_vec.at(k);

					line_pos1 = source_line.find(find_string1);
					if (line_pos1 != std::string::npos) {
						for (rm_it = pageID_to_path_map.rbegin(); rm_it != pageID_to_path_map.rend(); ++rm_it) {
							if (num_cases <= 0) {
								break;
							}

							if (rm_it->first == case_count) {
								new_string = PFunc.ExtractFilenameFromString(rm_it->second);

								v_it = output_file_data_vec.begin();
								output_file_data_vec.emplace(v_it + k, QPStr(strID_vec.at(3), std::to_string(case_count), new_string));

								++case_count;
								--num_cases;
							}
						}
					}
				}
			}
		}
	}
	Log.LogRecord(Logs::High, Logs::General, "num_cases %i ", num_cases); return;
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) { 
		if (i < 35) {
			continue;
		}
		if (i >= 68) {
			break; 
		}
		//std::cout << output_file_data_vec.at(i) << std::endl;
	}
}

// category is insert inside the page and insert outside the page
void QMLParseFunction::QML_SUPPORT_GETCONTINGENTDATA(int support_operation) // 24
{
	if (end_parse == true) {
		return;
	}

	std::string find_string1;
	std::string find_string2;
	std::string source_line;
	std::string id_string;
	std::string key_string;

	size_t line_pos = 0; 
	size_t num_sz = 0;

	int key_element = 0;
	int find_id1 = 0;
	int find_id2 = 0;
	int mult_elements = 0;

	bool exists = false;

	find_id1 = GetElementFromSupportOPList(support_operation, 1);
	find_id2 = GetElementFromSupportOPList(support_operation, 2);
	mult_elements = GetElementFromSupportOPList(support_operation, 4);

	Log.LogRecord(Logs::High, Logs::General, "    Support Operation: %i ", support_operation);
	Log.LogRecord(Logs::High, Logs::General, "transfer_int_vec.size:%i", transfer_int_vec.size());
	
	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		for (unsigned int j = 0; j < input_file_data_vec.size(); ++j) {
			source_line = input_file_data_vec.at(j);

			switch (find_id1) {
			case 1: // mult_elements = 1 for this case atm
				find_string1 = all_qmlcompstrings_map[transfer_int_vec.at(i)]; // comp code find string
				break;
			case 2:
				find_string1 = QPStr(215); // "KeyboardProp1 {"				
				break;		
			case 3:
				find_string1 = QPStr(212); // "SLText {"
				break;		
			}

			line_pos = source_line.find(find_string1);
			if (line_pos != std::string::npos) {
				Log.LogRecord(Logs::High, Logs::Test, "line: %i find_string:%s", j, find_string1.c_str());

				find_string2 = QPStr(strID_vec.at(0)); // "id:"
				for (unsigned int k = j; k < input_file_data_vec.size(); ++k) {
					source_line = input_file_data_vec.at(k);

					line_pos = source_line.find(find_string2);
					if (line_pos != std::string::npos) {
						id_string = source_line.substr(line_pos + find_string2.length(), source_line.length() - (line_pos + 1));

						line_pos = id_string.find_first_not_of(PFunc.PStr(505)); // " " - space
						if (line_pos != std::string::npos) {
							id_string = id_string.substr(line_pos, id_string.length() - line_pos);

							line_pos = id_string.find_first_not_of(ALPHANUM_CHAR);
							if (line_pos != std::string::npos) {
								id_string = id_string.substr(0, line_pos);
							}
							else {
								id_string = id_string.substr(0, id_string.length());
							}


							if (find_id2 == 0) {
								if (mult_elements == 0) {
									// this is specific to compIDs which are alphanumeric
									line_pos = id_string.find_last_not_of(NUMERIC_CHAR);
									if (line_pos != std::string::npos) {
										key_string = id_string.substr(line_pos + 1, id_string.length() - (line_pos + 1));
										key_element = stoi(key_string, &num_sz);
										transfer_i_string_map[key_element] = id_string;
										Log.LogRecord(Logs::High, Logs::Test, "key_element:%i--=--id_string:%s", key_element, id_string.c_str());
									}
									else {
										// ERROR - could not find proper ID
									}
								}
								else if (mult_elements == 1) {
									transfer_string_vec.push_back(id_string);
									Log.LogRecord(Logs::High, Logs::Test, "id_string:%s--=--find_string:%s", id_string.c_str(), find_string2.c_str());
								}
							}
							else if (find_id2 > 0) {
								switch (find_id2) {
								case 1:
									find_string1 = QPStr(20); // "fieldID:"
									break;
								case 2:
									find_string1 = QPStr(18); // "menuID:" -- this will not be used
									break;								
								}
								Log.LogRecord(Logs::High, Logs::Test, "find_string:%s", find_string1.c_str());

								// this is specific to find_id2 == 1,2
								for (unsigned int l = k; l < input_file_data_vec.size(); ++l) {
									source_line = input_file_data_vec.at(l);

									line_pos = source_line.find(find_string1);
									if (line_pos != std::string::npos) {
										key_string = source_line.substr(line_pos + find_string1.length(), source_line.length() - (line_pos + find_string1.length()));

										line_pos = key_string.find_first_not_of(PFunc.PStr(505)); // " " - space
										if (line_pos != std::string::npos) {
											key_string = key_string.substr(line_pos, key_string.length() - line_pos);
										}

										Log.LogRecord(Logs::High, Logs::Test, "key_string:%s", key_string.c_str());

										line_pos = key_string.find_first_not_of(NUMERIC_CHAR);
										if (line_pos != std::string::npos) {
											key_string = key_string.substr(0, line_pos);
										}
										else {
											key_string = key_string.substr(0, key_string.length());
										}

										key_element = stoi(key_string, &num_sz);
										transfer_i_string_map[key_element] = id_string;
										Log.LogRecord(Logs::High, Logs::Test, "key_element:%i--=--id_string:%s", key_element, id_string.c_str());
										break;
									}
								}
							}														
							break;
						}
					}
				}
			}
		}
	}	
}

// rename variable to support status/strings or something
// need another variable to allow multiple comp instances on page - will not need to create a mass amount of cases with this
// uses "transfer_string_vec" from QML_SUPPORT_GETCONTINGENTDATA(F 24)
void QMLParseFunction::QML_SUPPORT_UPDATECONTINGENTS(int support_operation) // F 25
{
	if (end_parse == true) {
		return;
	}

	std::vector<int>::reverse_iterator rv_it;
	std::map<int, std::string>::iterator m_it;

	std::string find_string;
	std::string source_line;
	std::string id_string;
	std::string page_str;
	std::string comp_id;

	size_t line_pos = 0;
	size_t num_sz = 0;

	unsigned int begin_line = 0;
	unsigned int end_line = 0;
	unsigned int num_cases = 0;
	unsigned int case_count = 0;
	unsigned int case_enum = 0;
	unsigned int next_case_pos = 0;

	int page_id = 0;
	int line_counter = 0;
	int mult_elements = 0;

	bool exists = false;

	Log.LogRecord(Logs::Normal, Logs::General, "    Support Operation: %i ", support_operation);

	switch (support_operation) { // does not need to change with new list implementation
	case 1:
		begin_line = 38; // "onSavepageInput: {"
		end_line = 39; // ".saveInput()"
		break;
	case 2:
		begin_line = 40; // "function setPageFocus(page_id) {"
		end_line = 41; // ".forceActiveFocus()"
		break;
	case 3:
		begin_line = 40; // "function setPageFocus(page_id) {"
		end_line = 41; // ".forceActiveFocus()"
		break;
	case 4:
		begin_line = 42; // "onStringData: {"
		end_line = 43; // ".textChange(str_data, str_pos)"
		break;
	case 5:
		begin_line = 44; // "onDataCode: {"
		end_line = 45; // ".menuPropertyChange(n_data, s_data)"
		break;
	case 6:
		begin_line = 46; // "onSendWords: {"
		end_line = 47; // ".saveWords(str_data)"
		break;
	case 7:
		begin_line = 48; // "onSendWordCount: {"
		end_line = 49; // ".saveWordCount(n_data, s_data)"
		break;
	case 8:
		begin_line = 50; // "onCloseMenu: {"
		end_line = 51; // ".state = \"\""
		break;
	}
	
	for (unsigned int i = begin_line; i <= end_line; ++i) {
		strID_vec.push_back(i); 
		Log.LogRecord(Logs::High, Logs::Test, "strID_vec: %i-%s", i, QPStr(i).c_str());
	}
	begin_line = 0;
	end_line = 0;

	begin_line = PFunc.GetLineNumberAtString(true, 0, 0, QPStr(strID_vec.at(0)), "", true);
	end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, PFunc.PStr(504), "", true); // "}"	
	next_case_pos = begin_line;	
	
	if (begin_line == 0 || end_line == 0) {
		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: could not find switch to check page inclusion.");
		end_parse = true;
		return;
	}
	num_cases = PFunc.GetSwitchCaseCt(true, begin_line, end_line);
	transfer_int_vec.clear();

	Log.LogRecord(Logs::High, Logs::Test, "Begin: num_cases: %i begin_line: %i end_line: %i", num_cases, begin_line, end_line);

	mult_elements = GetElementFromSupportOPList(support_operation, 4);

	// get case values from compIDs (it's the # at the end of the compID which is = pageID/case)
	if (mult_elements == 1) {
		for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
			id_string = transfer_string_vec.at(i);

			line_pos = id_string.find_last_not_of(NUMERIC_CHAR);
			if (line_pos != std::string::npos) {
				id_string = id_string.substr(line_pos + 1, id_string.length() - (line_pos + 1));
				case_enum = stoi(id_string, &num_sz);

				for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
					if (case_enum == transfer_int_vec.at(i)) {
						exists = true;
						break;
					}
				}
				if (exists == false) {
					transfer_int_vec.push_back(case_enum);
				}
				exists = false;
			}
		}
	}
	else {
		//m_it = transfer_i_string_map.begin();
		for (m_it = transfer_i_string_map.begin(); m_it != transfer_i_string_map.end(); ++m_it) {
			transfer_int_vec.push_back(m_it->first);
		}
	}

	// process - case_enum = page number or fieldID/menuID
	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		if (case_count >= num_cases) {
			break;
		}
		case_enum = transfer_int_vec.at(i);
		exists = false;

		Log.LogRecord(Logs::High, Logs::Test, "Process 1: begin_line: %i end_line: %i case: %i num_cases: %i", begin_line, end_line, case_enum, num_cases);

		for (unsigned int j = begin_line; j < output_file_data_vec.size(); ++j) {
			if (j >= end_line) {
				if (case_count <= num_cases) {
					Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: Could not finish processing.");
					end_parse = true;
					return;
				}
				break;
			}
			source_line = output_file_data_vec.at(j);

			find_string = PFunc.PStr(517) + PFunc.PStr(505) + std::to_string(case_enum) + PFunc.PStr(514); // "case" + " " + pgstr + ":" -> "case #:"
			begin_line = PFunc.GetLineNumberAtString(true, begin_line, end_line, PFunc.PStr(517), "", true);

			// get # lines between cases
			next_case_pos = PFunc.GetLineNumberAtString(true, begin_line, end_line, PFunc.PStr(517), PFunc.PStr(517), false); // "case"

			if (next_case_pos == 0) {
				next_case_pos = end_line;
			}
			line_counter = next_case_pos - begin_line;

			Log.LogRecord(Logs::High, Logs::Test, "Process 2: begin_line: %i next_case_pos: %i line_counter: %i", begin_line, next_case_pos, line_counter);

			if (PFunc.GetLineNumberAtString(true, begin_line, end_line, find_string, "", true) == begin_line) {
				exists = true;
				break;
			}
			else {
				exists = false;
				break;
			}
		}

		// erase incorrect case
		if (exists == false) {
			if (PFunc.EraseFileData(true, 0, begin_line, 0, 0, "", line_counter) == true) {
				Log.LogRecord(Logs::High, Logs::Test, "Erase-Replace 1: begin_line: %i end_line: %i", begin_line, end_line);

				InsertCompIDCase(support_operation, case_enum, begin_line, 3);

				end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, PFunc.PStr(504), "", true); // "}"				
				begin_line = PFunc.GetLineNumberAtString(true, begin_line + 1, end_line, PFunc.PStr(517), "", true); // "case"	

				Log.LogRecord(Logs::High, Logs::Test, "Erase-Replace 2: begin_line: %i end_line: %i", begin_line, end_line);

				line_counter = 0;
				++case_count;
			}
		}
		// check compid within correct case
		else if (exists == true) {
			if (mult_elements == 1) { // uses vector since the map key is the case
				Log.LogRecord(Logs::High, Logs::Test, "Vec Case Exists 1: case: %i line_counter: %i", case_enum, line_counter);

				for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
					id_string = transfer_string_vec.at(i);

					line_pos = id_string.find_last_not_of(NUMERIC_CHAR);
					if (line_pos != std::string::npos) {
						page_str = id_string.substr(line_pos + 1, id_string.length() - (line_pos + 1));
						page_id = stoi(page_str, &num_sz);

						// match new compid page id to case - compare IDs
						if (page_id == case_enum) {
							for (unsigned int j = begin_line + 1; j < output_file_data_vec.size(); ++j) {
								if (j >= end_line || j >= next_case_pos) {
									break;
								}
								source_line = output_file_data_vec.at(j);
								exists = false;
								
								// find the lines with the compids attached from file data
								find_string = QPStr(strID_vec.at(1));
								line_pos = source_line.find(find_string);
								if (line_pos != std::string::npos) {
									comp_id = source_line.substr(0, line_pos);
									comp_id = PFunc.RemoveWhitespace(comp_id);

									Log.LogRecord(Logs::High, Logs::Test, "comp_id:(%s):id_string:(%s):", comp_id.c_str(), id_string.c_str());

									if (comp_id == id_string) {
										Log.LogRecord(Logs::High, Logs::Test, "exists == true");
										exists = true;
										break;
									}
								}
							}
							if (exists == false) {
								Log.LogRecord(Logs::High, Logs::Test, "exists == false");
								break;
							}
						}
					}
				}
			}
			else {
				Log.LogRecord(Logs::High, Logs::Test, "Map Case Exists 1: case: %i line_counter: %i", case_enum, line_counter);

				// match new compid page id to case - compare IDs
				m_it = transfer_i_string_map.find(case_enum);
				if (m_it != transfer_i_string_map.end()) {
					id_string = m_it->second;

					for (unsigned int j = begin_line + 1; j < output_file_data_vec.size(); ++j) {
						if (j >= end_line || j >= next_case_pos) {
							break;
						}
						source_line = output_file_data_vec.at(j);
						exists = false;

						// find the lines with the compids attached from file data
						find_string = QPStr(strID_vec.at(1));
						line_pos = source_line.find(find_string);
						if (line_pos != std::string::npos) {
							comp_id = source_line.substr(0, line_pos);
							comp_id = PFunc.RemoveWhitespace(comp_id);

							Log.LogRecord(Logs::High, Logs::Test, "comp_id:(%s):id_string:(%s):", comp_id.c_str(), id_string.c_str());

							if (comp_id == id_string) {
								Log.LogRecord(Logs::High, Logs::Test, "exists == true");
								exists = true;
								break;
							}
						}
					}
					if (exists == false) {
						Log.LogRecord(Logs::High, Logs::Test, "exists == false %i ", exists);
						//break;
					}
				}
			}			
			// erase and rewrite the case, set begin/end line positions, increment case count and reset reused variables
			if (exists == false) {
				Log.LogRecord(Logs::High, Logs::Test, "  Erase and Rewrite: case %i line count %i", case_enum, line_counter);
				if (PFunc.EraseFileData(true, 0, begin_line, 0, 0, "", line_counter) == true) {
					InsertCompIDCase(support_operation, case_enum, begin_line, 3);
				}
			}
			Log.LogRecord(Logs::High, Logs::Test, "Case Exists 2: begin line: %i next_case_pos: %i end_line: %i", begin_line, next_case_pos, end_line);

			end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, PFunc.PStr(504), "", true); // "}"	
			begin_line = PFunc.GetLineNumberAtString(true, begin_line + 1, end_line, PFunc.PStr(517), "", true); // "case"

			if (begin_line == 0) {
				Log.LogRecord(Logs::High, Logs::Test, "  No more cases: begin_line = end_line");
				begin_line = end_line;
			}
			Log.LogRecord(Logs::High, Logs::Test, "Case Exists 3: begin line: %i next_case_pos: %i end_line: %i", begin_line, next_case_pos, end_line);

			exists = false;
			line_counter = 0;
			++case_count;
		}	
	}

	// erase extra cases
	if (num_cases > transfer_int_vec.size()) {
		Log.LogRecord(Logs::High, Logs::Test, "Erase Extra Case 1: begin_line: %i end_line: %i - erase %i case(s)", begin_line, end_line, num_cases - transfer_int_vec.size());

		end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, PFunc.PStr(504), "", true);
		line_counter = end_line - begin_line;
		Log.LogRecord(Logs::High, Logs::Test, "  Erase: line count %i", line_counter);
		PFunc.EraseFileData(true, 0, begin_line, 0, 0, "", line_counter);

		Log.LogRecord(Logs::High, Logs::Test, "Erase Extra Case 2: begin_line %i end_line %i - erase %i case(s)", begin_line, end_line, num_cases - transfer_int_vec.size());
	}

	// begin/end line are the same and simply insert what remains - possibly put this outside of this
	if (case_count < transfer_int_vec.size()) {
		num_cases = transfer_int_vec.size() - case_count;
		Log.LogRecord(Logs::High, Logs::Test, "Insert Case 1: begin_line: %i end_line: %i - add %i case(s)", begin_line, end_line, num_cases);

		end_line = PFunc.GetLineNumberAtString(true, begin_line, 0, PFunc.PStr(504), "", true);

		Log.LogRecord(Logs::High, Logs::Test, "Insert Case 2: begin_line: %i end_line: %i - add %i case(s)", begin_line, end_line, num_cases);

		for (rv_it = transfer_int_vec.rbegin(); rv_it != transfer_int_vec.rend(); ++rv_it) {
			if (num_cases <= 0) {
				break;
			}
			case_enum = *rv_it;

			InsertCompIDCase(support_operation, case_enum, end_line, 3);

			++case_count;
			--num_cases;
		}
	}
	Log.LogRecord(Logs::High, Logs::General, "num_cases %i ", num_cases); return;
	for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
		if (support_operation == 1) {
			if (i < 18) {
				continue;
			}
			if (i >= 35) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		if (support_operation == 2) {
			if (i < 50) {
				continue;
			}
			if (i >= 63) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		if (support_operation == 3) {
			if (i < 12) {
				continue;
			}
			if (i >= 45) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		if (support_operation == 4) {
			if (i < 105) {
				continue;
			}
			if (i >= 120) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		if (support_operation == 5 || support_operation == 6 || support_operation == 7) {
			if (i < 145) {
				continue;
			}
			if (i >= 200) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		if (support_operation == 8) {
			if (i < 68) {
				continue;
			}
			if (i >= 90) {
				break;
			}
			//std::cout << output_file_data_vec.at(i) << std::endl;
		}
		//std::cout << output_file_data_vec.at(i) << std::endl;
	}
}

void QMLParseFunction::ParseIntoPages()
{
	std::vector<std::string>::iterator v_it;

	std::string source_line;
	std::string find_string;
	std::string new_string;

	size_t line_pos = 0;

	unsigned int end_line = 0;

	bool found = false;

	// QPStr(52) second find string
	// QPStr(97) replace string, do not replace the whole line, only add the comp ID

	// onPreviousPage 133 uses previousbutton or transferstringvec.at(1)

	for (unsigned int h = 0; h < strID_vec.size(); ++h) {
		if (strID_vec.at(h) < 0) {
			continue;
		}
		find_string = QPStr(strID_vec.at(h)); // find compcode string
		end_line = PFunc.GetLineNumberAtString(true, 0, 0, find_string, PFunc.PStr(504), false); // "}"	
		found = false;

		Log.LogRecord(Logs::High, Logs::Test, "end_line (%i) find_string (%s)", end_line, find_string.c_str());
		
		v_it = output_file_data_vec.begin();
		for (unsigned int i = 0; i < output_file_data_vec.size(); ++i) {
			if (found == true) {
				break;
			}
			source_line = output_file_data_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {

				find_string = QPStr(52); // ".color = Qt."
				for (unsigned int j = i; j < output_file_data_vec.size(); ++j) {
					if (j > end_line) {
						break;
					}
					source_line = output_file_data_vec.at(j);

					line_pos = source_line.find(find_string);
					if (line_pos != std::string::npos) {
						if (strID_vec.at(h) == 233) {
							new_string = "\t\t\t" + transfer_string_vec.at(1) + source_line.substr(line_pos, source_line.length() - line_pos);
						}
						else {
							new_string = "\t\t\t" + transfer_string_vec.at(0) + source_line.substr(line_pos, source_line.length() - line_pos);
						}						 
						output_file_data_vec.at(j) = new_string;
						
						Log.LogRecord(Logs::High, Logs::Test, "output %s", output_file_data_vec.at(j).c_str());
						Log.LogRecord(Logs::High, Logs::Test, "new_string %s", new_string.c_str());
						
						found = true;
						break;
					}
				}
			}
		}
	}
}

// "tar_directory" is passed in place of filename for AppendQMLComponents()
void QMLParseFunction::QML_CREATE_MAIN(std::string tar_directory)
{
	if (end_parse == true) {
		return;
	}
	
	int compcode = 0;
	int compid_iter = 0;

	if (strID_vec.size() == 0) {		
		PFunc.GetArrayNumbers(QMLSubsets[MAINQMLNEW], "", 0);

		for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
			strID_vec.push_back(transfer_int_vec.at(i));
		}
	}
	transfer_int_vec.clear();

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		compcode = strID_vec.at(i);
		++compid_iter;

		if (compcode >= 200 || compcode < 100) {
			continue;
		}
		AppendQMLComponents(tar_directory, -1, compcode, compid_iter);
	}
	Log.LogRecord(Logs::Normal, Logs::General, "  New main.qml created");
}

void QMLParseFunction::GetCompIDFromMain()
{
	std::string source_line;
	std::string find_string;
	std::string id_string;

	size_t line_pos = 0;

	unsigned int end_line = 0;

	bool found = false;

	for (unsigned int h = 0; h < strID_vec.size(); ++h) {
		if (strID_vec.at(h) < 0) {
			continue;
		}
		find_string = QPStr(strID_vec.at(h)); // find compcode string
		end_line = PFunc.GetLineNumberAtString(false, 0, 0, find_string, PFunc.PStr(504), false); // "}"	
		found = false;

		Log.LogRecord(Logs::High, Logs::Test, "end_line (%i) find_string (%s)", end_line, find_string.c_str());

		for (unsigned int i = 0; i < input_file_data_vec.size(); ++i) {
			if (found == true) {
				break;
			}
			source_line = input_file_data_vec.at(i);

			line_pos = source_line.find(find_string);
			if (line_pos != std::string::npos) {

				find_string = QPStr(26); // "id:"
				for (unsigned int j = i; j < input_file_data_vec.size(); ++j) {
					if (j > end_line) {
						break;
					}
					source_line = input_file_data_vec.at(j);
					
					line_pos = source_line.find(find_string);
					if (line_pos != std::string::npos) {

						id_string = source_line.substr(line_pos + find_string.length(), source_line.length() - (line_pos + find_string.length()));

						line_pos = id_string.find_first_not_of(PFunc.PStr(505));
						if (line_pos != std::string::npos) {
							id_string.substr(line_pos, id_string.length() - line_pos);
						}

						line_pos = id_string.find_first_not_of(ALPHANUM_CHAR);
						if (line_pos != std::string::npos) {
							id_string.substr(0, line_pos);
						}
						transfer_string_vec.push_back(id_string);
						Log.LogRecord(Logs::High, Logs::Test, "  id_string %s", id_string.c_str());

						if (id_string.length() > 0) {
							found = true;
						}						
						break;
					}
				}
			}
		}
	}
}

std::string QMLParseFunction::GetSupportFile(int support_operation)
{
	std::string support_file;

	switch (support_operation) {
	case 1:
		support_file = QTOutputFile[OUT_INTERPAGEPROCESSES];
		break;
	case 2:
		support_file = QTOutputFile[OUT_WRITELINETEXT];
		break;
	case 3:
		support_file = QTOutputFile[OUT_KEYBOARDPROP1];
		break;
	}
	return support_file;
}

//  int find_stringid1, int find_stringid2, int insert_stringid, bool multiple
// operation_string
void QMLParseFunction::InsertCompIDCase(int support_op, int case_value, unsigned int line_offset, int indent)
{	
	std::map<int, std::string>::iterator m_it;

	std::string id_string;
	std::string page_str;
	std::string code_line;	
	
	size_t line_pos = 0;
	size_t num_sz = 0;

	int insert_stringid = 0;
	int mult_elements = 0;
	int page_id = 0;
	int elements = 0;

	insert_stringid = GetElementFromSupportOPList(support_op, 3);
	mult_elements = GetElementFromSupportOPList(support_op, 4);
	
	if (mult_elements == 0) {
		m_it = transfer_i_string_map.find(case_value);
		if (m_it != transfer_i_string_map.end()) {
			id_string = m_it->second;

			switch (insert_stringid) {
			case 2:
				code_line = QPStr(91, id_string);
				break;
			case 3:
				code_line = QPStr(92, id_string);
				break;
			case 4:
				code_line = QPStr(93, id_string);
				break;
			case 5:
				code_line = QPStr(94, id_string);
				break;
			case 6:
				code_line = QPStr(95, id_string);
				break;
			case 7:
				code_line = QPStr(96, id_string);
				break;
			}
			Log.LogRecord(Logs::High, Logs::General, "case_value %i page_id %i", case_value, page_id);
		}
	}
	else if (insert_stringid == 1)  {
		for (unsigned int i = 0; i < transfer_string_vec.size(); ++i) {
			id_string = transfer_string_vec.at(i);

			line_pos = id_string.find_last_not_of(NUMERIC_CHAR);
			if (line_pos != std::string::npos) {
				page_str = id_string.substr(line_pos + 1, id_string.length() - (line_pos + 1));
				page_id = stoi(page_str, &num_sz);

				if (page_id == case_value) {
					if (elements > 0) {
						code_line.append(PFunc.PStr(519) + QPStr(90, id_string)); // "\n"
					}
					else {
						code_line.append(QPStr(90, id_string));
					}
					++elements;
					Log.LogRecord(Logs::High, Logs::General, "case_value %i page_id %i", case_value, page_id);					
				}
			}
		}
	}
	PFunc.InsertCaseIntoSwitch(true, case_value, code_line, line_offset, indent);
	code_line.clear();
}

void PrepCompCodeArrayData(int subset_compcodes)
{

}

// transfer_int_vec is used between functions and reliant upon CPP_ARRAY_GETCOMPCODES(F 8)
// GetElementCtFromArrayFileData relies on tranfer_int_vec AND temp_int_vec
// add another variable?
void QMLParseFunction::SupportContingents(int subset_compcodes)
{
	ParseFunctions PFunc;
	
	typedef std::vector<int>comp_code;
	typedef std::vector<comp_code>page_comp;

	std::string in_file;

	int page_ct = 0;
	int num_page = 0;
	int element_ct = 0;
	int max_elements = 0;	
	
	bool exists = false;	

	// get # of pages and existing element count from vector
	PFunc.GetArrayNumbers(QMLSubsets[subset_compcodes], "", 0);

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		temp_int_vec.push_back(transfer_int_vec.at(i));
		Log.LogRecord(Logs::High, Logs::General, " transfer_int_vec.size() %i ", transfer_int_vec.size());		
	}
	transfer_int_vec.clear();

	for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
		transfer_int_vec.push_back(array_strID_vec.at(i));
	}
	num_page = PFunc.GetArrayDimFromFileData(1, "", 100, true);
	max_elements = PFunc.GetElementCtFromArrayFileData(1, "", 100, "", 0, false);
	transfer_int_vec.clear();	

	Log.LogRecord(Logs::High, Logs::General, " max_elements %i ", max_elements);

	// this could be used for many types of contingent relationships
	const size_t max_pages(num_page);
	const size_t max_comp(max_elements);

	page_comp comppage(max_pages, comp_code(max_comp, 0));

	for (unsigned int i = 0; i < array_strID_vec.size(); ++i) {
		if (array_strID_vec.at(i) == 100) {
			++page_ct;
			element_ct = 0;
		}

		for (unsigned int j = 0; j < temp_int_vec.size(); ++j) {
			if (array_strID_vec.at(i) == temp_int_vec.at(j)) {
				exists = true;
				break;
			}
		}
		// issue may be here with same data on same page for support operations > 3
		if (exists == true) {
			++element_ct;
			comppage[page_ct - 1][element_ct - 1] = array_strID_vec.at(i);
			Log.LogRecord(Logs::High, Logs::General, " page: %i element %i", page_ct - 1, array_strID_vec.at(i));
		}
		exists = false;
	}	
	temp_int_vec.clear();
	
	// setup compcode_to_string_map for page file parsing
	MapQMLCompIDtoStrID(1, true);

	for (unsigned int i = 0; i < comppage.size(); ++i) { // i = pageID  max_pages.at(j) = compcode
		comp_code & max_pages(comppage[i]);

		for (unsigned int j = 0; j < max_pages.size(); ++j) {
			if (max_pages.at(j) > 0) {
				if (all_qmlcompstrings_map[max_pages.at(j)].length() > 0) {
					in_file = PFunc.ExtractFilenameFromString(pageID_to_path_map[i]);

					if (subset_compcodes == CONTCODE_PGMENU) {
						for (unsigned int l = 0; l < transfer_int_vec.size(); ++l) {
							if (transfer_int_vec.at(l) == max_pages.at(j)) {
								exists = true;
								break;
							}
						}
						if (exists == false) {
							transfer_int_vec.push_back(max_pages.at(j));
							Log.LogRecord(Logs::High, Logs::Test, "CONTCODE_PGMENU - max_pages.at(j) %i", max_pages.at(j));
						}
						exists = false;
					}

					for (unsigned int k = 0; k < input_filename_vec.size(); ++k) {
						if (input_filename_vec.at(k) == in_file) {
							exists = true;
							break;
						}
					}					
					if (exists == false) {
						if (subset_compcodes != CONTCODE_PGMENU) {
							transfer_int_vec.push_back(max_pages.at(j));
							Log.LogRecord(Logs::High, Logs::Test, "max_pages.at(j) %i", max_pages.at(j));
						}						
						input_filename_vec.push_back(in_file);
						Log.LogRecord(Logs::High, Logs::Test, "in_file %s", in_file.c_str());
					}
					exists = false;
				}
				else {
					Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: comp code insertion error");
				}
			}
		}
	}	

//	// set the out file based on "subset_compcodes"
//	switch (subset_compcodes) {
//	case CONTCODE_SVINPUT:
//		//QTInputFile	output_filename_vec.push_back();
//		break;
//	default:
//		Log.LogRecord(Logs::Normal, Logs::Error, "ERROR: invalid strID subset: %i", subset_compcodes);
//		break;			
//	}
	

}

// atm only output_file_data_vec is used for file data - only strID_vec for component string data - as it should be, we're creating output
void QMLParseFunction::AppendQMLComponents(std::string filename, int page_number, int component, int compID_iter)
{
	ParseDirectory PDir;

	std::string new_string;
	std::string page_id;
	std::string num_page;
	std::string comp_id;

	std::string arg1;
	std::string arg2;

	std::size_t ext_pos;

	bool no_id = false;
	
	switch (component) {
	case 101: // col layout begin with id
		ext_pos = filename.find(FileType[QML]);
		page_id = filename.substr(0, ext_pos);
		page_id[0] = tolower(page_id[0]);
		new_string = QPStr(component, page_id);
		output_file_data_vec.push_back(new_string);
		break;
	case 161: // filename = app name
		arg1 = PDir.BoostGetFoldername(filename);
		new_string = QPStr(component, arg1);
		output_file_data_vec.push_back(new_string);
		break;
	case 163: // filename = app name; arg2 = relative path + filename (resources)
		arg1 = PDir.BoostGetFoldername(filename);
		arg2 = "Resources/";			
		new_string = QPStr(component, arg1, arg2);
		output_file_data_vec.push_back(new_string);
		break;
	case 168: // arg1 = (begin/splash page filename - FileType[QML]) - setup may not know so omit, can change in a different function
		new_string = QPStr(component);
		output_file_data_vec.push_back(new_string);
		break;
	default: // most generic standard components which either require an id or don't get an id
		num_page = std::to_string(page_number);
		comp_id = std::to_string(compID_iter);
		new_string = QPStr(component, comp_id, num_page);
		output_file_data_vec.push_back(new_string);
		break;
	}	
}

// puts to memory strings, strIDs and compIds - (i -100) may change if the number of components becomes greater than 100
void QMLParseFunction::MapQMLCompIDtoStrID(int id_range, bool clear)
{
	//	0 = unadjusted
	//	1 = QMLID_END - 40
	//	2 = QMLID_START + 50

	std::map<int, std::string>::iterator m_it;

	int begin_pos = 0;
	int end_pos = 0;

	if (clear == true) {
		all_qmlcomponents_map.clear();
		all_qmlcompstrings_map.clear();
	}

	if (id_range == 0) {
		begin_pos = QMLID_START;
		end_pos = QMLID_END;
	}
	else if (id_range == 1) {
		begin_pos = QMLID_START;
		end_pos = QMLID_END - 40;
	}
	else if (id_range == 2) {
		begin_pos = QMLID_START + 50;
		end_pos = QMLID_END;
	}

	for (int i = begin_pos; i < end_pos; ++i) {
		if (QPStr(i).length() > 0) {
			all_qmlcomponents_map[i] = i - 100;
			all_qmlcompstrings_map[i - 100] = QPStr(i);
			Log.LogRecord(Logs::High, Logs::General, "search ID:(%i) string ID:(%i)", i, i - 100);
			Log.LogRecord(Logs::High, Logs::General, "search ID:(%i) find string:%s--=", i, QPStr(i).c_str());
		}
	}
	//for (m_it = all_qmlcompstrings_map.begin(); m_it != all_qmlcompstrings_map.end(); ++m_it) { std::cout << m_it->first << " strID:str " << m_it->second << std::endl; }
}

// puts the compIDs into specific vecs in a certain order to be compared - this is not needed when changing the array, the order is already set
void QMLParseFunction::ArrangeCompIDsintoVec(int page_number)
{
	int num_pages = 0;

	content_strID_vec.clear();
	array_strID_vec.clear();

	for (unsigned int i = 0; i < strID_vec.size(); ++i) {
		if (strID_vec.at(i) == 100) {
			++num_pages;
		}
		if (num_pages - 1 == page_number) {
			array_strID_vec.push_back(strID_vec.at(i));
		}
	}
	num_pages = 0;

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		if (transfer_int_vec.at(i) == 100) {
			++num_pages;
		}
		if (num_pages - 1 == page_number) {
			content_strID_vec.push_back(transfer_int_vec.at(i));
		}
	}
	num_pages = 0;
}

void QMLParseFunction::CompareCompIDProcessSwitch(bool diff_found, bool size_diff, bool change_array, int size_variance, int diff_ct)
{
	// we parsed to EOF w/o finding similarities
	if (diff_found == true) {
		Log.LogRecord(Logs::Normal, Logs::General, "There are too many differences between the .qml file and .cpp array to accurately parse and keep existing data intact.\n  Completing the file manually and syncing to the array is the best option.");
		end_parse = true;
	}
	else if (size_diff == false && diff_ct == 0) { // below may not mean what it says - it's better than it was but possbily not failsafe
		Log.LogRecord(Logs::Normal, Logs::General, "\n    No descrepancies between the .qml file and the .cpp array found.");
		process_required = false;
	}
	else if (change_array == true) {
		process_required = true;
		Log.LogRecord(Logs::Normal, Logs::General, "\n    (%i) component ID difference(s) found in the array", diff_ct);
		if (size_diff == true) {
			if (size_variance < 0) {
				size_variance = -size_variance;
				Log.LogRecord(Logs::Normal, Logs::General, "(%i) component(s) were removed from the array.", size_variance);
			}
			else {
				Log.LogRecord(Logs::Normal, Logs::General, "(%i) component(s) were added to the array.", size_variance);
			}
		}
	}
	else if (change_array == false) {
		process_required = true;
		Log.LogRecord(Logs::Normal, Logs::General, "\n    (%i) component ID difference(s) found in the file.", diff_ct);
		if (size_diff == true) {
			if (size_variance < 0) {
				size_variance = -size_variance;
				Log.LogRecord(Logs::Normal, Logs::General, "(%i) component(s) were added to the file.", size_variance);
			}
			else {
				Log.LogRecord(Logs::Normal, Logs::General, "(%i) component(s) were removed from the file.", size_variance);
			}
		}
	}
}

bool QMLParseFunction::CompHasNoID(int component_id)
{
	bool exists = false;		

	if (transfer_int_vec.size() == 0) {
		PFunc.GetArrayNumbers(QMLSubsets[COMPNOID], "", 0);
	}

	for (unsigned int i = 0; i < transfer_int_vec.size(); ++i) {
		if (transfer_int_vec.at(i) == component_id) {
			exists = true;
			break;
		}		
	}
	return exists;
}

void QMLParseFunction::PrepareSupportOperation(int support_opid, bool clear_op_codes, bool clear_op_data, bool skip_support_filepop)
{
	if (end_parse == true) {
		return;
	}

	int subset_code = 0;	

	AddToSupportOPList(support_opid, clear_op_codes);

	if (clear_op_data == true) {
		transfer_i_string_map.clear();
		transfer_string_vec.clear();
		transfer_int_vec.clear();
	}

	if (skip_support_filepop == false) {	
		subset_code = GetElementFromSupportOPList(support_opid, 0);
		SupportContingents(subset_code);
	}	
}

// function to manipultate linked list here
// this function should be placed immediately after a successful login
void QMLParseFunction::AddToSupportOPList(int support_op, bool clear)
{
	if (support_op < 0) {
		Log.LogRecord(Logs::Normal, Logs::Error, "  Invalid support_op: (%i)\n    Aborting parse", support_op);
		end_parse = true;
		return;
	}

	if (clear == true) {
		support_oplist.clear();
	}

	SupportOperation_struct support_func;
	new SupportOperation_struct; // uncertain about this, if it's needed

	support_func.support_operation = support_op;

	for (int i = 0; i < SUPOP_YDIM; ++i) {
		switch (i) {
		case 0:
			support_func.qml_subsetid = QMLsupport_operation[i][support_op];
			break;
		case 1:
			support_func.file_strid1 = QMLsupport_operation[i][support_op];
			break;
		case 2:
			support_func.file_strid2 = QMLsupport_operation[i][support_op];
			break;
		case 3:
			support_func.insert_strid = QMLsupport_operation[i][support_op];
			break;
		case 4:
			support_func.mult_instances = QMLsupport_operation[i][support_op];
			break;
		}
	}
	support_oplist.push_back(support_func);
	Log.LogRecord(Logs::Med, Logs::General, "AddToSupportOPList -\n    opID: %i QMLsubset: %i strID1: %i strID2: %i instrID: %i multiple %i",
		support_func.support_operation, support_func.qml_subsetid, support_func.file_strid1, support_func.file_strid2, support_func.insert_strid, support_func.mult_instances);
}

int QMLParseFunction::GetElementFromSupportOPList(int support_op, int element_type)
{
	// element types
	// 1 = qml subsetID
	// 1 = find_stringid1
	// 2 = find_stringid2
	// 3 = insert_stringid
	// 4 = bool mult_instances

	std::list<SupportOperation_struct>::iterator l_it;
	int element = 0; // = "";
	
	for (l_it = support_oplist.begin(); l_it != support_oplist.end(); ++l_it) {
		if (l_it->support_operation == support_op) {
			switch (element_type) {
			case 0:
				element = l_it->qml_subsetid;
				break;
			case 1:
				element = l_it->file_strid1;
				break;
			case 2:
				element = l_it->file_strid2;
				break;
			case 3:
				element = l_it->insert_strid;
				break;
			case 4:
				element = l_it->mult_instances;
				break;
			}
			break;
		}
	}
	return element;
}

// the idea is to eventually use a counter within the parse function so the stringIDs do not need to be listed, just the counter
// copy two lines example: then be sure to reset strID = 0 at the end of the function
//			PStr(strID_vec[strID]);
//			++strID;
std::string QMLParseFunction::QPStr(int str_id, std::string arg1, std::string arg2)
{
	std::string r_str;

	if (str_id == 0) { r_str = "\n\tDummy component! {}"; } // testing

	// CPP_CODE_DCH
	if (str_id == 1) { r_str = "MaxDataCode"; }
	if (str_id == 2) { r_str = "\t" + arg1 + ","; }
	if (str_id == 3) { r_str = "DataFileString"; }
	if (str_id == 4) { r_str = "\t\"" + arg1 + "\","; }
	if (str_id == 5) { r_str = "class DataCodes"; }
	if (str_id == 6) { r_str = "\tstd::vector<std::string> " + arg1 + ";"; }

	// CPP_CODE_DCSRC
	if (str_id == 7) { r_str = "void DataCodes::PrepareFileData()"; }
	if (str_id == 8) { r_str = "\tfile_data_map[DataFileString[" + arg1 + "]] = " + arg2 + ";"; }
	if (str_id == 9) { r_str = "void DataCodes::GetFileData(std::string tar_directory)"; }
	if (str_id == 10) { r_str = "data_code_stream.close();"; }
	if (str_id == 11) { r_str = "\t\tcase DataCodeID::" + arg1 + ":\n\t\t\twhile (std::getline(data_code_stream, entry)) {\n\t\t\t\t" + arg2 + ".push_back(entry);\n\t\t\t}\n\t\t\tbreak;"; }

	// QML_ALTER_NEWPAGE - strings to alter a newly created .qml "page"
	// FILE_MATCH_PG - save to map multiple pageIDs to out_paths .qmls
	// QML_PG_GETCOMPCODES() - for compID content to array match
	// QML_PG_ORDER - to shift pageID due to page insertion
	if (str_id == 12) { r_str = "pageID:"; }

	// CPP_ARRAY_GETLOGICIDS - gets array data for various componentIDs; feildID, textID, menuID, responseID
	if (str_id == 13) { r_str = "fieldID_to_pageID"; }
	if (str_id == 14) { r_str = "fieldID_to_num_responses"; }
	if (str_id == 15) { r_str = "responseID_from_fieldID"; }

	// QML_PG_UPDATELOGICIDS - update various component IDs in all pages with a "PageID:" in a specific directory
	if (str_id == 16) { r_str = "textID:"; }
	if (str_id == 17) { r_str = "\t\ttextID: " + arg1; }
	if (str_id == 18) { r_str = "menuID:"; }
	if (str_id == 19) { r_str = "\t\t\tmenuID: " + arg1; }
	if (str_id == 20) { r_str = "fieldID:"; }
	if (str_id == 21) { r_str = "\t\t\tfieldID: " + arg1; }
	if (str_id == 22) { r_str = "responseID:"; }
	if (str_id == 23) { r_str = "\t\t\tresponseID: " + arg1; }

	// QMLUICOMPONENTS1 - gets array data for template componentIDs
	if (str_id == 24) { r_str = "componentID_from_pageID"; }

	// CPP_ARRAY_GETMAINCOMPCODES - gets array data for template componentIDs
	if (str_id == 25) { r_str = "componentID_from_main"; }

	// QML_PG_GETCOMPIDS - gets individual component ids
	if (str_id == 26) { r_str = "id:"; }
	if (str_id == 27) { r_str = "pg"; }

	// for setup/populate QML files function
	if (str_id == 28) { r_str = "DefinedComponents/"; }
	if (str_id == 29) { r_str = "StaticComponents/"; }
	if (str_id == 30) { r_str = "SupportComponents/"; }
	if (str_id == 31) { r_str = "Resources/"; }
	if (str_id == 32) { r_str = "Pages/"; }

	// QML_MAIN_CONTINGENTS - this needs to be implemented separately when pages are created into main.qml at 162	
	if (str_id == 33) { r_str = "switch(page_num) {"; }
	if (str_id == 34) { r_str = "return page_string;"; }
	if (str_id == 35) { r_str = "Pages/"; }
	if (str_id == 36) { r_str = "\t\tcase " + arg1 + ":\n\t\t\tpage_string = \"Pages/" + arg2 + "\";\n\t\t\tbreak;"; } // arg1 = pageID  arg2 = page filename
	
	// QML_SUPPORT_GETCONTINGENTDATA
	if (str_id == 37) { r_str = "id:"; }

	// QML_SUPPORT_UPDATECONTINGENTS - 112,113,114,115
	if (str_id == 38) { r_str = "onSavepageInput: {"; }
	if (str_id == 39) { r_str = ".saveInput()"; }
	if (str_id == 40) { r_str = "function setPageFocus(page_id) {"; }
	if (str_id == 41) { r_str = ".forceActiveFocus()"; }

	// half of these are located below
	if (str_id == 42) { r_str = "onStringData: {"; }
	if (str_id == 43) { r_str = ".textChange(str_data, str_pos)"; }
	if (str_id == 44) { r_str = "onDataCode: {"; }
	if (str_id == 45) { r_str = ".menuPropertyChange(s_data, n_data)"; }
	if (str_id == 46) { r_str = "onSendWords: {"; }
	if (str_id == 47) { r_str = ".saveWords(str_data)"; }
	if (str_id == 48) { r_str = "onSendWordCount: {"; }
	if (str_id == 49) { r_str = ".saveWordCount(n_data, s_data)"; }
	if (str_id == 50) { r_str = "onCloseMenu: {"; }
	if (str_id == 51) { r_str = ".state = \"\""; }

	if (str_id == 52) { r_str = ".color = Qt."; }

	// when certain compcodes are used, these strID values need to be parsed in
	// InsertCompIDCase() - contingent operations - 112,113,114,115
	if (str_id == 90) { r_str = "\t\t\t\t" + arg1 + ".saveInput();"; } // arg1 = componentID
	if (str_id == 91) { r_str = "\t\t\t" + arg1 + ".forceActiveFocus();"; } // arg1 = keyproppg1

	if (str_id == 92) { r_str = "\t\t\t" + arg1 + ".textChange(str_data, str_pos);"; } // arg1 = SLText componentID
	if (str_id == 93) { r_str = "\t\t\t" + arg1 + ".menuPropertyChange(n_data, s_data);"; } // arg1 = Menu componentID
	if (str_id == 94) { r_str = "\t\t\t" + arg1 + ".saveWords(str_data);"; } // arg1 = Menu componentID
	if (str_id == 95) { r_str = "\t\t\t" + arg1 + ".saveWordCount(n_data, s_data);"; } // arg1 = Menu componentID
	if (str_id == 96) { r_str = "\t\t\t" + arg1 + ".state = \"\";"; } // arg1 = Menu componentID

	if (str_id == 97) { r_str = "\t\t\t" + arg1 + ".color = Qt."; } // arg1 = Navigation component

	//.color = Qt.


	// .qml standard page components 
	if (str_id == 100) { r_str = "import QtQuick 2.7\nimport QtQuick.Layouts 1.3\nimport \"qrc:/DefinedComponents/\"\nimport \"qrc:/StaticComponents/\"\nimport \"qrc:/SupportComponents/\"\nimport \"qrc:/VKboard/\"\nimport UIProcess 1.0\n"; }
	if (str_id == 101) { r_str = "ColumnLayout { /*page begin*/\n\tid: " + arg1 + "\n"; } // arg1 = filename - extension
	if (str_id == 102) { r_str = "\tproperty int pageID\n\tproperty int responseID\n\t\n\tpageID: " + arg2 + "\n"; } // arg2 = page_id
	if (str_id == 103) { r_str = "\tComponent.onCompleted: {\n\t\tstackView.doNewPageProcess(pageID);\n\t\tkeyproppg" + arg2 + ".forceActiveFocus();   \n\t}\n}"; }

	// .qml standard custom page components - when the standard is set the buttons listed below(116,117,118) will need to be change along with their ID couterparts
	if (str_id == 110) { r_str = "\tSpacer { id: spacer" + arg1 + "pg" + arg2 + "; /*boxHeight: someitem.height * 1;*/ opacity: .0; }\n"; } // arg1 = compID page counter
	if (str_id == 111) { r_str = "\tReadOnlyText {\n\t\tid: rot" + arg1 + "pg" + arg2 + "\n\t\ttxtText: \"rot" + arg1 + "pg" + arg2 + "\"\n\t\ttxtSize: 16\n\t\ttxtColor: \"black\"\n\t\ttextID: -1\n\t}\n"; }
	if (str_id == 112) {
		r_str = "\tSLText {\n\t\tid: slt" + arg1 + "pg" + arg2 + "\n\t\ttxlTxtSz: 14\n\t\ttxlLabelTxt: \"slt" + arg1 + "pg" + arg2 + "\"\n\t\tmaxLineLen: 25"
			+ "\n\t\tfieldID: -1\n\t\tresponseID: 2\n\t\tinputStatus: 0\n\t}\n";
	}	
	if (str_id == 113) {
		r_str = "\tDDMenu {\n\t\tid: ddmenu" + arg1 + "pg" + arg2 + "\n\t\tshownItemText: \"ddmenu" + arg1 + "pg" + arg2 + "\"\n\t\tLayout.fillHeight: false"
			+ "\n\t\tfieldID: -1\n\t\tmenuID: -1\n\t\tresponseID: 3\n\t\tkbpropDDmenu.onFocusChanged: {\n\t\t\tif (kbpropDDmenu.activeFocus === false) {\n\t\t\t\tinterpgprocspg"
			+ arg2 + ".closeMenu(fieldID);\n\t\t\t}\n\t\t}\n\t}\n";
	}
	if (str_id == 114) {
		r_str = "\tACTMenu {\n\t\tid: actmenu" + arg1 + "pg" + arg2 + "\n\t\twltLabel: \"actmenu" + arg1 + "pg" + arg2 + "\"\n\t\tshownItemText: \"\""
			+ "\n\t\tLayout.fillHeight: false\n\t\tfieldID: -1\n\t\tmenuID: -1\n\t\tresponseID: 3"
			+ "\n\t\ttxtEdit.onFocusChanged: {\n\t\t\tif(txtEdit.activeFocus === false) {\n\t\t\t\tinterpgprocspg" + arg2 + ".closeMenu(fieldID);\n\t\t\t}\n\t\t}\n\t}\n";
	}
	if (str_id == 115) { r_str = "\tKeyboardProp1 {\n\t\tid: keyproppg" + arg2 + "\n\t}\n"; }
	if (str_id == 116) {
		r_str = "\tGreyButton {\n\t\tid: greybut" + arg1 + "pg" + arg2 + "\n\t\tbutTxt: \"greybut" + arg1 + "pg" + arg2 + "\"\n\t\t/*Layout.alignment: Qt.AlignHCenter;*/"
			+ "\n\t\tfieldID: -1\n\t\tresponseID: -1\n\t}\n";
	}
	if (str_id == 117) {
		r_str = "\tPreviousButton {\n\t\tid: butprev" + arg1 + "pg" + arg2 + "\n\t\tbutTxt: \"butprev" + arg1 + "pg" + arg2 + "\"\n\t\t/*Layout.fillWidth: false;*/"
			+ "\n\t\t/*Layout.fillHeight: false;*/\n\t\t/*Layout.alignment: Qt.AlignLeft | Qt.AlignBottom;*/\n\t}\n";
	}
	if (str_id == 118) {
		r_str = "\tNextButton {\n\t\tid: nbutton" + arg1 + "pg" + arg2 + "\n\t\tbutTxt: \"nbutton" + arg1 + "pg" + arg2 + "\"\n\t\t/*Layout.fillWidth: false;*/"
			+ "\n\t\t/*Layout.fillHeight: false;*/\n\t\t/*Layout.alignment: Qt.AlignLeft | Qt.AlignBottom;*/\n\t\t/*mouseNext.onClicked: { savepageInput(pageID); }*/\n\t}\n";
	}
	if (str_id == 119) { r_str = "\tConnections /*uiProcess*/ {\n\t\ttarget: uiProcess\n"; }
	if (str_id == 120) { r_str = "\tConnections /*stackView*/ {\n\t\ttarget: stackView\n"; }
	if (str_id == 121) { r_str = "\t} /*end connections*/\n"; }
	if (str_id == 122) { r_str = "\tInterpageProcesses {\n\t\tid: interpgprocspg" + arg2 + "\n\t}\n"; } // this is a contingent of 112,114 and multiple-field pages

	// .qml standard custom page source signals - navigation
	if (str_id == 130) { r_str = "\t\tonValidData: {\n\t\t\t" + arg1 + ".color = Qt.darker(\"tan\", 1.2);\n\t\t}\n"; } // arg1 = NextButton componentID
	if (str_id == 131) { r_str = "\t\tonInvalidData: {\n\t\t\t" + arg1 + ".color = Qt.darker(\"green\", .8);\n\t\t}\n"; } // arg1 = NextButton componentID
	if (str_id == 132) { r_str = "\t\tonNoData: {\n\t\t\t" + arg1 + ".color = Qt.darker(\"green\", .8);\n\t\t}\n"; } // arg1 = NextButton componentID
	if (str_id == 133) { r_str = "\t\tonPreviousPage: {\n\t\t\t" + arg1 + ".color = Qt.darker(\"yellow\", .7);\n\t\t}\n"; } // arg1 = PreviousButton componentID
	// .qml standard custom page source signals - state changes
	if (str_id == 134) { r_str = "\t\tonStringData: {\n\t\t\tswitch(str_pos) { // str_pos = fieldID\n\t\t\tcase 1:\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n"; }
	if (str_id == 135) { r_str = "\t\tonDataCode: {\n\t\t\tswitch(s_data) { // fieldID\n\t\t\tcase 1:\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n"; }
	// .qml standard custom page source signals - menu lists
	if (str_id == 136) { r_str = "\t\tonSendWords: {\n\t\t\tswitch (d_data) { // fieldID\n\t\t\tcase 1:\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n"; }
	if (str_id == 137) { r_str = "\t\tonSendWordCount: {\n\t\t\tswitch (n_data) { // fieldID\n\t\t\tcase 1:\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n"; }
	// .qml standard custom page source signals - VKeyboard - onSendFocusType goes with stackView connections
	if (str_id == 138) {
		r_str = "\t\tonSendFocusType: {\n\t\t\tif (s_data == 1) { // focus type - 1 = page focus\n\t\t\t\tSLText1 ID.txtLine.cursorVisible = false;\n\t\t\t\tSLText2 ID.txtLine.cursorVisible = false;\n\t\t\t\tkeyproppg"
			+ arg2 + ".forceActiveFocus();\n\t\t\t\tnextbutton.mouseNext.enabled = true;\n\t\t\t\tpreviousbutton.mousePrev.enabled = true;\n\t\t\t}\n\t\t}\n";
	}
	if (str_id == 139) { r_str = "\t\tonShowLine: {\n\t\t\tswitch(field_id) {\n\t\t\tcase 1:\n\t\t\t\tbreak;\n\t\t\t}\n\t\t}\n"; }

	// .qml row/column layout, 150 ends these layout components
	if (str_id == 150) { r_str = "\t} /*end layout*/\n"; }
	if (str_id == 151) {
		r_str = "\tRowLayout {\n\t\tid: rlayout" + arg1 + "pg" + arg2 + "\n\t\tspacing: 20" +
			"\n\t\tanchors { } // bottom: someitem.top; top: someitem.bottom; right: someitem.left; left: someitem.right;" +
			"\n\t\tLayout.alignment: Qt.AlignHCenter // | Qt.AlignBottom, Qt.AlignTop, Qt.AlignVCenter\n";
	}
	if (str_id == 152) {
		r_str = "\tColumnLayout {\n\t\tid: clayout" + arg1 + "pg" + arg2 + "\n\t\tspacing: 20" +
			"\n\t\tanchors { } // bottom: someitem.top; top: someitem.bottom; right: someitem.left; left: someitem.right;" +
			"\n\t\tLayout.alignment: Qt.AlignHCenter // | Qt.AlignBottom, Qt.AlignTop, Qt.AlignVCenter\n";
	}
	if (str_id == 153) { r_str = "\t\tRowLayout /*main*/ {\n\t\t\tid: mainrlayout" + arg1 + "\n\t\t\tanchors.fill: parent\n\t\t\tanchors.margins: 20\n\t\t\tLayout.alignment: Qt.AlignHCenter | Qt.AlignBottom;\n"; } // arg1 = componentct, if possible
	if (str_id == 154) { r_str = "\t\tColumnLayout /*main*/ {\n\t\t\tid: mainclayout" + arg1 + "\n\t\t\tanchors.fill: parent\n"; } // arg1 = componentct, if possible

	// main.qml standard mandatory components	
	if (str_id == 160) {
		r_str = "import QtQuick 2.7 /*main*/\nimport QtQuick.Window 2.2\nimport QtQuick.Controls 1.4\nimport QtQuick.Layouts 1.3\nimport \"qrc:/DefinedComponents/\""
			"\nimport \"qrc:/StaticComponents/\"\nimport \"qrc:/SupportComponents/\"\nimport \"qrc:/VKboard/\"\nimport \"qrc:/Pages/\"\nimport UIProcess 1.0\n\n";
	}
	if (str_id == 161) {
		r_str = "Window {\n\tproperty int initialWidth : 800\n\tproperty int initialHeight : 550\n\n\tid: mainWindow\n\twidth: initialWidth\n\theight: initialHeight\n\tminimumWidth: 400"
			"\n\tminimumHeight: 300\n\tmaximumWidth: 1000\n\tmaximumHeight: 750\n\tvisible: true\n\ttitle: qsTr(\"" + arg1 + "\")\n";
	} // arg1 = app name
	if (str_id == 162) {
		r_str = "\tproperty int pageNum\n\tproperty string nextPageURL\n\tproperty string prevPageURL\n\n\tfunction getPageURLstring(page_num) {" // getPageURLstring()
			"\n\t\tvar page_string;\n\t\tswitch(page_num) {\n\t\tcase 1:\n\t\t\tbreak;\n\t\t}\n\t\treturn page_string; \n\t}\n";
	}
	if (str_id == 163) {
		r_str = "\tImage {\n\t\tid: mainbkgrnd" + arg1 + "\n\t\tvisible: true\n\t\tfillMode: Image.Stretch"
			"\n\t\tanchors.fill: parent\n\t\tsource: \"" + arg2 + "\"\n\t}\n";
	} // arg1 = app name; arg2 = relative path + filename (resources)
	if (str_id == 164) { r_str = "\tStackView {\n\t\tid: stackView\n\t\tanchors.fill: parent\n\t\tanchors.margins: 10\n\t\tLayout.fillWidth: true\n"; }
	if (str_id == 165) { r_str = "\t\tUIProcess {\n\t\t\tid: uiProcess\n"; }
	if (str_id == 166) { r_str = "\t\t} /*end uiprocess*/\n"; }
	if (str_id == 167) { r_str = "\t\tKeyboardProp1 /*main*/ {\n\t\t\tid: keyinputStackView\n\t\t}\n"; }
	if (str_id == 168) { r_str = "\t\tinitialItem: " + arg1 + " {\n\t\t\tComponent.onCompleted: {\n\t\t\t\tkeyinputStackView.forceActiveFocus()\n\t\t\t}\n\t\t}\n\t}\n}\n"; } // arg1 = 1st page filename - extension

	// main.qml standard optional components - many of these should have defined prerequisite page compIDs and/or an option(switch)
	if (str_id == 169) { r_str = "\tPreviousButton /*main*/ {\n\t\tid: buttonPrevious\n\t\tbutTxt: \"Previous\"\n\t\t/*Layout.fillWidth: false;*/\n\t\t/*Layout.fillHeight: false;*/\n\t\t/*Layout.alignment: Qt.AlignLeft | Qt.AlignBottom;*/\n\t}\n"; }
	if (str_id == 170) {
		r_str = "\tNextButton /*main*/ {\n\t\tid: buttonNext\n\t\tbutTxt: \"Continue\"\n\t\t/*Layout.fillWidth: false;*/\n\t\t/*Layout.fillHeight: false;*/"
			"\n\t\t/*Layout.alignment: Qt.AlignRight | Qt.AlignBottom;*/\n\t\t\tproperty alias nbutText: buttonNext.butTxt\n\t\tmouseNext.onClicked: { stackView.savepageInput(pageNum); }\n\t}\n";
	}
	if (str_id == 171) {
		r_str = "\t\t\tReadOnlyText /*main*/ {\n\t\t\t\tid: mainheader\n\t\t\t\ttxtText: \"mainheader\"\n\t\t\t\ttxtSize: 24\n\t\t\t\tLayout.alignment : Qt.AlignHCenter | Qt.AlignTop"
			"\n\t\t\t\tLayout.leftMargin: initialWidth * .1\n\t\t\t\tLayout.rightMargin: initialWidth * .1\n\t\t\t}\n";
	}
	if (str_id == 172) { r_str = "\t\tQuitButton {\n\t\t\tid: buttonQuit\n\t\t\tLayout.alignment : Qt.AlignTop\n\t\t}\n"; }
	if (str_id == 173) { r_str = "\t\tErrorButton {\n\t\t\tid: buttonError\n\t\t}\n"; }
	if (str_id == 174) { r_str = "\t\tsignal navigateSideways(int field_id);\n\t\tsignal savepageInput (int page_id);\n"; } // InterpageProcesses - can be placed anywhere after stackView
	if (str_id == 175) {
		r_str = "\t\tdelegate: Transitions {\n\t\t\tid: svDelegate\n\t\t}\n\t\tfunction getRandomNumber(scope, shift) {\n\t\t\tsvDelegate.randnum = shift + (Math.random() * scope);\n\t\t\treturn svDelegate.randnum;\n\t\t}"
			"\n\t\tfunction setSVDrannum() {\n\t\t\tsvDelegate.ranDuration1 = getRandomNumber(200, 100);\n\t\t\tsvDelegate.ranDuration2 = getRandomNumber(100, 100);"
			"\n\t\t\tsvDelegate.ranX = getRandomNumber(mainWindow.width * 2, (-mainWindow.width));\n\t\t\tsvDelegate.ranY = getRandomNumber(mainWindow.height * 2, (-mainWindow.height));\n\t\t}"
			"\n\t\tfunction lockInput(lock) {\n\t\t\tif(lock === true) {\n\t\t\t\tstackView.enabled = false;\n\t\t\t\tlockinputTimer.start();\n\t\t\t}\n\t\t\telse {\n\t\t\t\tstackView.enabled = true;\n\t\t\t}\n\t\t}"
			"\n\t\tTimer {\n\t\t\tid: lockinputTimer\n\t\t\tinterval: 800\n\t\t\tonTriggered: {\n\t\t\t\tstackView.lockInput(false);\n\t\t\t}\n\t\t }\n";
	}
	if (str_id == 176) {
		r_str = "\t\tStaticTransitions {\n\t\t\tid: statrans\n\t\t}\n\t\tfunction doNewPageProcess(page_id) {\n\t\t\tstatrans.doPageStaticTransition();"
			"\n\t\t\tpageNum = page_id;\n\t\t\tuiProcess.sendPromptData(pageNum);\n\t\t\tuiProcess.sendMenuData(pageNum);\n\t\t\tuiProcess.sendFieldData(pageNum);\n\t\t\tuiProcess.getNextData(pageNum);"
			"\n\t\t\t/* if(buttonError.state == = \"on\") {\n\t\t\t\tbuttonError.toggle()\n\t\t\t} */\n\t\t}\n";
	}

	// main.qml standard custom source signals - navigation - within UIProcess
	if (str_id == 180) { r_str = "\t\t\tonValidData: /*main*/ {\n\t\t\t\tnextPageURL = Qt.resolvedUrl(getPageURLstring(n_data))\n\t\t\t\tstackView.lockInput(true);\n\t\t\t\tstackView.replace(nextPageURL)\n\t\t\t\tstackView.setSVDrannum();\n\t\t\t}\n"; }
	if (str_id == 181) { r_str = "\t\t\tonInvalidData: /*main*/ {\n\t\t\t\tbuttonNext.state = \"on\";\n\t\t\t\tbuttonNext.toggleWarning();\n\t\t\t}\n"; }
	if (str_id == 182) { r_str = "\t\t\tonNoData: /*main*/ {\n\t\t\t\tbuttonNext.state = \"on\";\n\t\t\t\tbuttonNext.toggleWarning();\n\t\t\t}\n"; }
	if (str_id == 183) { r_str = "\t\t\tonPreviousPage: /*main*/ {\n\t\t\t\tprevPageURL = Qt.resolvedUrl(getPageURLstring(n_data))\n\t\t\t\tstackView.lockInput(true);\n\t\t\t\tstackView.replace(prevPageURL)\n\t\t\t\tstackView.setSVDrannum();\n\t\t\t}\n"; }
	if (str_id == 184) { r_str = "\t\t\tonProcessedData: /*main*/ {\n\t\t\t\tQt.quit()\n\t\t\t}\n"; }

	// standard component identifiers - loaded into <all_qmlcompstrings_map> to assist with identifying components
	// these should be separated for more efficient parsing page/main
	// .qml components found on each page
	if (str_id == 200) { r_str = "import QtQuick 2.7"; }
	if (str_id == 201) { r_str = "ColumnLayout { /*page begin*/"; } // "id:" <- unique identifier = filename which signifies beginning of .qml code
	if (str_id == 202) { r_str = "property int pageID"; }
	if (str_id == 203) { r_str = "Component.onCompleted: {"; }
	// .qml standard custom components
	if (str_id == 210) { r_str = "Spacer {"; }
	if (str_id == 211) { r_str = "ReadOnlyText {"; }
	if (str_id == 212) { r_str = "SLText {"; }
	if (str_id == 213) { r_str = "DDMenu {"; }
	if (str_id == 214) { r_str = "ACTMenu {"; }
	if (str_id == 215) { r_str = "KeyboardProp1 {"; }
	if (str_id == 216) { r_str = "GreyButton {"; } // this will change
	if (str_id == 217) { r_str = "PreviousButton {"; } // this may change
	if (str_id == 218) { r_str = "NextButton {"; } // this may change
	if (str_id == 219) { r_str = "Connections /*uiProcess*/ {"; } // this may change
	if (str_id == 220) { r_str = "Connections /*stackView*/ {"; } // this may change
	if (str_id == 221) { r_str = "} /*end connections*/"; } // this may change
	if (str_id == 222) { r_str = "InterpageProcesses {"; }
	// .qml standard custom source signals - navigation
	if (str_id == 230) { r_str = "onValidData: {"; }
	if (str_id == 231) { r_str = "onInvalidData: {"; }
	if (str_id == 232) { r_str = "onNoData: {"; }
	if (str_id == 233) { r_str = "onPreviousPage: {"; }
	// .qml standard custom source signals - state changes
	if (str_id == 234) { r_str = "onStringData: {"; }
	if (str_id == 235) { r_str = "onDataCode: {"; }
	// .qml standard custom source signals - menu lists
	if (str_id == 236) { r_str = "onSendWords: {"; }
	if (str_id == 237) { r_str = "onSendWordCount: {"; }
	// .qml standard custom source signals - VKeyboard
	if (str_id == 238) { r_str = "onSendFocusType: {"; }
	if (str_id == 239) { r_str = "onShowLine: {"; }
	// .qml row/column layout, 150 ends these layout components
	if (str_id == 250) { r_str = "} /*end layout*/"; }
	if (str_id == 251) { r_str = "RowLayout {"; } // "id: rlayout" <- unique identifier
	if (str_id == 252) { r_str = "ColumnLayout {"; } // "id: clayout" <- unique identifier	
	if (str_id == 253) { r_str = "RowLayout /*main*/ {"; }
	if (str_id == 254) { r_str = "ColumnLayout /*main*/ {"; }
	// main.qml standard components									
	if (str_id == 260) { r_str = "import QtQuick 2.7 /*main*/"; }
	if (str_id == 261) { r_str = "Window {"; }
	if (str_id == 262) { r_str = "property int pageNum"; }
	if (str_id == 263) { r_str = "Image {"; }
	if (str_id == 264) { r_str = "StackView {"; }
	if (str_id == 265) { r_str = "UIProcess {"; }
	if (str_id == 266) { r_str = "} /*end uiprocess*/"; }
	if (str_id == 267) { r_str = "KeyboardProp1 /*main*/ {"; }
	if (str_id == 268) { r_str = "initialItem:"; }
	// main.qml standard custom components
	if (str_id == 269) { r_str = "PreviousButton /*main*/ {"; }
	if (str_id == 270) { r_str = "NextButton /*main*/ {"; }
	if (str_id == 271) { r_str = "ReadOnlyText /*main*/ {"; }
	if (str_id == 272) { r_str = "QuitButton {"; }
	if (str_id == 273) { r_str = "ErrorButton {"; }
	if (str_id == 274) { r_str = "signal navigateSideways(int field_id);"; }
	if (str_id == 275) { r_str = "delegate: Transitions {"; }
	if (str_id == 276) { r_str = "StaticTransitions {"; }
	// main.qml standard custom source signals - navigation			
	if (str_id == 280) { r_str = "onValidData: /*main*/ {"; }
	if (str_id == 281) { r_str = "onInvalidData: /*main*/ {"; }
	if (str_id == 282) { r_str = "onNoData: /*main*/ {"; }
	if (str_id == 283) { r_str = "onPreviousPage: /*main*/ {"; }
	if (str_id == 284) { r_str = "onProcessedData: /*main*/ {"; }

	// commonly used find strings
	if (str_id == 500) { r_str = "0"; }
	if (str_id == 501) { r_str = "["; } // arrays
	if (str_id == 502) { r_str = "]"; } // arrays
	if (str_id == 503) { r_str = "{"; } // arrays
	if (str_id == 504) { r_str = "}"; } // arrays
	if (str_id == 505) { r_str = " "; } // arrays - be aware of "\t" possibilites
	if (str_id == 506) { r_str = ","; } // arrays
	if (str_id == 507) { r_str = "."; }
	if (str_id == 508) { r_str = "_"; }
	if (str_id == 509) { r_str = "};"; } // end of array, class, enum, many things
	if (str_id == 510) { r_str = " \t"; } // code files - std::string whitespaces (" \t\f\v\n\r");
	if (str_id == 511) { r_str = "{}"; } // code files
	if (str_id == 512) { r_str = ";"; } // code files
	if (str_id == 513) { r_str = "\t"; } // code files
	if (str_id == 514) { r_str = ":"; } // code files
	if (str_id == 515) { r_str = "\""; } // code files
	if (str_id == 516) { r_str = "/\\"; }	// path separators

	return r_str;
}
