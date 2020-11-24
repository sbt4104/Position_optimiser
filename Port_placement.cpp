#include<bits/stdc++.h>
#include "Require_functions/port_structure.hpp"
#include "Require_functions/evaluate_placer.hpp"
#include "Require_functions/moves_placer.hpp"
#include "Require_functions/plotter.hpp"

using namespace std;

template <size_t N>
void splitString(string (&arr)[N], string str)
{
    int n = 0;
    istringstream iss(str);
    for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
        arr[n] = *it;
}

void save_data(string outputfile, vector<block> blk){
    ofstream gpOut;
    gpOut.open(outputfile);
    for(int i=0; i<blk.size();i++){
        /*
        cout<<"block name "<<blk[i].block_name<<endl
            <<"block number "<<blk[i].block_number<<endl
            <<"total ports "<<blk[i].total_ports<<endl
            <<"total edges "<<blk[i].total_edges<<endl
            <<"total placed ports "<<blk[i].total_placed_ports<<endl;
        */
        for(int j=0;j<blk[i].total_ports;j++){
            /*
            cout<<"curr port name "<<blk[i].port_of_block[j].port_name<<endl
            <<"curr port width "<<blk[i].port_of_block[j].width<<endl
            <<"curr port position "<<blk[i].port_of_block[j].port_position<<endl
            <<"curr edge number "<<blk[i].port_of_block[j].edge_number<<endl;
            */
            
            float x0,y0,x1,y1,x2,y2,x3,y3;
            x0 = blk[i].port_of_block[j].left_x_coord;
            y0 = blk[i].port_of_block[j].left_y_coord;
            if(x0 ==0 && y0 == 0){
                cout<<"port rejected"<<blk[i].port_of_block[j].port_name<<endl;
                continue;
            }
            bool rising_edge = check_if_rising_edge(j,blk[i]);
            if(blk[i].port_of_block[j].edge_number % 2){ // parallel to x axis
                if(rising_edge){
                    gpOut << blk[i].port_of_block[j].port_name <<", {"<<(int)x0<<" "<<(int)y0<<"} {"<< \
                    (int)(x0+blk[i].port_of_block[j].width)<<" "<<(int)y0<<"} {"<< \
                    (int)(x0+blk[i].port_of_block[j].width)<<" "<<(int)(y0-20)<<"} {"<< \
                    (int)x0<<" "<<(int)(y0-20)<<"}\n";                                
                }
                else{
                    gpOut << blk[i].port_of_block[j].port_name <<", {"<<(int)x0<<" "<<(int)y0<<"} {"<< \
                    (int)x0<<" "<<(int)(y0+20)<<"} {" << \
                    (int)(x0-blk[i].port_of_block[j].width)<<" "<<(int)(y0+20)<<"} {" << \
                    (int)(x0-blk[i].port_of_block[j].width)<<" "<<(int)y0<<"}\n";
                }
            }
            else{ //parallel to y axis
                if(rising_edge){
                    gpOut << blk[i].port_of_block[j].port_name <<", {"<<(int)x0<<" "<<(int)y0<<"} {"<< \
                    (int)x0<<" "<<(int)(y0+blk[i].port_of_block[j].width)<<"} {" << \
                    (int)(x0+20)<<" "<<(int)(y0+blk[i].port_of_block[j].width)<<"} {"<< \
                    (int)(x0+20)<<" "<<(int)y0<<"}\n";
                }
                else{
                    gpOut << blk[i].port_of_block[j].port_name <<", {"<<(int)x0<<" "<<(int)y0<<"} {"<< \
                    (int)x0<<" "<<(int)(y0-blk[i].port_of_block[j].width)<<"} {"<< \
                    (int)(x0-20)<<" "<<(int)(y0-blk[i].port_of_block[j].width)<<"} {"<<(int)(x0-20)<<" "<<(int)y0<<"}\n";                
                }
            }
        }
        
        /*
        for(int j=0;j<blk[i].total_edges;j++){
            cout<<"curr edge used "<<blk[i].edge_of_block[j].used_length <<endl
            <<"curr edge total "<<blk[i].edge_of_block[j].total_length<<endl
            <<"curr edge total_placed "<<blk[i].edge_of_block[j].total_placed<<endl;
        }
        cout<<endl<<endl;
        */
    }
    gpOut.close();
}

