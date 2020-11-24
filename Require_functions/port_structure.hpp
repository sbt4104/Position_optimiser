#include<bits/stdc++.h>
using namespace std;

/*
SHUBHAM THORAT
BLOCKS
abc_0, {300 350} {300  655} {610  655} {610 350}
abc_1, {300 950} {300 1150} {600 1150} {600 950}
xyz_0, {850 950} {850 1150} {1150 1150} {1150 950}
xyz_1, {850 350} {850 650}  {1150 650} {1150 350}
*/

/*
CONNECTIONS
srcPort, destPort, Width ,Frequency (Mhz)

abc_0.port_abc0_1, xyz_1.port_xyz1_1, 700, 400

abc_0.port_abc0_2, abc_1.port_abc1_1, 400, 90
*/


struct edge{
    float used_length;
    float total_length;
    int total_placed;
};

struct port{
    string port_name;
    float width;
    int port_position;
    int edge_number;
    float left_x_coord;
    float left_y_coord;
};

struct block{
    string block_name;
    vector<int> actual_coord;
    vector<edge> edge_of_block;
    vector<port> port_of_block;
    int block_number;
    int total_ports;
    int total_edges;
    int total_placed_ports;
};

bool check_if_rising_edge(int &j, block &blk);
          
void add_port_to_block(block &blk,int &index_port,int &pos_edge, map<string,pair<float,float> > &port_coords);
