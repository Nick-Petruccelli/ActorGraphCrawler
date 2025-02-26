#include "../inc/graph_crawler.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
  std::string src = "Tom Hanks";
  std::string dst = "Don Most";
  unsigned int depth = 2;

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
  std::chrono::high_resolution_clock clock;
  std::chrono::time_point<std::chrono::high_resolution_clock> start = clock.now();
  std::vector<std::string> path = gc.bfs(src, dst, depth);
  std::chrono::time_point<std::chrono::high_resolution_clock> end = clock.now();

  std::chrono::duration ns = end - start;
  double time = ns.count() / 1E9;


  std::cout << "Src: " << src << " Dst: " << dst << " Depth: " << depth << std::endl;
  std::cout << "Execution Time: " << time << std::endl;

  return 0;
};
