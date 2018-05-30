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

#include <deque>
#include <map>
#include "link.h"
#include <set>

struct info {
    info() {}
    info(unsigned n, double l) {
        nextHop = n;
        lat = l;
    }
    unsigned nextHop;
    double lat;
};

class Table {
  // Students should write this class
 public:
  Table();
  Table(unsigned n);

  bool NewLink(const Link *link);
  void UpdateTopo(const Link *link);
  void Dijkstra();
  unsigned GetNextHop(unsigned dest) const;
  ostream & Print(ostream &os) const;
private:
  unsigned nodeNum;
  set<unsigned> nodes;
  map<unsigned, info> rtable; 
  map<unsigned, map<unsigned, double> > topo;  // map<src, map<dest, c(src, dest)> >
  deque<const Link *> links;
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
