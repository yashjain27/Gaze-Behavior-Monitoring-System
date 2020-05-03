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
			wait(incoming1);
			if(incoming1 == true){
				while(!free);
				outgoing1 = true;
				free = false;
				wait(begin1);
				wait(end1);
				free = true;
			}			
		}
		while(true){
			wait(incoming2);
			if(incoming2 == true){
				while(!free);
				outgoing2 = true;
				free = false;
				wait(begin2);
				wait(end2);
				free = true;
			}
		}
		while(true){
			wait(incoming3);
			if(incoming3 == true){
				while(!free);
				outgoing3 = true;
				free = false;
				wait(begin3);
				wait(end3);
				free = true;
			}
		}		
	}
	
	SC_CTOR(server){
		SC_THREAD(monitor);
		sensitive << incoming1 << incoming2 << incoming3;
	}
};
