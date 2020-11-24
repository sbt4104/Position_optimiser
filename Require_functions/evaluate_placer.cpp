#include "port_structure.hpp"
#include "evaluate_placer.hpp"

float evaluate_cost(block &blk, int &curr_edge, int &total_ports_on_edge, vector<int> &store_ports_of_edge, \
                    vector<string> &store_ports_of_edge_names,     map<string,pair<float,float> > &port_coords, \
                    map<string, vector<string> > &port_connections){ //It updates total_ports_on_edge, sore_ports_of_edge 
                                                                     //store_ports_of_edge, 
    //evaluate current cost
    total_ports_on_edge = 0;
    store_ports_of_edge.clear();
    store_ports_of_edge_names.clear();
    
    for(int i=0;i<blk.port_of_block.size();i++){
        int port_edge = blk.port_of_block[i].edge_number;
        if(port_edge == curr_edge){
            store_ports_of_edge.push_back(i);
            store_ports_of_edge_names.push_back(blk.port_of_block[i].port_name);
            total_ports_on_edge++;
        }
    }
    
    /*
    cout<<"check "<<blk.edge_of_block[curr_edge].total_placed<<" "<<total_ports_on_edge<<endl;
    if(blk.edge_of_block[curr_edge].total_placed > 4){
        cout<<"current edge "<<curr_edge<<endl;
    }
    */
    
    //blk.edge_of_block[curr_edge].total_placed = total_ports_on_edge;
    //cout<<"here1\n";
    float curr_cost = 0;
    for(int j=0; j<total_ports_on_edge; j++){
        string main_port = store_ports_of_edge_names[j];
        float main_port_x = port_coords[main_port].first;
        float main_port_y = port_coords[main_port].second;
        vector<string> ports_connected_to_main_port = port_connections[main_port];
        for(int k=0;k<ports_connected_to_main_port.size();k++){
            string connected_port = ports_connected_to_main_port[k];
            float connected_port_x = port_coords[connected_port].first;
            float connected_port_y = port_coords[connected_port].second;
            curr_cost += sqrt(pow(connected_port_x - main_port_x,2) + pow(connected_port_y - main_port_y,2));
        }
    }
    //cout<<"here2\n";
    return curr_cost;
}

void  evaluate_coords(block &blk, int &curr_edge, int &total_ports_on_edge, vector<int> &store_ports_of_edge, \
                    map<string,pair<float,float> > &port_coords){
    
    int prt_index = store_ports_of_edge[0];
    
    bool rising_edge = check_if_rising_edge(prt_index,blk);            
    int prt_index_prev;
    blk.port_of_block[prt_index].left_x_coord = blk.actual_coord[2*curr_edge]; 
    blk.port_of_block[prt_index].left_y_coord = blk.actual_coord[2*curr_edge+1];
    
    port_coords[blk.port_of_block[prt_index].port_name] = make_pair(blk.port_of_block[prt_index].left_x_coord, \
                                                                    blk.port_of_block[prt_index].left_y_coord);
    
    
    //cout<<"index 0 "<<blk.port_of_block[prt_index].left_x_coord<< \
        //" "<<blk.port_of_block[prt_index].left_y_coord<<endl;
    //cout<<"current edge "<<curr_edge<<" rising_edge "<<rising_edge<<endl;
    if(curr_edge % 2){ //parallel to x axis
        if(rising_edge){
            for(int i=1;i<total_ports_on_edge;i++){
                prt_index = store_ports_of_edge[i];
                prt_index_prev = store_ports_of_edge[i-1];
                blk.port_of_block[prt_index].left_x_coord = \
                    blk.port_of_block[prt_index_prev].left_x_coord + blk.port_of_block[prt_index_prev].width; 
                blk.port_of_block[prt_index].left_y_coord \
                    = blk.port_of_block[prt_index_prev].left_y_coord;
            }
        }
        else{
            for(int i=1;i<total_ports_on_edge;i++){
                prt_index = store_ports_of_edge[i];
                prt_index_prev = store_ports_of_edge[i-1];
                blk.port_of_block[prt_index].left_x_coord = \
                    blk.port_of_block[prt_index_prev].left_x_coord - blk.port_of_block[prt_index_prev].width; 
                blk.port_of_block[prt_index].left_y_coord \
                    = blk.port_of_block[prt_index_prev].left_y_coord;                    
            }                
        }
    }
    else{    //parallel to y axis
        if(rising_edge){
            for(int i=1;i<total_ports_on_edge;i++){
                prt_index = store_ports_of_edge[i];
                prt_index_prev = store_ports_of_edge[i-1];
                blk.port_of_block[prt_index].left_y_coord = \
                    blk.port_of_block[prt_index_prev].left_y_coord + blk.port_of_block[prt_index_prev].width; 
                blk.port_of_block[prt_index].left_x_coord \
                    = blk.port_of_block[prt_index_prev].left_x_coord;                    
            }
        }
        else{
            for(int i=1;i<total_ports_on_edge;i++){
                prt_index = store_ports_of_edge[i];
                prt_index_prev = store_ports_of_edge[i-1];
                blk.port_of_block[prt_index].left_y_coord = \
                    blk.port_of_block[prt_index_prev].left_y_coord - blk.port_of_block[prt_index_prev].width; 
                blk.port_of_block[prt_index].left_x_coord \
                    = blk.port_of_block[prt_index_prev].left_x_coord;                                        
            }                
        }            
    }

    for(int i=0;i<total_ports_on_edge;i++){
        prt_index = store_ports_of_edge[i];
        port_coords[blk.port_of_block[prt_index].port_name] = make_pair(blk.port_of_block[prt_index].left_x_coord, \
                                                                    blk.port_of_block[prt_index].left_y_coord);
    }
}

