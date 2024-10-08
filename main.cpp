//#include "ring.h"
/*
// example of notFixedSize
int main(){
	//example simple
	Ring<LogUnit> *info_buffer;
	{
	    //case onlyInfo:
        info_buffer->forwardIterationThoughRing([&r_list](Ring<LogUnit>::iter& it){
            r_list.push_back(*it);
        },
        info_buffer->usedSize());
        //break;
	}

//example not simple
// represend get part of waterflow image relation

{//init..
	Ring<specSctr> *image =/* nullptr *\/;
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
    image->forwardIterationThoughRing(
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
*/

// example of fast ring
#include "fastRing.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <thread>
#include <functional> 

using namespace std::chrono_literals;
template <class Ring>
void parallel_insert(Ring *ring, std::size_t start, std::size_t end) {
    for (std::size_t i = start; i < end; ++i) {
        if (ring->maxSize() < 200){
            std::this_thread::sleep_for(20ms);
        }
        if (ring->maxSize() > 12345678){
            ring->push_front(i % 2);
        }else{
            ring->push_front(i);
        }
    }
}

int main()
{
    const std::size_t num_threads = 8; 
    std::vector<std::thread> threads;

    Ring<std::size_t, Size::s128> ring8;
    const std::size_t total_size8 = std::pow(2, 7);
    const std::size_t chunk_size8 = total_size8 / num_threads;

    for (std::size_t t = 0; t < num_threads; ++t) {
        std::size_t start = t * chunk_size8;
        std::size_t end = (t == num_threads - 1) ? total_size8 : start + chunk_size8;
        threads.emplace_back(parallel_insert<Ring<std::size_t, Size::s128>>, &ring8, start, end);
    }
    for (auto& th : threads) {
        th.join();
    }
    threads.clear(); 
    std::size_t counter = 0;
    auto lambda = [&counter](Ring<std::size_t,Size::s128>::iter it){
        std::cout << *it << std::endl;
        ++counter;
    };
    ring8.forwardIterationThoughRing(lambda,std::pow(2,7));
    std::cout << counter <<std::endl;;
    std::cout << std::endl;



    Ring<std::size_t,Size::s32768> ring16;
    const std::size_t total_size16 = std::pow(2, 15);
    const std::size_t chunk_size16 = total_size16 / num_threads;

    for (std::size_t t = 0; t < num_threads; ++t) {
        std::size_t start = t * chunk_size16;
        std::size_t end = (t == num_threads - 1) ? total_size16 : start + chunk_size16;
        threads.emplace_back(parallel_insert<Ring<std::size_t, Size::s32768>>, &ring16, start, end);
    }
    for (auto& th : threads) {
        th.join();
    }
    threads.clear(); 
    std::size_t counter2 = 0;
    auto lambda2 = [&counter2](Ring<std::size_t,Size::s32768>::iter it){
        //std::cout << *it << std::endl;
        ++counter2;
    };
    ring16.forwardIterationThoughRing(lambda2,std::pow(2,15));
    std::cout << counter2 <<std::endl;;
    std::cout << std::endl;

    const std::size_t total_size32 = std::pow(2, 31); 
    const std::size_t chunk_size32 = total_size32 / num_threads; 

    
    Ring<bool,Size::s2147483648> ring32;
    for (std::size_t t = 0; t < num_threads; ++t) {
        std::size_t start = t * chunk_size32;
        std::size_t end = (t == num_threads - 1) ? total_size32 : start + chunk_size32;  
        threads.emplace_back(parallel_insert<Ring<bool, Size::s2147483648>>, &ring32, start, end);
    }
    for (auto& th : threads) {
        th.join();
    }
    std::size_t counter3 = 0;
    auto lambda3 = [&counter3](Ring<bool,Size::s2147483648>::iter it){
        //std::cout << *it << std::endl;
        ++counter3;
    };
    ring32.forwardIterationThoughRing(lambda3,total_size32);
    std::cout << counter3 <<std::endl;;
    std::cout << std::endl;
}