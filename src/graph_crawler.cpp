#include "../inc/graph_crawler.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <map>
#include <queue>
#include <rapidjson/document.h>
#include <string>
#include <vector>

typedef struct reponse_chunk {
  char *mem = (char *)malloc(sizeof(char));
  size_t size = 1;
} ResponseChunk;

static size_t write_callback(char *contents, size_t size, size_t nmemb,
                             void *write_chunk) {
  ResponseChunk *chunk = (ResponseChunk *)write_chunk;
  size_t real_size = size * nmemb;
  chunk->size = real_size;
  char *ptr = (char *)realloc(chunk->mem, real_size + 1);
  assert(ptr != NULL);
  chunk->mem = ptr;
  memcpy(chunk->mem, contents, real_size);
  return real_size;
}

GraphCrawler::GraphCrawler() { curl_global_init(CURL_GLOBAL_ALL); }

const char *GraphCrawler::get_json(std::string node) {
  std::cout << "getting json" << std::endl;
  CURL *curl = curl_easy_init();
  assert(curl != NULL);
  CURLcode res;
  std::string url = m_server_addr + node;
  std::cout << url << std::endl;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  ResponseChunk res_chunk;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_chunk);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  res = curl_easy_perform(curl);
  std::cout << "result: " << std::endl;
  std::cout << res_chunk.mem << std::endl;
  return res_chunk.mem;
}

std::vector<std::string> GraphCrawler::get_neibors(std::string node) {
  const char *json = get_json(node);
  rapidjson::Document doc;
  doc.Parse(json);
  assert(doc.IsObject());
  assert(doc.HasMember("neighbors"));
  std::vector<std::string> out;
  for (auto &neighbor : doc["neighbors"].GetArray()) {
    out.push_back(neighbor.GetString());
  }
  return out;
}

std::vector<std::string> GraphCrawler::bfs(std::string src, std::string dst,
                                           unsigned int max_depth) {
  std::queue<std::string> next_visit_queue;
  std::map<std::string, std::string> visited;
  next_visit_queue.push(src);
  unsigned int depth = 0;
  std::string next_node = "";
  while (next_visit_queue.size() != 0 && depth <= max_depth) {
    std::string prev_node = next_node;
    next_node = next_visit_queue.front();
    next_visit_queue.pop();
    visited[prev_node] = next_node;
    if (next_node == dst)
      return retrace_path(next_node, visited);
    std::vector<std::string> neighbors = get_neibors(next_node);
    for (std::string neighbor : neighbors) {
      if (visited.count(neighbor) == 0)
        next_visit_queue.push(neighbor);
    }
    depth += 1;
  }
  std::vector<std::string> out;
  return out;
};

std::vector<std::string>
GraphCrawler::retrace_path(std::string dst,
                           std::map<std::string, std::string> visited) {
  std::vector<std::string> out;
  std::string node = dst;
  while (node != "") {
    out.push_back(node);
    node = visited[node];
  }
  std::reverse(out.begin(), out.end());
  return out;
}
