#include "HubI.h"
#include <fmt/format.h>

using namespace std;
using namespace dhub;

// Hub_i::Hub_i ()
// {
// }

// Hub_i::~Hub_i ()
// {
// }

char* Hub_i::id()
{
	return CORBA::string_dup(myid.c_str());
}

void Hub_i::add(const char * nid, ::dhub::Hub_ptr ref)
{
	fmt::print("[{}] add({})\n", myid, nid);
	if (!smap.contains(nid))
		smap.emplace(nid, ServInfo{});
	smap[nid].ref = dhub::Hub::_duplicate(ref);

	vector<string> known_servers;
	for (const auto& [s,_] : smap)
		known_servers.push_back(s);

	// remove ex from all
	auto exclude = [](vector<string>& all, const vector<string>& ex) {
		for (const auto& s : ex) {
			auto p = find(begin(all), end(all), s);
			if (p != end(all))
				all.erase(p);
		}
	};

	// send
	for (auto it = smap.begin(); it != smap.end(); ++it) {
		auto missing = known_servers;
		// remove current key
		missing.erase(find(begin(missing), end(missing), it->first));	// will always find
		exclude(missing, it->second.already_sent);
		// send missing to current
		for (const auto& c : missing) {
			it->second.ref->add(c.c_str(), smap[c].ref.in());
			it->second.already_sent.push_back(c);
		}
	}
}

void Hub_i::shutdown ()
{
	// todo
}

