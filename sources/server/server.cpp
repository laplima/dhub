#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <span>
#include <thread>
#include "HubC.h"
#include "HubI.h"

int main(int argc, char* argv[])
{
	using namespace std;
	span args(argv, argc);

	if (args.size() < 2) {
		fmt::print(stderr, "USAGE: {} <id>\n", args[0]);
		return 1;
	}

	try {
		colibry::ORBManager om{argc, argv};
		om.activate_rootpoa();

		dhub::Hub_i si{args[1]};
		dhub::Hub_var server = om.activate_object<dhub::Hub>(si);

		colibry::NameServer ns{om};
		ns.rebind(args[1], server.in());
		fmt::print("\"{}\" registered in the NS\n", args[1]);

		auto orbrun = [&om]() { om.run(); };
		std::thread ot{orbrun};

		fmt::print("Announcing to the hub\n");
		dhub::Hub_var hub = ns.resolve<dhub::Hub>("hub");
		hub->add(args[1], server.in());

		fmt::print("Press ENTER to shutdown the server\n");
		cin.get();

		om.shutdown();
		ot.join();

	} catch(const CORBA::Exception& e) {
		fmt::print(stderr, "ERROR: {}\n", fmt::streamed(e));
	}
}
