#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// Helper function to format binary representation of a marking
string format_binary_representation(const vector<int>& marking) {
    ostringstream oss;
    for (int token : marking) {
        oss << token;
    }
    return oss.str();
}

// Global variables for storing the reachability tree
vector<vector<int>> markings;          // Stores all discovered markings
vector<int> parent_nodes;              // Parent indices for edges
vector<int> child_nodes;               // Child indices for edges
vector<string> transition_labels;      // Transition labels for edges

// DFS function prototype
void dfs(const vector<int>& current_marking, int parent_idx,
         const vector<string>& transitions,
         const map<string, pair<vector<int>, vector<int>>>& transition_map);

// Helper to check if a marking is already visited
int find_marking_index(const vector<int>& marking) {
    auto it = find(markings.begin(), markings.end(), marking);
    if (it == markings.end()) return -1;
    return static_cast<int>(it - markings.begin());
}

int main() {
    // Define places and transitions
    vector<string> places = {"p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9"};
    vector<string> transitions = {"t1", "t2", "t3", "t4", "t5", "t6"};

    // Initial marking: only p9 has a token
    vector<int> initial_marking = {0,0,0,0,0,0,0,0,1};

    // Transition input/output mappings
    map<string, pair<vector<int>, vector<int>>> transition_map = {
        {"t1", {{0}, {1}}},          // p1 -> p2
        {"t2", {{2}, {3}}},          // p3 -> p4
        {"t3", {{4}, {5}}},          // p5 -> p6
        {"t4", {{6}, {7}}},          // p7 -> p8
        {"t5", {{8}, {0,2,4,6}}},    // p9 -> p1 or p3 or p5 or p7
        {"t6", {{1,3,5,7}, {8}}}     // p2 or p4 or p6 or p8 -> p9
    };

    // Initialize the markings with the initial marking
    markings.push_back(initial_marking);

    cout << "Constructing Reachability tree using DFS...\n";
    // The root of the tree is the initial marking at index 0
    dfs(initial_marking, 0, transitions, transition_map);

    cout << "Reachability tree constructed successfully!\n";


    // Define the target marking
    vector<int> target_marking = {0,1,1,0,0,1,1,1,0};

    // Check if the target marking is in 'markings'
    auto it = find(markings.begin(), markings.end(), target_marking);
    if (it != markings.end()) {
        cout << "The marking 011001110 is reachable." << endl;
    } else {
        cout << "The marking 011001110 is not reachable." << endl;
    }


    // Generate .dot file
    ofstream dot_file("reachability_tree.dot");
    dot_file << "digraph ReachabilityTree {\n";
    dot_file << "    rankdir=TB;\n";
    dot_file << "    node [shape=circle, style=filled, fillcolor=lightblue, fontcolor=black];\n";
    dot_file << "    edge [color=black, arrowhead=vee];\n";

    for (size_t i = 0; i < parent_nodes.size(); ++i) {
        if (parent_nodes[i] < (int)markings.size() && child_nodes[i] < (int)markings.size()) {
            dot_file << "    \"" << format_binary_representation(markings[parent_nodes[i]])
                     << "\" -> \"" << format_binary_representation(markings[child_nodes[i]])
                     << "\" [label=\"" << transition_labels[i] << "\"];\n";
        }
    }

    dot_file << "}\n";
    dot_file.close();

    cout << "Tree saved to 'reachability_tree.dot'. Use Graphviz to visualize it.\n";

    // Now we add the functionality to find a state that leads to the given final marking after firing t1,t2,t6,t5,t4,t3
    vector<int> target_final_marking = {1,0,1,0,0,1,0,1,0};
    vector<string> desired_sequence = {"t1", "t2", "t6", "t5", "t4", "t3"};

    // Build adjacency lists: from each state, which transitions lead to which next states?
    // We'll store: state_adjacency[state] = vector of (transition, next_state)
    vector<vector<pair<string,int>>> state_adjacency(markings.size());
    for (size_t i = 0; i < parent_nodes.size(); ++i) {
        int p = parent_nodes[i];
        int c = child_nodes[i];
        string t = transition_labels[i];
        state_adjacency[p].push_back({t, c});
    }

    // Try to find a state from which if we fire t1,t2,t6,t5,t4,t3 we end at target_final_marking
    // We'll check each reachable state as a potential starting point.
    bool found = false;
    for (size_t start = 0; start < markings.size(); ++start) {
        int current = (int)start;
        bool sequence_matched = true;
        // Follow the transitions in desired_sequence order
        for (auto &trans : desired_sequence) {
            // Find an edge with this transition from 'current'
            bool edge_found = false;
            for (auto &edge : state_adjacency[current]) {
                if (edge.first == trans) {
                    current = edge.second; // move to next state
                    edge_found = true;
                    break;
                }
            }
            if (!edge_found) {
                sequence_matched = false;
                break;
            }
        }

        if (sequence_matched && markings[current] == target_final_marking) {
            cout << "Found a state from which firing t1,t2,t6,t5,t4,t3 leads to [1 0 1 0 0 1 0 1 0]^T:\n";
            cout << "Starting State: " << format_binary_representation(markings[start]) << "\n";
            found = true;
            break; // If we only need one state, we can stop here
        }
    }

    if (!found) {
        cout << "No state found from which firing the sequence t1,t2,t6,t5,t4,t3 leads to [1 0 1 0 0 1 0 1 0]^T.\n";
    }

    return 0;
}

void dfs(const vector<int>& current_marking, int parent_idx,
         const vector<string>& transitions,
         const map<string, pair<vector<int>, vector<int>>>& transition_map) {

    cout << "Processing marking: ";
    for (int token : current_marking) {
        cout << token << " ";
    }
    cout << endl;

    // Try each transition
    for (const auto& t : transitions) {
        vector<int> input_places = transition_map.at(t).first;
        vector<int> output_places = transition_map.at(t).second;

        // Check if the transition can fire
        bool can_fire = true;
        for (int place : input_places) {
            if (current_marking[place] <= 0) {
                can_fire = false;
                break;
            }
        }

        if (can_fire) {
            // Fire the transition and get a new marking
            vector<int> new_marking = current_marking;

            // Consume tokens
            for (int place : input_places) {
                new_marking[place]--;
            }
            // Produce tokens
            for (int place : output_places) {
                new_marking[place]++;
            }

            cout << "Transition " << t << " fired. New marking: ";
            for (int token : new_marking) {
                cout << token << " ";
            }
            cout << endl;

            int idx = find_marking_index(new_marking);
            if (idx == -1) {
                // New marking discovered
                markings.push_back(new_marking);
                int child_idx = (int)markings.size() - 1;
                parent_nodes.push_back(parent_idx);
                child_nodes.push_back(child_idx);
                transition_labels.push_back(t);

                // Recurse (DFS) into the new marking
                dfs(new_marking, child_idx, transitions, transition_map);
            } else {
                // Already visited marking, just record the edge
                parent_nodes.push_back(parent_idx);
                child_nodes.push_back(idx);
                transition_labels.push_back(t);
            }
        } else {
            cout << "Transition " << t << " cannot fire for marking: ";
            for (int token : current_marking) {
                cout << token << " ";
            }
            cout << endl;
        }
    }
}
