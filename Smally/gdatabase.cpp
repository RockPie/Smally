#include "gdatabase.h"

Spectral::Spectral(const QString ele, const uint nucnum)
{
    Element = ele;
    NucleonNum = nucnum;
    CountingData = new uint64_t[ChannelNum];
}

Spectral::~Spectral()
{
    delete CountingData;
}

void Spectral::ReceiveCount(const uint Channel)
{
    if(Channel < ChannelNum)
        CountingData[Channel]++;
    else
        throw  Channel;
}
