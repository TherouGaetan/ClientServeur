#include "Packet.hpp"

namespace Network {
	namespace TSocket {
		Packet::Packet() {
			m_data = "";
		}

		Packet::~Packet() {
		}

		Packet Packet::operator<<(const std::string &msg) {
			m_data += msg;
			return *this;
		}

		Packet Packet::append(const std::string &append) {
			m_data += append;
			return *this;
		}

		const std::string &Packet::getBuff() const {
			return m_data;
		}

		void Packet::str(const std::string &msg) {
			m_data += msg;
		}

		const std::string &Packet::str() {
			return m_data;
		}

		void Packet::erase() {
			this->m_data.erase();
		}

		void Packet::clear() {
			m_data = "";
		}
	}
}