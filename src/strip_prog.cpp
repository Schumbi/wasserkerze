#include "strip.hpp"

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
								cur.B = 128;
								cur.G = 128;
								cur.R = 128;

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
																setNextColor(FLAMME);
																setNextColor(GLAS1);
																setNextColor(GLAS2);

																// reset counter to zero (restart)
																conf.ctr = conf.min;
								}
								else
																conf.ctr++;

								return ret;
}

bool CLedStrip::prog2(prog_param_t &conf)
{
								if(!this->_init)
																return this->_init;

								const uint8_t id = 2;
								// not started up to now
								// initialize
								if((conf.old_prog != this->_prog) && (this->_prog == id))
								{

																this->ClearTo(RgbColor(0,0,180));
																this->SetPixelColor(FLAMME, RgbColor(ORANGRED));  // unten
																conf.old_prog = id;
								}

								RgbColor flame = GetPixelColor(FLAMME);
								int16_t* fac = &(conf.period);
								if(conf.ctr > conf.max)
								{
																*fac *= -1;
								}

								if(conf.ctr < conf.min)
								{
																*fac *= -1;
								}
								flame.B += *fac;

								conf.ctr += *fac;
								this->SetPixelColor(FLAMME, flame);

								return this->GetPixelColor(FLAMME) == flame;
}
