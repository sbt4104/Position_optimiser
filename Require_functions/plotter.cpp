#include "port_structure.hpp"
#include "plotter.hpp"

using namespace std;

void save_data_to_plot(string outputfile,vector<block> blk){
    ofstream gpOut;
    gpOut.open(outputfile);
    for(int i=0; i<blk.size();i++){

        for(int j=0;j<blk[i].total_ports;j++){
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
    }
    gpOut.close();
}
