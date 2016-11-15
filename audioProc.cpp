/*
 * audioProc.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: USER
 */

#include "audioProc.h"
void audioProc::FFT( std::vector<double> &input, buffer &output)
{
	if (input.size()%2 != 0)
	{
		input.push_back(0);
	}
	double N = input.size();
	output.bufferpa.clear();
	output.bufferpb.clear();
	std::vector<double> oin, ein;
	buffer odd, even;//,top,bottom;
	double SINE;
	double COSINE;
	for(unsigned int i =0; i<N;i=i+2)
	{
		ein.push_back(input[i]);
		oin.push_back(input[i+1]);
	}

	this->DFT(oin,odd);
	this->DFT(ein,even);

	output.bufferpa.resize(N,0);
	output.bufferpb.resize(N,0);
	unsigned int n =0;
	for(unsigned int k=0; k<N;k++,n++)
	{
		SINE = sin(2*k*M_PI/N);
		COSINE = cos(2*k*M_PI/N);
		if(n==(N/2))
		{
			n=0;
		}

		output.bufferpa[k]=even.bufferpa[n]+(odd.bufferpa[n]*COSINE)+(odd.bufferpb[n]*SINE);
		output.bufferpb[k] = even.bufferpb[n]-(odd.bufferpa[n]*SINE)+(odd.bufferpb[n]*COSINE);

	}
}

void audioProc ::DFT(std::vector<double> &input, buffer &output)
{
	double N = input.size();

	output.bufferpa.clear();
	output.bufferpb.clear();
	output.bufferpa.resize(N,0);
	output.bufferpb.resize(N,0);


	for(unsigned int k =0;k<N;k++)
	{

		for(unsigned int n =0;n<N;n++)
		{

			double angle = 2 * M_PI * n * k / N;
			output.bufferpa[k] += (input[n]*cos(angle));
			output.bufferpb[k] += (-input[n]*sin(angle));
		}

		//output.bufferpa[k] = sqrt(output.bufferpa[k]*output.bufferpa[k])+(output.bufferpb[k]*output.bufferpb[k]);

	}
}



void audioProc::IDFT( buffer &input,std::vector<double> &output)
{
	const double N = input.bufferpa.size();
	buffer temp;
	output.clear();
	output.resize(N,0);
	temp.bufferpa.resize(N,0);
	temp.bufferpb.resize(N,0);

	for(unsigned int k =0;k < N;k++)
	{
		temp.bufferpa[k] = input.bufferpa[k]/2/N;
		temp.bufferpb[k] = -input.bufferpb[k]/2/N;
	}
	temp.bufferpa[0] = input.bufferpa[0]/N;
	temp.bufferpa[N/2] = input.bufferpa[N/2]/N;



	for(unsigned int n =0;n < N;n++)
	{
		double part1 =0;
		double part2 =0;
		for(unsigned int k =0;k < N/2;k++)
		{
			double angle = 2 * M_PI * n * k / N;
			part1 += temp.bufferpa[k]*cos(angle);
			part2 += temp.bufferpb[k]*sin(angle);
		}
		output[n] = part1 + part2;
	}
}


void audioProc::freqShift(int shift,buffer &data)
{
	if(shift>0)
	{
		data.bufferpa.insert(data.bufferpa.begin(),shift,0);
		data.bufferpb.insert(data.bufferpb.begin(),shift,0);
		data.bufferpa.erase(data.bufferpa.end()-shift,data.bufferpa.end());
		data.bufferpb.erase(data.bufferpb.end()-shift,data.bufferpb.end());

	}
	if(shift<0)
	{
		int pshift = shift*-1;
		data.bufferpa.insert(data.bufferpa.end(),pshift,0);
		data.bufferpb.insert(data.bufferpb.end(),pshift,0);
		data.bufferpa.erase(data.bufferpa.begin(),data.bufferpa.begin()+pshift);
		data.bufferpb.erase(data.bufferpb.begin(),data.bufferpb.begin()+pshift);

	}
}





