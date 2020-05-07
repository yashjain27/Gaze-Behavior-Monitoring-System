#include "systemc.h"
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

  //Deprecate
  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );

  incoming1 = 0;
  incoming2 = 0;
  incoming3 = 0;
  begin1 = 0;
  begin2 = 0;
  begin3 = 0;
  end1 = 0;
  end2 = 0;
  end3 = 0;

  sc_start(10, SC_NS);

  incoming1 = 1;
  incoming2 = 1;
  incoming3 = 1;

  sc_start(10, SC_NS);

  begin1 = 1;

  sc_start(10, SC_NS);

  begin1 = 0;

  sc_start(10, SC_NS);

  end1 = 1;

  sc_start(10, SC_NS);

  end1 = 0;
}
