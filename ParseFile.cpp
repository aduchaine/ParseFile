// ParseFile.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>

#include <iostream>

#include "parsefiles.h"
#include "parsedirectory.h"

#include "Logging\logging.h"


Logging Log;


int _tmain(int argc, _TCHAR* argv[])
{
	std::string t;
	std::cout << "\n\t\t Press enter." << std::endl;
	std::getline(std::cin, t);	
	
	ParseFiles PF;
	ParseDirectory PDir;

	Log.DefaultLogSettings();
	Log.CheckLogFiles();
	
	// POSSIBLE_USER_INPUT:  depends on directive -> a menu would be available for out_dir, in_dir where necessary	-  the <transfer_status> in getstringIDs will eventually be passed via user input
	int directive_type = 1;			// 1 = QT parses
	int parse_directive = 11;		// 2nd choice - // 12, 5, 13, 4, 10, 11 - complete create
	
	// these choices will be prompted as necessary by the choices above
	int ui_page_num = -1;			// -1 = all pages (default)
	int user_choice = 0;			// change .cpp array or .qml page file (could be a switch)
	std::string user_file;			// file naming	
	bool user_switch = false;		// 2 possibilities: change/sync or get componentIDs - reordering pageIDs
	bool do_all = false;			// process all switch available based on PD - if all ui_page_num = -1 or default to -1 and don't set

	PF.PrepareDirectiveType(directive_type);
	PDir.LoadData(directive_type);
	PF.PrepParseDirective(directive_type, parse_directive);
	PF.MainParseFunction(directive_type, parse_directive);
	
//	parse_directive = 5;
//	PF.PrepParseDirective(directive_type, parse_directive);
//	PF.MainParseFunction(directive_type, parse_directive);
//	
//	parse_directive = 13;
//	PF.PrepParseDirective(directive_type, parse_directive);
//	PF.MainParseFunction(directive_type, parse_directive);
//
//	parse_directive = 4;
//	PF.PrepParseDirective(directive_type, parse_directive);
//	PF.MainParseFunction(directive_type, parse_directive);
//
//	parse_directive = 10;
//	PF.PrepParseDirective(directive_type, parse_directive);
//	PF.MainParseFunction(directive_type, parse_directive);
//
//	parse_directive = 11;
//	PF.PrepParseDirective(directive_type, parse_directive);
//	PF.MainParseFunction(directive_type, parse_directive);

	std::cout << "\n\t\t Press enter." << std::endl;
	std::getline(std::cin, t);
	return 0;
}

// ToDo/Notes
/*
look into this -> <field_comp_iter = fieldID_to_num_responses_vec.at(page_field_iter);> throws error when there are files with the same pageID or more files than are in the array

MakeNewFilename() may be what is holding up the process as the filesuffix (000) increases
should either make user choose a new filename or allow program to do the naming
if pageID exists, all >= will be increased by 1 (page insertion) - need a function for this yet

--- this would require access to functions in a different class
change F7 to do the file looping within the function - there is no current reason to change logicIDs if not checking them all because they are all interrelated
the files should not be rewritten if there were no changes
change F9 to do the file looping within the function, if possible
change F10 to do the file looping within the function, if possible
*/
