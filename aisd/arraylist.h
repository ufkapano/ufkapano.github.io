// arraylist.h

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>     // deklaracje strumieni cout, cin, cerr
#include <cassert>    // assert()

template <typename T>
class ArrayList {
    T* tab;
    int msize; // najwieksza mozliwa liczba elementow
    int last; // pierwsza wolna pozycja
public:
    ArrayList(int s=10) : msize(s), last(0) {
        tab = new T[s];
        assert( tab != nullptr );
    } // default constructor
    ~ArrayList() { delete [] tab; }
    ArrayList(const ArrayList& other); // copy constructor
    // usage:   ArrayList<int> list2(list1);
    ArrayList(ArrayList&& other); // move constructor NIEOBOWIAZKOWE
    // usage:   ArrayList<int> list2(std::move(list1));
    ArrayList& operator=(const ArrayList& other); // copy assignment operator, return *this
    // usage:   list2 = list1; NIEOBOWIAZKOWE
    ArrayList& operator=(ArrayList&& other); // move assignment operator, return *this
    // usage:   list2 = std::move(list1);
    bool empty() const { return last == 0; } // checks if the container has no elements
    bool full() const { return last == msize; } // checks if the container is full
    int size() const { return last; } // liczba elementow na liscie
    int max_size() const { return msize; } // najwieksza mozliwa liczba elementow
    void push_front(const T& item); // dodanie na poczatek
    void push_front(T&& item); // dodanie na poczatek NIEOBOWIAZKOWE
    void push_back(const T& item); // dodanie na koniec
    void push_back(T&& item); // dodanie na koniec NIEOBOWIAZKOWE
    T& front(); // zwraca poczatek, nie usuwa, error dla pustej listy
    T& back(); // zwraca koniec, nie usuwa, error dla pustej listy
    void pop_front(); // usuwa poczatek, error dla pustej listy
    void pop_back(); // usuwa koniec, error dla pustej listy
    void clear(); // czyszczenie listy z elementow
    void display(); // lepiej zdefiniowac operator<<
    void reverse(); // odwracanie kolejnosci
    void sort(); // sortowanie listy
    void merge(ArrayList& other); //  merges two sorted lists into one
    // Operacje z indeksami. NIEOBOWIAZKOWE
    int erase(int pos); // return Iterator following the last removed element,
    // czyli u mnie pos, bo ten element za usunietym sie przesunie na pos;
    // ale jak usune ostatni, to chyba powinien zwrocic -1 (niewlasciwy indeks)
    //
    // https://en.cppreference.com/w/cpp/language/operators
    // Array subscript operator
    T& operator[](int pos); // podstawienie L[pos]=item
    const T& operator[](int pos) const; // odczyt L[pos]
    int index(const T& item); // jaki index na liscie (-1 gdy nie ma)
    int insert(int pos, const T& item); // inserts item before pos,
    int insert(int pos, T&& item); // inserts item before pos,
    // Jezeli pos=0, to wstawiamy na poczatek.
    // Jezeli pos=size(), to wstawiamy na koniec.
    // zwraca pozycje wstawionego elementu
    // zwraca pozycje wstawionego elementu
    friend std::ostream& operator<<(std::ostream& os, const ArrayList& L) {
        for (int i=0; i < L.last; ++i) { // odwolanie L.last
            os << L.tab[i] << " ";   // odwolanie L.tab
        }
        return os;
    }
    // usage:   std::cout << L << std::endl;
};

#endif

// EOF
