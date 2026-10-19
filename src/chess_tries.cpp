#include <vector>
#include <string>
using namespace std;

struct TrieNode {
    string move; // Represents the move made as a string (e.g. e4)
    bool isWhite = true; // Keeps track of which color made the move
    int whiteWins = 0; // Keeps track of how many times white won the game after reaching this position
    int blackWins = 0; // Keeps track of how many times black won the game after reaching this position
    int draws = 0; // Keeps track of ho many games ended in a draw after reaching this position
    vector<int> children; // A list of indicies of the child nodes
};

class Trie {
public:
    vector<TrieNode> nodes; // Nodes are stored in a vector instead of using pointers to simplify memory usage.

    Trie() {nodes.push_back(TrieNode());} // Trie constructor

    // Inserts a game (represented as a vector of moves, where each move is a string) into the trie
    // For now, assume a result of 1 = white wins, 0 = draw, -1 = black wins
    void insert(const vector<string>& moves, int result) {
        int current = 0;
        for (const string& move : moves) {
            current = findOrAddChild(current, move);
            if (result == -1) {
                nodes[current].blackWins++;
            }
            if (result == 0) {
                nodes[current].draws++;
            }
            if (result == 1) {
                nodes[current].whiteWins++;
            }
        }
    }

    // Finds the desired next move in the trie if it exists, otherwise adds it.
    // Returns the index of the next move
    int findOrAddChild(int parent, const string& move) {
        for (int childIndex : nodes[parent].children) {
            if (nodes[childIndex].move == move) {
                return childIndex; // Next move already exists in trie
            }
        }
        // Next move does not exist, so add it to the trie
        Node n;
        n.move = move;
        nodes.push_back(n);
        int index = nodes.size() - 1;
        nodes[parent].children.push_back(index); // Adds the index of the new move to the parent's "children vector"
        return index;
    }

private:
    pass;

};
