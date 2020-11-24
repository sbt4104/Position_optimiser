#include "port_structure.hpp"
#include "moves_placer.hpp"
#include "evaluate_placer.hpp"

void swap_on_same_edge(vector<block> &blk,     map<string,pair<float,float> > &port_coords,    map<string, vector<string>> &port_connections){
    //select the block
    int curr_block = rand()%blk.size();

    //swap 2 elements on the same edge

    //select the edge
    int curr_edge = rand()%blk[curr_block].total_edges;

    int total_ports_on_edge = 0;
    vector<int> store_ports_of_edge;
    vector<string> store_ports_of_edge_names;
    if(blk[curr_block].edge_of_block[curr_edge].total_placed ==0)
        return;
                    
    float curr_cost = evaluate_cost(blk[curr_block], curr_edge, total_ports_on_edge, \
                                    store_ports_of_edge, store_ports_of_edge_names, port_coords, port_connections);

    if(total_ports_on_edge < 2){
        //not a valid operation for this edge
        return;
    }

    //get 2 different ports
    int port1 = rand()%total_ports_on_edge;
    int port2;
    while((port2 = rand()%total_ports_on_edge) == port1);

    //map to logical ports
    int store_port1 = port1;
    int store_port2 = port2;
    port1 = store_ports_of_edge[port1];
    port2 = store_ports_of_edge[port2];

    //perfom swap <shubham, check swap operations again>
    int port1_pos = blk[curr_block].port_of_block[port1].port_position;
    int port2_pos = blk[curr_block].port_of_block[port2].port_position;

    struct port temp_port = blk[curr_block].port_of_block[port1];
    blk[curr_block].port_of_block[port1] = blk[curr_block].port_of_block[port2];
    blk[curr_block].port_of_block[port2] = temp_port;

    //update port position after swap
    blk[curr_block].port_of_block[port1].port_position = port2_pos;
    blk[curr_block].port_of_block[port2].port_position = port1_pos;

    //swap elements in store_ports_of_edge_names
    string temp_curr_string = store_ports_of_edge_names[store_port1];
    store_ports_of_edge_names[store_port1] = store_ports_of_edge_names[store_port2];
    store_ports_of_edge_names[store_port2] = temp_curr_string;
     //cout<<"here3\n";
     
    //evaluate coord
    evaluate_coords(blk[curr_block], curr_edge, total_ports_on_edge, store_ports_of_edge, port_coords);

    total_ports_on_edge = 0;
    store_ports_of_edge.clear();
    store_ports_of_edge_names.clear();

                    
    float updated_cost = evaluate_cost(blk[curr_block], curr_edge, total_ports_on_edge, \
                                    store_ports_of_edge, store_ports_of_edge_names, port_coords, port_connections);
    //cout<<"current and updated costs "<<curr_cost<<" "<<updated_cost<<endl;

    if(updated_cost > curr_cost) { //not a better solution, revert back to previous changes
        //get 2 different ports
        port1 = store_port1;
        port2 = store_port2;

        //map to logical ports
        store_port1 = port1;
        store_port2 = port2;
        port1 = store_ports_of_edge[port1];
        port2 = store_ports_of_edge[port2];
        
        //perfom swap
        int port1_pos = blk[curr_block].port_of_block[port1].port_position;
        int port2_pos = blk[curr_block].port_of_block[port2].port_position;
        
        struct port temp_port = blk[curr_block].port_of_block[port1];
        blk[curr_block].port_of_block[port1] = blk[curr_block].port_of_block[port2];
        blk[curr_block].port_of_block[port2] = temp_port;
        
        //update port position after swap
        blk[curr_block].port_of_block[port1].port_position = port2_pos;
        blk[curr_block].port_of_block[port2].port_position = port1_pos;
        
        //swap elements in store_ports_of_edge_names
        string temp_curr_string = store_ports_of_edge_names[store_port1];
        store_ports_of_edge_names[store_port1] = store_ports_of_edge_names[store_port2];
        store_ports_of_edge_names[store_port2] = temp_curr_string;
         //cout<<"here3\n";
         
        //evaluate coord
        evaluate_coords(blk[curr_block], curr_edge, total_ports_on_edge, store_ports_of_edge, port_coords);
    }
}

