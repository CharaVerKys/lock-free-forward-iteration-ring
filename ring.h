#ifndef RINGQUEUE_H
#define RINGQUEUE_H
#include <vector>
#include <cassert>
#include <std::mutex>

template <typename T>
class Ring
{
public:
    class iterator;
    using iter = typename std::vector<T>::iterator;

    explicit Ring(std::size_t size)
        : mutex_ifOfBegin(new std::mutex())
        , m_size(size)
        , m_usedSize(0)
        , m_vector(std::vector<T>(m_size*2))
    {
        m_itOfBegin = m_vector.begin();
        m_itOfEnd = m_itOfBegin;
    }
    ~Ring(){
        if(mutex_ifOfBegin){delete mutex_ifOfBegin;}
    }

    const std::size_t& size(){
        return m_size;
    }
    const std::size_t& usedSize(){
        return m_usedSize;
    }

    void push_front(const T& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            mutex_ifOfBegin->lock();
                ++m_itOfBegin;
                if(m_itOfBegin == m_vector.end()){
                    m_itOfBegin = m_vector.begin();
                }
            mutex_ifOfBegin->unlock();
        }
        *m_itOfEnd = value;
        ++m_itOfEnd;
        if(m_itOfEnd == m_vector.end()){
            m_itOfEnd = m_vector.begin();
        }
    }

    void push_front(T&& value) {
        if(m_usedSize != m_size){
            ++m_usedSize;
        }else{ // if shift
            mutex_ifOfBegin->lock();
                ++m_itOfBegin;
                if(m_itOfBegin == m_vector.end()){
                    m_itOfBegin = m_vector.begin();
                }
            mutex_ifOfBegin->unlock();
        }
        *m_itOfEnd = std::move(value);
        ++m_itOfEnd;
        if(m_itOfEnd == m_vector.end()){
            m_itOfEnd = m_vector.begin();
        }
    }

    void clear(){
        m_itOfBegin = m_vector.end();
        m_itOfEnd = m_itOfBegin;
        m_usedSize = 0;
    }

    template<class func> // only template work with lamda here // even std::function (without template) not work
    void forwartIterationThoughRing(func fnc, std::size_t size){

        mutex_ifOfBegin->lock();
            typename std::vector<T>::iterator it = m_itOfBegin;
        mutex_ifOfBegin->unlock();

        if(it + size-1 < m_vector.end()){
            it += size-1;
            assert(it!=m_vector.end());
        }else{
            std::size_t i = 0;
            while(it + i != m_vector.end()){++i;} // find end
            it += i;
            assert(it==m_vector.end());
            it = m_vector.begin() + size -1 -i;
        }

        for(std::size_t i = 0; i<size; ++i){
            assert(it!=m_vector.end());
            fnc(it);
            if(it == m_vector.begin()){it = m_vector.end();}
            --it;
        }
    }

/*    iterator begin(){

      }*/

private:
    std::mutex* mutex_ifOfBegin;
    std::size_t m_size;
    std::size_t m_usedSize;
    typename std::vector<T>::iterator m_itOfBegin;
    typename std::vector<T>::iterator m_itOfEnd;
    typename std::vector<T> m_vector;

public:
  /*
   * its template (of forward iterator)
    class iterator{
    public:
        using iterator_category= std::forward_iterator_tag;
        using value_type= T;
        using difference_type= std::ptrdiff_t;
        using pointer= T*;
        using reference = T&;

        iterator(pointer ptr = nullptr): m_ptr(ptr){}

        reference operator*() const {return *m_ptr;}
        pointer operator->() const {return m_ptr;}

        iterator& operator++(){ ++m_ptr; return *this;}
        iterator  operator++(int){iterator iter = *this; ++m_ptr; return iter;}

        bool operator==(const iterator& other) const {return m_ptr == other.m_ptr;}
        bool operator!=(const iterator& other) const {return m_ptr != other.m_ptr;}
    private:
        pointer m_ptr;
    };*/
};


























#ifdef obsolete
template <typename T>
class RingQueueObsolete : public std::list<T>// переопределены только push_front фунции
{
public:

RingQueueObsolete(int limit)
    : std::list<T>()
    , limit(limit)
{}

void push_front(const T& value) {
    if (std::list<T>::size() == limit) {
        std::list<T>::pop_back();
    }
    std::list<T>::push_front(value);
}

void push_front(T&& value) {
    if (std::list<T>::size() == limit) {
        std::list<T>::pop_back();
    }
    std::list<T>::push_front(std::move(value));
}

void push_back(const T&) {
    assert(false && "use push_front instead");
}

void push_back(T&&) {
    assert(false && "use push_front instead");
}

private:
    unsigned int limit;
};
#endif // obsolete
#endif // RINGQUEUE_H
