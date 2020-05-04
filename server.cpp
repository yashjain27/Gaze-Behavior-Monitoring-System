#include "systemc.h"

SC_MODULE(server){
	sc_in<bool> begin1, begin2, begin3, end1, end2, end3, incoming1, incoming2, incoming3;
	sc_out<bool> free, outgoing1, outgoing2, outgoing3;

	void monitor(){
		free = true;
		outgoing1 = false;
		outgoing2 = false;
		outgoing3 = false;

		while(true){
			wait(incoming1 | incoming2 | incoming3);

			//Mobile 1
			if(incoming1 == true){
				cout << "Mobile 1 received." << endl;
				while(!free); // Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing1 = true;
				free = false;
				wait(begin1);
				wait(end1);
				free = true;
			}

			//Mobile 2
			if(incoming2 == true){
				cout << "Mobile 2 received." << endl;
				while(!free);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing2 = true;
				free = false;
				wait(begin2);
				wait(end2);
				free = true;
			}

			//Mobile 3
			if(incoming3 == true){
				cout << "Mobile 3 received." << endl;
				while(!free);// Wait for the server to free up

				//Now that the server has freed up, proceed
				outgoing3 = true;
				free = false;
				wait(begin3);
				wait(end3);
				free = true;
			}
		}
	}

	SC_CTOR(server){
		outgoing1 = false;
		outgoing2 = false;
		outgoing3 = false;
		SC_THREAD(monitor);
		sensitive << incoming1.pos() << incoming2.pos() << incoming3.pos();
	}
};
