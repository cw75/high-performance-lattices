#include "monitor/monitoring_utils.hpp"

void add_node(std::shared_ptr<spdlog::logger> logger, std::string tier,
              unsigned number, unsigned& adding,
              const Address& management_address) {
  logger->info("Adding {} {} node.", std::to_string(number), tier);
  std::string shell_command = "curl -X POST http://" + management_address +
                              "/add/" + tier + "/" + std::to_string(number) +
                              " &";
  system(shell_command.c_str());
  adding = number;
}

void remove_node(std::shared_ptr<spdlog::logger> logger, ServerThread& node,
                 std::string tier, bool& removing_flag, SocketCache& pushers,
                 std::unordered_map<Address, unsigned>& departing_node_map,
                 MonitoringThread& mt,
                 std::unordered_map<unsigned, TierData>& tier_data_map) {
  auto connection_addr = node.get_self_depart_connect_addr();
  departing_node_map[node.get_ip()] = tier_data_map[1].thread_number_;
  auto ack_addr = mt.get_depart_done_connect_addr();
  logger->info("Removing {} node {}.", tier, node.get_ip());
  zmq_util::send_string(ack_addr, &pushers[connection_addr]);
  removing_flag = true;
}