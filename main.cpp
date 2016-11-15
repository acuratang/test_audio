#include <iostream>
#include "wavCore.h"
#include "audioProc.h"
#include <time.h>

using namespace std;




int main()
{
	time_t FFTstart,FFTend,IDFTend;



    wavFileIn file0("sine.wav");
    file0.printFileHeader();

    buffer buffer0,buffer1,DFTBuffer;
    file0.bufferData(buffer0);
    audioProc testing;

    FFTstart = time(NULL);
    testing.FFT(buffer0.bufferpa, DFTBuffer);
    FFTend = time(NULL);


    //testing.freqShift(200,DFTBuffer);

    testing.IDFT(DFTBuffer,buffer1.bufferpa);
    IDFTend = time(NULL);

    cout<<"FFT :"<<difftime(FFTend,FFTstart)<<endl;
    cout<<"IDFT :"<<difftime(IDFTend,FFTend)<<endl;


    cout<<"outputing"<<endl;
    wavFileOut fileOut0("shift.wav",file0.getSampleRate(),buffer1.bufferpa,16);




    return 0;
}




