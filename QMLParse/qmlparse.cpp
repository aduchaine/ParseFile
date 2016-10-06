#include <iostream>

#include "qmlparse.h"
#include "qmlparsefunction.h"

#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/Logging/logging.h"

#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/parsefiles.h"
#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/parsedirectory.h"
#include "C:/Users/Dude/Documents/Test programs/ParseFile/ParseFile/parsefunctions.h"


const int pd_type = 1;


QMLParse::QMLParse()
{

}

void QMLParse::QMLMainParse(int parse_directive)
{
	ParseFiles PF;
	ParseDirectory PDir;
	ParseFunctions PFunc;

	QMLParseFunction QPFunc;

	std::map<int, std::string>::iterator m_it;

	std::string out_dir;
	std::string in_dir;
	std::string file_ext;

	std::string out_file;
	std::string out_path;
	std::string in_file;
	std::string in_path;

	int p_func_count = 0;
	int p_subdir_count = 0;
	int p_func_id = 0;
	int num_pages = 0;
	unsigned int fbitmask_value = 0;
	int fbit_pos = 0;
	int supp_op = 0;

	// PD 6 erased compcodes incorrectly then put them back on a second run for the cpp array
	// populate from resources based on areas in file from parsing keywords - main.qml
	// AppendQMLComponents() should probably pass another argument(variable)

	/*
	mult directive arrangements
	1) 12, 5, 13, 4, 10, 11
	12 - create main.qml from cpp array data;
	5  - create .qml pages from cpp array data;
	13 - check comp code file dependencies;
	4  - sync global IDs;
	10 - parse main/pages navigation contingents;
	11 - parse contingents into support component files;

	PARSE DIRECTIVES
	case 1:		add .txt filenames to datacodes.h/.cpp for data lists - testApp/
	case 2:		create basic .qml "page" file - testApp/pages/ - (new page file with minimal components; will shift pageIDs if inserted) fix F 14 to not write if no change
	case 3:		add "page" components to .qml page - testApp/pages/ - (appends page file with components at a specific location) - only one purpose/function
	case 4:		sync global IDs in all .qml "page" files to .cpp arrays - testApp/ & testApp/pages/ (menuID, textID, fieldID, responseID)
	- this will not produce accurate results if components are changed - the .cpp arrays in testApp/ must be altered
	case 5:		create .qml "page" file from .cpp array - testApp/pages/ (creates as many pages as the array defines; component IDs set upon insertion)
	case 6:		change "page" components in .qml file from .cpp array  - testApp/pages/ (single page file or all pages; simple insert/erase/replace page components)
	change "page" components in .cpp array from .qml file - testApp/pages/ (single page file or all pages; array changes-> rewrite array)
	case 7:		reorder .qml component IDs within "page" file - testApp/pages/ (single page or all pages)
	retrieve .qml component IDs within "page" file - testApp/pages/ (single page or all pages)
	case 8:		change "main" components in .qml file from .cpp array  - testApp/ (simple insert/erase/replace page components)
	change "main" components in .cpp array from .qml file - testApp/ (array changes-> rewrite array)
	case 9:		change "main" contingent components .cpp array - testApp/ (array changes-> rewrite array) - assures no duplicates of certain comp codes, cross checks pages where necessary, cross checks main, inserts/changes codes/array dim as needed
	change "page" contingent components .cpp array - testApp/ (array changes-> rewrite array) - cross checks pages, cross checks main where necessary, inserts/changes codes/array dim as needed
	case 10:	alters "main" page filename navigation switch, alter page navigation contingents (enumerates, deletes, inserts in main; inserts in pages) - uses main.qml compIDs
	case 11:	alters all SupportComponents .qml files valid compIDs through all page files, (enumerates, deletes, inserts) - does not do "onNavigateSideways()" because the UI logic is not complete
	case 12:	create new main.qml from .cpp array data
	case 13:	check and populate all required .qml component file dependencies as needed - add resources to this

	PARSE SUBDIRECTIVES
	case -1: snyc filename to pageID, prepare content pg compIds, reorder compIDs within file and prepare compID map(no uses), prepare array pg compIds - testApp/
	- this will likely evolve into a setup sub which would be executed prior to changing anything related to pg compIDs or compIDs within file
	- under these circumstances, some of the functions included/soon-to-be-included in this sub would be executed at the end of the directive, as well
	case -2: entire check of TestApp UI arrays/files (main + pages) - not much or any file creation atm - disabled
	case -3: main.qml comp code preparation for writing - testApp/ - data is prepared based on choice of chnaging .cpp array or .qml file
	*/


	// proposed user input or test variables
	int ui_page_num = -1; // user input page number for all pages this must be set to -1, currently
	int user_choice = 0; // user input for test change .cpp array or .qml page file  1 = change_file 2 = change_array
	std::string user_file; // user input for file naming
	bool user_switch = false; // user input for(2): change/sync or get componentIDs - reordering pageIDs
	bool do_all = false; // poss user input or a switch based on input


	switch (parse_directive) {

	case 1: // add lists to header/source files for testApp			

		// F 1
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);
		file_ext = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 3);

		out_path = parse_outpaths_vec.at(p_func_count); // set to 0 when Directive is set - first position out path
		PDir.GetInputFiles(in_dir, file_ext); // parse input directories for new files, put filenames into memory
		PF.GetOutputData(out_path); // parse out file contents into memory

		QPFunc.CPP_CODE_DCH(file_ext); // parse the output file in memory and make changes using strings from GetStringIDs()
		PF.FileReWrite(out_path, true); // rewrite the actual file
		strID_vec.clear();
		++p_func_count; // add 1 to count for the next parse-function in this directive		


		// F 2
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count); // gets a new out path, the data from it, the strings to search for and replace, rewrites the memory then rewrites the file
		out_path = parse_outpaths_vec.at(p_func_count); // get the new output file path

		PF.GetOutputData(out_path);  // parse out file into memory 
		QPFunc.CPP_CODE_DCSRC(); // parse the output file in memory and make changes using strings from GetStringIDs()
		PF.FileReWrite(out_path, true); // rewrite in the second file
		
		break;

	case 2: // create new .qml page in testApp/pages
		// POSSIBLE_USER_INPUT:  pageID, filename

		user_file = "Test";
		ui_page_num = 5; // optional user input - if pageID exists, all >= will be increased by 1 (page insertion)
		user_switch = true;


		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 14 - automatically rewrites each file even if the pageID is correct, fix this
		fbit_pos = 3;
		p_func_id = PF.GetFunctionID(p_func_count);

		if (user_switch == true) {
			fbitmask_value = PF.GetBitValue((fbit_pos - 1) - 1, false); // don't clear data = 2
			HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value);
		}

		fbitmask_value = PF.GetBitValue((fbit_pos - 1) - 2, false) + PF.GetBitValue(fbit_pos - 1, false); // skip prep, make space for new page = val 1 + val 3
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value);
		++p_func_count;


		// F 3
		PF.PrepareFunction(pd_type, 1, p_func_count, p_func_count); // do not to overwrite an existing file - get all filenames with <in_filetype>		
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		file_ext = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 3);
		out_file = PF.MakeNewFilename(file_ext, user_file); // new name function and check if exists		

		PF.FileOpen(out_dir + out_file, CREATE);
		QPFunc.QML_CREATE_PAGE(out_file, ui_page_num); // parse strings will be retrieved within the function
		PF.FileReWrite(out_dir + out_file, true);
		Log.LogRecord(Logs::Normal, Logs::General, "\t%s created", out_file.c_str());
		
		break;

	case 3: // add to a new .qml page file in testApp/pages - current implementation is only mildly useful
		// POSSIBLE_USER_INPUT:  pageID, componentIDs		

		ui_page_num = 8; // user input

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 4
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			if (m_it->first == ui_page_num) {
				out_path = m_it->second;
				break;
			}
		}
		PF.GetOutputData(out_path);
		QPFunc.QML_ALTER_NEWPAGE();
		PF.FileReWrite(out_path, true);
		
		break;

	case 4: // sync all .qml file global IDs TESTAPP - menuID, textID, fieldID, responseID		

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 6
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() < 1) {
			end_parse = true;
		}
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
		}
		in_path = in_dir + in_file;

		PF.GetInputData(in_path);
		QPFunc.CPP_ARRAY_GETLOGICIDS();
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// F 7
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			out_path = m_it->second;

			if (out_path.length() > 0) {
				PF.GetOutputData(out_path);
				QPFunc.QML_PG_UPDATELOGICIDS(m_it->first);
				PF.FileReWrite(out_path, true);
			}
		}
		Log.LogRecord(Logs::Normal, Logs::General, "\n    All .qml logic IDs updated in:\n\t%s", out_dir.c_str());
		
		break;

	case 5: // create one or more new .qml pages in ../pages from .cpp array data	

		user_file = "Test"; // user input		

		// F 8
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		PF.PrepareFunction(pd_type, 2, p_func_count, p_func_count);
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		file_ext = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 3);
		
		for (unsigned int i = 0; i < strID_vec.size(); ++i) {
			if (strID_vec.at(i) == 100) { // get the number of files to be created - <strID_vec>(ID 100 = 1st line of new file)
				++num_pages;
			}
			else {
				continue;
			}
		}

		Log.LogRecord(Logs::Normal, Logs::General, "\n  Creating (%i) QML pages in:\n    %s", num_pages, out_dir.c_str());
		for (int i = 0; i < num_pages; ++i) {
			out_file = PF.MakeNewFilename(file_ext, user_file);
			PF.FileOpen(out_dir + out_file, CREATE);

			QPFunc.QML_PG_CREATE(out_file, file_ext, i + 1);
			PF.FileReWrite(out_dir + out_file, true);
		}
		transfer_int_vec.clear();

		break;

	case 6: // change current .qml page(s) or change array from .qml page(s) in testApp/pages
		// POSSIBLE_USER_INPUT:  pageID, componentIDs (this would change how this Directive works completely)

		do_all = true;
		ui_page_num = -1; // for testing, if do_all = true, change this to -1
		user_choice = 2; // 1 = change_file 2 = change_array		


		QMLSubdirective(-1, p_func_count, p_func_count); //  --- // subdirective -1
		p_func_count = p_func_count + PF.GetSubdirectiveFunctionCt(pd_type, -1);


		// must have array_strID_vec data
		// subdirective complete - cleanup directory residuals in memory and increase "p_func_count" to bypass the subdirective trigger in "parse_directive_vec" memory 
		PF.DoDirectiveCleanup(false, false);
		++p_func_count; // to bypass subdirective notation in "parseFunc_from_parseDir"
		Log.LogRecord(Logs::Normal, Logs::General, "\n Resuming main Directive");
		PF.GetParsePaths(pd_type, p_func_count);

		if (user_choice == 1) {
			if (do_all == false) {
				Log.LogRecord(Logs::Normal, Logs::General, "  Synchronizing .qml page (%i) components to .cpp array.", ui_page_num);
			}
			else {
				Log.LogRecord(Logs::Normal, Logs::General, "  Synchronizing all .qml page's components to .cpp array.");
			}

			PF.PrepareFunction(pd_type, 2, p_func_count, p_subdir_count);
			out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);		

			// this needs testing -- appears to work
			// allows proper data arrangement - data arrangement is done below 
			if (do_all == true) {
				for (unsigned int i = 0; i < content_strID_vec.size(); ++i) {
					if (i == 0) {
						PFunc.TransferVec(1, 0, true);
					}
					PFunc.TransferVec(1, content_strID_vec.at(i), false);
				}
			}

			for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
				if (do_all == false) {
					if (m_it->first == ui_page_num) {
						out_path = m_it->second;
						PF.GetOutputData(out_path);
						
						QPFunc.COMPARE_COMPCODES(false); // data compare
						Log.LogRecord(Logs::Normal, Logs::General, "\n\t\t...processing changes for page %i", m_it->first);

						QPFunc.QML_PG_CHANGE();
						PF.FileReWrite(out_path, true);
						break;
					}
				}
				else {
					out_path = m_it->second;
					PF.GetOutputData(out_path);

					QPFunc.ArrangeCompIDsintoVec(m_it->first); // data arrangement - appears to work
					
					QPFunc.COMPARE_COMPCODES(false); // data compare
					Log.LogRecord(Logs::Normal, Logs::General, "\n\t\t...processing changes for page %i", m_it->first);

					QPFunc.QML_PG_CHANGE(); // this would work in many cases but we need a way to alter the file in a smaller way
					PF.FileReWrite(out_path, true);
				}
			}
		}		
		else if (user_choice == 2) {
			if (do_all == false) {
				Log.LogRecord(Logs::Normal, Logs::General, "  Rewriting .cpp array to match .qml page (%i) components", ui_page_num);
			}
			else {
				Log.LogRecord(Logs::Normal, Logs::General, "  Rewriting .cpp array to match all .qml page's components.");
			}

			++p_func_count; // must increase p_func_count and p_subdir_count to access proper PrepareFunction() data output
			++p_subdir_count;

			// p_subdir_count(0), begins the function count for the next portion of the directive the numeric notation will be based on user input 1 or 2
			PF.PrepareFunction(pd_type, 3, p_func_count, p_subdir_count);		
			out_path = parse_outpaths_vec.at(p_subdir_count); // any directory access after completion of a subirective must use "p_subdir_count"		

			QPFunc.COMPARE_COMPCODES(true);

			PF.GetOutputData(out_path);
			QPFunc.CPP_ARRAY_CHGCOMPCODES(ui_page_num);			
			PF.FileReWrite(out_path, true);
		}

		break;

	case 7: // sync or get content compIDs - this cannot and should not be used when changing pages' components, this would be used before/after

		ui_page_num = 0;
		user_switch = true; // write = true, get compIDs = false
		do_all = false;

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component IDs from file data");
		
		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			if (do_all == false) {
				if (m_it->first == ui_page_num) {
					in_path = m_it->second;

					PF.GetInputData(in_path);
					QPFunc.QML_PG_GETCOMPCODES();
				}
			}
			else {
				in_path = m_it->second;

				PF.GetInputData(in_path);
				QPFunc.QML_PG_GETCOMPCODES();
			}
		}
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		if (user_switch == true) {
			Log.LogRecord(Logs::Normal, Logs::General, "  Reordering component IDs in .qml files");
		}
		else {
			Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component IDs in .qml files");
		}

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			if (do_all == false) {
				if (m_it->first == ui_page_num) {
					out_path = m_it->second;
					PF.GetOutputData(out_path);

					// we need to try and rewrite the file from the saved map data otherwise - the map data is for another PD
					QPFunc.QML_PG_GETCOMPIDS(ui_page_num, user_switch, do_all);
					if (user_switch == true) {
						PF.FileReWrite(out_path, true);
					}
				}
			}
			else {
				out_path = m_it->second;
				PF.GetOutputData(out_path);

				QPFunc.QML_PG_GETCOMPIDS(m_it->first, user_switch, do_all);
				if (user_switch == true) {
					PF.FileReWrite(out_path, true);
				}
			}
		}

		break;

	case 8: // main.qml array/file sync/write - some of the PStr() string-arguments could be user input for SETUP, particularly, the background image

		user_choice = 1; // 1 = change_file 2 = change_array

		// F 20
		PF.PrepareFunction(pd_type, 1, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			in_path = in_dir + in_file;

			Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving main.qml component codes from file data");

			PF.GetInputData(in_path);
			QPFunc.QML_MAIN_GETCOMPCODES();
		}
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// F 18
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 17 - there is no need to prepare the function, it doesn't access any dir/files - we could simply increase the p_func_count after completion
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		
		if (user_choice == 1) { // for .qml change
			QPFunc.COMPARE_COMPCODES(false);
		}		
		else if (user_choice == 2) { // for array change
			QPFunc.COMPARE_COMPCODES(true);
		}
		++p_func_count;


		PF.DoDirectiveCleanup(false, false);
		++p_func_count; // to bypass subdirective notation in "parseFunc_from_parseDir"
		Log.LogRecord(Logs::Normal, Logs::General, "\n Resuming main Directive");
		PF.GetParsePaths(pd_type, p_func_count);


		//F 21 - for .qml change
		if (user_choice == 1) {
			Log.LogRecord(Logs::Normal, Logs::General, "  Synchronizing all .qml page's components to .cpp array.");
			PF.PrepareFunction(pd_type, 0, p_func_count, p_subdir_count);
			out_path = parse_outpaths_vec.at(p_subdir_count);

			Log.LogRecord(Logs::Normal, Logs::General, "\n\t\t...processing changes for main.qml");

			PF.GetOutputData(out_path);
			QPFunc.QML_MAIN_CHANGE();
			PF.FileReWrite(out_path, true);
		}
		//F 19 - for array change
		else if (user_choice == 2) {
			Log.LogRecord(Logs::Normal, Logs::General, "  Rewriting .cpp array to match all .qml page's components.");
			++p_func_count;
			++p_subdir_count;

			PF.PrepareFunction(pd_type, 3, p_func_count, p_subdir_count);
			out_path = parse_outpaths_vec.at(p_subdir_count); // any directory access after completion of a subirective must use "p_subdir_count"		

			PF.GetOutputData(out_path);
			QPFunc.CPP_ARRAY_CHGMAINCOMPCODES();
			PF.FileReWrite(out_path, true);
		}

		break;

	case 9: // component checking/insertion/deletion for contingent components		

		// F 18
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 22
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			in_path = in_dir + in_file;
			Log.LogRecord(Logs::Normal, Logs::General, "  Parsing contingent components");

			PF.GetInputData(in_path);

			QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(true, true, false);

			QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(true, false, false);
		}
		input_filename_vec.clear();
		++p_func_count;


		// F 8
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		--p_func_count;


		//F 22
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			in_path = in_dir + in_file;
			Log.LogRecord(Logs::Normal, Logs::General, "  Parsing contingent components");

			PF.GetInputData(in_path);

			QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(false, true, false);

			QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(false, false, false);

			QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(false, false, true);
		}
		input_filename_vec.clear();
		++p_func_count;
		++p_func_count;


		// F 12 - write to page array
		PF.PrepareFunction(pd_type, 2, p_func_count, p_func_count); // (2) doesn't follow the normal logic 
		out_path = parse_outpaths_vec.at(p_func_count);

		PF.GetOutputData(out_path);
		QPFunc.CPP_ARRAY_CHGCOMPCODES(ui_page_num);
		PF.FileReWrite(out_path, true);

		strID_vec.clear();
		p_func_count = 0;


		// F 18
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 22
		QPFunc.CPP_ARRAY_CONTINGENTOPERATIONS(true, false, true);
		p_func_count = 4;


		// F 19 - write to main array
		PF.PrepareFunction(pd_type, 2, p_func_count, p_func_count); // (2) doesn't follow the normal logic 
		out_path = parse_outpaths_vec.at(p_func_count);

		PF.GetOutputData(out_path);
		QPFunc.CPP_ARRAY_CHGMAINCOMPCODES();
		PF.FileReWrite(out_path, true);

		break;

	case 10: // parse into main.qml page file names for navigation and page file navigation contingents - all components should be populated/sync'd by this point

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 14		
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 23
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			out_path = in_dir + in_file;

			PF.GetOutputData(out_path);

			QPFunc.QML_MAIN_CONTINGENTS();
			PF.FileReWrite(out_path, true);
		}
		output_file_data_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// 230,231,232,233 
		// get compIDs for contingents within pages from main
		// F 26
		PF.PrepareFunction(pd_type, 1, p_func_count, p_func_count);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			in_path = in_dir + in_file;

			PF.GetInputData(in_path);
			QPFunc.GetCompIDFromMain();
		}
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// F 27
		PF.PrepareFunction(pd_type, 1, p_func_count, p_func_count);

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			out_path = m_it->second;
			if (out_path.length() > 0) {
				PF.GetOutputData(out_path);

				QPFunc.ParseIntoPages();
				PF.FileReWrite(out_path, true);
			}
		}

		break;

	case 11: // parse into all SupportComponents files

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 14
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 8
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;

		return;
		// ---------  this can likely be done a little different due to several areas of the same file that can be altered at the same time		
		// ---------  combine onNavigateSideways into this - they belong in InterpageProcesses
		// need to work on UI logic for onNavigateSideways() before adding it here
		// do InterpageProcesses operations - the point of this function is to get compIDs strings from each page and put into memory - maybe writing to the file may also be reasonable

		// F 24 Part I.1
		supp_op = 1;
		fbit_pos = STANDARD_BITS + supp_op;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		Log.LogRecord(Logs::High, Logs::General, "supp_op: %i support_bit: %i support_bit_val %i", supp_op, fbit_pos, PF.GetBitValue(fbit_pos - 1, false));
		++p_func_count;


		// F 25 variables - getoutput_data, write, out_dir(multi-specific), out_file(multi-specific or mult-nondescript), supp_op - not worth it
		// F 25 Part I.1
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = QTOutputDirectory[APP_OUTSUPPORT];
		out_file = QPFunc.GetSupportFile(1);
		out_path = out_dir + out_file;
		PF.GetOutputData(out_path);
		QPFunc.QML_SUPPORT_UPDATECONTINGENTS(1);
		strID_vec.clear();
		p_func_count = 3;


		// F 24 Part I.2
		supp_op = 8;
		fbit_pos = STANDARD_BITS + supp_op;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		++p_func_count;


		// F 25 Part I.2
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		QPFunc.QML_SUPPORT_UPDATECONTINGENTS(8);
		PF.FileReWrite(out_path, true); // output_file_data_vec.clear();
		strID_vec.clear();
		p_func_count = 3;


		// F 24 Part II - do WriteLineText operations
		supp_op = 2;
		fbit_pos = STANDARD_BITS + supp_op;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		++p_func_count;


		// F 25 Part II
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = QTOutputDirectory[APP_OUTSUPPORT];
		out_file = QPFunc.GetSupportFile(2);
		out_path = out_dir + out_file;
		PF.GetOutputData(out_path);
		QPFunc.QML_SUPPORT_UPDATECONTINGENTS(2);
		PF.FileReWrite(out_path, true); // output_file_data_vec.clear();
		strID_vec.clear();
		p_func_count = 3;


		// F 24 Part III - do KeyboardProp1 operations
		supp_op = 3;
		fbit_pos = STANDARD_BITS + supp_op;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		++p_func_count;


		// F 25 Part III
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = QTOutputDirectory[APP_OUTSUPPORT];
		out_file = QPFunc.GetSupportFile(3);
		out_path = out_dir + out_file;
		PF.GetOutputData(out_path);
		QPFunc.QML_SUPPORT_UPDATECONTINGENTS(3);
		PF.FileReWrite(out_path, true); // output_file_data_vec.clear();
		strID_vec.clear();
		p_func_count = 3;


		// F 24 Part IV - do SLText page operations - don't clear input_file_vec flag bit_pos 2
		supp_op = 4;
		fbit_pos = STANDARD_BITS + supp_op;
		fbitmask_value = PF.GetBitValue(fbit_pos - 1, false) + PF.GetBitValue(2 - 1, false);
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value);
		++p_func_count;


		// F 25 Part IV
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = QTOutputDirectory[APP_OUTPAGES];
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
			out_path = out_dir + in_file;
			PF.GetOutputData(out_path);
			QPFunc.QML_SUPPORT_UPDATECONTINGENTS(4);
			PF.FileReWrite(out_path, true); // output_file_data_vec.clear();			
		}
		strID_vec.clear();
		input_filename_vec.clear();
		p_func_count = 3;


		// ---------  this can likely be done a little different due to several areas of the same file that can be altered at the same time
		// F 24 Part V - do DDMenu/ACTMenu operations - don't clear input_file_vec flag bit_pos 2
		supp_op = 5;
		fbit_pos = STANDARD_BITS + supp_op;
		fbitmask_value = PF.GetBitValue(fbit_pos - 1, false) + PF.GetBitValue(2 - 1, false);
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value);
		++p_func_count;

		// F 25 Part V
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = QTOutputDirectory[APP_OUTPAGES];
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
			out_path = out_dir + in_file;
			PF.GetOutputData(out_path);

			QPFunc.QML_SUPPORT_UPDATECONTINGENTS(5);
			strID_vec.clear();

			QPFunc.PrepareSupportOperation(6, true, false, true);
			QPFunc.QML_SUPPORT_UPDATECONTINGENTS(6);
			strID_vec.clear();

			QPFunc.PrepareSupportOperation(7, true, false, true);
			QPFunc.QML_SUPPORT_UPDATECONTINGENTS(7);

			PF.FileReWrite(out_path, true); // output_file_data_vec.clear();
		}
		//strID_vec.clear();
		//transfer_string_vec.clear();
		//transfer_int_vec.clear();
		//p_func_count = 3;
		break;

	case 12: // check/create directories, create main .qml

		// F 15
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 1, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 18
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 28
		PF.PrepareFunction(pd_type, 2, p_func_count, p_func_count); // do not to overwrite an existing file - get all filenames with <in_filetype>					
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		out_path = parse_outpaths_vec.at(p_func_count);

		QPFunc.QML_CREATE_MAIN(out_dir);
		PF.FileOpen(out_path, CREATE);
		PF.FileReWrite(out_path, true);

		break;

	case 13: // check component file existence from array compIDs - no actual .qml files are required to exist

		// F 15
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 1, p_func_count, p_func_count, 0);
		++p_func_count;


		// F 18 - main
		fbit_pos = 3;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		++p_func_count;


		// F 16 - pop files
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (PF.GetInputFileCt(pd_type, p_func_count) > input_filename_vec.size()) {
			Log.LogRecord(Logs::Normal, Logs::Error, "    Input Files missing from directory. Cannot continue Directive:\n  %s", in_dir.c_str());
			end_parse = true;
		}
		// parse files in input_filename_vec for filenames
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			if (input_filename_vec.at(i) == QTInputFile[QML_FILENAMES]) {
				in_path = in_dir + input_filename_vec.at(i);

				PF.GetInputData(in_path);
				QPFunc.FILE_CHECK_DEPEND(true, out_dir);
			}
		}
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			if (input_filename_vec.at(i) == QTInputFile[QML_DEPENDANTS]) {
				in_path = in_dir + input_filename_vec.at(i);

				PF.GetInputData(in_path);
				QPFunc.FILE_CHECK_DEPEND(false, out_dir);
			}
		}
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// F 8 - pages
		fbit_pos = 3;
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, PF.GetBitValue(fbit_pos - 1, false));
		p_func_count = 2;


		// F 16 - pop files
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (PF.GetInputFileCt(pd_type, p_func_count) > input_filename_vec.size()) {
			Log.LogRecord(Logs::Normal, Logs::Error, "    Input Files missing from directory. Cannot continue Directive:\n  %s", in_dir.c_str());
			end_parse = true;
		}
		// parse files in input_filename_vec for filenames
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			if (input_filename_vec.at(i) == QTInputFile[QML_FILENAMES]) {
				in_path = in_dir + input_filename_vec.at(i);

				PF.GetInputData(in_path);
				QPFunc.FILE_CHECK_DEPEND(true, out_dir);
			}
		}
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			if (input_filename_vec.at(i) == QTInputFile[QML_DEPENDANTS]) {
				in_path = in_dir + input_filename_vec.at(i);

				PF.GetInputData(in_path);
				QPFunc.FILE_CHECK_DEPEND(false, out_dir);
			}
		}

		break;

	default:

		Log.LogRecord(Logs::Normal, Logs::Error, "Invalid Parse Directive: (%i)", parse_directive);

		break;

		PF.ShowDirectiveTypeData(pd_type);
		PF.GetOutDirs();
		PF.GetOutPath();
		PF.GetInDirs();
		PF.GetInFiles();

		Log.LogRecord(Logs::Normal, Logs::Test, "out_file: %s", out_file.c_str());
		Log.LogRecord(Logs::Normal, Logs::Test, "output_file_data_vec size: %i", output_file_data_vec.size());
		Log.LogRecord(Logs::Normal, Logs::Test, "input_file_data_vec size: %i", input_file_data_vec.size());

		for (unsigned int i = 0; i < strID_vec.size(); ++i) { std::cout << i << " strID_vec " << strID_vec.at(i) << std::endl; }		

		// get subdirective code or should it be assumed?
		QMLSubdirective(-2, p_func_count, p_func_count); //  --- // subdirective -2
		p_func_count = p_func_count + PF.GetSubdirectiveFunctionCt(pd_type, -2);	

	}

	// cleaup should be done in the Directive type, as well
	pageID_to_path_map.clear();
}

