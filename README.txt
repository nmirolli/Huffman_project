#####################################################################################################
#																									#
#	Program: Algorithms Project 2 Huffman Coding													#
#	File:    README.txt																				#
#	Author:  Nicholas Mirolli																		#
#	Email:   ndm28@zips.uakron.edu																	#
#																									#
#####################################################################################################

=====================================================================================================
RUNNING THE PROJECT
=====================================================================================================

1) First download the zip file.  This project is written in C++ using the netbeans IDE. The zipped
	file is a netbeans project that has been zipped.  If you want to use something other than
	netbeans to open the project please grab the main.cpp file from the extracted project and include
	all the necessary test cases.  *NOTE* this project has only been tested using netbeans

2) Open the project using netbeans.  Build and clean the project and then click run.

3) Follow the prompts of the program through the standard output.  Make sure to include the .txt
	extension on the file names when you type them in.  The program will say that the file cannot
	be found if you exclude the .txt extension.

4) When the program finishes running check the file path where the program is saved for the .hzip
	file.  In order to check the size of the .hzip file right click on the file and go to properties.


=====================================================================================================
EXPLAINATION OF SOURCE CODE
=====================================================================================================

Classes and Functions 
-----------------------------------------------------------------------------------------------------
class Node{} 
	This is the basic node class.
	Attributes include: frequency of letters, a constructor, and a virtual destructor.

class IntNode : public Node {}
	This is the internal node class.
	Attributes include: a left child of class Node, a right child of class Node, a constructor, and 
		a destructor which deletes the children.

class LeafNode : public Node {}
	This is the leaf node class.
	Attributes include: a char to store the unique character, a constructor, and a destructor

class Comparison {}
	This is a comparison class that is used to compare to nodes to find out their order in the tree.
		It compares the left child's frequency to the right child's frequency and returns true if 
		the left child's frequency is greater, and false if the left child's frequency is less.

Node* treeBuild(const int (&frequency)[uniqueChar]) {}
	This function constructs the huffman tree by using a priority queue.  It pushes all the leaf
		nodes into the queue and then combines the two with the lowest frequency in order to 
		create an internal node and build the tree.

void codeGen(const Node* node, const codeVector& prefix, codeMap& outCodes) {}
	This function generates all the huffman codes for the unique characters in the file and stores
		them in a Map.  If the node is a leaf node it generates the code for that node and if the
		node is an internal node it keeps traversing the tree until it reaches a leaf node.

void postTraverse(const Node* node, codeMap outCodes, codeMap::const_iterator itr, int freq[]) {}
	This function traverses the tree in post order and outputs the traversal to standard output.
		If the node is a leaf node it prints the node's character and frequency, if the node is 
		an internal node it recursively calls the function until another leave node is reached 
		or the tree is empty.  


*NOTE* For more detailed information and examples please refer to the comments in the source code

=====================================================================================================
INTERESTING THINGS ABOUT THIS PROGRAM
=====================================================================================================

This program was the first time I used the storage container of type MAP so it was interesting to 
	learn how to use them.