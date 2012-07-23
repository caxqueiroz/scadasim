//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SimTrace.h"


SimTrace::SimTrace() {
    initialize();
}

SimTrace::~SimTrace() {
    // close database
    if (db) {
        sqlite3_close(db);
        cout << "Closed database successfully\n";
    }
}

HexCharStruct SimTrace::hexC(unsigned char _c){
    return hex(_c);
}

void SimTrace::initialize() {

//    char *szErrMsg = 0;

    const char* file = "traces.sqlite";
    // open database
    int rc = sqlite3_open(file, &db);

    if (rc) {
        cout << "Can't open database\n";
    } else {
        cout << "Open database successfully\n";
    }
}

void SimTrace::dump(string data, string source, string dest, string comment) {
    char *sqlQuery;
    sqlite3_stmt *stmt;
    const char *pzTest;
    sqlQuery = "insert into traces (source, dest, data,comment) values (?,?,?,?)";
    if (db) {
        int rc = sqlite3_prepare(db, sqlQuery, strlen(sqlQuery), &stmt,
                &pzTest);
        if (rc == SQLITE_OK) {
            // bind the value
            sqlite3_bind_text(stmt, 1, source.c_str(), source.size(), 0);
            sqlite3_bind_text(stmt, 2,dest.c_str(), dest.size(), 0);
            sqlite3_bind_text(stmt, 3, data.c_str(),data.size(),0);
            sqlite3_bind_text(stmt, 4, comment.c_str(),comment.size(),0);
            // commit
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

    }

}
