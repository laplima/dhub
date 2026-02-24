#include "HubI.h"
#include <algorithm>
#include <print>
#include <spdlog/spdlog.h>
#include <algorithm>

using namespace std;
using namespace dhub;
using spdlog::error;

char* Hub_i::id()
{
	return CORBA::string_dup(myid.c_str());
}

void Hub_i::add(const char * nid, ::dhub::Hub_ptr ref)
{
	print("[{}] add({})\n", myid, nid);

	// add nid to server map (or update reference)
	if (!smap.contains(nid))
		smap.emplace(nid, ServInfo{});
	smap[nid].ref = dhub::Hub::_duplicate(ref);

	// create vector of known servers
	vector<string> known_servers;
	for (const auto& [s,_] : smap)
		known_servers.push_back(s);

	// remove ex from all
	auto exclude = [](vector<string>& all, const vector<string>& ex) {
		for (const auto& s : ex) {
			auto p = std::ranges::find(all, s);
			if (p != end(all))
				all.erase(p);
		}
	};

	// send
	for (auto& [nnid, info] : smap) {
		auto missing = known_servers;
		// remove current key
		missing.erase(std::ranges::find(missing, nnid));	// will always find
		exclude(missing, info.already_sent);
		// send missing to current
		for (const auto& c : missing) {
			info.ref->add(c.c_str(), smap[c].ref.in());
			info.already_sent.push_back(c);
		}
	}
}

void Hub_i::shutdown ()
{
	// todo
}

