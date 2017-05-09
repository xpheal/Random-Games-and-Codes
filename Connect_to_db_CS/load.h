#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "sqlite3.h"

using namespace std;

// return a vector of tokens separated by '^', '~' is ignored
vector<string> get_token(string line);

/* 
	Return a sql code for creating table
	table_name = name of the table
	num_col = number of columns in the table
	col_name = name of columns
	type = array of types
	size_1 = array of parameter 1 for type, 0 if not exist
	size_2 = array of parameter 2 for type, 0 if not exist
	is_primary_key = array of boolean for whether is it the primary key, 1 = true, 0 = false
	not_null = array of boolean for whether not_null is set, 1 = not_null, 0 = can be null
	extra_param = sql query that can't be generalized easily, overrides the query for the column
*/

string create_table(const string table_name, const int num_col, const string col_name[], const string type[], const int size_1[], const int size_2[], const int is_primary_key[], const int not_null[], const string extra_param);

// Return a sql query insert string for loading the data from file <filename> into table <table_name>
// num_col is the number of columns of the data and col_type[] is the type of data, either "char" or "numeric" for now
string insert_data_from_file(string filename, string table_name, const int num_col, const string col_type[]);

// Generate a line of insert code for the table <table_name> with <num_col> columns
// Each column has its type <col_type[]> and its value <tokens>, currently col_type accepts only "CHAR" and "NUMERIC"
string generate_insert_code(string table_name, const int num_col, const string col_type[], vector<string> tokens);

// Convert the token into the correct format for a sql query
// For now, <type> supports "CHAR" and "NUMERIC"
string convert_token(string token, string type);

// Execute the sql query
void execute_query(string query, sqlite3 *db, string message);

// struct table_data{
// 	int num_col;
// 	string col_name[];
// 	string type[];
// 	int size_1[];
// 	int size_2[];
// 	int is_primary_key[];
// 	int not_null[];
// };


// FOOD DES
const static string food_des_table_name = "FOOD_DES";
const static int food_des_num_col = 14;
const static string food_des_col_name[food_des_num_col] = {"NDB_No","FdGrp_Cd","Long_Desc","Shrt_Desc","ComName","ManufacName","Survey","Ref_desc","Refuse","SciName","N_Factor","Pro_Factor","Fat_Factor","CHO_Factor"};
const static string food_des_col_type[food_des_num_col] = {"CHAR", "CHAR", "CHAR", "CHAR", "CHAR", "CHAR", "CHAR", "CHAR", "NUMERIC", "CHAR", "NUMERIC", "NUMERIC", "NUMERIC", "NUMERIC"};
const static int food_des_size_1[food_des_num_col] = {5, 4, 200, 60, 100, 65, 1, 135, 2, 65, 4, 4, 4, 4};
const static int food_des_size_2[food_des_num_col] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2};
const static int food_des_is_primary_key[food_des_num_col] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const static int food_des_not_null[food_des_num_col] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const static string food_des_extra_param= {", FOREIGN KEY(FdGrp_Cd) REFERENCES FD_GROUP(FdGrp_Cd), FOREIGN KEY(NDB_No) REFERENCES NUT_DATA(NDB_No)"};

// FOOD GROUP
const static string food_group_table_name = "FD_GROUP";
const static int food_group_num_col = 2;
const static string food_group_col_name[food_group_num_col] = {"FdGrp_Cd", "FdGrp_Desc"};
const static string food_group_col_type[food_group_num_col] = {"CHAR", "CHAR"};
const static int food_group_size_1[food_group_num_col] = {4, 60};
const static int food_group_size_2[food_group_num_col] = {0, 0};
const static int food_group_is_primary_key[food_group_num_col] = {1, 0};
const static int food_group_not_null[food_group_num_col] = {1, 1};
const static string food_group_extra_param;

// NUT_DATA
const static string nut_data_table_name = "NUT_DATA";
const static int nut_data_num_col = 18;
const static string nut_data_col_name[nut_data_num_col] = {"NDB_no", "Nutr_No", "Nutr_Val","Num_Data_Pts","Std_Error","Src_Cd","Deriv_Cd","Ref_NDB_No","Add_Nutr_Mark","Num_Studies","Min","Max","DF","Low_EB","Up_EB","Stat_cmt","AddMod_Date","CC"};
const static string nut_data_col_type[nut_data_num_col] = {"CHAR", "CHAR", "NUMERIC","NUMERIC","NUMERIC","CHAR","CHAR","CHAR","CHAR","NUMERIC","NUMERIC","NUMERIC","NUMERIC","NUMERIC","NUMERIC","CHAR","CHAR","CHAR"};
const static int nut_data_size_1[nut_data_num_col] = {5, 3, 10, 5, 8, 2, 4, 5, 1, 2, 10, 10, 4, 10, 10, 10, 10, 1};
const static int nut_data_size_2[nut_data_num_col] = {0, 0, 3, 0, 3, 0, 0, 0, 0, 0, 3, 3, 0, 3, 3, 0, 0, 0};
const static int nut_data_is_primary_key[nut_data_num_col] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const static int nut_data_not_null[nut_data_num_col] = {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const static string nut_data_extra_param = {" , PRIMARY KEY(NDB_No, Nutr_No), FOREIGN KEY(NDB_No) REFERENCES FOOD_DES(NDB_No), FOREIGN KEY(Nutr_No) REFERENCES NUTR_DEF(Nutr_No)"};

//NUTR_DEF
const static string nutr_def_table_name="NUTR_DEF";
const static int nutr_def_num_col=6;
const static string nutr_def_col_name[nutr_def_num_col]={"Nutr_No","Units","Tagname","NutrDesc","Num_Dec","SR_Order"};
const static string nutr_def_col_type[nutr_def_num_col]={"CHAR","CHAR","CHAR","CHAR","CHAR","NUMERIC"};
const static int nutr_def_size_1[nutr_def_num_col]={3, 7, 20, 60, 1, 6};
const static int nutr_def_size_2[nutr_def_num_col]={0, 0, 0, 0, 0, 0};
const static int nutr_def_is_primary_key[nutr_def_num_col]={1, 0, 0, 0, 0, 0};
const static int nutr_def_not_null[nutr_def_num_col]={1, 1, 0, 1, 1, 1};
const static string nutr_def_extra_param={", FOREIGN KEY(Nutr_No) REFERENCES NUT_DATA(Nutr_No)"};