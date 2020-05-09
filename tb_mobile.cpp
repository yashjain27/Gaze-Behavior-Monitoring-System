#include "systemc.h"
#include "mobile.cpp"

int sc_main(int argc, char* argv[]){
  //Inputs
  sc_signal<bool> clock;
  sc_signal<bool> incoming1;
  sc_signal<bool> outgoing1;
  sc_signal<bool> begin1;
  sc_signal<bool> end1;
  sc_signal<bool> free[2];
  sc_signal<bool> tx_end;
  sc_signal<bool> tx_begin;

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
  mob1.rx_begin(tx_begin);
  mob1.rx_end(tx_end);

  for(int i = 0; i < 5000; i++){
    clock = 0;
    sc_start(10, SC_MS);
    clock = 1;
    sc_start(10, SC_MS);
  }

}
