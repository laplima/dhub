#include <print>
#include <format>
#include <iostream>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <spdlog/spdlog.h>
#include <span>
#include <thread>
#include "HubI.h"

int main(int argc, char* argv[])
{
	using namespace std;
	span args(argv, argc);

	string name = "hub";
	if (args.size() > 1)
		name = args[1];

	try {
		colibry::ORBManager om{argc, argv};
		om.activate_rootpoa();

		dhub::Hub_i hi{name};
		dhub::Hub_var hub = om.activate_object<dhub::Hub>(hi);

		colibry::NameServer ns{om};
		ns.rebind(name, hub.in());
		println("\"{}\" registered in the NS", name);

		auto orbrun = [&om]() { om.run(); };
		std::thread ot{orbrun};

		println("Press ENTER to shutdown the server");
		cin.get();
		om.shutdown();
		ot.join();

	} catch(const CORBA::Exception& e) {
		ostringstream ss;
		ss << e;
		spdlog::error("ERROR: {}", ss.str());
	}
}
