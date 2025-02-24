#pragma once
#include <string>
#include <vector>

class GraphCrawler{
public:
	GraphCrawler(void);
	std::vector<std::string> bfs(std::string src, std::string dst);
private:
	std::vector<std::string> get_neibors(std::string node);
	const char* get_json(std::string node);
private:
	std::string m_server_addr = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
};
