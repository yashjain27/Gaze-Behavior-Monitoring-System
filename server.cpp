#include "systemc.h"

SC_MODULE(server){
	sc_in<bool> begin1{"begin1"}, begin2{"begin2"}, begin3{"begin3"};
	sc_in<bool> end1{"end1"}, end2{"end2"}, end3{"end3"};
	sc_in<bool> incoming1{"incoming1"}, incoming2{"incoming2"}, incoming3{"incoming3"};
	sc_in<bool> clk;
	sc_vector<sc_out<bool> > free{"free",2};
	sc_out<bool> outgoing1{"outgoing1"};
	sc_out<bool> outgoing2{"outgoing2"};
	sc_out<bool> outgoing3{"outgoing3"};
	sc_vector<sc_out<bool> > tx_begin{"tx_begin",3};
	//sc_out<bool> tx_begin[3];//tx_begin1, tx_begin2, tx_begin3;
	sc_vector<sc_out<bool> > tx_end{"tx_end",3};
	//sc_out<bool> tx_end[3];//tx_end1, tx_end2, tx_end3;
	sc_event startTransmission;

	//Variables
	bool serverIsFree; //Internal server flag
	double packet_duration;
	double image[5] = {200, 220, 225, 230, 250}; //sc_time image[5] = {(200,SC_SEC), (220,SC_SEC), (225,SC_SEC), (230,SC_SEC), (200,SC_SEC)};
	int image_tracker;
	double preTime;
	int delta;
	int totalPackets;
	int packetsPerMobile;

	void monitor(){
		free[0] = true;
		serverIsFree = true;
		outgoing1 = false;
		outgoing2 = false;
		outgoing3 = false;

		while(true){
			wait(incoming1.posedge_event() | incoming2.posedge_event() | incoming3.posedge_event());

			//Mobile 1
			if(incoming1 == true){
				//cout << "Mobile 1 received." << endl;
				// Wait for the server to free up
				while(!serverIsFree){
					wait(sc_time(delta/2, SC_MS));
				};

				//Now that the server has freed up, proceed
				free[0] = false;
				serverIsFree = false;
				outgoing1 = true;
				//cout << "Outgoing1 sent." << endl;
				wait(begin1.posedge_event());
				//cout << "Begin1 @" << sc_time_stamp().to_seconds() << endl;
				wait(end1.posedge_event());
				//cout << "End1 @" << sc_time_stamp().to_seconds() << endl << endl;
				free[0] = true;
				serverIsFree = true;
				outgoing1 = false;
			}
			wait(sc_time(delta*2, SC_MS));

			//Mobile 2
			if(incoming2 == true){
				//cout << "Mobile 2 received." << endl;
				//Wait for the server to free up
				while(!serverIsFree){
				 	wait(sc_time(delta/2, SC_MS));
				}

				//Now that the server has freed up, proceed
				free[0] = false;
				serverIsFree = false;
				outgoing2 = true;
				//cout << "Outgoing2 sent." << endl;
				wait(begin2.posedge_event());
				//cout << "Begin2 @" << sc_time_stamp().to_seconds() << endl;
				wait(end2.posedge_event());
				//cout << "End2 @" << sc_time_stamp().to_seconds() << endl << endl;
				free[0] = true;
				serverIsFree = true;
				outgoing2 = false;
			}
			wait(sc_time(delta*2, SC_MS));

			//Mobile 3
			if(incoming3 == true){
				//cout << "Mobile 3 received." << endl;
				// Wait for the server to free up
				while(!serverIsFree){
					wait(sc_time(delta/2, SC_MS));
				}

				//Now that the server has freed up, proceed
				free[0] = false;
				serverIsFree = false;
				outgoing3 = true;
				//cout << "Outgoing3 sent." << endl;
				wait(begin3.posedge_event());
				//cout << "Begin3 @" << sc_time_stamp().to_seconds() << endl;
				wait(end3.posedge_event());
				//cout << "End3 @" << sc_time_stamp().to_seconds() << endl << endl;
				free[0] = true;
				serverIsFree = true;
				outgoing3 = false;
			}
			wait(sc_time(delta*2, SC_MS));
		}
	}

	void timer(){
		double time = sc_time_stamp().to_seconds();
		////cout << "Time: " << time << endl;
		if(time > 99.999 && time < 100.001){
			//cout << "Time2: " << time << endl;
			image_tracker++;
			startTransmission.notify();
		}
	}

	void tx_server(){
		int totalPacketCounter = 0; //Count how many packets have been sent
		int packetsPerMobileCounter = 0; //Counts packets up until a full image is transferred and loops back to 0 after transmission
		int mobileNum = 0; //The mobile which the packets are being sent to{0,1,2}
		free[1] = true;

		//cout << "Pre-transmit!" <<  endl;
		wait(startTransmission); //Don't proceed if transmit == false
		//cout << "Transmission started!" <<  endl;

		while(true){
			if(serverIsFree){
				////cout << "Begin out true" <<  endl;
				free[1] = false;
				serverIsFree = false;

				//Send a packets to Mobile 1,2,3
				tx_begin[mobileNum] = 1;
				wait(sc_time(packet_duration, SC_MS));
				////cout << "End out true" <<  endl;
				tx_begin[mobileNum] = 0;
				tx_end[mobileNum] = 1;
				totalPacketCounter++;
				packetsPerMobileCounter++;
				if(packetsPerMobileCounter == totalPackets/15){
					packetsPerMobileCounter = 0;
					mobileNum++; //After image tx, increment to next mobile
					mobileNum = mobileNum % 3; //3 mobiles
				}

				//Server is free
				free[1] = true;
				serverIsFree = true;

				// If all packets have been sent, stop transmission
				if(totalPackets == totalPacketCounter) break;
				wait(sc_time(delta, SC_MS));
				tx_end[mobileNum] = 0;
				//cout << "Time: " << sc_time_stamp().to_seconds() << " Packets sent: " << packetsPerMobileCounter << " totalPackets: " << totalPacketCounter << endl;
			}else{
				wait(sc_time(delta, SC_MS));
			}
		}
		//cout << "All " << totalPackets << " packets sent" << endl;
	}

	SC_HAS_PROCESS(server);

	server(sc_module_name name_, int bw, int packetsize){
			totalPackets = 5 * 3 * (8000 / packetsize);
			packetsPerMobile = 8000 / packetsize;
			packet_duration = ((double) packetsize / bw) * 1000;
			image_tracker = 0;
			preTime = 70;
			delta = 10;
			//cout << "Server packet_duration: " << packet_duration <<  endl;
			SC_METHOD(timer);
			sensitive << clk.pos();
			SC_THREAD(tx_server);
			SC_THREAD(monitor);
		}

};
