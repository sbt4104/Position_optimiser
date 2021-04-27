#include "port_structure.hpp"

using namespace std;

/*
###
#check if rising edge or falling edge
#
*/
bool check_if_rising_edge(int &j, block &blk){ 
    //j is port index
    bool rising_edge = true;

        if(blk.port_of_block[j].edge_number == (blk.total_edges-1)){
            rising_edge = false; 
        }
        else{
            float x_temp0 = blk.actual_coord[2*blk.port_of_block[j].edge_number];
            float y_temp0 = blk.actual_coord[2*blk.port_of_block[j].edge_number+1];
            float x_temp1 = blk.actual_coord[2*blk.port_of_block[j].edge_number+2];
            float y_temp1 = blk.actual_coord[2*blk.port_of_block[j].edge_number+3];
            
            if((x_temp1 - x_temp0) < 0 || (y_temp1 - y_temp0) < 0){
                rising_edge = false;
            }
            else{
                rising_edge = true;
            }
        }
    
    return rising_edge;
}

void add_port_to_block(block &blk,int &index_port,int &pos_edge, map<string,pair<float,float> > &port_coords){
    //assing port to a edge
    blk.port_of_block[index_port].edge_number = pos_edge;
    
    //assign port position and update all other parameters;
    blk.total_placed_ports++;
    
    blk.port_of_block[index_port].port_position = blk.edge_of_block[pos_edge].total_placed;
    blk.edge_of_block[pos_edge].total_placed++;
    
    bool rising_edge = check_if_rising_edge(index_port, blk);
    
    if(pos_edge % 2){ //parallel to x axis
        if(rising_edge){
            blk.port_of_block[index_port].left_x_coord = \
                blk.actual_coord[2*pos_edge] + blk.edge_of_block[pos_edge].used_length;
        }
        else{
            blk.port_of_block[index_port].left_x_coord = \
                blk.actual_coord[2*pos_edge] - blk.edge_of_block[pos_edge].used_length;                        
        }
        
        blk.port_of_block[index_port].left_y_coord = blk.actual_coord[2*pos_edge + 1];
    }
    else{ //parallel to y axis
        if(rising_edge){
            blk.port_of_block[index_port].left_y_coord = \
                blk.actual_coord[2*pos_edge + 1] + blk.edge_of_block[pos_edge].used_length;
        }
        else{
            blk.port_of_block[index_port].left_y_coord = \
                blk.actual_coord[2*pos_edge + 1] - blk.edge_of_block[pos_edge].used_length;                        
        }
        blk.port_of_block[index_port].left_x_coord = blk.actual_coord[2*pos_edge];                    
    }
    
    //map port name to its coordinate
    port_coords[blk.port_of_block[index_port].port_name] = \
        make_pair(blk.port_of_block[index_port].left_x_coord,blk.port_of_block[index_port].left_y_coord);
    blk.edge_of_block[pos_edge].used_length += blk.port_of_block[index_port].width;
}


