#pragma once

#include <string>

namespace Network {
	namespace TSocket {
		class Packet {
		private:
			std::string m_data;

		public:
			Packet();

			~Packet();

		public:
			Packet operator<<(const std::string &msg);

		public:
			Packet append(const std::string &append);

			const std::string &getBuff() const;

			void str(const std::string &msg);

			const std::string &str();

			void erase();

			void clear();
		};
	}
}