void QMLParse::QMLSubdirective(int parse_subdirective, int parse_function_count, int subdirective_count)
{
	ParseFiles PF;
	ParseDirectory PDir;
	ParseFunctions PFunc;

	QMLParseFunction QPFunc;

	std::map<int, std::string>::iterator m_it;

	std::string out_dir;
	std::string out_file;
	std::string out_path;

	std::string in_dir;
	std::string in_file;
	std::string in_path;

	std::string file_ext;

	int p_func_count = 0;
	int p_subdir_count = 0;
	int p_func_id = 0;
	int num_pages = 0;

	unsigned int bitmask_value = 0;
	int num_bits = 0;
	int bit_pos = 0;
	unsigned int fbitmask_value = 0;
	int fbit_pos = 0;

	bool switch1 = false;
	bool switch2 = false;

	Log.LogRecord(Logs::High, Logs::General, "\n Branching into Parse Subdirective: %i", parse_subdirective);

	// proposed user input or test variables
	int ui_page_num = -1; // if do_all = true, ui_page_num = -1
	bool do_all = true;

	// define where we're at in the Directive as a whole first
	p_func_count = parse_function_count;
	p_subdir_count = subdirective_count;

	switch (parse_subdirective) {

	case -1:

		// F 5
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;


		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component IDs from file data in memory");
		
		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			if (do_all == false) {
				if (m_it->first == ui_page_num) {
					in_path = m_it->second;

					if (in_path.length() > 0) {
						PF.GetInputData(in_path);
						QPFunc.QML_PG_GETCOMPCODES();
					}
					break;
				}
			}
			else {
				in_path = m_it->second;

				if (in_path.length() > 0) {
					PF.GetInputData(in_path);
					QPFunc.QML_PG_GETCOMPCODES();
				}
			}

			if (in_path.length() == 0) {
				end_parse = true;
				break;
			}
		}
		input_filename_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// reordering isn't necessary
		PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
		Log.LogRecord(Logs::Normal, Logs::General, "  Reordering component IDs in .qml files");
		Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component IDs in .qml files");

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			if (do_all == false) {
				if (m_it->first == ui_page_num) {
					out_path = m_it->second;

					PF.GetOutputData(out_path);
					QPFunc.QML_PG_GETCOMPIDS(ui_page_num, true, false);
					PF.FileReWrite(out_path, false);
					QPFunc.QML_PG_GETCOMPIDS(ui_page_num, false, false);
				}
			}
			else {
				out_path = m_it->second;

				PF.GetOutputData(out_path);
				QPFunc.QML_PG_GETCOMPIDS(m_it->first, true, true);
				PF.FileReWrite(out_path, false);
				QPFunc.QML_PG_GETCOMPIDS(m_it->first, false, true);
			}
		}
		transfer_int_vec.clear();
		strID_vec.clear();
		++p_func_count;


		// F 8
		p_func_id = PF.GetFunctionID(p_func_count);
		HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);
		++p_func_count;

		break;

	case -2: // 4 possiblities currently in this subdir - singlepage/all,  checkfile/check accuracy, 

		Log.LogRecord(Logs::Normal, Logs::General, "  Performing setup routine.");
		num_bits = 10;

		while (bitmask_value < 1023) {
			p_func_count = 0;

			// F 15 - create directories
			bit_pos = 1;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) { // flag 1 - bitmask_value = 0
				Log.LogRecord(Logs::Normal, Logs::General, "  Checking for application directories");

				p_func_id = PF.GetFunctionID(p_func_count);
				HandleFunctions(p_func_id, 1, p_func_count, p_func_count, 0);

				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 1
			}
			p_func_count = 1;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// F 15 - create main .qml if not exists
			bit_pos = 2;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) { // flag 2 - bitmask_value = 1

				PF.PrepareFunction(pd_type, 2, p_func_count, p_func_count);
				out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
				out_path = parse_outpaths_vec.at(p_func_count);

				PF.GetOutputData(out_path);

				if (output_file_data_vec.size() < 50) {
					output_file_data_vec.clear();

					Log.LogRecord(Logs::Normal, Logs::General, "  Creating main.qml component template.");
					QPFunc.QML_CREATE_MAIN(out_dir);
					PF.FileOpen(out_path, CREATE);
					PF.FileReWrite(out_path, true);
				}

				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 2
			}
			output_file_data_vec.clear();
			strID_vec.clear();
			p_func_count = 2;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);


			// F 8 - compIDs for file checking
			bit_pos = 3;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) { // flag 3 - bitmask_value = 3				
				fbit_pos = 3;
				fbitmask_value = PF.GetBitValue(fbit_pos - 1, false) + PF.GetBitValue((fbit_pos - 1) - 1, false); // don't clear data 2 and get unique component codes 3
				p_func_id = PF.GetFunctionID(p_func_count);

				if (HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value) == 0) {
					bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 4
					switch1 = true;
				}
				else {
					switch1 = false;
				}
			}

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// ----------------  something may not work here - solution below may work

			// we can't overwrite the "array_strID_vec" before we check for files, we need to be able to check the files and then come back to this
			// F 8 - compIDs for page writing and component checking
			if (bitmask_value > PF.GetBitValue(bit_pos, true)) {
				if (switch1 == true) { // file exists1 - flag 6 - bitmask_value = 31, 23 these will change
					fbit_pos = 1;
					fbitmask_value = PF.GetBitValue(fbit_pos - 1, false);
					p_func_id = PF.GetFunctionID(p_func_count);

					HandleFunctions(p_func_id, 0, p_func_count, p_func_count, fbitmask_value); // skip prepfunction 1					
					switch1 = false;
				}
			}
			p_func_count = 3;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// F 5
			bit_pos = 4;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) { // flag 4 - bitmask_value = 3, 7
				p_func_id = PF.GetFunctionID(p_func_count);

				if (HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0) == 0) {
					bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 8
					switch2 = false;
				}
				else {
					Log.LogRecord(Logs::Normal, Logs::General, "    No input files located.");
					switch2 = true;
				}
			}
			p_func_count = 4;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// F 14		
			bit_pos = 5;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false && switch2 == false) { //  file exists2 - flag 5 - bitmask_value = 15, 11				
				p_func_id = PF.GetFunctionID(p_func_count);

				HandleFunctions(p_func_id, 0, p_func_count, p_func_count, 0);

				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 16
			}
			p_func_count = 5;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			if (bitmask_value == 19) {
				Log.LogRecord(Logs::Normal, Logs::Error, "Setup cannot continue without array data and page files.\n  Aborting directive.");
				end_parse = true;
				break;
			}

			// F 16 - check component file existence from array compIDs - this does not populate files from comp codes on main.qml
			bit_pos = 6;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false && switch1 == true) { // file exists1 - flag 6 - bitmask_value = 31, 23				
				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
				out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
				in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

				if (PF.GetInputFileCt(pd_type, p_func_count) > input_filename_vec.size()) {
					Log.LogRecord(Logs::Normal, Logs::Error, "    Input Files missing from directory. Cannot continue Directive:\n  %s", in_dir.c_str());
					end_parse = true;
				}
				// parse files in input_filename_vec for filenames
				for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
					if (input_filename_vec.at(i) == QTInputFile[QML_FILENAMES]) {
						in_path = in_dir + input_filename_vec.at(i);

						PF.GetInputData(in_path);
						QPFunc.FILE_CHECK_DEPEND(true, out_dir);
					}
				}
				for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
					if (input_filename_vec.at(i) == QTInputFile[QML_DEPENDANTS]) {
						in_path = in_dir + input_filename_vec.at(i);

						PF.GetInputData(in_path);
						QPFunc.FILE_CHECK_DEPEND(false, out_dir);
					}
				}
				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 32
			}
			input_filename_vec.clear();
			strID_vec.clear();
			p_func_count = 6;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			if (PF.CheckBitFlags(num_bits, bitmask_value, 4) == false) { // bitmask_value = 23 -> 4 = the bit_pos corresponding to matching pages to files
				// create page files
				// continue -> go back to beginning of subdir loop to match page files for the next steps		
			}

			return;

			// F 10
			bit_pos = 7;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false && switch2 == false) { // file exists2 - flag 7 - bitmask_value = 63, 27
				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
				Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component codes from file data in memory");

				// prepare the page compIDs from file content
				for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
					if (do_all == false) {
						if (m_it->first == ui_page_num) {
							in_path = m_it->second;

							if (in_path.length() > 0) {
								PF.GetInputData(in_path);
								QPFunc.QML_PG_GETCOMPCODES();
							}
							break;
						}
					}
					else {
						in_path = m_it->second;

						if (in_path.length() > 0) {
							PF.GetInputData(in_path);
							QPFunc.QML_PG_GETCOMPCODES();
						}
					}
					if (in_path.length() == 0) {
						end_parse = true;
						break;
					}
				}
				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 64		
			}
			input_filename_vec.clear();
			strID_vec.clear();
			p_func_count = 7;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			if (PF.CheckBitFlags(num_bits, bitmask_value, 3) == false) { // bitmask_value = 27 -> 3 = the bit_pos corresponding to retrieving array pg compIDs
				// rewrite array
				// continue -> go back to beginning of subdir loop to retrieve array pg compIDs for the next steps		
			}

			// go back to beginning of subdir loop to retrieve all array pg compIDs for the next steps
			if (switch1 == true) {
				continue;
			}

			// F 17
			// F 11,12 alter the page, array respectively - this should be a choice or only notification
			bit_pos = 8;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) {
				Log.LogRecord(Logs::Normal, Logs::General, "  Comparing component codes between array and content data");

				// need to loop over files from pagematch map

				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);

				QPFunc.COMPARE_COMPCODES(false);

				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 128
			}
			p_func_count = 8;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// the result of function above will ask to rewrite array or page files
			// after this clear array/content_strID_vecs - or not, I think we need the data below although it may not be ordered properly

			// not needed for setup unless ordering the ids will be part of it
			// F 13 - function also incrementally orders the ids
			bit_pos = 9;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) {
				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
				Log.LogRecord(Logs::Normal, Logs::General, "Reordering component IDs in .qml files");
				Log.LogRecord(Logs::Normal, Logs::General, "Retrieving component IDs in .qml files");

				for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
					if (do_all == false) {
						if (m_it->first == ui_page_num) {
							out_path = m_it->second;

							PF.GetOutputData(out_path);
							QPFunc.QML_PG_GETCOMPIDS(ui_page_num, true, false);
							PF.FileReWrite(out_path, false);
							QPFunc.QML_PG_GETCOMPIDS(ui_page_num, false, false);
						}
					}
					else {
						out_path = m_it->second;

						PF.GetOutputData(out_path);
						QPFunc.QML_PG_GETCOMPIDS(m_it->first, true, true);
						PF.FileReWrite(out_path, false);
						QPFunc.QML_PG_GETCOMPIDS(m_it->first, false, true);
					}
				}
			}
			strID_vec.clear();
			p_func_count = 9;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			// F 6
			bit_pos = 9;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false) {
				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
				in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

				Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving .qml logic IDs from source file");

				if (input_filename_vec.size() < 1) {
					end_parse = true;
				}
				for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
					in_file = input_filename_vec.at(i);
				}
				in_path = in_dir + in_file;

				PF.GetInputData(in_path);
				QPFunc.CPP_ARRAY_GETLOGICIDS();

				if (process_required == false) {
					end_parse = true;
					Log.LogRecord(Logs::Normal, Logs::Error, "    Logic IDs missing from source array. Cannot check .qml logic.");
				}
				else {
					bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 256
				}
			}
			input_filename_vec.clear();
			strID_vec.clear();
			p_func_count = 10;

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			bit_pos = 10;
			if (PF.CheckBitFlags(num_bits, bitmask_value, bit_pos) == false && PF.GetBitValue(bit_pos - 2, true) <= bitmask_value) {
				PF.PrepareFunction(pd_type, 0, p_func_count, p_func_count);
				Log.LogRecord(Logs::Normal, Logs::General, "  Updating .qml logic IDs in all page files");
				for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
					out_path = m_it->second;

					if (out_path.length() > 0) {
						PF.GetOutputData(out_path);
						QPFunc.QML_PG_UPDATELOGICIDS(m_it->first);
						PF.FileReWrite(out_path, true);
					}
				}
				Log.LogRecord(Logs::High, Logs::General, "\n    All .qml logic IDs updated in:\n\t", out_dir.c_str());
				bitmask_value = bitmask_value + PF.GetBitValue(bit_pos - 1, false); // + 512
			}

			Log.LogRecord(Logs::High, Logs::General, "\tp_func_count:bit_pos:bitmask_value  %i:%i:%i", p_func_count, bit_pos, bitmask_value);

			if (end_parse == true) {
				Log.LogRecord(Logs::Normal, Logs::Error, "  Parse incomplete, invalid data. Unable to continue.\n    bitmask code:  %i", bitmask_value);
				break;
			}
		}

		break;
	default:

		PF.ShowDirectiveTypeData(pd_type);
		PF.GetOutDirs();
		PF.GetOutPath();
		PF.GetInDirs();
		PF.GetInFiles();

		Log.LogRecord(Logs::Normal, Logs::General, "out_file: %s", out_file);

		Log.LogRecord(Logs::Normal, Logs::Error, "Invalid Parse Subdirective: (%i)", parse_subdirective);
		break;
	}

}

