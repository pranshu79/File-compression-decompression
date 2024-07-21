#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
// for creating tree node of a Huffman tree.
struct Node
    {
        public:
            string val;//character for which node is created
            Node* left;//pointer to left child
            Node* right;//pointer to right child
            //parameterized constructor for creating node
            Node(string data){
                this->val = data;
            }
    };

class Decode {
public:
    int ind = 0;
    vector<string> splitter(const string& text) {
        stringstream ss(text);
        string token;
        vector<string> tokens;
        vector<string> data(3);
        char delimiter = '\n';
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        int i = 0;
        while(tokens[i].length() > 1){
            data[0] += tokens[i];
            i++;
            if(tokens[i].length() == 1)break;
            data[0] += '\n';
        }
        data[1] = tokens[i];
        i++;
        while(i < tokens.size()){
            data[2] += tokens[i];
            i++;
            if(i == tokens.size())break;
            data[2] += '\n';
        }
        return data;
    }
void insert_node(Node* root, string path, char val){
    for(int i = 0; i < path.length()-1; i++){
        if(path[i] == '0'){
            if(root->left == NULL){
                Node* node = new Node("terminal");
                root->left = node;
            }
            root = root->left;
        }
        else {
            if(root->right == NULL){
                Node* node = new Node("terminal");
                root->right = node;
            }
            root = root->right;
        }
    }
    string s(1,val);
    Node* node = new Node(s);
    if(path[path.length()-1] == '0')root->left = node;
    else root->right = node;
}
Node* destringify(string encoded_tree){
    Node* root = new Node("terminal");
    Node* node = root;
    string path;
    for(int i = 0; i < encoded_tree.length(); i++){
        if(encoded_tree[i] == 39){
            i++;
            insert_node(node, path, encoded_tree[i]);
            i++;
            node = root;
            if(path[path.length()-1] == '0')path.pop_back();
            else{
                path.pop_back();
                path.pop_back();
            }
        }
        path += encoded_tree[i];
    }
    return root;
}
};
void readFileContent(const string& filename, string& content) {
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
// void preorder(Node* root, vector<string> &pre){
//    if(root == NULL)return;
//    pre.push_back(root->val);
//    preorder(root->left, pre);
//    preorder(root->right, pre);
// }
int main() {
    string input_name, output_name, text;
    cout << "Enter The Name Of Input File" << endl;
    cin >> input_name;
    // cout << "Enter The Name Of Output File" << endl;
    // cin >> output_name;
    readFileContent(input_name, text);
    // Read text directly from standard input
    Decode decoder;
    vector<string> data = decoder.splitter(text);
    // cout << data[0] << endl;
    Node* root = decoder.destringify(data[0]);
    // vector<string> pre;
    // preorder(root, pre);
    // for(auto it : pre)cout << it << endl;
    string encoded_text = data[2];
    cout << encoded_text << endl;
    string binary_string = "";
    for(int i = 0; i < encoded_text.length(); i++){
        int num = encoded_text[i];
        if(num < 0)num+=256;
        string temp = "00000000";
        for(int i = 0; (i < 8 && num > 0); i++){
            if(num % 2 != 0)temp[7-i] = '1';
            num /= 2;
        }
        binary_string += temp;
    }
    int i = 0;
    int rem = data[1][0]-'0';
    while(i < rem){
       binary_string.pop_back();
       i++;
    }
    cout << binary_string.length() << endl;
    Node* node = root;
    string resultant = "";
    for(int i = 0; i < binary_string.length(); i++){
        if(binary_string[i] == '0')node = node->left;
        else node = node->right;
        if(node->val.length() == 1){
            resultant.push_back(node->val[0]);
            node = root;
        }
    }
    cout << resultant;
    return 0;
}
