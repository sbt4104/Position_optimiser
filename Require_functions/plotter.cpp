#include "port_structure.hpp"
#include "plotter.hpp"

using namespace std;

void save_data_to_plot(string outputfile,vector<block> blk){
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
