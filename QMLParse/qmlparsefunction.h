#ifndef QMLPARSEFUNCTION_H
#define QMLPARSEFUNCTION_H


#include <map>
#include <list>


#define SUPOP_YDIM 5			// artificial limit of QMLsupport_operation lines - this is mainly for convenience


// currently, there will only be one item in memory called after PrepareFunction() is called - this is used to quickly/easily get a single element
struct SupportOperation_struct
{
	int support_operation;		// basically the key or ID - operation ID
	int qml_subsetid;			// code for comp codes to search for
	int file_strid1;			// string code for strings to find
	int file_strid2;			// if not 0, string code for additional strings to find
	int insert_strid;			// string code for strings to insert, will generally correspond with operation ID
	int mult_instances;			// flag for mult statements per case
};

static const int QMLsupport_operation[5][12] = { // support_operation = 9 is not implemented, see PD description
	//		  support_operation:
	//0  1  2  3  4  5  6  7  8  9 10 11

	{ 0, 5, 5, 6, 7, 8, 8, 8, 8, 9, 0, 0 },		// QMLSubsets ID - used in SupportContingents() - code for specific comp codes to use to get find strings
	{ 0, 1, 2, 2, 3, 1, 1, 1, 1, 1, 0, 0 },		// find string ID 1 - used in QML_SUPPORT_GETCONTINGENTDATA() - 1 = multiple IDs to search
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },		// find string ID 2 - used in QML_SUPPORT_GETCONTINGENTDATA()
	{ 0, 1, 2, 2, 3, 4, 5, 6, 7, 0, 0, 0 },		// insert string ID - used in InsertCompIDCase()
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },		// mult elements - flag to denote mult objects within case acceptable = 1
};

enum FunctionIDs {

	CPP_CODE_DCH = 1,
	CPP_CODE_DCSRC,
	QML_CREATE_PAGE,
	QML_ALTER_NEWPAGE,
	FILE_MATCH_PG,
	CPP_ARRAY_GETLOGICIDS,
	QML_PG_UPDATELOGICIDS,
	CPP_ARRAY_GETCOMPCODES,
	QML_PG_CREATE,
	QML_PG_GETCOMPCODES,
	QML_PG_CHANGE,
	CPP_ARRAY_CHGCOMPCODES,
	QML_PG_GETCOMPIDS,
	QML_PG_ORDER,
	APP_SETUP,
	FILE_CHECK_DEPEND,
	COMPARE_COMPCODES,
	CPP_ARRAY_GETMAINCOMPCODES,
	CPP_ARRAY_CHGMAINCOMPCODES,
	QML_MAIN_GETCOMPCODES,
	QML_MAIN_CHANGE,
	CPP_ARRAY_CONTINGENTOPERATIONS,
	QML_MAIN_CONTINGENTS,
	QML_SUPPORT_GETCONTINGENTDATA,
	QML_SUPPORT_UPDATECONTINGENTS,
	GetCompIDFromMain,
	ParseIntoPages,
	QML_CREATE_MAIN,

};

class QMLParseFunction
{
public:
	QMLParseFunction();

	void CPP_CODE_DCH(std::string extension); // 1
	void CPP_CODE_DCSRC(); // 2
	void QML_CREATE_PAGE(std::string filename, int page_number = -1); // 3											F3: 1 bit	- all/single
	void QML_ALTER_NEWPAGE(); // 4
	void FILE_MATCH_PG(std::string tar_directory); // 5 - excellent support function
	void CPP_ARRAY_GETLOGICIDS(); // 6
	void QML_PG_UPDATELOGICIDS(int page_number); // 7
	void CPP_ARRAY_GETCOMPCODES(bool unique, int page_number); // 8																			F8: 2 bits	- all/single, check files/check file accuracy
	void QML_PG_CREATE(std::string filename, std::string extension, int page_count); // 9
	void QML_PG_GETCOMPCODES(); // 10
	void QML_PG_CHANGE(); // 11 - alters .qml content from array																			F11: 1 bit	- all/single
	void CPP_ARRAY_CHGCOMPCODES(int page_number); // 12 - alters .cpp array from page contents												F12: 1 bit	- all/single
	void QML_PG_GETCOMPIDS(int page_id, bool change_ids, bool process_all); // 13															F13: 2 bits	- all/single, write/retieve ids
	void QML_PG_ORDER(bool reorder, int new_page_id, int cur_page_ct); // 14			fix to not write if no change						F14: 1 bit	- reorder/create single-unit gap
	void APP_SETUP(std::string tar_directory); // 15
	void FILE_CHECK_DEPEND(bool get_filenames, std::string tar_directory); // 16
	void COMPARE_COMPCODES(bool use_file_data); // 17  // WIP unfinished? - this should be a util function - no dir/files/strIDs

	void CPP_ARRAY_GETMAINCOMPCODES(bool unique); // 18
	void CPP_ARRAY_CHGMAINCOMPCODES(); // 19
	void QML_MAIN_GETCOMPCODES(); // 20
	void QML_MAIN_CHANGE(); // 21
	void CPP_ARRAY_CONTINGENTOPERATIONS(bool search_main_qml, bool main_qml_contingents, bool verify); // 22 - checks array for existence of contingent components based on existing components
	void QML_MAIN_CONTINGENTS(); // 23 - adds to switch statement governing page navigation

	void QML_SUPPORT_GETCONTINGENTDATA(int support_operation); // 24 
	void QML_SUPPORT_UPDATECONTINGENTS(int support_operation); // 25 - rename

	void GetCompIDFromMain(); // 26
	void ParseIntoPages(); // 27

	void QML_CREATE_MAIN(std::string tar_directory); // 28

	// .qml util
	std::string GetSupportFile(int support_operation);
	void InsertCompIDCase(int support_op, int case_value, unsigned int line_offset, int indent);
	void SupportContingents(int subset_compcodes);
	void AppendQMLComponents(std::string filename, int page_number, int component, int compID_iter);						// used for creating new pages
	void MapQMLCompIDtoStrID(int id_range, bool clear);																		// useful to bypass matching a search string to a component string constantly when inserting/replacing components
	void ArrangeCompIDsintoVec(int page_number);																			// arranges compIDs into vectors for accurate comparison - used only for TESTAPP_QML_PG_CHANGE()
	void CompareCompIDProcessSwitch(bool diff_found, bool size_diff, bool use_file_data, int size_variance, int diff_ct);	// notifies and changes process switch accordingly
	bool CompHasNoID(int component_id);																						// some components don't get unique ids, this checks if the component gets one
	std::string QPStr(int str_id, std::string arg1 = "", std::string arg2 = "");

	// .qml support operations
	void PrepareSupportOperation(int support_opid, bool clear_op_codes, bool clear_op_data, bool skip_support_filepop);
	void AddToSupportOPList(int support_op, bool clear); // clears previous
	int GetElementFromSupportOPList(int support_op, int element);	
	std::list<SupportOperation_struct> support_oplist;

};

// function 5
extern std::map<int, std::string> pageID_to_path_map;

#endif // QMLPARSEFUNCTION_H
