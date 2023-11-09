#include <algorithm>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using GraphType = std::vector<std::unordered_set<size_t>>;

enum class SortOrder { None, ASC, DESC, DESC_SHUFFLE, ASC_SHUFFLE, SHUFFLE };

class CliqueProblem {
private:
  GraphType graph;
  GraphType proceededGraph;
  std::vector<size_t> indexes;
  std::vector<size_t> colors;
  size_t maxColor = 0;
  double timeSpent = 0;
  std::string problemFile;

  template <typename T>
  std::vector<size_t> findItems(std::vector<T> const &v, T target) {
    std::vector<size_t> indices;
    auto it = v.begin();
    while ((it = std::find_if(it, v.end(), [&](T const &e) {
              return e == target;
            })) != v.end()) {
      indices.push_back(static_cast<size_t>(std::distance(v.begin(), it)));
      it++;
    }
    return indices;
  }

public:
  void readFile(std::string filename) {
    std::ifstream file;
    file.open(filename);

    problemFile = filename;

    std::string line;

    while (!file.eof() && getline(file, line)) {
      if (line[0] == 'c')
        continue;

      std::stringstream line_input(line);
      if (line[0] == 'p') {
        std::string command, problemTitle;
        size_t numberOfNodes, numberOfEdges;
        line_input >> command >> problemTitle >> numberOfNodes >> numberOfEdges;
        graph.resize(numberOfNodes);
        indexes.resize(numberOfNodes);
      }

      if (line[0] == 'e') {
        std::string command;
        size_t node1, node2;
        line_input >> command >> node1 >> node2;
        graph[node1 - 1].insert(node2);
        graph[node2 - 1].insert(node1);
      }
    }

    std::iota(indexes.begin(), indexes.end(), 0);
  }

  // TODO remove
  void testInit() {
    graph.resize(4);
    graph[0].insert({1, 2, 3});

    for (size_t i = 1; i < graph.size(); ++i) {
      graph[i].insert({0});
    }

    indexes.resize(graph.size());

    std::iota(indexes.begin(), indexes.end(), 0);
  }

  void printSortedGraph() {
    std::cout << "Sorted Graph\n";

    for (size_t i = 0; i < indexes.size(); ++i) {
      std::cout << indexes[i] + 1 << ": ";

      for (auto node : graph[indexes[i]]) {
        std::cout << node << " ";
      }
      std::cout << "\n";
    }
  }

  void printGraph(GraphType graphToPrint) {
    for (size_t i = 0; i < graphToPrint.size(); ++i) {
      for (auto j : graphToPrint[i]) {
        std::cout << j << " ";
      }
      std::cout << "\n";
    }
  }

  void printGraph() {
    for (size_t i = 0; i < graph.size(); ++i) {
      for (auto j : graph[i]) {
        std::cout << j << " ";
      }
      std::cout << "\n";
    }
  }

  void solveProblem() {

    std::clock_t start = std::clock();
    proceededGraph = graph;

    size_t numberOfFalsyConnectedVariables = 0;

    while (true) {
      std::vector<size_t> indexesToSwap = {};

      for (size_t i = 0; i < proceededGraph.size(); ++i) {
        if (proceededGraph[i].size() == 1) {

          for (auto j : proceededGraph[i]) {
            proceededGraph[j].erase(proceededGraph[j].find(i));
          }
          proceededGraph[i].erase(0);

          indexesToSwap.push_back(i);
        }
      }

      for (auto i : indexesToSwap) {
        numberOfFalsyConnectedVariables++;
        std::swap(proceededGraph[i],
                  proceededGraph[proceededGraph.size() -
                                 numberOfFalsyConnectedVariables]);
      }

      if (!indexesToSwap.size())
        break;
    }

    // TODO переписать тоже самое, но для индексов для сохранения
    std::sort(proceededGraph.begin(),
              std::prev(proceededGraph.end(),
                        static_cast<long int>(numberOfFalsyConnectedVariables)),
              [&](std::unordered_set<long unsigned int> left,
                  std::unordered_set<long unsigned int> right) {
                return left.size() > right.size();
              });

    std::cout << "\nnot sorted graph\n";
    for (size_t i = 0; i < graph.size(); ++i) {
      std::cout << i << ": ";
      for (auto j : graph[i]) {
        std::cout << j << " ";
      }
      std::cout << "\n";
    }

    std::cout << "\nsorted graph\n";
    for (size_t i = 0; i < proceededGraph.size(); ++i) {
      std::cout << i << ": ";
      for (auto j : proceededGraph[i]) {
        std::cout << j << " ";
      }
      std::cout << "\n";
    }

    // for (auto i : indexes) {
    //   if (colors[i])
    //     continue;

    //   ++maxColor;

    //   std::unordered_set<size_t> banned_nodes(graph[i]);
    //   colors[i] = maxColor;
    //   for (size_t node = 0; node < graph.size(); ++node) {
    //     if (colors[node] || banned_nodes.count(node + 1)) {
    //       continue;
    //     }
    //     colors[node] = maxColor;
    //     banned_nodes.insert(graph[node].begin(), graph[node].end());
    //   }
    // }

    timeSpent =
        static_cast<double>(std::clock() - start) / (double)CLOCKS_PER_SEC;
  }

  inline size_t getMaxColor() { return maxColor; }
  inline double getTime() { return timeSpent; }

  void resetResults() {
    maxColor = 0;
    std::fill(colors.begin(), colors.end(), 0);
    std::iota(indexes.begin(), indexes.end(), 0);
    timeSpent = 0;
  }

  void saveResults(std::string filename) {
    std::ofstream output(filename, std::ios_base::app);
    output << problemFile << ";" << maxColor << ";" << timeSpent << ";[";

    for (size_t i = 1; i <= maxColor; ++i) {
      std::vector<size_t> indices = findItems(colors, i);
      output << "[";
      for (auto &e : indices) {
        output << e + 1 << ',';
      }

      output << ((i != maxColor) ? "]," : "]\n");
    }
    output.close();
  }
};

int main() {
  std::string baseTestPath = "data/";
  std::vector<std::string> files = {"test.clq"};
  // std::vector<std::string> files = {
  //     "brock200_1.clq",     "brock200_2.clq",     "brock200_3.clq",
  //     "brock200_4.clq",     "brock400_1.clq",     "brock400_2.clq",
  //     "brock400_3.clq",     "brock400_4.clq",     "C125.9.clq",
  //     "gen200_p0.9_44.clq", "gen200_p0.9_55.clq", "hamming8-4.clq",
  //     "johnson16-2-4.clq",  "johnson8-2-4.clq",   "keller4.clq",
  //     "MANN_a27.clq",       "MANN_a9.clq",        "p_hat1000-1.clq",
  //     "p_hat1000-2.clq",    "p_hat1500-1.clq",    "p_hat300-3.clq",
  //     "p_hat500-3.clq",     "san1000.clq",        "sanr200_0.9.clq",
  //     "sanr400_0.7.clq"};

  // std::string baseResPath = "results/";
  // std::string resultFile = "res.csv";

  // std::ofstream output(baseResPath + resultFile);

  // output << "Problem name;Number of colors;Time\n";

  // TODO проверить правильность сортировки на кастомном графе
  for (auto filename : files) {
    CliqueProblem cliqueProblem;
    cliqueProblem.testInit();

    cliqueProblem.printGraph();

    cliqueProblem.solveProblem();
  }
  return 0;
}