int QMLParse::HandleFunctions(int function_id, int str_transfer_status, int function_count, int subfunction_count, int bit_value)
{
	/*
	bit_pos static flags:
	0 = do nothing
	1 = skip PrepareFunction()
	2 = skip clearing data containers
	3+ = variable, dependant upon the function
	return_status:
	0 = do nothing, everything is fine

	2 = input_file_vec size = 0

	*/
	ParseFiles PF;
	ParseDirectory PDir;
	ParseFunctions PFunc;

	QMLParseFunction QPFunc;

	std::map<int, std::string>::iterator m_it;

	std::string out_dir;
	std::string in_dir;
	std::string file_ext;

	std::string out_file;
	std::string out_path;
	std::string in_file;
	std::string in_path;

	int num_pages = 0;
	int return_status = 0;
	int num_bits = 0;
	int adj_bit_value = 0;
	int fbit_pos = 0;
	int supp_op = 0;

	// create a linked list of user input to access here
	// proposed user input or test variables
	int ui_page_num = -1; // user input page number for all pages this must be set to -1, currently
	int user_choice = 0; // user input for test change .cpp array or .qml page file 1 = change_file 2 = change_array
	std::string user_file; // user input for file naming
	bool user_switch = false; // user input for(2): change/sync or get componentIDs - reordering pageIDs
	bool do_all = false;

	// use bitmasks to alter specific function variables and pass the value to this function
	num_bits = 3;
	if (PF.CheckBitFlags(num_bits, bit_value, 1) == false) {
		PF.PrepareFunction(pd_type, str_transfer_status, function_count, subfunction_count);
		Log.LogRecord(Logs::Normal, Logs::General, "  Prepare Function directories, files and stringIDs");
	}
	else {
		Log.LogRecord(Logs::Normal, Logs::General, "  Skipping Prepare Function operations");
	}

	switch (function_id) {
	case CPP_CODE_DCH:
		break;
	case CPP_CODE_DCSRC:
		break;
	case QML_CREATE_PAGE:
		break;
	case QML_ALTER_NEWPAGE:
		break;
	case FILE_MATCH_PG:
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);
		file_ext = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 3);

		PDir.GetInputFiles(in_dir, file_ext);
		Log.LogRecord(Logs::Normal, Logs::General, "  Matching page IDs to filenames");
		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
			in_path = in_dir + in_file;

			PF.GetInputData(in_path);
			QPFunc.FILE_MATCH_PG(in_path);
		}
		if (end_parse == true) {
			break;
		}
		if (input_filename_vec.size() < 1) {
			return_status = 2;
		}
		input_filename_vec.clear();
		strID_vec.clear();
		break;
	case CPP_ARRAY_GETLOGICIDS:
		break;
	case QML_PG_UPDATELOGICIDS:
		break;
	case CPP_ARRAY_GETCOMPCODES:
		fbit_pos = 3;

		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
			process_required = true;
		}
		in_path = in_dir + in_file;
		PF.GetInputData(in_path);

		if (PF.CheckBitFlags(num_bits, bit_value, fbit_pos) == false) {
			Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving component IDs from array data");
			QPFunc.CPP_ARRAY_GETCOMPCODES(false, ui_page_num);
		}
		else {
			Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving unique component codes from array data");
			QPFunc.CPP_ARRAY_GETCOMPCODES(true, ui_page_num);
		}

		if (process_required == false) {
			Log.LogRecord(Logs::Normal, Logs::Error, "  Unable to retrieve array data. Aborting directive.");
			end_parse = true;
		}
		if (input_filename_vec.size() < 1 || array_strID_vec.size() < 1) {
			return_status = 2;
		}

		if (PF.CheckBitFlags(num_bits, bit_value, 2) == false) {
			strID_vec.clear();
			input_filename_vec.clear();
		}
		break;
	case QML_PG_CREATE:
		break;
	case QML_PG_GETCOMPCODES:
		break;
	case QML_PG_CHANGE:
		break;
	case CPP_ARRAY_CHGCOMPCODES:
		break;
	case QML_PG_GETCOMPIDS:
		break;
	case QML_PG_ORDER:
		fbit_pos = 3;

		Log.LogRecord(Logs::Normal, Logs::General, "  Reordering page IDs in .qml files");

		for (m_it = pageID_to_path_map.begin(); m_it != pageID_to_path_map.end(); ++m_it) {
			out_path = m_it->second;

			if (out_path.length() > 0) {
				PF.GetOutputData(out_path);

				if (PF.CheckBitFlags(num_bits, bit_value, fbit_pos) == false) {
					QPFunc.QML_PG_ORDER(true, ui_page_num, num_pages);
					++num_pages;
				}
				else {
					QPFunc.QML_PG_ORDER(false, ui_page_num, num_pages);
				}
				PF.FileReWrite(out_path, true);
			}
		}

		if (PF.CheckBitFlags(num_bits, bit_value, 2) == false) {
			strID_vec.clear();
		}
		num_pages = 0;
		break;
	case APP_SETUP:
		out_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 1);
		QPFunc.APP_SETUP(out_dir);
		strID_vec.clear();
		break;
	case FILE_CHECK_DEPEND:
		break;
	case COMPARE_COMPCODES:
		break;
	case CPP_ARRAY_GETMAINCOMPCODES:
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		if (input_filename_vec.size() > 0) {
			for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
				in_file = input_filename_vec.at(i);
			}
			in_path = in_dir + in_file;
			Log.LogRecord(Logs::Normal, Logs::General, "  Retrieving main.qml component codes from array data");

			PF.GetInputData(in_path);

			if (PF.CheckBitFlags(num_bits, bit_value, fbit_pos) == false) {
				QPFunc.CPP_ARRAY_GETMAINCOMPCODES(false);
			}
			else {
				QPFunc.CPP_ARRAY_GETMAINCOMPCODES(true);
			}
		}
		else {
			return_status = 2;
		}
		input_filename_vec.clear();
		strID_vec.clear();
		break;
	case CPP_ARRAY_CHGMAINCOMPCODES:
		break;
	case QML_MAIN_GETCOMPCODES:
		break;
	case QML_MAIN_CHANGE:
		break;
	case CPP_ARRAY_CONTINGENTOPERATIONS:
		break;
	case QML_MAIN_CONTINGENTS:
		break;
	case QML_SUPPORT_GETCONTINGENTDATA:
		num_bits = 10;

		if (PF.CheckBitFlags(num_bits, bit_value, 2) == true) {
			adj_bit_value = PF.GetBitValue((2 - 1), false);
			fbit_pos = PF.GetBitPos(10, bit_value - adj_bit_value);
		}
		else {
			fbit_pos = PF.GetBitPos(10, bit_value);
		}
		supp_op = fbit_pos - STANDARD_BITS;
		Log.LogRecord(Logs::High, Logs::General, "fbit_pos: %i support_op: %i ", fbit_pos, supp_op);

		QPFunc.PrepareSupportOperation(supp_op, true, true, false);
		in_dir = PF.GetElementFromPrepFuncList(PF.GetFuncIDFromList(0), 2);

		for (unsigned int i = 0; i < input_filename_vec.size(); ++i) {
			in_file = input_filename_vec.at(i);
			in_path = in_dir + in_file;

			PF.GetInputData(in_path);
			QPFunc.QML_SUPPORT_GETCONTINGENTDATA(supp_op);
			input_file_data_vec.clear();
		}

		if (PF.CheckBitFlags(num_bits, bit_value, 2) == false) {
			input_filename_vec.clear();
		}
		strID_vec.clear();
		break;
	case QML_SUPPORT_UPDATECONTINGENTS:
		break;
	case GetCompIDFromMain:
		break;
	case ParseIntoPages:
		break;
	case QML_CREATE_MAIN:
		break;
	default:
		break;
	}
	return return_status;
}
