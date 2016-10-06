#ifndef QMLPARSE_H
#define QMLPARSE_H


#define QMLID_START 200			// corresponds to the low range of search strIDs
#define QMLID_END 299			// corresponds to the high range of search strIDs
#define STANDARD_BITS 2			// used for HandleFunction() for constant bit_pos


enum QTOutDir {

	Nada = 0,
	APP_OUTROOT,
	APP_OUTPAGES,
	APP_OUTSUPPORT,
	MaxOutputDir

};

static const char* QTOutputDirectory[QTOutDir::MaxOutputDir] = {

	"",
	"C:/Users/Dude/Documents/Test programs/QtTest/TestParsedApp/",
	"C:/Users/Dude/Documents/Test programs/QtTest/TestParsedApp/Pages/",
	"C:/Users/Dude/Documents/Test programs/QtTest/TestParsedApp/SupportComponents/",

};

enum QTOutFile {

	Zilch = 0,
	DATACODES_H,
	DATACODES_CPP,
	UIPROCESS_CPP,
	OUT_MAIN_QML,
	OUT_INTERPAGEPROCESSES,
	OUT_KEYBOARDPROP1,
	OUT_WRITELINETEXT,
	MaxOutputFile

};

static const char* QTOutputFile[QTOutFile::MaxOutputFile] = {

	"",
	"datacodes.h",
	"datacodes.cpp",
	"uiprocess.cpp",
	"main.qml",
	"InterpageProcesses.qml",
	"KeyboardProp1.qml",
	"WriteLineText.qml",

};

enum QTInDir {

	NothNess = 0,
	APP_DATALISTS,
	APP_INPAGES,
	APP_INROOT,
	QML_FILES,
	MaxQMLInDir

};

static const char* QTInputDirectory[QTInDir::MaxQMLInDir] = {

	"",
	"C:/MyProgram/DataFiles/Lists/",
	"C:/Users/Dude/Documents/Test programs/QtTest/TestParsedApp/Pages/",
	"C:/Users/Dude/Documents/Test programs/QtTest/TestParsedApp/",
	"C:/Users/Dude/Documents/Test programs/QtTest/Qml_files/",

};

enum QTInFile {

	Dull = 0,
	APP_UIPROCESS,
	QML_FILENAMES,
	QML_DEPENDANTS,
	QML_CONTINGENTS,
	IN_MAIN_QML,
	MaxQMLInFile

};

static const char* QTInputFile[QTInFile::MaxQMLInFile] = {

	"",
	"uiprocess.cpp",
	"qml_filenames.txt",
	"qml_dependants.txt",
	"qml_contingents.txt",
	"main.qml",

};

enum QMLStrIDSubsets {

	NOTHING = 0,
	MAINQMLNEW,					// main.qml template
	SPEC_COMPCODE_MAIN,			// main.qml special case parse strings - these are in a specific order
	PAGEQMLNEW,					// new page .qml template
	COMPNOID,					// comp codes with no comp id - better option may be to check those WITH an ID
	CONTCODE_WLT,				// contingent component codes which use WriteLineText qml functions
	CONTCODE_KP,				// contingent component codes which use KeyboardProp1 qml functions
	CONTCODE_PGSLT,				// contingent component codes for SLText qml page functions
	CONTCODE_PGMENU,			// contingent component codes for DDMenu/ACTMenu qml page functions
	CONTCODE_PGFIELD,			// contingent component codes for all components which qualify as a field (only 4)
	CONTCODE_NAV,				// contingent component codes for main.qml components which // make sthis more efficient with what exists and below
	CONTCODE_PGNAV,				// contingent component codes for page .qmls components
	MaxSubsets

};

static const char * QMLSubsets[QMLStrIDSubsets::MaxSubsets] = {

	"",
	"{ 160, 161, 162, 163, 164, 165, 166, 167, 168 }",
	"{ 262, 274, 268 }",
	"{ 100, 101, 102, 103 }",
	"{ 100, 101, 102, 103, 115, 119, 120, 121, 122, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 150, 153, 154, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 180, 181, 182, 183, 184 }",
	"{ 112, 114 }",
	"{ 115 }",
	"{ 112 }",
	"{ 113, 114 }",
	"{ 112, 113, 114, 116 }",
	"{ 270, 269 }",
	"{ 230, 231, 232, 233 }",

};

