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

#ifndef SINGLE_NODE_H
#define SINGLE_NODE_H

#ifndef nullptr
#define nullptr 0
#endif

#include <string>
#include <cctype>
#include "ece250.h"

class Trie;

class Trie_node {
	private:
		Trie_node **children;
		bool is_terminal;
		static int const CHARACTERS = 26;


	public:
		Trie_node();
		Trie_node *child( int ) const;

		bool member( std::string const &, int ) const;

		bool insert( std::string const &, int );
		bool erase( std::string const &, int, Trie_node *& );
		void clear();

		friend class Trie;

		// if ptr is a pointer to a Trie_node object
		// in one of the friendly classes, you should:
		//    use   ptr->next_   to modify it
		//    use   ptr->next()      to access it
};

Trie_node::Trie_node():
children( nullptr ),
is_terminal( false ) {
	// empty constructor
}

Trie_node *Trie_node::child( int n ) const {
	if ( children == nullptr ) {
		return nullptr;
	} else {
		return children[n];
	}
}

bool Trie_node::member( std::string const &str, int depth ) const {
    
    //Checks if length is less than depth, should never happen but just in case
    if(str.length() < depth)
    {
        return false;
    }
    
    // If the length of the string and the depth is the same I return the value of the is_terminal
    if(str.length() == depth)
    {
        return is_terminal;
    }
    
    
    // Calculates offsets and new depth
    int ndepth = depth +1;
    char letter = tolower(str[depth]);
    int offset = static_cast<int>( letter - 'a' );
    
    // If the node at is nullptr I return empty because there word must not be there
    if(child(offset) == nullptr)
    {
        return false;
    }
    else
    {
        // Recursing
        return child(offset)->member(str, ndepth);
    }

}

bool Trie_node::insert( std::string const &str, int depth ) {
    
    //Checks if length is less than depth, should never happen but just in case
    if(str.length() < depth)
    {
        return false;
    }
    
    // If the length of the string and the depth is the same I return false if the is_terminal is true
    // beacuse the word already exits, if the is_terminal is false I set the is_terminal to true and return true
    if(str.length() ==  depth)
    {
        if(is_terminal == true)
        {
            return false;
        }
        else
        {
            is_terminal = true;
            return true;
        }
    }
    
    // Calculates new depth and offset
    int ndepth = depth +1;
    char letter = tolower(str[depth]);
    int offset = static_cast<int>( letter - 'a' );
    
    // If the children array is nullptr I create it and set each entry to nullptr
    if (children == nullptr)
    {
        children =  new Trie_node*[CHARACTERS];
        for (int i = 0; i < CHARACTERS; i++){
            children[i] = nullptr;
        }
    }
    
    // If the node doesn't exit I set instantiate a new node
    if(children[offset] == nullptr)
    {
        children[offset] = new Trie_node;
    }
    
    // Recursing
    return child(offset)->insert(str, ndepth);
}

bool Trie_node::erase( std::string const &str, int depth, Trie_node *&ptr_to_this )
{
    // Two booleans used to track conditions
    static bool endgame;
    static bool deleteahead = false;
    
    //Checks if length is less than depth, should never happen but just in case
    if(str.length() < depth)
    {
        return false;
    }
    
    // Checks when the length of the string and depth is the same along with if the is_terminal is true
    if(str.length() == depth && is_terminal==true)
    {
        // If children is nullptr it deletes the node its at, sets one bool condition to false and returns true
        // (This means that theres not entries below this now
        if(children == nullptr)
        {
            delete this;
            endgame = false;
            return true;
        }
        
        //Opposite of the above, this means that there are other nodes below this node.
        // In this case we just set the is_terminal to false and set the same bool condition to true instead and return true.
        else
        {
            is_terminal = false;
            endgame = true;
            return true;
        }
        
        
    }
    // If the last node we are looking for has a false for an is_terminal its not a word we inserted
    // Therefore we don't delete it
    else if(str.length() == depth && is_terminal==false)
    {
        return false;
    }
    
    // Calculates new depth and offset
    int ndepth = depth +1;
    char letter = tolower(str[depth]);
    int offset = static_cast<int>( letter - 'a' );
    
    // If a node I'm at doesn't exist I return false.
    if(child(offset) == nullptr)
    {
        return false;
    }
    
    // If statement starts the recursion
    if(child(offset)->erase(str, ndepth, children[offset]))
    {
        // First Case: Checks the array for any other nodes if there is a value it doesnt delete anything and returns true
        // If there are no values in the array I delete the array and the node.
        if(endgame == false && is_terminal == false && deleteahead == false)
        {
            children[offset] = nullptr;
            for(int i = 0; i < CHARACTERS; ++i)
            {
                if(child(i) != nullptr)
                {
                    // Deleteahead sets the second bool condition, this means that I don't delete anything else on the way up.
                    deleteahead = true;
                    
                    //Since I created my bool to be static I need to reset the deleteahead condition back to false as it doesn't do
                    // this automatically
                    if(depth == 0)
                    {
                        deleteahead = false;
                    }
                    return true;
                }
            }
            
            delete [] ptr_to_this->children;
            delete this;
            ptr_to_this->children = nullptr;
            return true;
        }
        
        // Similar to the condition above but the node I am at not has an is_terminal value of true instead.
        else if(endgame == false && is_terminal == true)
        {
            children[offset] = nullptr;
            for(int i = 0; i < CHARACTERS; ++i)
            {
                if(child(i) != nullptr)
                {
                    deleteahead = true;
                    if(depth == 0)
                    {
                        deleteahead = false;
                    }
                    return true;
                }
            }
            
            
            delete [] ptr_to_this->children;
            ptr_to_this->children = nullptr;
            deleteahead = true;
            return true;
        }
        
        // This condition doesn't delete anything as the word I am trying to delete is vital as it contains
        // other nodes
        else
        {
            deleteahead = true;
            
            if(depth == 0)
            {
                deleteahead = false;
            }
            
            return true;
        }
    }
    else
    {
        return false;
    }

}

void Trie_node::clear()
{
    // If children is not nullptr checks each entry in the array for a value. If it has a value
    // I recurse again. Once I found a leaf node I delete the array and the node and begin recursing back up
    // and checking other nodes.
    if(children != nullptr)
    {
        for(int i = 0; i < CHARACTERS; ++i)
        {
            if(children[i] != nullptr)
            {
                child(i)->clear();
                delete children[i];
            }
        }
    }
    
    delete [] children;
}


// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
