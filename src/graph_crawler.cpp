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
  CURL *curl = curl_easy_init();
  assert(curl != NULL);
  CURLcode res;
  std::string url = m_server_addr;
  const char *esc_node_name =
      curl_easy_escape(curl, node.c_str(), node.length());
  url.append(esc_node_name);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  ResponseChunk res_chunk;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res_chunk);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  res = curl_easy_perform(curl);
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
  std::queue<Node> next_visit_queue;
  std::map<std::string, std::string> visited;
  Node first_entry;
  first_entry.depth = 0;
  first_entry.from = "";
  first_entry.name = src;
  next_visit_queue.push(first_entry);
  while (next_visit_queue.size() != 0) {
    Node node = next_visit_queue.front();
    std::string prev_node = node.from;
    next_visit_queue.pop();
    visited[node.name] = node.from;
    if (node.name == dst)
      return retrace_path(node.name, visited);
    if(node.depth >= max_depth)
      continue;
    std::vector<std::string> neighbors = get_neibors(node.name);
    for (std::string neighbor : neighbors) {
      if (visited.count(neighbor) == 0){
        Node n;
        n.name = neighbor;
        n.from = node.name;
        n.depth = node.depth + 1;
        next_visit_queue.push(n);
      }
    }
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
