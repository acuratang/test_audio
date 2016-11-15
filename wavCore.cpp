#include "wavCore.h"

using namespace std;


wavFileIn::wavFileIn(string file)
{
    wavInput = fopen(file.c_str(),"rb");
    if(wavInput == NULL)
    {
        cout<<"read failed"<<endl;
    }

    fread(chunkID,sizeof(char),4,wavInput);
    fread(&chunkSize,sizeof(int),1,wavInput);
    fread(format,sizeof(char),4,wavInput);
    fread(subChunkID,sizeof(char),4,wavInput);
    fread(&subChinkSize,sizeof(int),1,wavInput);
    fread(&audioFormat,sizeof(short),1,wavInput);
    fread(&numchenals,sizeof(short),1,wavInput);
    fread(&sampleRate,sizeof(int),1,wavInput);
    fread(&byteRate,sizeof(int),1,wavInput);
    fread(&blockAlign,sizeof(short),1,wavInput);
    fread(&bitsPerSample,sizeof(short),1,wavInput);
    fread(subchunk2ID,sizeof(char),4,wavInput);
    fread(&subchunk2Size,sizeof(int),1,wavInput);

}





void wavFileIn::printFileHeader(void)
{
    printf("%c%c%c%c\n",chunkID[0],chunkID[1],chunkID[2],chunkID[3]);
    printf("%c%c%c%c\n",format[0],format[1],format[2],format[3]);
    printf("%c%c%c%c\n",subChunkID[0],subChunkID[1],subChunkID[2],subChunkID[3]);
    printf("%c%c%c%c\n",subchunk2ID[0],subchunk2ID[1],subchunk2ID[2],subchunk2ID[3]);

    cout<<"chunk size: "<<chunkSize<<endl;
    cout<<"audioFomat: "<<audioFormat<<endl;
    cout<<"chenles: "<<numchenals<<endl;
    cout<<"blockAlifn: "<<blockAlign<<endl;
    cout<<"sample rate: "<<sampleRate<<endl;
    cout<<"bit per sample: "<<bitsPerSample<<endl;
    cout<<"subchunk2Size: "<<subchunk2Size<<endl;
}


int wavFileIn::getSampleRate(void)
{
    return sampleRate;
}

void wavFileIn::bufferData(buffer &buffer0)
{
    switch(bitsPerSample)
    {
    case 8:
        if(numchenals ==1)
        {
            uint8_t temp;
            for(int endCounter = subchunk2Size;endCounter>0;endCounter--)
            {
                fread(&temp,sizeof(uint8_t),1,wavInput);
                buffer0.bufferpa.push_back(temp);
            }
            fclose(wavInput);
        }
        else if(numchenals == 2)
        {
            uint8_t temp1, temp2;
            for(int endCounter = subchunk2Size;endCounter>0;endCounter = (endCounter-(numchenals*blockAlign)))
            {
                for(int i=0;i<blockAlign;i++)
                {
                    fread(&temp1,sizeof(uint8_t),1,wavInput);
                    buffer0.bufferpa.push_back(temp1);
                }
                for(int i=0;i<blockAlign;i++)
                {
                    fread(&temp2,sizeof(uint8_t),1,wavInput);
                    buffer0.bufferpb.push_back(temp2);
                }
            }
            fclose(wavInput);
        }
        else
        {
            fclose(wavInput);
            cout<<"Buffer error: Do not support"<<numchenals<<"chenales"<<endl;

        }
        break;



    case 16://normalized
        if(numchenals ==1)
        {
            int16_t temp;
            double normalized;
            for(int endCounter = (subchunk2Size/2);endCounter>0;endCounter--)
            {
                fread(&temp,sizeof(int16_t),1,wavInput);
                normalized = temp/32768.0;
                buffer0.bufferpa.push_back(normalized);
            }
            fclose(wavInput);
            cout<<"closed wavInput"<<endl;
        }
        else if(numchenals == 2)
        {
            int16_t temp1, temp2;
            double normalized;
            for(int endCounter = (subchunk2Size/2);endCounter>0;endCounter = (endCounter-(numchenals*blockAlign)))
            {
                for(int i=0;i<blockAlign;i++)
                {
                    fread(&temp1,sizeof(int16_t),1,wavInput);
                    normalized = temp1/32768.0;
                    buffer0.bufferpa.push_back(normalized);
                }
                for(int i=0;i<blockAlign;i++)
                {
                    fread(&temp2,sizeof(int16_t),1,wavInput);
                    normalized = temp2/32768.0;
                    buffer0.bufferpb.push_back(normalized);
                }
            }
            fclose(wavInput);
        }
        else
        {
            cout<<"Buffer error: Do not support"<<numchenals<<"chenales"<<endl;

        }

        break;


/*
    case 32:


        break;

*/
    default:
        cout<<"Buffer error: Bit type not supported"<<endl;

    }
}

