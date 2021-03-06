//  SoapyFCDPP.hpp
//  Copyright (c) 2018 Albin Stigo
//  SPDX-License-Identifier: BSL-1.0

#ifndef SoapyFCDPP_hpp
#define SoapyFCDPP_hpp

#include <SoapySDR/Device.hpp>
#include <SoapySDR/Registry.hpp>
#include <SoapySDR/ConverterRegistry.hpp>

#include <cstdint>
#include <iostream>
#include <fstream>
#include <hidapi.h>

#include "fcd.h"
#include "alsa.h"

// This is for the funcube dongle pro+
#define FCDPP_VENDOR_ID     0x04d8
#define FCDPP_PRODUCT_ID    0xfb31

class SoapyFCDPP : public SoapySDR::Device
{
    
private:
    snd_pcm_t* d_pcm_handle;
    uint32_t d_period_size;
    std::vector<int32_t> d_buff;

    // Device properties
    const double d_sample_rate;
    double d_frequency;
    double d_lna_gain;
    double d_mixer_gain;
    double d_if_gain;
    const std::string d_hid_path;
    const std::string d_alsa_device;
    
    SoapySDR::ConverterRegistry::ConverterFunction d_converter_func;

    // hid
    hid_device *d_handle;
    
public:
    SoapyFCDPP(const std::string &hid_path, const std::string &alsa_device);
    ~SoapyFCDPP();
    
    // Identification API
    std::string getDriverKey() const;
    std::string getHardwareKey() const;
    
    // Channels API
    size_t getNumChannels(const int dir) const;
    bool getFullDuplex(const int direction, const size_t channel) const;
    
    // Stream API
    std::vector<std::string> getStreamFormats(const int direction, const size_t channel) const;
    std::string getNativeStreamFormat(const int direction, const size_t channel, double &fullScale) const;
    SoapySDR::ArgInfoList getStreamArgsInfo(const int direction, const size_t channel) const;
    
    SoapySDR::Stream* setupStream(const int direction, const std::string &format, const std::vector<size_t> &channels = std::vector<size_t>(), const SoapySDR::Kwargs &args = SoapySDR::Kwargs());
    
    void closeStream(SoapySDR::Stream *stream);
    size_t getStreamMTU(SoapySDR::Stream *stream) const;
    int activateStream(SoapySDR::Stream *stream,
                       const int flags = 0,
                       const long long timeNs = 0,
                       const size_t numElems = 0);
    int deactivateStream(SoapySDR::Stream *stream, const int flags = 0, const long long timeNs = 0);
    
    int readStream(SoapySDR::Stream *stream,
                   void * const *buffs,
                   const size_t numElems,
                   int &flags,
                   long long &timeNs,
                   const long timeoutUs = 100000);

    // Antennas
    std::vector<std::string> listAntennas(const int direction, const size_t channel) const;
    void setAntenna(const int direction, const size_t channel, const std::string &name);
    std::string getAntenna(const int direction, const size_t channel) const;
    
    // DC offset
    bool hasDCOffsetMode(const int direction, const size_t channel) const;
    
    // Gain
    std::vector<std::string> listGains(const int direction, const size_t channel) const;
    bool hasGainMode(const int direction, const size_t channel) const;
    void setGainMode(const int direction, const size_t channel, const bool automatic);
    bool getGainMode(const int direction, const size_t channel) const;
    void setGain(const int direction, const size_t channel, const double value);
    void setGain(const int direction, const size_t channel, const std::string &name, const double value);
    double getGain(const int direction, const size_t channel, const std::string &name) const;
    SoapySDR::Range getGainRange(const int direction, const size_t channel, const std::string &name) const;
    
    // Frequency
    void setFrequency(const int direction,
                      const size_t channel,
                      const std::string &name,
                      const double frequency,
                      const SoapySDR::Kwargs &args = SoapySDR::Kwargs());
    double getFrequency(const int direction, const size_t channel, const std::string &name) const;
    std::vector<std::string> listFrequencies(const int direction, const size_t channel) const;
    SoapySDR::RangeList getFrequencyRange(const int direction, const size_t channel, const std::string &name) const;
    SoapySDR::ArgInfoList getFrequencyArgsInfo(const int direction, const size_t channel) const;
    
    // Sample Rate API
    void setSampleRate(const int direction, const size_t channel, const double rate);
    double getSampleRate(const int direction, const size_t channel) const;
    std::vector<double> listSampleRates(const int direction, const size_t channel) const;

    // Bandwidth API
    void setBandwidth(const int direction, const size_t channel, const double bw);
    double getBandwidth(const int direction, const size_t channel) const;
    std::vector<double> listBandwidths(const int direction, const size_t channel) const;
    
    // Setting API?
    SoapySDR::ArgInfoList getSettingInfo(void) const;
    void writeSetting(const std::string &key, const std::string &value);
    std::string readSetting(const std::string &key) const;
};

#endif /* SoapyFCDPP_hpp */
