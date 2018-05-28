#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const unsigned n, const map<unsigned, double> *v) {
    nodeNum = n;
    dv = v;
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{
	nodeNum = rhs.nodeNum;
    dv = rhs.dv;
}

#endif

