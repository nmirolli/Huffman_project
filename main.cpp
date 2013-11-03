/* 
 * File:   main.cpp
 * Author: Nicholas Mirolli
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>

using namespace std;

const int uniqueChar = 1 << CHAR_BIT;
typedef vector<bool> codeVector;
typedef map<char, codeVector> codeMap;
string postTraverseStr = "";

class Node {                            //basic node class
        public: 
            const int freq;             //frequency of letters
            Node(int f) : freq(f) {}    //constructor
            virtual ~Node() {}          //virtual destructor for polymorphism later in program
};

class IntNode : public Node {           //internal node class
        public:
            Node *const left;           //left child node
            Node *const right;          //right child node
            IntNode(Node* l, Node* r) : Node(l->freq + r->freq), left(l), right(r) {}   //constructor
            ~IntNode() {                //destructor
                delete left;            //deletes the left child
                delete right;           //deletes the right child
            }
};

class LeafNode : public Node {          //leaf node class
        public:
            const char c;               //character 
            LeafNode(int f, char character) : Node(f), c(character) {}  //constructor
            ~LeafNode() {}              //destructor
};

class Comparison {      //Compares nodes in order to properly sort the tree
        public:
            bool operator()(const Node* left, const Node* right) const { 
                return (left->freq > right->freq); 
            }
};

Node* treeBuild(const int (&frequency)[uniqueChar]) {           //Builds the tree
    priority_queue<Node*, vector<Node*>, Comparison> tree;      //creates priority queue
    for (int i = 0; i < uniqueChar; ++i) {                      //pushes leaf nodes of unique character onto tree
        if(frequency[i] != 0)
            tree.push(new LeafNode(frequency[i], (char)i));
    }
    while (tree.size() > 1) {                                   //loops through tree combining leaf nodes to make internal nodes
        Node* rightChild = tree.top();
        tree.pop();
        Node* leftChild = tree.top();
        tree.pop();
        Node* parent = new IntNode(rightChild, leftChild);
        tree.push(parent);
    }
    return tree.top();                                          //returns the node which is the top of the tree
}

void codeGen(const Node* node, const codeVector& prefix, codeMap& outCodes) {   //generates the huffman codes for the characters
    if (const LeafNode* leaf = dynamic_cast<const LeafNode*>(node)) {           //if the node is a leaf node generate it's code
        outCodes[leaf->c] = prefix;
    }
    else if (const IntNode* in = dynamic_cast<const IntNode*>(node)) {          //if the node is an internal node keep traversing
        codeVector lPrefix = prefix;
        lPrefix.push_back(false);
        codeGen(in->left, lPrefix, outCodes);                                   //recursive call on left child
 
        codeVector rPrefix = prefix;
        rPrefix.push_back(true);
        codeGen(in->right, rPrefix, outCodes);                                  //recursive call on right child
    }
}

void postTraverse(const Node* node, codeMap outCodes, codeMap::const_iterator itr, int freq[]) {        //post order traversal of the huffman tree
    if(node) {
        stringstream strStream;
        string str;
        if(const LeafNode* leaf = dynamic_cast<const LeafNode*>(node)) {        //if node is a leaf node
            strStream << leaf->c;                                               //sends character to stringstream
            strStream << leaf->freq;                                            //sends frequency to stringstream
            strStream >> str;                                                   //sends stringstream to string
            postTraverseStr.append(str);                                        //appends to global string
        }
        else if(const IntNode* in = dynamic_cast<const IntNode*>(node)) {       //if node is an internal node
            postTraverse(in->left, outCodes, itr++, freq);                      //recursive call on left child
            postTraverse(in->right, outCodes, itr++, freq);                     //recursive call on right child
            strStream << "@";                                                   //adds @ to identify internal node
            strStream << in->freq;                                              //sends frequency to stringstream
            strStream >> str;                                                   //sends stringstream to string
            postTraverseStr.append(str);                                        //appends to global string
        }
    }
}

int main(int argc, char** argv) {
    
    //get file to be compressed
    string filename;
    cout << "Name of file to be compressed: " << endl;
    getline(cin, filename);
    string outputFilename = filename.substr(0, filename.size()-4);
    
    //read file in and stores letter and frequency in a map
    ifstream inputFile;
    int letterCount[uniqueChar] = {0};
    
    // line length is limited to 1023 bytes
    char buffer[1024];
    
    //open file for reading
    inputFile.open(filename.c_str());
    if (inputFile.is_open()) {
        string file_contents;
        
        //send contents of file to a string 
        while (inputFile.good()) {
            inputFile.getline(buffer, sizeof(buffer));
            file_contents.append(buffer);
            file_contents.append("\n");
        }
        file_contents = file_contents.substr(0, file_contents.size()-1);

        //get the frequency of each unique letter in the file contents string
        char* ptr = new char[file_contents.size()+1];
        copy(file_contents.begin(), file_contents.end(), ptr);
        ptr[file_contents.size()] = '\0';
        while (*ptr != '\0') {
            ++letterCount[*ptr++];
        }
        
        //create and build the Tree
        Node* root = treeBuild(letterCount);
        
        //create Map of codes and generate the codes
        codeMap huffCodes;
        codeGen(root, codeVector(), huffCodes);
        
        //creates iterators and calls post traversal on the tree
        codeMap::const_iterator itr = huffCodes.begin();
        codeMap::const_iterator itr2 = huffCodes.begin();
        postTraverse(root, huffCodes, itr, letterCount);
        delete root;
        
        //Print out letter(frequency): code
        for (itr2; itr2 != huffCodes.end(); ++itr2) {
            cout << itr2->first << "(" << letterCount[itr2->first] <<")" << ": ";
            copy(itr2->second.begin(), itr2->second.end(), ostream_iterator<bool>(cout));
            cout << endl;
        }
        
        //prints post order traversal of huffman tree
        cout << "postTraverseStr = " << postTraverseStr << endl;
        
        //outputs the binary file with all ascii values replaced by their huffman code equivalents 
        string code;
        int length = file_contents.length();
        for(int i=0; i<length; i++) {
            for (codeMap::const_iterator itr3 = huffCodes.begin(); itr3 != huffCodes.end(); ++itr3) {
                if(file_contents[i] == itr3->first) {
                    stringstream strStream;
                        copy(itr3->second.begin(), itr3->second.end(), ostream_iterator<bool>(strStream));
                        string str = strStream.str();
                        code.append(str);
                }
            }
        }
        inputFile.close();
        
        //code from Duan example
        ofstream outputFile;
        outputFilename.append(".hzip");
        outputFile.open(outputFilename.c_str(), ios::binary);
        string zeros = "00000000";
        if (code.size()%8!=0)
           code += zeros.substr(0, 8-code.size()%8);

        int b; 
        for (int i = 0; i < code.size(); i+=8) {
           b = 1;
           for (int j = 0; j < 8; j++) {
              b = b<<1;
              if (code.at(i+j) == '1')
                b+=1;
           }
           char c = (char) (b & 255);
           outputFile.write(&c, 1);  
        }
        outputFile.close();
    }
    else {
        cout << "Unable to open "<< filename << endl;
    }
    
    return 0;
}

