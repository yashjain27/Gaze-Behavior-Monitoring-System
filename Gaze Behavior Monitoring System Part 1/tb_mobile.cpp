#include "systemc.h"
#include "mobile.cpp"

int sc_main(int argc, char* argv[]){
  //Inputs
  sc_signal<bool> clock;
  sc_signal<bool> free;
  sc_signal<bool> incoming;
  sc_signal<bool> outgoing;
  sc_signal<bool> begin;
  sc_signal<bool> end;

  mobile mob("mob");
  mob.clock(clock);
  mob.free(free);
  mob.incoming(incoming);
  mob.outgoing(outgoing);
  mob.begin(begin);
  mob.end(end);

  for(int i = 0; i < 30; i++){
    clock = 0;
    sc_start(10, SC_MS);
    clock = 1;
    sc_start(10, SC_MS);
  }

}

int image1 [5][5] = {
  {1, 50, 20, 400, 320},
  {2, 50, 370, 450, 1000},
  {3, 470, 20, 600, 900},
  {4, 670, 40, 950, 550},
  {5, 680, 700, 1000, 1000}
};

sc_signal<sc_int<4> > ROI, tupleROI;
sc_signal<sc_uint<32> > tupleTstart, tupleTend;
