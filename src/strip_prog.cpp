#include "strip.hpp"

#define GLAS1 0
#define GLAS2 1
#define FLAMME 2

#define CORAL      255,127,80
#define TOMATO     255,99,71
#define ORANGRED   255,69,0
#define GOLD       255,165,80
#define ORANGE     255,165,80
#define DARKORANGE 255,140,80

void CLedStrip::setNextColor(int LEDid)
{
								RgbColor cur = this->GetPixelColor(LEDid);


								if(cur.G < 160)
								{
																cur.G += 5;
								}
								else
								{
																cur.G = 0;
								}

								if(cur.G > 130 && cur.G < 160)
								{
																cur.B = 20;
								}
								else
								{
																cur.B = 0;
								}
								cur.R = 255;

								this->SetPixelColor(LEDid, cur);
}

bool CLedStrip::prog0(prog_param_t &conf)
{
								const uint8_t id = 0;
								if(!this->_init)
																return this->_init;

								if((conf.old_prog != this->_prog) && (this->_prog == id))
								{
																this->ClearTo(RgbColor(0,0,0));
																conf.old_prog = id;
								}
								//Serial.printf("[Debug] Strip prog %d\n", id);
								// do, well, nothing...
								//delay(500);

								return true;
}

bool CLedStrip::prog1(prog_param_t &conf)
{
								const uint8_t id = 1;
								bool ret = false;
								if(!this->_init)
																return this->_init;

								// not started up to now
								if((conf.old_prog != this->_prog) && (this->_prog == id))
								{
																this->SetPixelColor(GLAS1, RgbColor(CORAL)); // oben
																this->SetPixelColor(GLAS2, RgbColor(ORANGE)); // mitte
																this->SetPixelColor(FLAMME, RgbColor(DARKORANGE)); // unten
																conf.old_prog = id;
																ret = true;
								}

								//Serial.printf("[Debug] Strip prog %d\n", id);
								// do stuff
								if(conf.ctr >= conf.max)
								{
																setNextColor(GLAS1);
																setNextColor(GLAS2);
																setNextColor(FLAMME);

																// reset counter to zero (restart)
																conf.ctr = conf.min;
								}
								else
																conf.ctr++;

								return ret;
}

bool CLedStrip::prog2(prog_param_t &conf)
{
								static int period = 0;

								const uint8_t id = 2;
								bool ret = false;

								uint8_t r = 0;
								uint8_t g = 0;
								uint8_t b = 0;

								// not started up to now
								// initialize
								if((conf.old_prog != this->_prog) && (this->_prog == id))
								{
																r = 80;
																g = 20;
																b = 200;
																this->SetPixelColor(0, RgbColor(0,0,b)); // oben
																this->SetPixelColor(1, RgbColor(r,g,b)); // mitte
																this->SetPixelColor(2, RgbColor(r,0,b)); // unten
																conf.old_prog = id;
																ret = true;
								}

								if(!period)
								{
																//Serial.printf("[Debug] Strip prog %d\n", id);
																uint8_t br = conf.ctr;


																b = br*2;
																r = br + conf.min;
																g = br*2;

																if(conf.ctr <= 1)
																{
																								conf.state = false;
																}

																if(conf.ctr >= 126)
																{
																								conf.state = true;
																}

																for(uint8_t ctr=0; ctr < 2; ctr++)
																{
																								this->SetPixelColor(ctr, RgbColor(r,g,b));
																}

																if(conf.state == true)
																								conf.ctr--;
																else
																								conf.ctr++;

																//Serial.println(conf.ctr);
																period = conf.period;;
								}
								else
																period--;

								return ret;
}
