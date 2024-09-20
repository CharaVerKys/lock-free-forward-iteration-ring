#include "ring.h"

int main(){
	//example simple
	Ring<LogUnit> *info_buffer;
	{
	    //case onlyInfo:
        info_buffer->forwartIterationThoughRing([&r_list](Ring<LogUnit>::iter& it){
            r_list.push_back(*it);
        },
        info_buffer->usedSize());
        //break;
	}

//example not simple
// represend get part of waterflow image relation

{//init..
	Ring<specSctr> *image =/* nullptr*/;
}

{// adding new item just
	image->push_front(packet);
}

{//get function	
	//init vals (example)
	uint32_t elemsInBlock = length*width;
    specap* vectorImage = new specap[imageHeight * width]; 

	//logic value and additional controll flags for lamda
    bool ever = false;
    using Time = decltype (specSctr()->time);
    Time time; 
    
    int i = 0; // int value for iterations
    image->forwartIterationThoughRing(
         [vectorImage, &ever, &time, &i, &elemsInBlock] // vals
    	  (Ring<specSctr>::iter& it) // using as forward iterator
       {
        if(!ever){
            ever = true;
            time = (*it)->time;
        }
        specap* currentBlock = vectorImage + (i * elemsInBlock);
        ++i;
        std::memcpy(currentBlock, (*it)->vec, elemsInBlock * sizeof(specap));
        assert(elemsInBlock == (*it)->vecSize);
       },
    nodesForImage);

    return makeSctr(vectorImage,imageHeight * width,time);
}
        
	return 0;
}
