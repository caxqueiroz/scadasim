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

#ifndef __SCADASIM_SIMTRACE_H_
#define __SCADASIM_SIMTRACE_H_

#include <sqlite3.h>
#include <string>
#include <iostream>
using namespace std;

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}
/**
 * TODO - Generated class
 */
class SimTrace
{
private:
    sqlite3 *db;
  protected:
    void initialize();
  public:
    HexCharStruct hexC(unsigned char _c);
    void dump(string data, string source, string dest, string comment);
    SimTrace();
    ~SimTrace();

};

#endif
