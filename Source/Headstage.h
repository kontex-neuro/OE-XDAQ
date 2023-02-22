/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2021 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __HEADSTAGE_H_2C4CBD67__
#define __HEADSTAGE_H_2C4CBD67__

#include <DataThreadHeaders.h>

#include <array>
#include <atomic>
#include <string>
#include <vector>

#include "DeviceThread.h"
#include "rhythm-api/okFrontPanelDLL.h"
#include "rhythm-api/rhd2000datablock.h"
#include "rhythm-api/rhd2000evalboard.h"
#include "rhythm-api/rhd2000registers.h"

namespace RhythmNode
{


/**
    A headstage object represents a data source containing
    one or more Intan chips.

    Each headstage can send 1 or 2 data streams, each with
    up to 64 channels.

    A headstage can be identified in the following ways:
    - dataSource   : port (A1-D2Ddr) that the headstage is connected to
    - streamIndex  : location in the array of active streams
    - startChannel : index of the first channel acquired by this headstage,
                     out of all actively acquired neural data channels

*/
class Headstage
{
public:
    const int max_streams;
    const std::string prefix;
    const std::string port;
    /** Constructor */
    Headstage(std::string port, std::string prefix, int chip_idx, int max_streams);

    /** Destructor*/
    ~Headstage() = default;

    /** Returns the index of this headstage's data stream (offset = 0 or 1) */
    int getStreamIndex(int offset) const;

    /** Sets the index of this headstage's data stream */
    void setFirstStreamIndex(int streamIndex);

    /** Sets the index of this headstage's first neural data channel*/
    void setFirstChannel(int channelIndex);

    /** Returns the number of channels this headstage sends*/
    int getNumChannels() const;

    /** Sets the number of channels per stream*/
    void setChannelsPerStream(int nchan);
    int getNumStreams() const { return streams; }
    /** Sets the number of streams for this headstage (1 or 2)*/
    void setNumStreams(int num);

    /** Returns the number of actively acquired neural data channels*/
    int getNumActiveChannels() const;

    /** Returns the name of a channel at a given index*/
    String getChannelName(int ch) const;


    const std::vector<std::string> &get_channel_names();

    /** Sets the name of a channel at a given index*/
    void setChannelName(std::string name, int ch);

    /** Sets the channel naming scheme*/
    void setNamingScheme(ChannelNamingScheme scheme);

    /** Returns true if the headstage is connected*/
    bool isConnected() const;


    /** Sets the number of half-channels; mainly used for the 16-ch RHD2132 board */
    void setHalfChannels(bool half);

    /** Returns true if the headstage is in half-channels mode */
    bool getHalfChannels() { return halfChannels; }

    /** Auto-generates the channel names, based on the naming scheme*/
    void generateChannelNames();

    /** Sets impedance values after measurement*/
    void setImpedances(Impedances &impedances);

    /** Returns the impedance magnitude for a channel (if it exists)*/
    float getImpedanceMagnitude(int channel) const;

    /** Returns the impedance phase for a channel (if it exists)*/
    float getImpedancePhase(int channel) const;

    /** Returns true if impedance has been measured*/
    bool hasImpedanceData() const { return impedanceMagnitudes.size() > 0; }

private:
    int streams;
    int streamIndex;
    int firstChannelIndex;

    int numStreams;
    int channelsPerStream;

    bool halfChannels;

    int MAX_NUM_HEADSTAGES;

    ChannelNamingScheme namingScheme;

    std::vector<std::string> channelNames;

    Array<float> impedanceMagnitudes;
    Array<float> impedancePhases;
};


}  // namespace RhythmNode
#endif  // __HEADSTAGE_H_2C4CBD67__
