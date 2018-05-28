#ifndef _table
#define _table


#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include <map>
#include "link.h"


class Table {
public:
	Table();
	Table(unsigned nodeNum);

    unsigned GetNextHop(const unsigned dest) const;
    bool Update(const Link *link);
    bool Update(const unsigned n, const map<unsigned, double> *v);
    map<unsigned, double> *GetDV();
    ostream & Print(ostream &os) const;

private:
	map<unsigned, map<unsigned, double> > rtable;
	map<unsigned, double> neighbor;  // direct cost to neighbor
    unsigned nodeNum;
    


};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
