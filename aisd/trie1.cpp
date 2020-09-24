// Przygotowane na podstawie programow ze stron:
// https://www.geeksforgeeks.org/trie-insert-and-search/
// https://www.geeksforgeeks.org/trie-delete/

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <utility>  // make_pair()
#include <cassert>    // assert()

//using namespace std;

const int ALPHABET_SIZE = 26;

std::string alphabet("abcdefghijklmnopqrstuvwxyz");

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE]; // tablica wskaznikow

    // isEndOfWord is true if the node represents end of a word 
    bool isEndOfWord;
};

int find_index(const char& ch) {
    return (ch - 'a');
}

// Returns new trie node (initialized to nullptr)
struct TrieNode *getNode(void) {
    struct TrieNode *node =  new TrieNode;
    assert( node != nullptr );
    node->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = nullptr;
    return node;
}

TrieNode* insert(struct TrieNode *root, std::string key) {
    if (root == nullptr)
        root = getNode();
    struct TrieNode *node = root;

    for (auto ch : key) { // petla zakresowa jak w pythonie
        int index = find_index(ch);
        if (node->children[index] == nullptr)
            node->children[index] = getNode();
        node = node->children[index];
    }
    // mark last node as leaf
    node->isEndOfWord = true;
    return root;
}

bool search(struct TrieNode *root, std::string key) {
    if (root == nullptr)
        return false;
    struct TrieNode *node = root;

    for (auto ch : key) { // petla zakresowa jak w pythonie
        int index = find_index(ch);
        if (node->children[index] == nullptr)
            return false;
        node = node->children[index];
    }
    return (node != nullptr && node->isEndOfWord);
}

// Returns true if root has no children, else false.
// Musialem dopuscic mozliwosc ze root to nullptr.
bool isEmpty(TrieNode* root) {
    if (root == nullptr)
        return true;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != nullptr)
            return false;
    return true;
}

// Recursive function to delete a key from given Trie.
// Jest rekurencja, wiec trzeba dopuscic, ze root bedzie nullptr.
// Dla konsystencji trzeba to uwzglednic w innych funkcjach.
TrieNode* remove(TrieNode* root, std::string key, int depth = 0) {
    // If tree is empty 
    if (root == nullptr)
        return nullptr;

    // If last character of key is being processed
    if (depth == int(key.size())) {
        // This node is no more end of word after removal of given key
        if (root->isEndOfWord)
            root->isEndOfWord = false;

        // If given is not prefix of any other word.
        // Jezeli to byl ostatni wezel, to mozna usunac.
        if (isEmpty(root)) {
            delete (root);
            root = nullptr;
        }
        return root;
    }
    // If not last character, recur for the child obtained using ASCII value 
    int index = find_index(key[depth]);
    root->children[index] = remove(root->children[index], key, depth + 1);

    // If root does not have any child (its only child got deleted),
    // and it is not end of another word.
    if (isEmpty(root) && root->isEndOfWord == false) {
        delete (root);
        root = nullptr;
    }
    return root;
}

std::vector<std::string> show_keys(TrieNode* root) {
    // If tree is empty 
    std::vector<std::string> v;
    if (root == nullptr)
        return v;
    if (root->isEndOfWord)
        v.push_back("");
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != nullptr)
            for (auto word : show_keys(root->children[i]))
                v.push_back(alphabet[i] + word);
    return v;
}

std::vector<std::string> show_prefix(TrieNode* root, std::string prefix) {
    TrieNode* node = root;
    std::vector<std::string> v;
    // Idziemy w dol drzewa po prefiksie.
    for (auto ch : prefix) { // petla zakresowa jak w pythonie
        int index = find_index(ch);
        if (node->children[index] == nullptr) {
            return v;
        } else {
            node = node->children[index];
        }
    }
    // Prefix jest w drzewie, szukamy uzupelnien w poddrzewie.
    for (auto word : show_keys(node))
        v.push_back(prefix + word);
    return v;
}

// Driver
int main() {
    // Input keys (use only 'a' through 'z' and lower case)
    std::string keys[] = { "the", "a", "there", "answer", "any", 
        "by", "bye", "their","hero", "heroplane" };
    int n = sizeof(keys) / sizeof(keys[0]);

    //struct TrieNode *root = getNode();
    struct TrieNode *root = nullptr;
    assert( isEmpty(root) );

    // Construct trie 
    for (int i = 0; i < n; i++)
        root = insert(root, keys[i]);
    assert( !isEmpty(root) );

    assert( search(root, "the") );
    assert( !search(root, "these") );
    // Test remove().
    root = remove(root, "heroplane");
    assert( search(root, "hero") );
    root = remove(root, "the");
    assert( search(root, "there") );
    std::cout << "show_keys ..." << std::endl;
    for (auto word : show_keys(root))
    //for (auto word : show_prefix(root, ""))
        std::cout << word << std::endl;
    std::cout << "show_prefix a ..." << std::endl;
    for (auto word : show_prefix(root, "a"))
        std::cout << word << std::endl;
    std::cout << "show_prefix th ..." << std::endl;
    for (auto word : show_prefix(root, "th"))
        std::cout << word << std::endl;
    return 0;
}

// EOF
