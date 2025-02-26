#include "../inc/graph_crawler.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
  GraphCrawler gc;
  std::vector<std::string> path = gc.bfs("Tom Hanks", "The Green Mile", 10);
  for (std::string node : path) {
    std::cout << node << std::endl;
  }

  return 0;
};
