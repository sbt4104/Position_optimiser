#include<bits/stdc++.h>
using namespace std;

float evaluate_cost(block &blk, int &curr_edge, int &total_ports_on_edge, vector<int> &store_ports_of_edge, \
                    vector<string> &store_ports_of_edge_names,     map<string,pair<float,float> > &port_coords, \
                    map<string, vector<string> > &port_connections);
                
void  evaluate_coords(block &blk, int &curr_edge, int &total_ports_on_edge, vector<int> &store_ports_of_edge, \
                    map<string,pair<float,float> > &port_coords);
