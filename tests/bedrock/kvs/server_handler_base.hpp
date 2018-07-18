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


MockZmqUtil mock_zmq_util;
ZmqUtilInterface* kZmqUtil = &mock_zmq_util;

MockHashRingUtil mock_hash_ring_util;
HashRingUtilInterface* kHashRingUtil = &mock_hash_ring_util;

unsigned kDefaultLocalReplication = 1;
unsigned kSelfTierId = 1;
unsigned kThreadNum = 1;
std::vector<unsigned> kSelfTierIdVector = {kSelfTierId};

std::shared_ptr<spdlog::logger> logger =
    spdlog::basic_logger_mt("mock_logger", "mock_log.txt", true);

class ServerHandlerTest : public ::testing::Test {
 protected:
  Address ip = "127.0.0.1";
  unsigned thread_id = 0;
  std::unordered_map<unsigned, GlobalHashRing> global_hash_ring_map;
  std::unordered_map<unsigned, LocalHashRing> local_hash_ring_map;
  std::unordered_map<Key, KeyInfo> placement;
  std::unordered_map<Key, unsigned> key_size_map;
  ServerThread wt;
  zmq::context_t context;
  SocketCache pushers = SocketCache(&context, ZMQ_PUSH);
  Serializer* serializer;
  MemoryKVS* kvs;

  ServerHandlerTest() {
    kvs = new MemoryKVS();
    serializer = new MemorySerializer(kvs);
    wt = ServerThread(ip, thread_id);
    global_hash_ring_map[1].insert(ip, thread_id);
  }

  virtual ~ServerHandlerTest() {
    delete kvs;
    delete serializer;
  }

 public:
  void SetUp() {
    // reset all global variables
    kDefaultLocalReplication = 1;
    kSelfTierId = 1;
    kThreadNum = 1;
    kSelfTierIdVector = {kSelfTierId};
  }

  void TearDown() {
    // clear all the logged messages after each test
    mock_zmq_util.sent_messages.clear();
  }

  std::vector<std::string> get_zmq_messages() {
    return mock_zmq_util.sent_messages;
  }
};
