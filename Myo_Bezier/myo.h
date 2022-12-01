#pragma once
#include <iostream>
#include <myo/myo.hpp>
#include <array>
#include "resource.h"		

class dealMYO : public myo::DeviceListener
{

public:

	//dealMYO() :emgSamples()
	//{
	//}

	// Every time Myo Connect successfully pairs with a Myo armband, this function will be called.
//
// You can rely on the following rules:
//  - onPair() will only be called once for each Myo device
//  - no other events will occur involving a given Myo device before onPair() is called with it
//
// If you need to do some kind of per-Myo preparation before handling events, you can safely do it in onPair().
//	void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) ;


	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);


	//	void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) ;


	//	void onDisconnect(myo::Myo* myo, uint64_t timestamp) ;


		// This is a utility function implemented for this sample that maps a myo::Myo* to a unique ID starting at 1.
		// It does so by looking for the Myo pointer in knownMyos, which onPair() adds each Myo into as it is paired.
	//	size_t identifyMyo(myo::Myo* myo) ;

	//	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) ;

private:
	// We store each Myo pointer that we pair with in this list, so that we can keep track of the order we've seen
	// each Myo and give it a unique short identifier (see onPair() and identifyMyo() above).
	std::vector<myo::Myo*> knownMyos;
	//	std::array<int8_t, 8> emgSamples;
	myo::Pose currentPose;
};