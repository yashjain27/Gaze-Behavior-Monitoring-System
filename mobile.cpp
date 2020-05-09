#include "systemc.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

SC_MODULE(mobile){
	sc_in<bool> clock;
	sc_vector<sc_in<bool> > free{"free",2};
	sc_in<bool> incoming{"incoming"};
	sc_out<bool> outgoing{"outgoing"};
	sc_out<bool> begin{"begin"};
	sc_out<bool> end{"end"};
	sc_in<bool> rx_begin{"rx_begin"};
	sc_in<bool> rx_end{"rx_end"};
	sc_signal<sc_int<4> > ROI, tupleROI;
	sc_signal<bool> packet_counter{"packet_counter"};

	//Variables
	int image_tracker, tuple_counter, x, y;
	int size[5] = {5, 4, 4, 7, 7};
	int packet_size, i;
	int image1 [5][5] = {
		{1, 50, 20, 400, 320},
		{2, 50, 370, 450, 1000},
		{3, 470, 20, 600, 900},
		{4, 670, 40, 950, 550},
		{5, 680, 700, 1000, 1000}
	};
	int image2 [4][5] = {
		{1, 10, 10, 150, 700},
		{2, 300, 10, 980, 250},
		{3, 300, 270, 980, 700},
		{4, 10, 740, 950, 1000}
	};
	int image3 [4][5] = {
		{1, 10, 10, 260, 900},
		{2, 270, 10, 520, 1000},
		{3, 570, 20, 700, 950},
		{4, 730, 10, 950, 950}
	};
	int image4 [7][5] = {
		{1, 10, 10, 160, 1000},
		{2, 170, 10, 220, 1000},
		{3, 230, 10, 380, 1000},
		{4, 390, 10, 540, 1000},
		{5, 550, 10, 700, 1000},
		{6, 710, 10, 860, 1000},
		{7, 870, 10, 1010, 1000}
	};
	int image5 [7][5] = {
		{1, 10, 10, 1000, 160},
		{2, 10, 170, 1000, 220},
		{3, 10, 230, 1000, 380},
		{4, 100, 390, 1000, 540},
		{5, 10, 550, 1000, 700},
		{6, 10, 710, 1000, 860},
		{7, 10, 870, 1000, 1010}
	};
	//int packet [20][3];
	bool continueMob;
	int packet_max;
	int rx_packet_counter;
	int image_counter;
	double packet_duration;
	string name;

	//File stuff
	string str;
	ifstream in;

	void sensor(){
		if(!continueMob) return;

		if(!(image_counter > 0 && sc_time_stamp().to_seconds() > 199.99)) return;

		if(sc_time_stamp().to_seconds() > 249.99){
			image_tracker = 4;
		}else if(sc_time_stamp().to_seconds() > 229.99){
			image_tracker = 3;
		}else if(sc_time_stamp().to_seconds() > 224.99){
			image_tracker = 2;
		}else if(sc_time_stamp().to_seconds() > 219.99){
			image_tracker = 1;
		}

		getline(in, str);
		if(!str.length() == 0){
			x = atoi(str.substr(0,3).c_str());
	    y = atoi(str.substr(4,str.length()).c_str());
		}
    //cout << "X: " << x << " Y: " << y << endl;
		convert(); //Send as a parameter double int array
	}

	void convert(){
		tupleROI = ROI; //Assign old value of ROI to tupleROI
		int temp[size[image_tracker]][5];
		switch (image_tracker) {
			case 0:
			for(int i = 0; i < size[image_tracker]; i++){
        for( int j = 0; j < 5; j++){
            temp[i][j] = image1[i][j];
        }
    	}
			break;
			case 1:
			for(int i = 0; i < size[image_tracker]; i++){
        for( int j = 0; j < 5; j++){
            temp[i][j] = image2[i][j];
        }
    	}
			break;
			case 2:
			for(int i = 0; i < size[image_tracker]; i++){
        for( int j = 0; j < 5; j++){
            temp[i][j] = image3[i][j];
        }
    	}
			break;
			case 3:
			for(int i = 0; i < size[image_tracker]; i++){
        for( int j = 0; j < 5; j++){
            temp[i][j] = image4[i][j];
        }
    	}
			break;
			case 4:
			for(int i = 0; i < size[image_tracker]; i++){
        for( int j = 0; j < 5; j++){
            temp[i][j] = image5[i][j];
        }
    	}
			break;
		}

		for(i = 0; i < size[image_tracker]; i++){
			if(temp[i][1] <= x && temp[i][3] >= x & temp[i][2] <= y & temp[i][4] >= y){
				//cout << "ROI: " << i << endl;
				ROI = i;
				break;
			}
		}
		compress();
	}

	void compress(){
		if(ROI != tupleROI){
			packetize();
		}
	}

	void packetize(){
		if(tuple_counter != 19){
			tuple_counter++;
			storage();
			packet_counter = 0;
		}else{
			storage();
			tuple_counter = 0;
			packet_counter = 1;
		}
	}

	void prc_transmit(){
		while(true){
			begin = 0;
			end = 0;
			outgoing = 0;
			wait(packet_counter.posedge_event());
			bool success;
			while(true){
				continueMob = false;
				if(!free[0] && !free[1]){
					int time = (rand() % 5000);
					storage();
					wait(sc_time(time, SC_MS));
				}else{
					success = false;
					while(success == false){
						outgoing = 1;
						if(incoming){
							begin = 1;
							storage();
							wait(sc_time(packet_duration, SC_MS));
							end = 1;
							storage();
							wait(incoming.negedge_event());
							success = true;
						}else{
							int time = (rand() % 5000);
							storage();
							wait(time, SC_MS);
						}
					}
				}
				continueMob = true;
				image_counter--; //Decrement image counter
				storage();
				break;
			}
		}
	}

	void rx_mobile(){
		while(true){
			//If rx_begin signal received from the server increment rx_packet_counter
			wait(rx_begin.posedge_event());
			rx_packet_counter++;
			wait(rx_end.posedge_event());

			storage();
			if(rx_packet_counter == packet_max){
				image_counter++;
				rx_packet_counter = 0;
			}
		}
	}

	void storage(){
		int tuples = tuple_counter*3*64;
		int rx_packets = rx_packet_counter * packet_size;
		int images = image_counter * 8000;
		if(name == "mob1" ){
			//cout << name << " Storage= " << (tuples + rx_packets + images) << " @" << sc_time_stamp().to_seconds() << endl;
			//cout << (tuples + rx_packets + images) << endl;
			cout << sc_time_stamp().to_seconds() << endl;
		}
	}

	SC_HAS_PROCESS(mobile);
	mobile(sc_module_name name_, int bw, int packetsize) :
    sc_module(name_), in("gaze_out.txt"){
			name = name_;
			i = 0, tuple_counter = 0, x = 0, y = 0, continueMob = true;
			image_tracker = 0;
			packet_size = packetsize;
			packet_max = 8000/packetsize;
			packet_duration = (3.84 / bw) * 1000;
			rx_packet_counter = 0;
			image_counter = 0;
			SC_METHOD(sensor);
			sensitive << clock.pos();
			SC_THREAD(rx_mobile);
			SC_THREAD(prc_transmit);
		}
};
