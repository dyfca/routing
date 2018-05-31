#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"
#include "table.h"

#if defined(GENERIC)
class RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
class RoutingMessage {
public:
  const Link *link;

  RoutingMessage();
  RoutingMessage(const Link *l,int seq_num);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);
  int seq;
  ostream & Print(ostream &os) const;
  
};
#endif

#if defined(DISTANCEVECTOR)
class RoutingMessage {
public:
  unsigned nodeNum;
  const map<unsigned, double> *dv;
  RoutingMessage();
  RoutingMessage(const unsigned nodeNum, const map<unsigned, double> *dv);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
