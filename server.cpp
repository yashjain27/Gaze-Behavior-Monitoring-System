#include "systemc.h"

SC_MODULE(server){
	sc_in<bool> begin1{"begin1"}, begin2{"begin2"}, begin3{"begin3"};
	sc_in<bool> end1{"end1"}, end2{"end2"}, end3{"end3"};
	sc_in<bool> incoming1{"incoming1"}, incoming2{"incoming2"}, incoming3{"incoming3"};
	sc_out<bool> free{"free"};
	sc_out<bool> outgoing1{"outgoing1"};
	sc_out<bool> outgoing2{"outgoing2"};
	sc_out<bool> outgoing3{"outgoing3"};

	void monitor(){
		free = true;
		outgoing1 = false;
		outgoing2 = false;
		outgoing3 = false;

		while(true){
			cout << "Inside this loop" << endl;
			wait(incoming1.posedge_event() | incoming2.posedge_event() | incoming3.posedge_event());
			cout << "After first wait" << endl;

			//Mobile 1
			if(incoming1 == true){
				cout << "Mobile 1 received." << endl;
				while(!free); // Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing1 = true;
				free = false;
				wait(begin1.posedge_event());
				cout << "Begin1 @" << sc_simulation_time() << endl;
				wait(end1.posedge_event());
				cout << "End1 @" << sc_simulation_time() << endl;
				free = true;
			}

			//Mobile 2
			if(incoming2 == true){
				cout << "Mobile 2 received." << endl;
				while(!free);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing2 = true;
				free = false;
				wait(begin2.posedge_event());
				cout << "Begin2 @" << sc_simulation_time() << endl;
				wait(end2.posedge_event());
				cout << "End2 @" << sc_simulation_time() << endl;
				free = true;
			}

			//Mobile 3
			if(incoming3 == true){
				cout << "Mobile 3 received." << endl;
				while(!free);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing3 = true;
				free = false;
				wait(begin3.posedge_event());
				cout << "Begin3 @" << sc_simulation_time() << endl;
				wait(end3.posedge_event());
				cout << "End3 @" << sc_simulation_time() << endl;
				free = true;
			}
		}
	}

	SC_CTOR(server){
		//outgoing1 = false;
		//outgoing2 = false;
		//outgoing3 = false;'
		cout << "Started" << endl;
		SC_THREAD(monitor);
		//sensitive << incoming1.pos() << incoming2.pos() << incoming3.pos();
	}
};
