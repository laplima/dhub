#ifndef HUBI_NDANZO_H_
#define HUBI_NDANZO_H_

#include "HubS.h"
#include <string>
#include <map>

namespace dhub {

	class Hub_i : public virtual POA_dhub::Hub {
	public:
		Hub_i(std::string theid) : myid{std::move(theid)} {}
		~Hub_i () override = default;
		char* id() override;
		void add(const char * nid, ::dhub::Hub_ptr ref) override;
		void shutdown() override;
	private:
		std::string myid;
		std::map<std::string, Hub_var> nmap;	// id -> servinfo
	};

}

#endif