wavFileOut::wavFileOut(std::string file, int sampleRate,  std::vector<double> &buffer, int bits)
{

    FILE * wavOutput = NULL;
    wavOutput = fopen (file.c_str(), "wb");

    if(wavOutput != NULL)
    {
    	if(bits==16)
    	{
    		short numChannels = 1;
    		short bitsPerSample = 16;
    		int subChink1Size = 16;
        	short audioFomat = 1;
        	int byteRate = sampleRate*bitsPerSample*numChannels/8;
        	short blockAlign = numChannels*bitsPerSample/8;
        	this->file = file;
        	this->sampleRate = sampleRate;
        	this->bufferpa = &buffer;
        	this->subchunk2Size = (buffer.size()*2);
        	this->chunkSize = (36+subchunk2Size);

        	fwrite("RIFF",sizeof(char),4,wavOutput);
        	fwrite(&chunkSize,sizeof(int),1,wavOutput);
        	fwrite("WAVE",sizeof(char),4,wavOutput);
        	fwrite("fmt ",sizeof(char),4,wavOutput);
        	fwrite(&subChink1Size,sizeof(int),1,wavOutput);
        	fwrite(&audioFomat,sizeof(short),1,wavOutput);
        	fwrite(&numChannels,sizeof(short),1,wavOutput);
        	fwrite(&sampleRate,sizeof(int),1,wavOutput);
        	fwrite(&byteRate,sizeof(int),1,wavOutput);
        	fwrite(&blockAlign,sizeof(short),1,wavOutput);
        	fwrite(&bitsPerSample,sizeof(short),1,wavOutput);
        	fwrite("data",sizeof(char),4,wavOutput);
        	fwrite(&subchunk2Size,sizeof(int),1,wavOutput);
        	int16_t tamp;
        	for(unsigned int i=0;i<buffer.size();i++)
        	{
        		tamp = buffer[i]*32768.0;
        		fwrite(&tamp,sizeof(int16_t),1,wavOutput);
        	}
    	}
    }
    fclose(wavOutput);

}

/*
wavFileOut::wavFileOut(std::string file, int sampleRate, std::vector<int16_t> *buffera, std::vector<int16_t> *bufferb)
{

    FILE * wavOutput = NULL;
    wavOutput = fopen (file.c_str(), "wb");

    if(wavOutput != NULL)
    {
        short numChannels = 2;
        short bitsPerSample = 16;
        int subChink1Size = 16;
        short audioFomat = 1;
        int byteRate = sampleRate*bitsPerSample*numChannels/8;
        short blockAlign = numChannels*bitsPerSample/8;
        this->file = file;
        this->sampleRate = sampleRate;
        this->buffer16a = buffera;
        this->buffer16b = bufferb;
        this->subchunk2Size = ((buffera->size()+bufferb->size())*2);
        this->chunkSize = (36+subchunk2Size);

        fwrite("RIFF",sizeof(char),4,wavOutput);
        fwrite(&chunkSize,sizeof(int),1,wavOutput);
        fwrite("WAVE",sizeof(char),4,wavOutput);
        fwrite("fmt ",sizeof(char),4,wavOutput);
        fwrite(&subChink1Size,sizeof(int),1,wavOutput);
        fwrite(&audioFomat,sizeof(short),1,wavOutput);
        fwrite(&numChannels,sizeof(short),1,wavOutput);
        fwrite(&sampleRate,sizeof(int),1,wavOutput);
        fwrite(&byteRate,sizeof(int),1,wavOutput);
        fwrite(&blockAlign,sizeof(short),1,wavOutput);
        fwrite(&bitsPerSample,sizeof(short),1,wavOutput);
        fwrite("data",sizeof(char),4,wavOutput);
        fwrite(&subchunk2Size,sizeof(int),1,wavOutput);
        int16_t tamp1,tamp2;
        for(unsigned int i=0;i<(buffera->size());i++)
        {
            tamp1 = buffera->at(i);
            tamp2 = bufferb->at(i);
            fwrite(&tamp1,sizeof(int16_t),1,wavOutput);
            fwrite(&tamp2,sizeof(int16_t),1,wavOutput);

        }

    }
    fclose(wavOutput);

}*/

















