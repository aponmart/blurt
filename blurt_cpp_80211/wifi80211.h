#ifndef WIFI80211_H
#define WIFI80211_H
#include "blurt.h"
#include "constellation.h"
#include "rate.h"
#include "ofdm.h"
#include "cc.h"
#include "crc.h"

class DecodeResult {
public:
    std::vector<int> payload;
    int startIndex, endIndex;
    float lsnr;
};

class WiFi80211 {
private:
    void plcp_bits(const Rate &rate, int octets, bitvector &output);
    void subcarriersFromBits(const bitvector &bits, const Rate &rate, int scramblerState, std::vector<complex> &output);
    void autocorrelate(const std::vector<complex> &input, std::vector<float> &output);
    void synchronize(const std::vector<complex> &input, std::vector<int> &startIndices);
    void wienerFilter(const std::vector<complex> &lts, std::vector<complex> &G, float &snr, float &lsnr_estimate);
    void train(std::vector<complex> &input, std::vector<complex> &G, float &uncertainty, float &var_ni, int &offset, float &lsnr_estimate);
    void demodulate(const std::vector<complex> &input, const std::vector<complex> &G, float uncertainty, float var_n,
            std::vector<int> &coded_bits, int &length_bits, int &offset);
    void decodeFromLLR(const std::vector<int> &input, int length_bits, bitvector &output);
public:
    OFDM ofdm;
    ConvolutionalCode code;
    std::vector<Rate> rates;
    CRC crc;
    WiFi80211();
    void encode(const std::vector<int> &input, int rate_index, std::vector<complex> &output);
    void decode(const std::vector<complex> &input, std::vector<DecodeResult> &output);
};
#endif
