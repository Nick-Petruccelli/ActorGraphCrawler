#include "../inc/graph_crawler.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
  GraphCrawler gc;
  std::vector<std::string> path = gc.bfs("Tom Hanks", "Don Most", 3);
  for (std::string node : path) {
    std::cout << node << std::endl;
  }

  return 0;
};
