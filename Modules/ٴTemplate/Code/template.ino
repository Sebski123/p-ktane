#include "KTANECommon.h"

SWireClient client(MY_ADDRESS);
KTANEModule module(client, 3, 4);

void setup()
{
  Serial.begin(19200);

  Serial.println("### Template Module ###");

  while (!module.getConfig())
  {
    module.interpretData();
  }

  /*
    Do setup here
  */

  module.sendReady();
}

void loop()
{
  module.interpretData();

  if (!module.is_solved)
  {
    /*
    checkInputs();
    if(they_solved_it) {
      module.win();
    }
    if(they_messed_up) {
      module.strike();
    }
    updateOutputs();
    */
  }
}