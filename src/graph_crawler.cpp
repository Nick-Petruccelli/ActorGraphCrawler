#include "../inc/graph_crawler.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <cstring>

typedef struct reponse_chunk {
	char* mem = (char*)malloc(sizeof(char));
	size_t size = 1;
}ResponseChunk;

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *write_chunk){
	ResponseChunk *chunk = (ResponseChunk*)write_chunk;
	size_t real_size = size * nmemb;
	chunk->size = real_size;
	char * ptr = (char*)realloc(chunk->mem, real_size+1);
	assert(ptr != NULL);
	chunk->mem = ptr;
	memcpy(chunk->mem, contents, real_size);
	return real_size;
}

GraphCrawler::GraphCrawler(){
	curl_global_init(CURL_GLOBAL_ALL);
}

const char* GraphCrawler::get_json(std::string node){
	std::cout << "getting json" << std::endl;
	CURL* curl = curl_easy_init();
	assert(curl != NULL);
	CURLcode res;
	std::string url = m_server_addr+node;
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

std::vector<std::string> GraphCrawler::bfs(std::string src, std::string dst){
	std::vector<std::string> out;

	get_json(src);

	return out;	
};
