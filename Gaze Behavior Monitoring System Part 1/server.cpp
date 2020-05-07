#include "systemc.h"

SC_MODULE(server){
	sc_in<bool> begin1{"begin1"}, begin2{"begin2"}, begin3{"begin3"};
	sc_in<bool> end1{"end1"}, end2{"end2"}, end3{"end3"};
	sc_in<bool> incoming1{"incoming1"}, incoming2{"incoming2"}, incoming3{"incoming3"};
	sc_out<bool> free{"free"};
	sc_out<bool> outgoing1{"outgoing1"};
	sc_out<bool> outgoing2{"outgoing2"};
	sc_out<bool> outgoing3{"outgoing3"};

	//Variables
	bool serverIsFree; //Internal server flag

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

	SC_CTOR(server){
		SC_THREAD(monitor);
	}
};
