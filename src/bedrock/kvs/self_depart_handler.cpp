#include <fstream>
#include <string>

#include "common.hpp"
#include "hash_ring.hpp"
#include "kvs/kvs_handlers.hpp"
#include "kvs/rc_pair_lattice.hpp"
#include "spdlog/spdlog.h"
#include "zmq/socket_cache.hpp"

void self_depart_handler(
    unsigned thread_num, unsigned thread_id, unsigned& seed, std::string ip,
    std::shared_ptr<spdlog::logger> logger, zmq::socket_t* self_depart_puller,
    std::unordered_map<unsigned, GlobalHashRing>& global_hash_ring_map,
    std::unordered_map<unsigned, LocalHashRing>& local_hash_ring_map,
    std::unordered_map<std::string, KeyStat>& key_stat_map,
    std::unordered_map<std::string, KeyInfo>& placement,
    std::vector<std::string> routing_address,
    std::vector<std::string> monitoring_address, ServerThread wt,
    SocketCache& pushers, Serializer* serializer) {
  std::string ack_addr = zmq_util::recv_string(self_depart_puller);
  logger->info("Node is departing.");
  remove_from_hash_ring<GlobalHashRing>(global_hash_ring_map[kSelfTierId], ip,
                                        0);

  // thread 0 notifies other nodes in the cluster (of all types) that it is
  // leaving the cluster
  if (thread_id == 0) {
    std::string msg = std::to_string(kSelfTierId) + ":" + ip;

    for (auto it = global_hash_ring_map.begin();
         it != global_hash_ring_map.end(); it++) {
      auto hash_ring = &(it->second);
      std::unordered_set<std::string> observed_ip;

      for (auto iter = hash_ring->begin(); iter != hash_ring->end(); iter++) {
        if (observed_ip.find(iter->second.get_ip()) == observed_ip.end()) {
          zmq_util::send_string(
              msg, &pushers[(iter->second).get_node_depart_connect_addr()]);
          observed_ip.insert(iter->second.get_ip());
        }
      }
    }

    msg = "depart:" + std::to_string(kSelfTierId) + ":" + ip;

    // notify all routing nodes
    for (auto it = routing_address.begin(); it != routing_address.end(); it++) {
      zmq_util::send_string(
          msg, &pushers[RoutingThread(*it, 0).get_notify_connect_addr()]);
    }

    // notify monitoring nodes
    for (auto it = monitoring_address.begin(); it != monitoring_address.end();
         it++) {
      zmq_util::send_string(
          msg, &pushers[MonitoringThread(*it).get_notify_connect_addr()]);
    }

    // tell all worker threads about the self departure
    for (unsigned tid = 1; tid < thread_num; tid++) {
      zmq_util::send_string(
          ack_addr,
          &pushers[ServerThread(ip, tid).get_self_depart_connect_addr()]);
    }
  }

  AddressKeysetMap addr_keyset_map;
  bool succeed;

  for (auto it = key_stat_map.begin(); it != key_stat_map.end(); it++) {
    std::string key = it->first;
    auto threads = get_responsible_threads(
        wt.get_replication_factor_connect_addr(), key, is_metadata(key),
        global_hash_ring_map, local_hash_ring_map, placement, pushers, kAllTierIds,
        succeed, seed);

    if (succeed) {
      // since we already removed itself from the hash ring, no need to exclude
      // itself from threads
      for (auto iter = threads.begin(); iter != threads.end(); iter++) {
        addr_keyset_map[iter->get_gossip_connect_addr()].insert(key);
      }
    } else {
      logger->info(
          "Error: key missing replication factor in node depart routine");
    }
  }

  send_gossip(addr_keyset_map, pushers, serializer);
  zmq_util::send_string(ip + "_" + std::to_string(kSelfTierId),
                        &pushers[ack_addr]);
}
