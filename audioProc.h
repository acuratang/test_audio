/*
 * audioProc.h
 *
 *  Created on: Mar 24, 2016
 *      Author: USER
 */
#include "wavCore.h"
#include "math.h"


#ifndef AUDIOPROC_H_
#define AUDIOPROC_H_


class audioProc
{
public:
	void FFT( std::vector<double> &input, buffer &output);//N/2 point DFT
	void DFT( std::vector<double> &input, buffer &output);//slow as fuckkkkkkkkkkkkkkk
	void IDFT( buffer &input,std::vector<double> &output);
	void freqShift(int shift,buffer &data);

};





#endif /* AUDIOPROC_H_ */
