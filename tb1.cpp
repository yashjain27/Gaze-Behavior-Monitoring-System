#include "systemc.h"
#include "mobile.cpp"
#include "server.cpp"

int sc_main(int argc, char* argv[]){
  //Inputs
  sc_signal<bool> begin1;
  sc_signal<bool> begin2;
  sc_signal<bool> begin3;
  sc_signal<bool> end1;
  sc_signal<bool> end2;
  sc_signal<bool> end3;
  sc_signal<bool> incoming1;
  sc_signal<bool> incoming2;
  sc_signal<bool> incoming3;
  sc_signal<bool> outgoing1;
  sc_signal<bool> outgoing2;
  sc_signal<bool> outgoing3;
  sc_signal<bool> free;

  mobile mob1("mob1");
  mob1.clock(clock);
  mob1.free(free);
  mob1.incoming(outgoing1);
  mob1.outgoing(incoming1);
  mob1.begin(begin1);
  mob1.end(end1);

  mobile mob2("mob2");
  mob2.clock(clock);
  mob2.free(free);
  mob2.incoming(outgoing2);
  mob2.outgoing(incoming2);
  mob2.begin(begin2);
  mob2.end(end2);

  mobile mob3("mob3");
  mob3.clock(clock);
  mob3.free(free);
  mob3.incoming(outgoing3);
  mob3.outgoing(incoming3);
  mob3.begin(begin3);
  mob3.end(end3);

  server srvr("server");
  srvr.begin1(begin1);
  srvr.begin2(begin2);
  srvr.begin3(begin3);
  srvr.end1(end1);
  srvr.end2(end2);
  srvr.end3(end3);
  srvr.incoming1(incoming1);
  srvr.incoming2(incoming2);
  srvr.incoming3(incoming3);
  srvr.outgoing1(outgoing1);
  srvr.outgoing2(outgoing2);
  srvr.outgoing3(outgoing3);
  srvr.free(free);

  for(int i = 0; i < 30; i++){
    clock = 0;
    sc_start(10, SC_MS);
    clock = 1;
    sc_start(10, SC_MS);
  }

}
