#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct TrieNode {
    string move; // Represents the move made as a string in UCI format (e.g. e2e4)
    int whiteWins = 0; // Keeps track of how many times white won the game after reaching this position
    int blackWins = 0; // Keeps track of how many times black won the game after reaching this position
    int draws = 0; // Keeps track of how many games ended in a draw after reaching this position
    vector<int> children; // A list of indices of the child nodes
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
        TrieNode n;
        n.move = move;
        nodes.push_back(n);
        int index = (int)(nodes.size() - 1);
        nodes[parent].children.push_back(index); // Adds the index of the new move to the parent's "children vector"
        return index;
    }

    // Finds the index of the last move in the sequence if it exists, otherwise returns -1
    int find(const vector<string>& moves) const {
        int current = 0;
        for (const string& move : moves) {
            current = step(current, move);
            if (current == -1) {
                return -1; // Move not found in trie
            }
        }
        return current; // Return the index of the last move in the sequence
    }

    // Steps from the given node to the next node corresponding to the move, if it exists. Returns -1 if not found.
    int step(int node, const string& move) const {
        for (int childIndex : nodes[node].children) {
            if (nodes[childIndex].move == move) {
                return childIndex;
            }
        }
        return -1; // Move not found
    }

    // Returns a vector of the 3 most frequently played replies in order
    vector<TrieNode> returnNMoves(const vector<string>& moves, int n) const {
        int currentIndex = find(moves);
        if (currentIndex == -1) {
            cout << "Position not found." << endl;
            return;
        }
        vector<int> sorted = nodes[currentIndex].children;
        sort(sorted.begin(), sorted.end(), [this](int a, int b) {
            int totalA = nodes[a].whiteWins + nodes[a].blackWins + nodes[a].draws;
            int totalB = nodes[b].whiteWins + nodes[b].blackWins + nodes[b].draws;
            return totalA > totalB; // Sort in descending order of total games played
        });
        int numToPrint = min(n, (int)sorted.size());
        vector<TrieNode> topMoves;
        for (int i = 0; i < numToPrint; i++) {
            topMoves.push_back(nodes[sorted[i]]);
        }
        return topMoves;

    }

    size_t memoryBytes() const {
        size_t total = nodes.capacity() * sizeof(TrieNode);
        for (const TrieNode& node : nodes) {
            total += node.children.capacity() * sizeof(int);
        }
        return total;
    }

};

// Loads games from a file into the trie. Each line in the file should represent a game in the format:
// result move1 move2 move3 ...
// Returns the number of games successfully loaded into the trie.
int loadFile(Trie& trie, const string& filepath, int maxGames, int maxPly) {
    ifstream in(filepath);
    if (!in) {
        cout << "Could not open " << filepath << endl;
        return 0;
    }
    string line;
    int count = 0;
    while (count < maxGames && getline(in, line)) {
        istringstream ss(line);
        string res, tok;
        ss >> res;
        int result;
        if (res == "1-0") {
            result = 1;
        } else if (res == "0-1") {
            result = -1;
        } else if (res == "1/2-1/2") {
            result = 0;
        } else {
            continue; // Skip invalid results
        }

        vector<string> moves;
        while ((int)moves.size() < maxPly && ss >> tok) {
            moves.push_back(tok);
        }

        trie.insert(moves, result);
        count++;
    }
    return count;
}
