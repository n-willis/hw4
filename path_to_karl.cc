#include <sys/time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

constexpr char nicknamesTextPath[] = "nicknames.txt";
constexpr char linksTextPath[] = "links.txt";

class Timer {
public:
  Timer(const string& tag): tag_(tag) {
    timeval tv;
    gettimeofday(&tv, nullptr);
    begin_ = tv.tv_sec + tv.tv_usec / 1e6;
    cout << "=== Begin: " << tag_ << " ===" << endl;
  }

  ~Timer() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    double end = tv.tv_sec + tv.tv_usec / 1e6;

    cout << "Time elapsed: " << setprecision(3) << end - begin_ << " sec" << endl;
    cout << "=== End: " << tag_ << " ===" << endl;
  }

private:
  double begin_;
  string tag_;
};

// Print path
void print(vector<int> const &input) {
  for(auto const& i: input) {
    cout << i << " ";
  }
}

// Define a graph
class Graph {
public:
  int V;
  list<int> *nodes;
  Graph(int size) {
    V = size;
    nodes = new list<int>[V];
  }
  void addEdge(int from, int to) {
    nodes[from].emplace_back(to);
  }
};

// Depth-First Search
int DFS(Graph g, int start, int end) {
  vector<bool> visited(g.V, false);
  stack<int> stack;
  vector<int> path;
  int steps = 0;
  
  stack.push(start);
 
  while(!stack.empty()) {
    int current  = stack.top();
    stack.pop();
    steps++;
    
    if(current == end) {
      path.emplace_back(current);
      break;
    } else {
      if(!visited[current]) { 
	visited[current] = true;
	path.emplace_back(current);
      }

      for(auto i = g.nodes[current].begin(); i != g.nodes[current].end(); ++i) {
	if(!visited[*i]) {
	  stack.push(*i);
	}
      }
    }
  }
  print(path);
  return steps;
}

// Breadth-First Search
int BFS(Graph g, int start, int end) {
  vector<bool> visited(g.V, false);
  queue<int> queue;
  vector<int> path;
  int steps = 0;
  
  queue.push(start);
 
  while(!queue.empty()) {
    int current  = queue.front();
    queue.pop();
    steps++;
    
    if(current == end) {
      path.emplace_back(current);
      break;
    } else {
      if(!visited[current]) { 
	visited[current] = true;
	path.emplace_back(current);
      }

      for(auto i = g.nodes[current].begin(); i != g.nodes[current].end(); ++i) {
	if(!visited[*i]) {
	  queue.push(*i);
	}
      }
    }
  }
  print(path);
  return steps;
}


int main() {
  int steps;
  int maxNodes = 0;
  int startNode = -1, endNode = -1;
  
  // Search nicknames for start (jacob) and end goal (karl)
  fstream nickname_file(nicknamesTextPath);
  if(nickname_file.fail()) {
    cerr << "file not found: " << nicknamesTextPath << endl;
    return 1;
  }
  while(true) {
    int id;
    string nickname;
    nickname_file >> id >> nickname;
    if(nickname_file.eof()) {
      break;
    }
    if(nickname.empty()) {
      cerr << "unexpected error in the file" << endl;
      return 1;
    }
    if(nickname == "jacob") {
      startNode = id;
    } else if(nickname == "karl") {
      endNode = id;
    }
    maxNodes++;
  }
  if(startNode == -1 || endNode == -1) {
    cerr << "nickname not found" << endl;
    return 1;
  }
  cout << "From jacob (" << startNode << ") to karl (" << endNode << ")" << endl;

  // Construct a graph
  Graph sns(maxNodes);
  fstream links_file(linksTextPath);
  if(links_file.fail()) {
    cerr << "file not found: " << linksTextPath << endl;
    return 1;
  }
  while(!links_file.eof()) {
    int from, to;
    links_file >> from >> to;
    sns.addEdge(from, to);
  }

  // Search path by DFS
  {
    Timer t("DFS");
    steps = DFS(sns, startNode, endNode);
    cout << endl << steps << " steps" << endl;
  }

  // Search path by BFS
  {
    Timer t("BFS");
    steps  = BFS(sns, startNode, endNode);
    cout << endl << steps << " steps" << endl;
  }
  
  return 0;
}      
