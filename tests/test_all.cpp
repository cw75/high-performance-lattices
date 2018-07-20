//  Copyright 2018 U.C. Berkeley RISE Lab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdlib.h>
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"
#include "spdlog/spdlog.h"

#include "misc.pb.h"
#include "replication.pb.h"
#include "requests.pb.h"
#include "types.hpp"
#include "utils/server_utils.hpp"

#include "bedrock/kvs/server_handler_base.hpp"
#include "bedrock/kvs/test_gossip_handler.hpp"
#include "bedrock/kvs/test_node_depart_handler.hpp"
#include "bedrock/kvs/test_node_join_handler.hpp"
#include "bedrock/kvs/test_rep_factor_change_handler.hpp"
#include "bedrock/kvs/test_rep_factor_response_handler.hpp"
#include "bedrock/kvs/test_self_depart_handler.hpp"
#include "bedrock/kvs/test_user_request_handler.hpp"

#include "bedrock/route/routing_handler_base.hpp"
#include "bedrock/route/test_address_handler.hpp"
#include "bedrock/route/test_membership_handler.hpp"
#include "bedrock/route/test_replication_change_handler.hpp"
#include "bedrock/route/test_replication_response_handler.hpp"
#include "bedrock/route/test_seed_handler.hpp"

#include "include/lattices/test_bool_lattice.hpp"
#include "include/lattices/test_map_lattice.hpp"
#include "include/lattices/test_max_lattice.hpp"
#include "include/lattices/test_set_lattice.hpp"

unsigned kDefaultLocalReplication = 1;
unsigned kSelfTierId = 1;
unsigned kThreadNum = 1;
std::vector<unsigned> kSelfTierIdVector = {kSelfTierId};
std::unordered_map<unsigned, TierData> kTierDataMap = {};

unsigned kEbsThreadCount = 1;
unsigned kMemoryThreadCount = 1;
unsigned kRoutingThreadCount = 1;

int main(int argc, char* argv[]) {
  logger->set_level(spdlog::level::info);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
