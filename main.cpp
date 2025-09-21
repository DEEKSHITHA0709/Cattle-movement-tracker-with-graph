#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

class CattleMovementTracker {
    unordered_map<string, list<string>> graph;           // For internal graph logic
    vector<pair<string, string>> movementList;           // For preserving order of input

public:
    void addMovement(string from, string to) {
        graph[from].push_back(to);                       // Used for path and cycle logic
        movementList.push_back({from, to});              // Used for ordered display
    }

    void printMovements() {
        cout << "\nCattle Movements (in entry order):\n";
        for (auto& move : movementList) {
            cout << move.first << " -> " << move.second << "\n";
        }
    }

    void shortestPath(string start, string end) {
        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        queue<string> q;

        // Check if start exists
        if (graph.find(start) == graph.end()) {
            cout << "Start location does not exist!\n";
            return;
        }

        // Check if end exists as key or value
        bool endExists = false;
        if (graph.find(end) != graph.end()) {
            endExists = true;
        } else {
            for (auto& pair : graph) {
                for (string dest : pair.second) {
                    if (dest == end) {
                        endExists = true;
                        break;
                    }
                }
            }
        }

        if (!endExists) {
            cout << "End location does not exist!\n";
            return;
        }

        q.push(start);
        visited[start] = true;
        parent[start] = "";

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            if (current == end) break;

            for (string neighbor : graph[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        if (!visited[end]) {
            cout << "No path found!\n";
            return;
        }

        stack<string> path;
        for (string at = end; at != ""; at = parent[at])
            path.push(at);

        cout << "Shortest Path: ";
        while (!path.empty()) {
            cout << path.top() << " ";
            path.pop();
        }
        cout << "\n";
    }

    bool hasCycleUtil(string node, unordered_map<string, bool>& visited, unordered_map<string, bool>& recStack) {
        visited[node] = true;
        recStack[node] = true;

        for (string neighbor : graph[node]) {
            if (!visited[neighbor] && hasCycleUtil(neighbor, visited, recStack))
                return true;
            else if (recStack[neighbor])
                return true;
        }

        recStack[node] = false;
        return false;
    }

    void detectCycle() {
        unordered_map<string, bool> visited;
        unordered_map<string, bool> recStack;

        for (auto& pair : graph) {
            string node = pair.first;
            if (!visited[node] && hasCycleUtil(node, visited, recStack)) {
                cout << "Cycle Detected in Movement\n";
                return;
            }
        }
        cout << "No Cycle Detected\n";
    }

    void countUniqueLocations() {
        unordered_set<string> allLocations;

        for (auto& move : movementList) {
            allLocations.insert(move.first);
            allLocations.insert(move.second);
        }

        cout << "Total Unique Locations: " << allLocations.size() << "\n";
        cout << "Locations: ";
        for (auto& loc : allLocations)
            cout << loc << " ";
        cout << "\n";
    }
};

// Main with menu
int main() {
    CattleMovementTracker tracker;
    int choice;
    string from, to, start, end;

    do {
        cout << "\n--- Cattle Movement Tracker ---\n";
        cout << "1. Add Movement\n";
        cout << "2. Display Movements\n";
        cout << "3. Find Shortest Path\n";
        cout << "4. Detect Cycle\n";
        cout << "5. Count Unique Locations\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter from location: ";
            cin >> from;
            cout << "Enter to location: ";
            cin >> to;
            tracker.addMovement(from, to);
            break;
        case 2:
            tracker.printMovements();
            break;
        case 3:
            cout << "Enter start location: ";
            cin >> start;
            cout << "Enter end location: ";
            cin >> end;
            tracker.shortestPath(start, end);
            break;
        case 4:
            tracker.detectCycle();
            break;
        case 5:
            tracker.countUniqueLocations();
            break;
        case 6:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}