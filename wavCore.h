/*  wavCore.h
 *  Author: Acura Tang
 *  2016
 *  Description: Read and write .wav file, only support 8 , 16, 32 Bit PCM format with 1 or 2 chenals.
 *  The data will be tored in bufferXXa or bufferXXb depaning on the chenal and the bit format
 *
 */

#ifndef WAVCORE
#define WAVCORE
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <vector>

class buffer
{
public:
	std::vector<double> bufferpa,bufferpb;

};


class wavFileIn
{
protected:
    FILE *wavInput = NULL;
    char chunkID[4];
    char format[4];
    char subChunkID[4];
    char subchunk2ID[4];
    int chunkSize,subChinkSize,sampleRate,byteRate,subchunk2Size;
    short audioFormat,numchenals,blockAlign,bitsPerSample;



public:

    wavFileIn(std::string file);
    void printFileHeader(void);
    void bufferData(buffer &buffer0);//only 16bit worls atm
    int getSampleRate(void);

};

class wavFileOut
{
protected:
    std::string file;
    int chunkSize,subChink1Size,sampleRate,byteRate,subchunk2Size;
    short blockAlign;
    std::vector<double> *bufferpa,*bufferpb;


public:
    wavFileOut(std::string file, int sampleRate,  std::vector<double> &buffer, int bits);
    //wavFileOut(const std::string file, const int sampleRate, const std::vector<double> *buffera, const std::vector<int16_t> *bufferb,const int bits=16);


    void wavFilePush(std::string file, int sampleRate, std::vector<int16_t> *buffer);
};

#endif // WAVCORE

