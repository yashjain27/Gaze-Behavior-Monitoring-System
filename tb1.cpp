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
  sc_signal<bool> free[2];
  sc_signal<bool> clock;
  sc_signal<bool> tx_begin[3];
  sc_signal<bool> tx_end[3];

  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );

  int packetsize = 1000;
  int bandwidth = 1000;

  mobile mob1("mob1", bandwidth, packetsize);
  mob1.clock(clock);
  mob1.free[0](free[0]);
  mob1.free[1](free[1]);
  mob1.incoming(outgoing1);
  mob1.outgoing(incoming1);
  mob1.begin(begin1);
  mob1.end(end1);
  mob1.rx_begin(tx_begin[0]);
  mob1.rx_end(tx_end[0]);

  mobile mob2("mob2", bandwidth, packetsize);
  mob2.clock(clock);
  mob2.free[0](free[0]);
  mob2.free[1](free[1]);
  mob2.incoming(outgoing2);
  mob2.outgoing(incoming2);
  mob2.begin(begin2);
  mob2.end(end2);
  mob2.rx_begin(tx_begin[1]);
  mob2.rx_end(tx_end[1]);

  mobile mob3("mob3", bandwidth, packetsize);
  mob3.clock(clock);
  mob3.free[0](free[0]);
  mob3.free[1](free[1]);
  mob3.incoming(outgoing3);
  mob3.outgoing(incoming3);
  mob3.begin(begin3);
  mob3.end(end3);
  mob3.rx_begin(tx_begin[2]);
  mob3.rx_end(tx_end[2]);

  server srvr("server", bandwidth, packetsize);
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
  srvr.free[0](free[0]);
  srvr.free[1](free[1]);
  srvr.clk(clock);
  srvr.tx_begin[0](tx_begin[0]);
  srvr.tx_begin[1](tx_begin[1]);
  srvr.tx_begin[2](tx_begin[2]);
  srvr.tx_end[0](tx_end[0]);
  srvr.tx_end[1](tx_end[1]);
  srvr.tx_end[2](tx_end[2]);

  for(int i = 0; i < 50000; i++){
    sc_start(1, SC_MS);
    clock = 0;
    sc_start(1, SC_MS);
    clock = 1;
  }

  for(int i = 0; i < 80000; i++){
    sc_start(1, SC_MS);
    clock = 0;
    sc_start(1, SC_MS);
    clock = 1;
  }

}
