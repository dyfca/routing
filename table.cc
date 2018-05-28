#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

#endif

#if defined(DISTANCEVECTOR)

#include "float.h"
Table::Table() {}

Table::Table(unsigned n){
    nodeNum = n;
    rtable = map<unsigned, map<unsigned, double>>();
    rtable[nodeNum][nodeNum] = 0;
    neighbor = map<unsigned, double>();
    neighbor[nodeNum] = 0;
}

unsigned Table::GetNextHop(const unsigned dest) const{
	unsigned nextHop;
	double a, b;
    for(map<unsigned, map<unsigned, double>>::iterator it = rtable.begin(); it != rtable.end(); ++it){
    	double cost;
    	double min = DBL_MAX;
    	if(it->first == nodeNum)
    		continue;
    	a = neighbor[it->first];
    	map<unsigned, double>::iterator it1; 
    	it1 = it->second.find(dest);
    	if(it1 != it->second.end())
    		b = it1->second;
    	else
    		continue;
    	cost = a + b;
    	if(cost <= min){
    		nextHop = it->first;
    		min = cost;
    	}
    }
    return nextHop;
}

bool Table::Update(const Link *link){
	unsigned dest = link->GetDest();
	double cost = link->GetLatency();
	bool flag = false;  // if DV is changed
	neighbor[dest] = cost;
    if(rtable[nodeNum].find(dest) == rtable[nodeNum].end()){  // new link added
    	rtable[nodeNum][dest] = cost;
    	flag = true;
    	return flag;
    }
    else{
        for(map<unsigned, double>::iterator it = rtable[dest].begin(); it != rtable[dest].end(); ++it){
        	double temp;
            temp = cost + it->second;
            if(temp < rtable[nodeNum][it->first]){
            	flag = true;
            	rtable[nodeNum][it->first] = temp;
            }
        }
        return flag;
        // for(map<unsigned, double>::iterator it = rtable[nodeNum].begin(); it != rtable[nodeNum].end(); ++it){
        // 	if(it->first == nodeNum)
        // 		continue;
        // 	for(map<unsigned, double>::iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
        // 		double min = DBL_MAX;
        // 		temp = 
        // 	}
        // }
    }
}

map<unsigned, double> *Table::GetDV(){
	return &(rtable[nodeNum]);
}

bool Table::Update(const unsigned n, const map<unsigned, double> *v){
	bool flag = false;
	rtable[n] = map<unsigned, double>(*v);
	for(map<unsigned, double>::iterator it = rtable[n].begin(); it != rtable[n].end(); ++it){
		double temp;
		temp = neighbor[n] + it->second;
		if(temp < rtable[nodeNum][it->first]){
            flag = true;
            rtable[nodeNum][it->first] = temp;
		}
	}
}
#endif
