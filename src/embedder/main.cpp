#include <mirai/util/debug.hpp>
#include <mirai/util/time.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/ds/unordered_dense.hpp>
#include <regex>
#include <filesystem>
using namespace mirai;
namespace fs = std::filesystem;
vector<string> header_index(1);
unordered_map<string, ll> header_table;
ll header_count;
graph<void> G;
vector<ll> id, topo;
struct _config {
	CONFIG_ITEM topo_order = topo;
	CONFIG_ITEM in_degree = id;
} config;
string get_header(const string& s) {
	static std::regex pattern("<(.*?)>");
	std::smatch matches;
	if (std::regex_search(s, matches, pattern)) {
		return matches[1];
	}
	else {
		return "";
	}
}
bool register_header(const string& s) {
	if (!header_table.contains(s)) {
		header_table.insert({ s, ++header_count });
		header_index.push_back(s);
		return true;
	}
	return false;
}
int main(int argc, char** argv) {
	auto_timer _t;
	if (argc < 3) {
		cout << "error: not enough argument." << endl;
		return 0;
	}
	else if (argc > 3) {
		cout << "error: too many arguments." << endl;
		return 0;
	}
	fs::path src_path = argv[1], mirai_path = argv[2];
	ifstream fin(src_path);
	if (!fin.is_open()) {
		cout << "error: cannot open source file." << endl;
		return 0;
	}
	queue<ll> q;
	auto scan_file = [&](ll u) {
		while (!fin.eof()) {
			string line;
			getline(fin, line);
			string header = get_header(line);
			if (header == "" || header.substr(0, 5) != "mirai") continue;
			debug("found header:", header);
			if (register_header(header)) q.push(header_table[header]);
			G.insert(header_table[header], u);
		}
		fin.close();
	};
	scan_file(0);
	while (!q.empty()) {
		ll u = q.front();
		q.pop();
		fs::path header_path = mirai_path / header_index[u];
		fin.open(header_path);
		if (!fin.is_open()) {
			cout << "error: cannot open header file <" << header_index[u] << ">" << endl;
			return 0;
		}
		debug("scanning header", header_index[u]);
		scan_file(u);
	}
	id.resize(G.node_count() + 1);
	calc_graph_degree<G, config>();
	topo_sort<G, config>();
	debug("sorted header:");
	ofstream fout("output.cpp");
	for (auto i : topo | transform([](ll x) {
					  if (x == 0)
						  return string("<source file>");
					  else
						  return header_index[x];
				  })) {
		debug(i);
		if (i == "<source file>")
			fin.open(src_path);
		else
			fin.open(mirai_path / i);
		while (!fin.eof()) {
			string line;
			getline(fin, line);
			if(line.starts_with("#pragma once")||line.starts_with("//")||line.empty())continue;
			string header = get_header(line);
			if (header == "" || header.substr(0, 5) != "mirai") fout << line << endl;
		}
		fin.close();
	}
	system("code output.cpp");
	return 0;
}