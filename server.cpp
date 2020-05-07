#include "systemc.h"

SC_MODULE(server){
	sc_in<bool> begin1{"begin1"}, begin2{"begin2"}, begin3{"begin3"};
	sc_in<bool> end1{"end1"}, end2{"end2"}, end3{"end3"};
	sc_in<bool> incoming1{"incoming1"}, incoming2{"incoming2"}, incoming3{"incoming3"};
	sc_in<bool> clk;
	sc_out<bool> free{"free"};
	sc_out<bool> outgoing1{"outgoing1"};
	sc_out<bool> outgoing2{"outgoing2"};
	sc_out<bool> outgoing3{"outgoing3"};
	sc_out<bool> tx_begin[3];//tx_begin1, tx_begin2, tx_begin3;
	sc_out<bool> tx_end[3];//tx_end1, tx_end2, tx_end3;

	//Variables
	bool serverIsFree; //Internal server flag
	bool transmit;
	double packet_duration;
	int image[5] = {200, 220, 225, 230, 250}; //sc_time image[5] = {(200,SC_SEC), (220,SC_SEC), (225,SC_SEC), (230,SC_SEC), (200,SC_SEC)};
	int image_tracker;
	int preTime;
	int delta;
	int totalPackets;
	int packetsPerMobile;

	void monitor(){
		free = true;
		serverIsFree = true;
		outgoing1 = false;
		outgoing2 = false;
		outgoing3 = false;

		while(true){
			wait(incoming1.posedge_event() | incoming2.posedge_event() | incoming3.posedge_event());

			//Mobile 1
			if(incoming1 == true){
				cout << "Mobile 1 received." << endl;
				while(!serverIsFree); // Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing1 = true;
				cout << "outgoing1 sent." << endl;
				free = false;
				serverIsFree = false;
				wait(begin1.posedge_event());
				cout << "Begin1 @" << sc_simulation_time() << endl;
				wait(end1.posedge_event());
				cout << "End1 @" << sc_simulation_time() << endl;
				free = true;
				serverIsFree = true;
				outgoing1 = false;
			}

			//Mobile 2
			if(incoming2 == true){
				cout << "Mobile 2 received." << endl;
				while(!serverIsFree);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing2 = true;
				cout << "outgoing2 sent." << endl;
				free = false;
				serverIsFree = false;
				wait(begin2.posedge_event());
				cout << "Begin2 @" << sc_simulation_time() << endl;
				wait(end2.posedge_event());
				cout << "End2 @" << sc_simulation_time() << endl;
				free = true;
				serverIsFree = true;
				outgoing2 = false;
			}

			//Mobile 3
			if(incoming3 == true){
				cout << "Mobile 3 received." << endl;
				while(!serverIsFree);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing3 = true;
				cout << "outgoing3 sent." << endl;
				free = false;
				serverIsFree = false;
				wait(begin3.posedge_event());
				cout << "Begin3 @" << sc_simulation_time() << endl;
				wait(end3.posedge_event());
				cout << "End3 @" << sc_simulation_time() << endl;
				free = true;
				serverIsFree = true;
				outgoing3 = false;
			}
		}
	}

	void timer(){
		double time = sc_time_stamp().to_seconds();
		if(time == image[image_tracker] - preTime){
			cout << "Time2: " << time << endl;
			image_tracker++;
			transmit = true;
		}
	}

	void tx_server(){
		int totalPacketCounter = 0; //Count how many packets have been sent
		int packetsPerMobileCounter = 0; //Counts packets up until a full image is transferred and loops back to 0 after transmission
		int mobileNum = 0; //The mobile which the packets are being sent to{0,1,2}

		while(true){
			tx_begin[0] = 0;
			tx_begin[1] = 0;
			tx_begin[2] = 0;
			tx_end[0] = 0;
			tx_end[1] = 0;
			tx_end[2] = 0;
			while(!transmit); //Don't proceed if transmit == false

			if(serverIsFree){
				free = false;
				serverIsFree = false;
				//Send a packets to Mobile 1,2,3
				tx_begin[mobileNum] = 1;
				wait(sc_time(packet_duration, SC_MS));
				tx_end[mobileNum] = 1;
				totalPacketCounter++;
				packetsPerMobileCounter++;
				if(packetsPerMobileCounter == totalPackets){
					packetsPerMobileCounter = 0;
					mobileNum++; //After image tx, increment to next mobile
					mobileNum = mobileNum % 3; //3 mobiles
				}

				//Server is free
				free = true;
				serverIsFree = true;

				// If all packets have been sent, stop transmission
				if(totalPackets == totalPacketCounter) transmit = false;
				wait(sc_time(delta, SC_MS));
			}
		}
	}

	SC_HAS_PROCESS(server);

	server(sc_module_name name_, int bw, int packetsize){
			transmit = false;
			totalPackets = 5 * 3 * (8000 / packetsize);
			packetsPerMobile = 8000 / packetsize;
			packet_duration = ((double) packetsize / bw) * 1000;
			image_tracker = 0;
			preTime = 70;
			delta = 10;
			cout << "Server packet_duration: " << packet_duration <<  endl;
			SC_METHOD(tx_server);
			sensitive << clk.pos();
			SC_THREAD(tx_server);
			SC_THREAD(monitor);
		}

};
