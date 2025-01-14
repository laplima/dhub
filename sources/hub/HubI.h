#ifndef HUBI_NDANZO_H_
#define HUBI_NDANZO_H_

#include "HubS.h"
#include <map>
#include <string>
#include <vector>

namespace dhub {

	struct ServInfo {
		Hub_var ref = Hub::_nil();
		std::vector<std::string> already_sent;
	};

	class Hub_i : public virtual POA_dhub::Hub {
	public:
		Hub_i(std::string theid) : myid{std::move(theid)} {}
		~Hub_i () override = default;
		char* id() override;
		void add(const char * nid, ::dhub::Hub_ptr ref) override;
		void shutdown() override;
	private:
		std::string myid;
		std::map<std::string, ServInfo> smap;	// id -> servinfo
	};

}

#endif
