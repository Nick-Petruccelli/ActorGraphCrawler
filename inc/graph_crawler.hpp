#pragma once
#include <map>
#include <string>
#include <vector>

class GraphCrawler {
public:
  GraphCrawler(void);
  std::vector<std::string> bfs(std::string src, std::string dst,
                               unsigned int depth);

private:
  std::vector<std::string> get_neibors(std::string node);
  const char *get_json(std::string node);
  std::vector<std::string>
  retrace_path(std::string dst, std::map<std::string, std::string> visited);

private:
  std::string m_server_addr =
      "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
};
