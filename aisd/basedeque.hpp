#ifndef BASEDEQUE_HPP
#define BASEDEQUE_HPP

template <typename T>
class BaseDeque {
public:
    BaseDeque() = default; // default constructor
    ~BaseDeque() = default;
    BaseDeque(const BaseDeque& other) = default; // copy constructor
    BaseDeque(BaseDeque&& other) = default; // move constructor
    BaseDeque& operator=(const BaseDeque& other) = default; // copy assignment operator, return *this
    BaseDeque& operator=(BaseDeque&& other) = default; // move assignment operator, return *this
    virtual bool empty() const = 0; // checks if the container has no elements
    virtual bool full() const = 0; // checks if the container is full
    virtual std::size_t size() const = 0; // liczba elementow
    virtual std::size_t max_size() const = 0; // najwieksza mozliwa liczba elementow
    virtual void push_front(const T& item) = 0; // dodanie na poczatek
    virtual void push_front(T&& item) = 0; // dodanie na poczatek
    virtual void push_back(const T& item) = 0; // dodanie na koniec
    virtual void push_back(T&& item) = 0; // dodanie na koniec
    virtual T& front() = 0; // zwraca poczatek, nie usuwa
    virtual T& back() = 0; // zwraca koniec, nie usuwa
    virtual void pop_front() = 0; // usuwa poczatek
    virtual void pop_back() = 0; // usuwa koniec
    virtual void clear() = 0; // czyszczenie z elementow
    virtual void display() const = 0;
    virtual void display_reversed() const = 0;
    // Operacje z indeksami.
    virtual T& operator[](std::size_t pos) = 0; // podstawienie L[pos]=item, odczyt L[pos]
    virtual const T& operator[](std::size_t pos) const = 0; // dostep do obiektu const
    virtual void erase(std::size_t pos) = 0;
    virtual int index(const T& item) const = 0; // jaki index (-1 gdy nie ma)
    virtual void insert(std::size_t pos, const T& item) = 0; // inserts item before pos,
    virtual void insert(std::size_t pos, T&& item) = 0; // inserts item before pos,
};

#endif

// EOF
