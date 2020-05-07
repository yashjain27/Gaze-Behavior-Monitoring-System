#include "systemc.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

SC_MODULE(mobile){
	sc_in<bool> clock;
	sc_in<bool> free;
	sc_in<bool> incoming;
	sc_out<bool> outgoing;
	sc_out<bool> begin;
	sc_out<bool> end;
	sc_out<bool> rx_begin;
	sc_out<bool> rx_end;
	sc_signal<sc_int<4> > ROI, tupleROI;
	sc_signal<bool> packet_counter{"packet_counter"};

	//Variables
	int image_tracker, tuple_counter, x, y;
	int size[5] = {5, 4, 4, 7, 7};
	int index, i;
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

	//File stuff
	string str;
	ifstream in;

	void sensor(){
		if(!continueMob) return;

		if(image_counter == 0) return;

		getline(in, str);
		x = atoi(str.substr(0,3).c_str());
    y = atoi(str.substr(4,7).c_str());
		cout << endl << name() << endl;
    cout << "X: " << x << " Y: " << y << endl;
		convert(); //Send as a parameter double int array
	}

	void convert(){
		tupleROI = ROI; //Assign old value of ROI to tupleROI
		if(index == 1){
			ROI = -1;
			for(i = 0; i < size[index - 1]; i++){
				if(image1[i][1] <= x && image1[i][3] >= x & image1[i][2] <= y & image1[i][4] >= y){
					cout << "ROI: " << i << endl;
					ROI = i;
					break;
				}
			}
		}else if(index == 2){
			ROI = -1;
			for(i = 0; i < size[index - 1]; i++){
				if(image2[i][1] <= x & image2[i][3] >= x & image2[i][2] <= y & image2[i][4] >= y){
					cout << "ROI: " << i << endl;
					ROI = i;
					break;
				}
			}
		}else if(index == 3){
			ROI = -1;
			for(i = 0; i < size[index - 1]; i++){
				if(image3[i][1] <= x & image3[i][3] >= x & image3[i][2] <= y & image3[i][4] >= y){
					cout << "ROI: " << i << endl;
					ROI = i;
					break;
				}
			}
		}else if(index == 4){
			ROI = -1;
			for(i = 0; i < size[index - 1]; i++){
				if(image4[i][1] <= x & image4[i][3] >= x & image4[i][2] <= y & image4[i][4] >= y){
					cout << "ROI: " << i << endl;
					ROI = i;
					break;
				}
			}
		}else if(index == 5){
			ROI = -1;
			for(i = 0; i < size[index - 1]; i++){
				if(image5[i][1] <= x & image5[i][3] >= x & image5[i][2] <= y & image5[i][4] >= y){
					cout << "ROI: " << i << endl;
					ROI = i;
					break;
				}
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
		cout << "time: " << sc_simulation_time() << endl << endl;

		if(tuple_counter != 19){
			tuple_counter++;
			cout << "Tuple counter1: " << tuple_counter << endl;
			packet_counter = 0;
		}else{
			cout << "Tuple counter2: " << tuple_counter << endl;
			tuple_counter = 0;
			packet_counter = 1;
		}
	}

	void prc_transmit(){
		while(true){
			begin = 0;
			end = 0;
			outgoing = 0;
			cout << "Before entering packet_counter " << name() << endl;
			wait(packet_counter.posedge_event());
			bool success;
			while(true){
				continueMob = false;
				if(!free){
					int time = (rand() % 5000);
					cout << "Random time of " << name() << " is: " << time << endl;
					wait(sc_time(time, SC_MS));
				}else{
					success = false;
					while(success == false){
						outgoing = 1;
						if(incoming){
							begin = 1;
							cout << "Send Begin High for " << name() << endl;
							wait(sc_time(packet_duration, SC_MS));
							end = 1;
							cout << "Send End High for " << name() << endl;
							wait(incoming.negedge_event());
							success = true;
						}else{
							int time = (rand() % 5000);
							cout << "Wait Random time of " << name() << " is: " << time << endl;
							wait(time, SC_MS);
						}
					}
				}
				continueMob = true;
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

			//If rx_packet_counter == packet_max - 1, increment image_counter
			if(rx_packet_counter == packet_max - 1){
				image_counter++;
			}
		}
	}

	SC_HAS_PROCESS(mobile);

	mobile(sc_module_name name_, int bw, int packetsize) :
    sc_module(name_), in("gaze_out.txt"){
			index = 1, i = 0, tuple_counter = 0, x = 0, y = 0, continueMob = true;
			packet_max = 8000/packetsize;
			packet_duration = ((double) packetsize / bw) * 1000;
			rx_packet_counter = 0;
			image_counter = 0;
			SC_METHOD(sensor);
			sensitive << clock.pos();
			SC_THREAD(rx_mobile);
			SC_THREAD(prc_transmit);
		}
};
