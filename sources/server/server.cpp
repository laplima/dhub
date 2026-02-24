#include <format>
#include <print>
#include <iostream>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <spdlog/spdlog.h>
#include <span>
#include <thread>
#include <sstream>
#include "HubC.h"
#include "HubI.h"

using spdlog::error;
using spdlog::info;

int main(int argc, char* argv[])
{
	using namespace std;
	span args(argv, argc);

	if (args.size() < 2) {
		error("USAGE: {} <id>\n", args[0]);
		return 1;
	}

	try {
		colibry::ORBManager om{argc, argv};
		om.activate_rootpoa();

		dhub::Hub_i si{args[1]};
		dhub::Hub_var server = om.activate_object<dhub::Hub>(si);

		colibry::NameServer ns{om};
		ns.rebind(args[1], server.in());
		std::print("\"{}\" registered in the NS\n", args[1]);

		auto orbrun = [&om]() { om.run(); };
		std::thread ot{orbrun};

		std::print("Announcing to the hub\n");
		dhub::Hub_var hub = ns.resolve<dhub::Hub>("hub");
		hub->add(args[1], server.in());

		std::print("Press ENTER to shutdown the server\n");
		cin.get();

		om.shutdown();
		ot.join();

	} catch(const CORBA::Exception& e) {
		ostringstream ss;
		ss << e;
		error("ERROR: {}\n", ss.str());
	}
}
