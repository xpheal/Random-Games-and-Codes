#include "load.h"

//Note, this snippet callback is from tutorials point for SQLite3
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++)
   {
      	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   cout<<"\n";
   return 0;
}

// Execute the sql query
void execute_query(string query, sqlite3 *db, string message){
	char *errMsg = 0;

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errMsg);
	int rc = sqlite3_exec(db, query.c_str(), callback, 0, &errMsg);
	if(rc != SQLITE_OK){
		cout << "SQL error: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
	else{
		cout << message << "\n";
	}
	sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &errMsg);
}

// Return a sql code for creating table
string create_table(const string table_name, const int num_col, const string col_name[], 
					const string type[], const int size_1[], const int size_2[], 
					const int is_primary_key[], const int not_null[], const string extra_param){

	string code = "CREATE TABLE " + table_name + "(";

	int i = 0;
	for(; i < num_col; i++){
		// Extra param overrides the sql query for the column if it is not null
		code += col_name[i] + " " + type[i] + "(";
		code += to_string(size_1[i]);

		if(type[i] == "NUMERIC"){
			code += "," + to_string(size_2[i]);
		}

		code += ")";

		if(is_primary_key[i]){
			code += " PRIMARY KEY";
		}

		if(not_null[i]){
			code += " NOT NULL";
		}

		if(i != num_col - 1){
			code += ", ";
		}
	}

	code += extra_param + ");";

	return code;
}

// Convert the token into the correct format for a sql query
// For now, <type> supports "CHAR" and "NUMERIC"
string convert_token(string token, string type){
	if(type == "NUMERIC"){
		if(token == ""){
			return "NULL";
		}
	}
	else if(type == "CHAR"){
		replace(token.begin(), token.end(), '\"', '\'');
		return "\"" + token + "\"";
	}
	else{
		fprintf(stderr, "ERROR: convert_token function currently does support %s.", type.c_str());
		exit(-1);
	}

	return token;
}

// Generate a line of insert code for the table <table_name> with <num_col> columns
// Each column has its type <col_type[]> and its value <tokens>, currently col_type accepts only "CHAR" and "NUMERIC"
string generate_insert_code(string table_name, const int num_col, const string col_type[], vector<string> tokens){
	string code = "INSERT INTO " + table_name + " VALUES(";

	// Loop through the columns
	int i = 0;
	for(; i < num_col; i++){
		code += convert_token(tokens.at(i), col_type[i]);

		if(i != num_col - 1){
			code += ", ";
		}
	}

	code += ");";

	return code;
}

// Return a sql query insert string for loading the data from file <filename> into table <table_name>
// num_col is the number of columns of the data and col_type[] is the type of data, either "CHAR" or "NUMERIC" for now
string insert_data_from_file(string filename, string table_name, const int num_col, const string col_type[]){
	string code = "";

	string line = "";
	ifstream inFile(filename.c_str());

	// Read line by line
	while(inFile){
		getline(inFile, line);
		vector<string> tokens = get_token(line);

		if(tokens.size() > 3){
			vector<string> temp;
			int i = 0;
			for(i = 0; i < num_col; i++){
				temp.push_back(tokens.at(i));
			}

			tokens = temp;
		}

		if((int)tokens.size() == num_col){
			code += generate_insert_code(table_name, num_col, col_type, tokens) + "\n";
		}
	}

	inFile.close();

	return code;
}

// Return a vector of string tokens
vector<string> get_token(string line){
	vector<string> tokens;

	string buf = "";

	for(char& c : line){
		// Separate tokens by '^'
		if(c != '^' && c != '~'){
			buf += c;
		}
		else if(c != '~'){
			tokens.push_back(buf);
			buf = "";
		}
	}

	// add the last token to the list (check for carriage return)
	if(buf.back() == '\r'){
		tokens.push_back(buf.substr(0, buf.length() - 1));
	}
	else{
		tokens.push_back(buf);
	}

	return tokens;
}

int main(int argc, char* argv[]){
	sqlite3 *db;
	int conn = sqlite3_open("nutrients.db", &db);

	// Connect to database
	if(conn){
		fprintf(stderr, "Unable to open the database: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	else{
		fprintf(stderr, "database opened successfully\n");
	}

	// Drop table if exists
	execute_query("DROP TABLE IF EXISTS '" + food_group_table_name + "';", db, "Table " + food_group_table_name + " dropped successfully");
	execute_query("DROP TABLE IF EXISTS '" + food_des_table_name + "';", db, "Table " + food_des_table_name + " dropped successfully");
	execute_query("DROP TABLE IF EXISTS '" + nut_data_table_name + "';", db, "Table " + nut_data_table_name + " dropped successfully");
	execute_query("DROP TABLE IF EXISTS '" + nutr_def_table_name + "';", db, "Table " + nutr_def_table_name + " dropped successfully");

	// Create table
	string create_table_code = create_table(food_group_table_name, food_group_num_col, food_group_col_name, food_group_col_type, food_group_size_1, food_group_size_2, food_group_is_primary_key, food_group_not_null, food_group_extra_param);
	execute_query(create_table_code, db, "Table " + food_group_table_name + " created successfully");

	create_table_code = create_table(food_des_table_name, food_des_num_col, food_des_col_name, food_des_col_type, food_des_size_1, food_des_size_2, food_des_is_primary_key, food_des_not_null, food_des_extra_param);
	execute_query(create_table_code, db, "Table " + food_des_table_name + " created successfully");

	create_table_code = create_table(nut_data_table_name, nut_data_num_col, nut_data_col_name, nut_data_col_type, nut_data_size_1, nut_data_size_2, nut_data_is_primary_key, nut_data_not_null, nut_data_extra_param);
	execute_query(create_table_code, db, "Table " + nut_data_table_name + " created successfully");

	create_table_code = create_table(nutr_def_table_name, nutr_def_num_col, nutr_def_col_name, nutr_def_col_type, nutr_def_size_1, nutr_def_size_2, nutr_def_is_primary_key, nutr_def_not_null, nutr_def_extra_param);
	execute_query(create_table_code, db, "Table " + nutr_def_table_name + " created successfully");

	// Open file and insert data
	string insert_table_code = insert_data_from_file("./sr28asc/" + food_group_table_name + ".txt", food_group_table_name, food_group_num_col, food_group_col_type);
	execute_query(insert_table_code, db, "Table data " + food_group_table_name + " inserted successfully");

	insert_table_code = insert_data_from_file("./sr28asc/" + food_des_table_name + ".txt", food_des_table_name, food_des_num_col, food_des_col_type);
	execute_query(insert_table_code, db, "Table data " + food_des_table_name + " inserted successfully");

	insert_table_code = insert_data_from_file("./sr28asc/" + nut_data_table_name + ".txt", nut_data_table_name, nut_data_num_col, nut_data_col_type);
	execute_query(insert_table_code, db, "Table data " + nut_data_table_name + " inserted successfully");

	insert_table_code=insert_data_from_file("./sr28asc/" + nutr_def_table_name + ".txt", nutr_def_table_name, nutr_def_num_col, nutr_def_col_type);
	execute_query(insert_table_code, db, "Table data " + nutr_def_table_name + " inserted successfully");

	return 0;
}