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

ostream & Table::Print(ostream &os) const
{
	// WRITE THIS
	os << "Print Table: "<<endl;
	for (map<unsigned, map<unsigned, double> >::const_iterator i = rtable.begin(); i != rtable.end(); ++i) {
		cout << "Node: " << i->first << ": " << endl;
		for (map<unsigned, double>::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
			cout << "Node: " << j->first << "  cost: " << j->second << ", ";
		}
		cout << endl;
	}
	return os;
}

Table::Table(unsigned n){
    nodeNum = n;
    rtable = map<unsigned, map<unsigned, double> >();
	rtable[nodeNum][nodeNum] = 0;
    neighbor = map<unsigned, double>();
    neighbor[nodeNum] = 0;
}

unsigned Table::GetNextHop(const unsigned dest) const{
	unsigned nextHop;
	double a, b;
	double min = DBL_MAX;
    for(map<unsigned, map<unsigned, double> >::const_iterator it = rtable.begin(); it != rtable.end(); ++it){
    	double cost;

    	if(it->first == nodeNum)
    		continue;
    	a = neighbor.at(it->first);
		//print neighbor Table
		cout << "Neighbor of " << nodeNum << " : " << endl;
		for (map<unsigned, double>::const_iterator s = neighbor.begin(); s != neighbor.end(); ++s) {
			cout << s->first << ": " << s->second << " , ";
		}
		cout << endl;
		//
		map<unsigned, double>::const_iterator it1; 
    	it1 = it->second.find(dest);
    	if(it1 != it->second.end())
    		b = it1->second;
    	else
    		continue;
    	cost = a + b;
		cout << endl << "cost: " << cost << ", min = " << min << ", it->first= " << it->first << endl;
    	if(cost <= min){
    		nextHop = it->first;
    		min = cost;
    	}
    }
	cout << endl << endl;
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
        	if(it->first != dest){
        		temp = cost + it->second;
                if(temp < rtable[nodeNum][it->first]){
            	    flag = true;
            	    rtable[nodeNum][it->first] = temp;
                }
        	}
            else{
            	double min = DBL_MAX;
                for(map<unsigned, double>::iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
                	temp = it1->second + rtable[it1->first][dest];
                	if(temp < min)
                		min = temp;
                }
                if(min != rtable[nodeNum][dest]){
                	rtable[nodeNum][dest] = min;
                	flag = true;
                }
            }
        }
		cout << endl << "Update: " << *this << endl;
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

	cout << endl << "update_overload_before: " << *this << endl;

	bool flag = false;
	rtable[n] = map<unsigned, double>(*v);
	cout << endl << "update_overload_1: " << *this << endl;
	for(map<unsigned, double>::iterator it = rtable[n].begin(); it != rtable[n].end(); ++it){
		double temp;
		temp = neighbor[n] + it->second;
		if (rtable[nodeNum].find(it->first) == rtable[nodeNum].end()) {
			rtable[nodeNum][it->first] = DBL_MAX;
		}
		if(temp < rtable[nodeNum][it->first]){
            flag = true;
            rtable[nodeNum][it->first] = temp;
		}
	}
	cout << endl << "update_overload_after: " << *this << endl;
	return flag;
}
#endif