void move_port_to_other_edge(vector<block> &blk,map<string,pair<float,float> > &port_coords, map<string, vector<string>> &port_connections){
    
    //cout<<"inside move to other edge\n";
    //select block
    int curr_block = rand()%blk.size();
    
    struct block reverse_block = blk[curr_block];
    map<string,pair<float,float> > reverse_port_coords = port_coords;
    map<string, vector<string>> reverse_port_connections = port_connections;
    float curr_cost = 0;
    float updated_cost = 0;
    //select the edge
    int curr_edge1=0;
    
    int total_ports_on_edge = 0;
    vector<int> store_ports_of_edge;
    vector<string> store_ports_of_edge_names;
    
    int loop_counter = blk[curr_block].total_edges;
    total_ports_on_edge = 0;
    while(total_ports_on_edge == 0 && loop_counter){
        curr_edge1 = rand()%blk[curr_block].total_edges;
        total_ports_on_edge = blk[curr_block].edge_of_block[curr_edge1].total_placed;
        loop_counter--;
    }
    if(total_ports_on_edge ==0)
        return;

    total_ports_on_edge = 0;
    store_ports_of_edge.clear();
    store_ports_of_edge_names.clear(); 

    curr_cost = evaluate_cost(blk[curr_block], curr_edge1, total_ports_on_edge, \
                                    store_ports_of_edge, store_ports_of_edge_names, port_coords, port_connections);

    int curr_port1 = rand()%total_ports_on_edge;
    int store_curr_port1 = curr_port1;
    curr_port1 = store_ports_of_edge[curr_port1];
    
    //select edge 2 to place curr_port1
    int curr_edge2;

    while((curr_edge2 = rand()%blk[curr_block].total_edges) == curr_edge1); 
    
    int left_in_curr_edge2 = blk[curr_block].edge_of_block[curr_edge2].total_length - blk[curr_block].edge_of_block[curr_edge2].used_length;
    
    if(left_in_curr_edge2 >= blk[curr_block].port_of_block[curr_port1].width){ //check if legal step
        //remove curr_port1
        struct port port_temp = blk[curr_block].port_of_block[curr_port1];
        
        blk[curr_block].total_ports--;
        blk[curr_block].edge_of_block[curr_edge1].total_placed--;
        blk[curr_block].edge_of_block[curr_edge1].used_length -= port_temp.width;
        
        for(int i=store_curr_port1;i< total_ports_on_edge; i++){
            int actual_port_pos = store_ports_of_edge[i];
            blk[curr_block].port_of_block[actual_port_pos].port_position--;
        }
        //<shubham, check this statement's behavious in detail>
        blk[curr_block].port_of_block.erase(blk[curr_block].port_of_block.begin() + curr_port1);
        blk[curr_block].total_placed_ports--;

        total_ports_on_edge = 0;
        store_ports_of_edge.clear();
        store_ports_of_edge_names.clear(); 
                        
        if(blk[curr_block].edge_of_block[curr_edge1].total_placed <= 0){
            updated_cost = 0;
            blk[curr_block].edge_of_block[curr_edge1].total_placed = 0;
        }
        else{
            updated_cost = evaluate_cost(blk[curr_block], curr_edge1, total_ports_on_edge, \
                                            store_ports_of_edge, store_ports_of_edge_names, port_coords, port_connections);    
            
            /*for(int i=0;i<total_ports_on_edge;i++){
                cout<<store_ports_of_edge[i]<<" "<<store_ports_of_edge_names[i]<<endl;
            }*/
            
            if(total_ports_on_edge!=0)
                evaluate_coords(blk[curr_block], curr_edge1, total_ports_on_edge, store_ports_of_edge, port_coords);
        }
                
        vector<port> vec_temp_port = blk[curr_block].port_of_block;
        
        port_temp.edge_number = curr_edge2;
        blk[curr_block].edge_of_block[curr_edge2].total_placed++;
        port_temp.port_position = blk[curr_block].edge_of_block[curr_edge2].total_placed;
        blk[curr_block].edge_of_block[curr_edge2].used_length += port_temp.width;
        vec_temp_port.push_back(port_temp);
        blk[curr_block].port_of_block = vec_temp_port;
        blk[curr_block].total_ports++;
        blk[curr_block].total_placed_ports++;
        
        total_ports_on_edge = 0;
        store_ports_of_edge.clear();
        store_ports_of_edge_names.clear(); 
                        
        float temp_cost = evaluate_cost(blk[curr_block], curr_edge2, total_ports_on_edge, \
                                        store_ports_of_edge, store_ports_of_edge_names, port_coords, port_connections);    
        
        evaluate_coords(blk[curr_block], curr_edge2, total_ports_on_edge, store_ports_of_edge, port_coords);

        string main_port = port_temp.port_name;
        float main_port_x = port_coords[main_port].first;
        float main_port_y = port_coords[main_port].second;
        vector<string> ports_connected_to_main_port = port_connections[main_port];
        for(int k=0;k<ports_connected_to_main_port.size();k++){
            string connected_port = ports_connected_to_main_port[k];
            float connected_port_x = port_coords[connected_port].first;
            float connected_port_y = port_coords[connected_port].second;
            updated_cost += sqrt(pow(connected_port_x - main_port_x,2) + pow(connected_port_y - main_port_y,2));
        }

        //cout<<"current and updated cost "<<curr_cost<<" "<<updated_cost<<endl;
        
        if(curr_cost < updated_cost){ //reverse changes
            blk[curr_block] = reverse_block;
            port_coords = reverse_port_coords;
            port_connections = reverse_port_connections;
        }
    }
    else{
        return;
    }    
    
}
