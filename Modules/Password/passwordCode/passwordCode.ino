#include "KTANECommon.h"

/*                                                                                                                            
                                                    dddddddd                                                                 
UUUUUUUU     UUUUUUUU                               d::::::d                          tttt                                   
U::::::U     U::::::U                               d::::::d                       ttt:::t                                   
U::::::U     U::::::U                               d::::::d                       t:::::t                                   
UU:::::U     U:::::UU                               d:::::d                        t:::::t                                   
 U:::::U     U:::::Uppppp   ppppppppp       ddddddddd:::::d   aaaaaaaaaaaaa  ttttttt:::::ttttttt        eeeeeeeeeeee         
 U:::::D     D:::::Up::::ppp:::::::::p    dd::::::::::::::d   a::::::::::::a t:::::::::::::::::t      ee::::::::::::ee       
 U:::::D     D:::::Up:::::::::::::::::p  d::::::::::::::::d   aaaaaaaaa:::::at:::::::::::::::::t     e::::::eeeee:::::ee     
 U:::::D     D:::::Upp::::::ppppp::::::pd:::::::ddddd:::::d            a::::atttttt:::::::tttttt    e::::::e     e:::::e     
 U:::::D     D:::::U p:::::p     p:::::pd::::::d    d:::::d     aaaaaaa:::::a      t:::::t          e:::::::eeeee::::::e     
 U:::::D     D:::::U p:::::p     p:::::pd:::::d     d:::::d   aa::::::::::::a      t:::::t          e:::::::::::::::::e      
 U:::::D     D:::::U p:::::p     p:::::pd:::::d     d:::::d  a::::aaaa::::::a      t:::::t          e::::::eeeeeeeeeee       
 U::::::U   U::::::U p:::::p    p::::::pd:::::d     d:::::d a::::a    a:::::a      t:::::t    tttttte:::::::e                
 U:::::::UUU:::::::U p:::::ppppp:::::::pd::::::ddddd::::::dda::::a    a:::::a      t::::::tttt:::::te::::::::e               
  UU:::::::::::::UU  p::::::::::::::::p  d:::::::::::::::::da:::::aaaa::::::a      tt::::::::::::::t e::::::::eeeeeeee       
    UU:::::::::UU    p::::::::::::::pp    d:::::::::ddd::::d a::::::::::aa:::a       tt:::::::::::tt  ee:::::::::::::e       
      UUUUUUUUU      p::::::pppppppp       ddddddddd   ddddd  aaaaaaaaaa  aaaa         ttttttttttt      eeeeeeeeeeeeee       
                     p:::::p                                                                                                 
                     p:::::p                                                                                                 
                    p:::::::p                                                                                                
                    p:::::::p                                                                                                
                    p:::::::p                                                                                                
                    ppppppppp                                                                                                                                                                                                                
*/

SWireClient client(MY_ADDRESS);
KTANEModule module(client, 3, 4);

void setup()
{
  Serial.begin(19200);

  Serial.println("### Password Module ###");

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