#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// for creating tree node of a Huffman tree.
struct Node
    {
        public:
            char character;//character for which node is created
            int frequency;//frequency of character in document
            Node* left;//pointer to left child
            Node* right;//pointer to right child
            //parameterized constructor for creating node
            Node(char data, int freq){
                this->character = data;
                this->frequency = freq;
            }
    };
// custom comparator class for sorting inside priority queue based on frequency.
struct compare{
public:
    bool operator()(Node* p, Node* q)
    {
        return p->frequency > q->frequency; 
    } 
};
class encode{

public:

// function to create Huffman Tree.
Node* generateHuffmanTree(priority_queue<Node*,vector<Node*>,compare> pq)
{
    while(pq.size() > 1){
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* root = new Node('#',left->frequency+right->frequency);
        root->right = right;
        root->left = left;
        pq.push(root);
    }
    return pq.top();
}
// function to map characters to corresponding codes.
void saveCodes(Node* root, string s, unordered_map<char,string> &codes) {
    if (root == nullptr) return;
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->character] = s;
        return;
    }
    saveCodes(root->left, s+'0', codes);
    saveCodes(root->right, s+'1', codes);
}
string stringify(Node* root){
    if(root == NULL)return "";
    if(root->left == NULL && root->right == NULL){
        string temp = "'";
        temp.push_back(root->character);
        return temp;
    }
    return '0' + stringify(root->left) + '1' + stringify(root->right);
}
void preorder(Node* root, vector<int> &pre){
    if(root == NULL)return;
    pre.push_back(root->frequency);
    preorder(root->left,pre);
    preorder(root->right, pre);
}
unordered_map<char,string> huffmanCodes(unordered_map<char,int> m, string &tree) {
    priority_queue<Node*, vector<Node*>, compare> pq;
    for(auto it : m){
        Node* leafNode = new Node(it.first,it.second);
        pq.push(leafNode);
    }
    Node* root = generateHuffmanTree(pq);
    vector<int> pre;
    tree = stringify(root);
    // preorder(root,pre);
    string s;
    unordered_map<char,string> codes;
    saveCodes(root, s, codes);
    return codes;
}
};
void readFileContent(string& filename, string& content) {
    ifstream file(filename.c_str());
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            content += line + '\n'; // Assuming you want to keep newlines
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
        exit(1);
    }
}
// Function to create a text file with the given text
void createTextFile(const string& filename, const string& text) {
    ofstream file(filename.c_str());
    if (file.is_open()) {
        file << text;
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}
int main() {
    string input_name,output_name;
    cout << "Enter The Name Of Input File" << endl; 
    cin >> input_name;
    cout << "Enter The Name Of Output File" << endl; 
    cin >> output_name;
    string text;
    readFileContent(input_name, text);
    unordered_map<char,int> m;
    for(int i = 0; i < text.length()-1; i++){m[text[i]]++;}
    encode code;
    string tree;
    unordered_map<char,string> codes = code.huffmanCodes(m,tree);
    string binary_string = "";
    // creating binary string after encoding the text
    for(auto it : text){
        binary_string += codes[it];
    }
    int remaining = (8-binary_string.length()%8)%8;
    for(int i = 0; i < remaining; i++){
        binary_string.push_back('0');
    }
    string result = "";
    for(int i = 0; i < binary_string.length(); i += 8){
        int num = 0;
        for(int j = i; j < i+8; j++){
            num += (binary_string[j]-'0')*(1 << (i+7-j));
        }
        result.push_back(char(num));
    }
    string final_result = tree + '\n' + to_string(remaining) + '\n' + result;
    createTextFile(output_name,final_result);
    string info = "Compression Ratio : " + to_string(float(text.length())/float(final_result.length()));
    cout << "Compression Successfull ✅" << endl;
    cout << info << endl;
    return 0;
}