int main() {
    //create a pseudo data to workout a solution
    int seed = time(0);
    //cout<<"enter the seed please, later it will be automated\n";
    //cin>>seed;
    cout<<"seed: "<<seed<<endl;
    srand(seed);
    map<string,int> map_block;
    map<string,int> seen_ports;
    map<string,pair<float,float> > port_coords;
    map<string, vector<string>> port_connections;
    vector<block> blk;
    vector<edge> edg;
    vector<port> prt;
    
    fstream newfile;
    
    
    newfile.open("tests/block_3.csv",ios::in); //parse blocks file
    int skipfirst = 1;
    int block_index = 0;
    
    if (newfile.is_open()){   //checking whether the file is open
        string tp;
        while(getline(newfile, tp)){ //read data from file object and put it into string.
            if(skipfirst){
                skipfirst--;
                continue;
            }
        string arr[200];    
        if(tp.length()==0)
            continue;
        splitString(arr, tp);
        //cout<<"tp "<<tp<<endl;
        string block_name;
        vector<int> block_coord_vec;
        block_coord_vec.clear();

        for(int tu =0;tu<200;tu++){
            if(arr[tu].length()==0)
                continue;
        
            string curr = "";
            //cout<<"here "<<arr[tu]<<" "<<arr[tu].length()<<endl;
            if(tu%2){
                for(int i=1;i<arr[tu].length();i++)
                    curr += arr[tu][i];
            }
            else{
                for(int i=0;i<arr[tu].length()-1;i++)
                    curr += arr[tu][i];                     
            }

            bool is_integer = true;
            for(int i=0;i<curr.length();i++){
                if(curr[i]>'9' || curr[i]<'0')
                    is_integer = false;
            }
            
            int num = 0;
            if(is_integer){
                for(int i=0;i<curr.length();i++)
                    num = num*10 + (curr[i]-'0');
                block_coord_vec.push_back(num);
            }

            if(!is_integer)
                block_name = curr;
            //cout<<curr<<" "<<num<<endl;
            }
            
            prt.clear();
            edg.clear();
            float size_of_edge = 0;
            for(int i=0;i<((block_coord_vec.size()/2)-1);i++){
                //cout<<block_coord_vec[2*i]<<" "<<block_coord_vec[2*i +1]<<" ";
                size_of_edge = abs(block_coord_vec[2*i] - block_coord_vec[2*i+2]) + \
                                abs(block_coord_vec[2*i + 1] - block_coord_vec[2*i + 3]); 
                //cout<<"size of edge "<<size_of_edge<<endl;
                edg.push_back({0,(float)(size_of_edge),0});
            }

            //size of last edge
            size_of_edge = abs(block_coord_vec[block_coord_vec.size()-2] - block_coord_vec[0]) + \
                            abs(block_coord_vec[block_coord_vec.size()-1] - block_coord_vec[1]); 
            //cout<<"size of edge "<<size_of_edge<<endl;
            edg.push_back({0,(int)(size_of_edge),0});
            //cout<<endl;
            map_block[block_name] = block_index;
            int num_edges_in_block = block_coord_vec.size()/2;
            blk.push_back({block_name,block_coord_vec,edg,prt,block_index,0,num_edges_in_block,0});
            block_index++;
        }
        newfile.close(); //close the file object.
    }


    //Parse connectivity file
    fstream newfile1;
    string arr1[200];
    newfile1.open("tests/connectivity_3.csv",ios::in); //open a file to perform read operation using file object
    int skipfirst1 = 1;
    
    if (newfile1.is_open()){   //checking whether the file is open
        string tp;
        while(getline(newfile1, tp)){ //read data from file object and put it into string.
            if(skipfirst1){
                skipfirst1--;
                continue;
            }

            if(tp.length()==0)
                continue;
            splitString(arr1, tp);

            vector<string> vec_parent_child;
            vector<float> width_freq;
            for(int tu =0;tu<90;tu++){
                if(arr1[tu].length()==0)
                continue;

                string parent="",child="";
                int i=0;
                if(tu<2){
                    while(i<(arr1[tu].length()-1) && arr1[tu][i]!='.'){
                        parent += arr1[tu][i];
                        i++;
                    }
                    i++;
                    
                    while(i<(arr1[tu].length()-1)){
                        child += arr1[tu][i];
                        i++;
                    }
                    vec_parent_child.push_back(parent);
                    vec_parent_child.push_back(child);
                    continue;
                }
                
                int num = 0;
                for(int i=0;i<arr1[tu].length();i++)
                    if(arr1[tu][i] <='9' && arr1[tu][i] >= '0')
                        num = num*10 + (arr1[tu][i] - '0');
                width_freq.push_back(num);    
            }

            float this_port_width = width_freq[0];
            
            //connections of child1
            if(port_connections.find(vec_parent_child[1]) == port_connections.end()){
                vector<string> temp_connections;
                temp_connections.push_back(vec_parent_child[3]);
                port_connections[vec_parent_child[1]] = temp_connections;
            }
            else{
                vector<string> temp_connections = port_connections[vec_parent_child[1]];
                temp_connections.push_back(vec_parent_child[3]);
                port_connections[vec_parent_child[1]] = temp_connections;
            }
            
            //connections of child2
            if(port_connections.find(vec_parent_child[3]) == port_connections.end()){
                vector<string> temp_connections;
                temp_connections.push_back(vec_parent_child[1]);
                port_connections[vec_parent_child[3]] = temp_connections;
            }
            else{
                vector<string> temp_connections = port_connections[vec_parent_child[3]];
                temp_connections.push_back(vec_parent_child[1]);
                port_connections[vec_parent_child[3]] = temp_connections;
            }
            
            if(!seen_ports.count(vec_parent_child[1])){
                int this_block_index1 = map_block[vec_parent_child[0]];
                vector<port> temp_port1 = blk[this_block_index1].port_of_block;
                temp_port1.push_back({vec_parent_child[1],0.14*this_port_width,0,-1,0,0});
                blk[this_block_index1].port_of_block = temp_port1;
                blk[this_block_index1].total_ports++;
                seen_ports[vec_parent_child[1]]++;
            }

            if(!seen_ports.count(vec_parent_child[3])){                           
                int this_block_index2 = map_block[vec_parent_child[2]];
                vector<port> temp_port2 = blk[this_block_index2].port_of_block;
                temp_port2.push_back({vec_parent_child[3],0.14*this_port_width,0,-1,0,0});
                blk[this_block_index2].port_of_block = temp_port2;
                blk[this_block_index2].total_ports++;
                seen_ports[vec_parent_child[3]]++;
            }

        }
        newfile1.close(); //close the file object.
    }

        
        
    /*vector<int> coord = {300 ,350 ,300 ,655 ,610 ,655 ,610 ,350};
    prt.push_back({"port_abc0_1",0.14*700,0,-1,0,0});
    prt.push_back({"port_abc0_2",0.14*400,1,-1,0,0});
    edg.push_back({0,305,0});
    edg.push_back({0,310,0});
    edg.push_back({0,305,0});
    edg.push_back({0,310,0});
    blk.push_back({"abc_0",coord,edg,prt,0,2,4,0});
    //cout<<blk[0].edge_block.size()<<endl;
    
    edg.clear();
    prt.clear();
    coord.clear();
    coord = {300, 950, 300, 1150, 600, 1150, 600 ,950};
    edg.push_back({0,200,0});
    edg.push_back({0,300,0});
    edg.push_back({0,200,0});
    edg.push_back({0,300,0});
    prt.push_back({"port_abc1_1",0.14*400,0,-1,0,0});
    blk.push_back({"abc_1",coord,edg,prt,1,1,4,0});
    
    edg.clear();
    prt.clear();
    coord.clear();
    coord = {850, 950, 850, 1150, 1150, 1150, 1150, 950};
    edg.push_back({0,200,0});
    edg.push_back({0,300,0});
    edg.push_back({0,200,0});
    edg.push_back({0,300,0});
    blk.push_back({"xyz_0",coord,edg,prt,2,0,4,0});
    
    edg.clear();
    prt.clear();
    coord.clear();
    coord = {850, 350, 850, 650, 1150, 650, 1150, 350};
    prt.push_back({"port_xyz1_1",0.14*700,0,-1,0,0});
    edg.push_back({0,300,0});
    edg.push_back({0,300,0});
    edg.push_back({0,300,0});
    edg.push_back({0,300,0});
    blk.push_back({"xyz_1",coord,edg,prt,3,1,4,0});
    //cout<<blk[3].edge_block.size()<<endl;
    */
    
    //placement for each block without considering nets
    for(int i=0;i<blk.size();i++){
        int total_num_ports = blk[i].total_ports;
        int total_num_edges = blk[i].total_edges;
        for(int index_port = 0; index_port < total_num_ports; index_port++){
            
            //check until you get a valid positiom
            int flag = 0;
            
            //find position for current edge
            int pos_edge = rand()%total_num_edges;
            int counter_num_edges = total_num_edges;        
            while(!flag && counter_num_edges>=0){
    
                //check if edge can accomodate this port
                float curr_edge_used = blk[i].edge_of_block[pos_edge].used_length;
                float curr_edge_cap = blk[i].edge_of_block[pos_edge].total_length;
                float curr_edge_left = curr_edge_cap - curr_edge_used;
                
                if(curr_edge_left >= blk[i].port_of_block[index_port].width){ //found a legal position
                    add_port_to_block(blk[i], index_port, pos_edge, port_coords);
                    //blk[i].edge_of_block[pos_edge].total_placed++;
                    //blk[i].total_placed_ports++;
                    flag = 1;
                }
                else{
                    //check for next position
                    pos_edge = (pos_edge + 1)%total_num_edges;
                }
                counter_num_edges--;
            }
        }
    }
    
    
    int moves = 50000;
    while(moves--){
        //three valid operations
        //1] swap 2 elements of the same edge
        //2] move block to another edge
        //3] swap 2 elements of 2 different edges
        
        swap_on_same_edge(blk, port_coords, port_connections);
        move_port_to_other_edge(blk, port_coords, port_connections);
    }
    
    string outputfile = "output/dataset_3_output.csv";
    
    save_data_to_plot(outputfile,blk);


    return 0;
}

