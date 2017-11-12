#ifndef _STACK_HPP_
#define _STACK_HPP_

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <iterator>

namespace sc{ 
    template <typename T>
    class stack
    {
        private:
            std::size_t m_top;      //<! Topo da pilha
            std::size_t m_capacity; //<! Tamanho físico
            T *  m_data;            //<! Area de armazenamento

            /** 
            * @brief Reserva memória para uma dada quantidade de elementos.
            * @param new_cap Nova capacidade.
            */
            void reserve(std::size_t new_cap);

        public:
            /// Construtor.
            stack(std::size_t cap = 1);
            
            /// Destutor.
            ~stack();
            /**
            * @brief Insere elemento no topo da pilha.
            * @param value Novo elemento.
            */
            void push(const T & value);

            /**
            * @brief Remove elemento do topo da pilha, se houver.
            */
            void pop();
            
            /**
            * @return Elemento no topo da pilha, se houver.
            */
            T top();  

            /**
            * @return Verificação se na pilha existem elementos.
            */
            bool empty();
    };

    template <typename T>
    sc::stack<T>::stack( std::size_t cap): m_top(0), m_capacity(cap), m_data(new T[cap]){}

    template <typename T>
    sc::stack<T>::~stack(){ delete [] m_data; }

    template <typename T>
    void sc::stack<T>::push(const T & value){
        if(m_top == m_capacity) reserve(m_capacity * 2);

        m_data[m_top++] = value;
    }

    template <typename T>
    void sc::stack<T>::pop( void ){
        if(m_top == 0) throw std::out_of_range("pop() - não há elementos na pilha");
        --m_top;
    }

    template <typename T>
    T sc::stack<T>::top() {
        if(m_top == 0) throw std::out_of_range("top() - não há elementos na pilha");
        return m_data[m_top-1]; 
    }

    template <typename T>
    void sc::stack<T>::reserve( std::size_t new_cap ){
        if(new_cap > m_capacity){
            T *temp = new T[ new_cap ];

            std::copy(m_data, m_data + new_cap, temp);

            delete m_data;

            m_data = temp;
            m_capacity = new_cap;
        }
    }

    template <typename T>
    bool sc::stack<T>::empty() {  return m_top == 0; }
}

#endif
