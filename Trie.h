/*****************************************
 * UW User ID:  uwuserid
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter|Spring|Fall) 20NN
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#ifndef TRIE_H
#define TRIE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Trie_node.h"
#include "Exception.h"
#include <iostream>
#include <string>
#include <cctype>

class Trie {
	private:
		Trie_node *root_node;
		int trie_size;

	public:
		Trie();
		~Trie();

		// Accessors

		int size() const;
		bool empty() const;
		bool member( std::string const & ) const;
		Trie_node *root() const; 
		// Mutators

		bool insert( std::string const & );
		bool erase( std::string const & );
		void clear();

	// Friends

	friend std::ostream &operator<<( std::ostream &, Trie const & );
};

Trie::Trie():
root_node( nullptr ),
trie_size( 0 ) {
	// empty constructor
}

Trie::~Trie() {
    // Calls clear method, deletes all nodes, arrays and the actual Trie
    clear();
}

int Trie::size() const {
    // Returns the number of words in the Trie
	return trie_size;
}

bool Trie::empty() const {
    // Returns true/false depending if Trie is empty or not
	return (size() == 0);
}

Trie_node *Trie::root() const {
    // Returns the root_node
	return root_node;
}

bool Trie::member( std::string const &str ) const {
    
    // Checks if theres any charcters in the string that are not the alaphabet
    for(int i = 0; i < str.length(); ++i)
    {
        if(!isalpha(str[i]))
        {
            throw illegal_argument();
        }
    }
    
    // If the string is empty or the root_node is nullptr returns false
    // root_node cannot be nullptr because then that means theres no words in Trie
    if(str.empty() || !root_node)
    {
        return false;
    }
    
    // Starts recursing
    else
    {
        return root_node->member(str, 0);
    }
    
}

bool Trie::insert( std::string const &str ) {
    
    // Checks if theres any charcters in the string that are not the alaphabet
    for(int i = 0; i < str.length(); ++i)
    {
        if(!isalpha(str[i]))
        {
            throw illegal_argument();
        }
    }
    
    // If root_node is null, creates the root_node
    if(root_node == nullptr)
    {
        root_node = new Trie_node;
    }
    
    //Starts recursing
    bool result = root_node->insert(str, 0);
    
    
    // If the insert was made I increase the counter
    if(result == true)
    {
        ++trie_size;
    }
    
    return result;
}

bool Trie::erase( std::string const &str ) {
    
    // Checks if theres any charcters in the string that are not the alaphabet
    for(int i = 0; i < str.length(); ++i)
    {
        if(!isalpha(str[i]))
        {
            throw illegal_argument();
        }
    }
    
    // If root_node is nullptr that means theres no words in the Trie, likewise for an empty string
    if(str.empty() || !root_node)
    {
        return false;
    }
    
    // Start recursing
    bool result = root_node->erase(str, 0, root_node);
    
    // If erase was made I decrease the counter
    if(result == true)
    {
        --trie_size;
    }
    
    // If the counter is 0 I delete the root_node
    if(trie_size == 0)
    {
        root_node = nullptr;
    }
    
    return result;
}

void Trie::clear() {
    
    // If root_node is not nullptr then I begin recursing
    // After the recursions are complete I delete the root_node and set the counter to 0
    // and lastly set the root_node to nullptr.
    if(root_node != nullptr)
    {
        root_node->clear();
        delete root_node;
        trie_size = 0;
    }
    root_node = nullptr;
}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Trie const &trie ) {
/*	for ( Trie_node *ptr = trie.head(); ptr != nullptr; ptr = ptr->next() ) {
		out << "->" << ptr->value();
	} */

	out << "->0";

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
