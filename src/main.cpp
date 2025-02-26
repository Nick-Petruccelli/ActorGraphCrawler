#include "../inc/graph_crawler.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string src = "Tom Hanks";
  std::string dst = "Don Most";
  unsigned int depth = 4;

  if(argc == 3){
    src = argv[1];
    dst = "";
    depth = atoi(argv[2]);
  }else if(argc == 4){
    src = argv[1];
    dst = argv[2];
    depth = atoi(argv[3]);
  }else if(argc > 4){
    std::cout << "Invalid input params, try agian" << std::endl;
    return 1;
  }

  GraphCrawler gc;
  std::vector<std::string> path = gc.bfs(src, dst, depth);

  for (std::string node : path) {
    std::cout << node << std::endl;
  }

  return 0;
};
