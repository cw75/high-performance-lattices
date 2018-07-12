#ifndef SRC_INCLUDE_MONITOR_MONITORING_HANDLERS_HPP_
#define SRC_INCLUDE_MONITOR_MONITORING_HANDLERS_HPP_

#include "hash_ring.hpp"
#include "spdlog/spdlog.h"

void membership_handler(
    std::shared_ptr<spdlog::logger> logger, zmq::socket_t* notify_puller,
    std::unordered_map<unsigned, GlobalHashRing>& global_hash_ring_map,
    unsigned& adding_memory_node, unsigned& adding_ebs_node,
    std::chrono::time_point<std::chrono::system_clock>& grace_start,
    std::vector<Address>& routing_address, StorageStat& memory_tier_storage,
    StorageStat& ebs_tier_storage, OccupancyStat& memory_tier_occupancy,
    OccupancyStat& ebs_tier_occupancy,
    std::unordered_map<Key, std::unordered_map<Address, unsigned>>&
        key_access_frequency);

void depart_done_handler(
    std::shared_ptr<spdlog::logger> logger, zmq::socket_t* depart_done_puller,
    std::unordered_map<Address, unsigned>& departing_node_map,
    Address management_address, bool& removing_memory_node,
    bool& removing_ebs_node,
    std::chrono::time_point<std::chrono::system_clock>& grace_start);

void feedback_handler(
    zmq::socket_t* feedback_puller,
    std::shared_ptr<spdlog::logger> logger,
    std::unordered_map<std::string, double>& user_latency,
    std::unordered_map<std::string, double>& user_throughput,
    std::unordered_map<Key, std::pair<double, unsigned>>& latency_miss_ratio_map);

#endif // SRC_INCLUDE_MONITOR_MONITORING_HANDLERS_HPP_
