/*
  ==============================================================================

    Alerts.h
    Created: 12 Apr 2012 2:16:00am
    Author:  peter

  ==============================================================================
*/

#ifndef __ALERTS_H_2AC9A37F__
#define __ALERTS_H_2AC9A37F__

class Alerts {
private:
	Alerts();
	~Alerts();
public:
	static void missingFaust();
	static void missingPerl();
	static void missingSAM();

};


#endif  // __ALERTS_H_2AC9A37F__
