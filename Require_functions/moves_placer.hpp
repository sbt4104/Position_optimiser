#include<bits/stdc++.h>
using namespace std;

void swap_on_same_edge(vector<block> &blk,     map<string,pair<float,float> > &port_coords,    map<string, vector<string>> &port_connections);

void move_port_to_other_edge(vector<block> &blk,map<string,pair<float,float> > &port_coords, map<string, vector<string>> &port_connections);
