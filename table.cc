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

#include "float.h"

Table::Table() {}

Table::Table(unsigned n) {
  nodeNum = n;
  links = deque<const Link *>();
  rtable = map<unsigned, info>();
  nodes = set<unsigned>();
  topo = map<unsigned, map<unsigned, pair<double,int> > >();
}

bool Table::NewLink(const Link *link, int seq){
  //for (deque<const Link *>::const_iterator it = links.begin(); it != links.end(); ++it)
  //    if ((*it)->GetSrc() == link->GetSrc() && (*it)->GetDest() == link->GetDest() && (*it)->GetLatency() == link->GetLatency())
  //        return false;
	unsigned dest = link->GetDest(), src = link->GetSrc();
	if (topo[src][dest].second >= seq) return false;
  return true;
}

int Table::UpdateTopo(const Link *link){
  links.push_back(link);
  unsigned dest = link->GetDest(), src = link->GetSrc();
  nodes.insert(src);
  nodes.insert(dest);
  double cost = link->GetLatency();
  topo[src][dest].first = cost;
  topo[src][dest].second += 1;
  return topo[src][dest].second;
}

unsigned Table::GetNextHop(unsigned dest) const{
  return rtable.at(dest).nextHop;
}

void Table::Dijkstra(){
  set<unsigned> N(nodes);  // unknown
  map<unsigned, double> D;  // shortest path value: src -> dest
  map<unsigned, unsigned> p;  
  D[nodeNum] = 0;
  N.erase(nodeNum);
  for (set<unsigned>::const_iterator it = N.begin(); it != N.end(); ++it) {  // initialization
      if(topo[nodeNum].find(*it) != topo[nodeNum].end()){
        D[*it] = topo[nodeNum][*it].first;
        p[*it] = nodeNum;
      }
      else{
        D[*it] = DBL_MAX;
        p[*it] = -1;
      }
  }
  while(!N.empty()){
      double minimum = DBL_MAX;
      unsigned minNode;
      for(set<unsigned>::const_iterator it = N.begin(); it != N.end(); ++it)
        if(D[*it] < minimum){
          minimum = D[*it];
          minNode = *it;
        }
      N.erase(minNode);
      for(map<unsigned, pair<double,int> >::const_iterator it = topo[minNode].begin(); it != topo[minNode].end(); ++it){
        if(N.find(it->first) != N.end()){
            double temp = D[it->first];
            D[it->first] = std::min(D[it->first], D[minNode] + it->second.first);
            if(temp != D[it->first])
              p[it->first] = minNode;
        }
      }
  }
  for(set<unsigned>::const_iterator it = nodes.begin(); it != nodes.end(); ++it){
    if(*it != nodeNum){
      unsigned next = *it;
      while(p[next] != nodeNum)
        next = p[next];
      rtable[*it] = info(next, D[*it]);
    }
  }
}

#endif

#if defined(DISTANCEVECTOR)

#include "float.h"
Table::Table() {}

ostream & Table::Print(ostream &os) const
{
	// WRITE THIS
	os << "Print Table: "<<endl;
	for (map<unsigned, map<unsigned, double> >::const_iterator i = rtable.begin(); i != rtable.end(); ++i) {
		cout << "Node: " << i->first << ": ( " ;
		for (map<unsigned, double>::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
			cout << "Node: " << j->first << "  cost: " << j->second << ", ";
		}
		cout << " )"<<endl;
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
	cout << endl << "Before link update" << *this << endl;
	unsigned dest = link->GetDest();
	cout << "dest = " << dest << endl;
	double cost = link->GetLatency();
	cout << "cost = " << cost << endl;
	bool flag = false;  // if DV is changed
	neighbor[dest] = cost;
    if(rtable[nodeNum].find(dest) == rtable[nodeNum].end()){  // new path added
    	rtable[nodeNum][dest] = cost;
    	flag = true;
    	cout << endl << "After link update" << *this << endl;
    	return flag;
    }
    else{
    	for(map<unsigned, double>::iterator it = rtable[nodeNum].begin(); it != rtable[nodeNum].end(); ++it){
    		if(it->first == nodeNum)
    			continue;
    		double min = DBL_MAX;
    		double org = it->second;
    		for(map<unsigned, double>::const_iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
    			double temp;
    			if(it1->first == nodeNum)
    				continue;
    			if(rtable[it1->first].find(dest) == rtable[it1->first].end())
    				continue;
    			temp = it1->second + rtable[it1->first][dest];
    			if(temp < min)
    				min = temp;
    		}
    		if(min != org){
    			flag = true;
    			it->second = min;
    		}
    	}
    	cout << endl << "After link update" << *this << endl;
    	return flag;
    }
  //   else{
  //       for(map<unsigned, double>::iterator it = rtable[dest].begin(); it != rtable[dest].end(); ++it){
  //       	double temp;
  //       	if(it->first != dest){
  //       		temp = cost + it->second;
  //               if(temp < rtable[nodeNum][it->first]){
  //           	    flag = true;
  //           	    rtable[nodeNum][it->first] = temp;
  //               }
  //       	}
  //           else{
  //           	double min = DBL_MAX;
  //               for(map<unsigned, double>::iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
  //               	temp = it1->second + rtable[it1->first][dest];
  //               	if(temp < min)
  //               		min = temp;
  //               }
  //               if(min != rtable[nodeNum][dest]){
  //               	rtable[nodeNum][dest] = min;
  //               	flag = true;
  //               }
  //           }
  //       }
		// cout << endl << "Update: " << *this << endl;
  //       return flag;
  //       // for(map<unsigned, double>::iterator it = rtable[nodeNum].begin(); it != rtable[nodeNum].end(); ++it){
  //       // 	if(it->first == nodeNum)
  //       // 		continue;
  //       // 	for(map<unsigned, double>::iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
  //       // 		double min = DBL_MAX;
  //       // 		temp = 
  //       // 	}
  //       // }
  //   }
}

map<unsigned, double> *Table::GetDV(){
	return &(rtable[nodeNum]);
}

bool Table::Update(const unsigned n, const map<unsigned, double> *v){

	cout << endl << "Before receive" << *this << endl;
	cout << "Node " << n << " DV received." << endl;

	bool flag = false;
	rtable[n] = map<unsigned, double>(*v);
	cout << endl << "After recieve" << *this << endl;
	for(map<unsigned, double>::iterator it = rtable[nodeNum].begin(); it != rtable[nodeNum].end(); ++it){
    	if(it->first == nodeNum)
    		continue;
    	double min = DBL_MAX;
    	double org = it->second;
    	for(map<unsigned, double>::const_iterator it1 = neighbor.begin(); it1 != neighbor.end(); ++it1){
    		double temp;
    		if(it1->first == nodeNum)
    			continue;
    		if(rtable[it1->first].find(it->first) == rtable[it1->first].end())
    			continue;
    		temp = it1->second + rtable[it1->first][it->first];
    		if(temp < min)
    			min = temp;
    	}
    	if(min != org){
    		flag = true;
    		it->second = min;
    	}
    }
    for(map<unsigned, double>::iterator it = rtable[n].begin(); it != rtable[n].end(); ++it){
    	if(rtable[nodeNum].find(it->first) == rtable[nodeNum].end()){
    		rtable[nodeNum][it->first] = neighbor[n] + it->second;
    		flag = true;
    	}
    }
	cout << endl << "After update" << *this << endl;
	return flag;
}
#endif
