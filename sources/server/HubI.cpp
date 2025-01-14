#include "HubI.h"
#include <fmt/format.h>

using namespace std;
using namespace dhub;

char* Hub_i::id()
{
	return CORBA::string_dup(myid.c_str());
}

void Hub_i::add(const char * nid, ::dhub::Hub_ptr ref)
{
	fmt::print("[{}] add({})\n", myid, nid);
	nmap[nid] = dhub::Hub::_duplicate(ref);
}

void Hub_i::shutdown ()
{
	// todo
}