// remove F 17 from these arrays, F 24 atm also - anything which doesn't require a file or strID_vec find strings does not need to be included here and can be classified differently
static const int QMLfunction_dirFiles[5][29] = { // equivalent LOCAL_PARSE = dirFile_from_parseFunc
	//							parse_function:
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28

	{ 0, 1, 1, 2, 2, 0, 0, 2, 0, 2, 0, 2, 1, 2, 2, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 2, 1 },
	{ 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 4, 0, 4, 0, 0, 0, 0, 4 },
	{ 0, 1, 1, 0, 0, 2, 3, 0, 3, 0, 2, 0, 0, 0, 0, 0, 4, 0, 3, 0, 3, 0, 4, 3, 2, 0, 3, 0, 0 },
	{ 0, 2, 2, 3, 0, 3, 4, 0, 4, 3, 3, 0, 0, 0, 0, 0, 2, 0, 4, 0, 3, 0, 2, 3, 0, 0, 4, 0, 0 },	// extensions - No_type = 0,ALL_TYPES,TEXT,QML,CPP,HEADER,MaxFileType
	{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0 },

};

static const int QMLfunction_inputFiles[3][29] = { // equivalent LOCAL_PARSE = inputFile_from_parseFunc
	//							parse_function:
	//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28

	{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 5, 0, 4, 5, 0, 0, 5, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 5, 0, 4, 5, 0, 0, 5, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static const int QMLfunction_stringIDs[3][29] = { // equivalent LOCAL_PARSE = stringIDs_from_parseFunc
	//									parse_function:
	//0  1   2   3   4   5   6   7   8  9  10 11 12  13  14  15  16 17  18 19  20 21 22  23  24 25   26   27 28

	{ 0, 1,  7,  0, 12, 12, 13, 16, 24, 0, 12, 0, 0, 26, 12, 28, 28, 0, 25, 0,  0, 0, 0, 33, 37, 0,   0,   0, 0 },
	{ 0, 6, 11,  0, 12, 12, 15, 23, 24, 0, 12, 0, 0, 27, 12, 32, 30, 0, 25, 0,  0, 0, 0, 36, 37, 0,   0,   0, 0 },
	{ 0, 0,  0, -3,  0,  0,  0,  0,  0, 0,  0, 0, 0,  0,  0,  0,  0, 0,  0, 0, -2, 0, 0,  0,  0, 0, -10, -11, 0 },
};

static const int QMLdirective_functions[5][14] = { // equivalent LOCAL_PARSE = parseFunc_from_parseDir
	//			parse_directive:
	//0  1   2  3  4  5   6   7   8   9  10  11  12  13

	{ 0, 1,  5, 5, 5, 8, -1,  5, -3, 18,  5,  5, 15, 15 },
	{ 0, 2, 14, 4, 6, 9, 11, 10, 21, 22, 14, 14, 18, 18 },
	{ 0, 0,  3, 0, 7, 0, 12, 13, 19,  8, 23,  8, 28, 16 },
	{ 0, 0,  0, 0, 0, 0,  0,  0,  0, 12, 26, 24,  0,  8 },
	{ 0, 0,  0, 0, 0, 0,  0,  0,  0, 19, 27, 25,  0,  0 },
};

static const int QMLsubDir_functions[11][4] = { // equivalent LOCAL_PARSE = parseFunc_from_parseSubDir
	//parse_subdirective:
	//0  -1	 -2  -3

	{ 0,  5, 15, 20 },
	{ 0, 10, 28, 18 },
	{ 0, 13,  8, 17 },
	{ 0,  8,  5,  0 },
	{ 0,  0, 14,  0 },
	{ 0,  0, 16,  0 },
	{ 0,  0, 10,  0 },
	{ 0,  0, 17,  0 },
	{ 0,  0, 13,  0 },
	{ 0,  0,  6,  0 },
	{ 0,  0,  7,  0 },

};


class QMLParse
{
public:
	QMLParse();

	void QMLMainParse(int parse_directive);
	void QMLSubdirective(int parse_subdirective, int parse_function_count, int subdirective_count);
	int HandleFunctions(int function_id, int str_transfer_status, int function_count, int subfunction_count, int bit_value);

};

#endif // QMLPARSE_H
