#pragma once
#include <memory>
#include <map>
#include <string>
#include <stdexcept>

template<typename Resource, typename Identifier>
class ResourceHolder {
private:
	std::map<Identifier, std::unique_ptr<Resource>> m_ResourceMap;

public:

	void add(Identifier id, const std::string& file) {
		std::unique_ptr<Resource> res(new Resource());
		if (!res->loadFromFile(file)) {
			throw std::exception("ResourceHolder::add, could not add resource");
		}
		m_ResourceMap.insert(std::make_pair(id, std::move(res)));
	}

	Resource& get(Identifier id) {
		auto found = m_ResourceMap.find(id);
		if (found == m_ResourceMap.end()) {
			throw std::exception("ResourceHolder::get, could not find resource");
		}
		return *found->second;
	}
};