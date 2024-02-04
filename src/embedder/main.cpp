#define MR_DEBUG
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
	if (s.find("#include") == string::npos) return "";
	static std::regex pattern("<(.*?)>");
	std::smatch matches;
	if (std::regex_search(s, matches, pattern)) {
		return matches[1];
	}
	else {
		return "";
	}
}
string get_define(const string& s) {
	if (s.find("#define") == string::npos) return "";
	static std::regex pattern("#define\\s+(.+)");
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
	else if (argc > 4) {
		cout << "error: too many arguments." << endl;
		return 0;
	}
	fs::path src_path = argv[1], mirai_path = argv[2];
	fs::path dst_path = "output.cpp";
	if (argc == 4) dst_path = argv[3];
	ifstream fin(src_path);
	if (!fin.is_open()) {
		cout << "error: cannot open source file." << endl;
		return 0;
	}
	queue<ll> q;
	unordered_set<string> flag_defines;
	auto scan_file = [&](ll u) {
		while (!fin.eof()) {
			string line;
			getline(fin, line);
			bool in_embed_trunk = false, embed_flag = false;
			while (!line.empty() && (line[0] == '\t' || line[0] == ' ')) line = line.substr(1);
			if (line.starts_with("// EMBED_IFDEF")) {
				debug("enter embed precompile trunk");
				in_embed_trunk = true;
				// 获取后面的内容
				string flag = line.substr(15);
				debug("flag is:", flag);
				if (flag_defines.contains(flag)) {
					embed_flag = true;
					debug("embed flag is true");
				}
				continue;
			}
			else if (line.starts_with("// EMBED_ENDIF")) {
				debug("leave embed precompile trunk");
				in_embed_trunk = false;
				embed_flag = false;
				continue;
			}
			if (in_embed_trunk && embed_flag) continue;
			string header = get_header(line);
			if (header.starts_with("mirai")) {
				debug("found header:", header);
				if (register_header(header)) q.push(header_table[header]);
				G.insert(header_table[header], u);
				continue;
			}
			string define = get_define(line);
			if (define.starts_with("MR_FLAG")) {
				debug("found flag define:", define);
				flag_defines.insert(define);
				continue;
			}
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
	ofstream fout(dst_path);
	for (auto i : flag_defines) fout << "#define " << i << endl;
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
		bool in_embed_trunk = false, embed_flag = false;
		while (!fin.eof()) {
			string line;
			getline(fin, line);
			string tmp = line;
			while (!tmp.empty() && (tmp[0] == '\t' || tmp[0] == ' ')) tmp = tmp.substr(1);
			if (tmp.starts_with("// EMBED_IFDEF")) {
				debug("enter embed precompile trunk");
				in_embed_trunk = true;
				// 获取后面的内容
				string flag = tmp.substr(15);
				debug("flag is:", flag);
				if (flag_defines.contains(flag)) {
					embed_flag = true;
					debug("embed flag is true");
				}
				continue;
			}
			else if (tmp.starts_with("// EMBED_ENDIF")) {
				debug("leave embed precompile trunk");
				in_embed_trunk = false;
				embed_flag = false;
				continue;
			}
			if (tmp.starts_with("#pragma once") || tmp.starts_with("//") || tmp.empty()) continue;
			string header = get_header(line);
			if (!header.starts_with("fmt") && header.substr(0, 5) != "mirai") {
				if (in_embed_trunk && embed_flag) continue;
				fout << line << endl;
			}
		}
		fin.close();
	}
	system(format("start notepad3 {}", dst_path.string()).c_str());
	return 0;
}