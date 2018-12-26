/*
 * Copyright (c) 2018 Zilliqa
 * This source code is being disclosed to you solely for the purpose of your
 * participation in testing Zilliqa. You may view, compile and run the code for
 * that purpose and pursuant to the protocols and algorithms that are programmed
 * into, and intended by, the code. You may not do anything else with the code
 * without express permission from Zilliqa Research Pte. Ltd., including
 * modifying or publishing the code (or any part of it), and developing or
 * forming another public or private blockchain network. This source code is
 * provided 'as is' and no warranties are given as to title or non-infringement,
 * merchantability or fitness for purpose and, to the extent permitted by law,
 * all liability for your use of the code is disclaimed. Some programs in this
 * code are governed by the GNU General Public License v3.0 (available at
 * https://www.gnu.org/licenses/gpl-3.0.en.html) ('GPLv3'). The programs that
 * are governed by GPLv3.0 are those programs that are located in the folders
 * src/depends and tests/depends and which include a reference to GPLv3 in their
 * program files.
 */

#include <array>
#include <string>
#include <thread>
#include <vector>

#include "libPersistence/BlockStorage.h"
#include "libPersistence/DB.h"
#include "libTestUtils/TestUtils.h"

#define BOOST_TEST_MODULE persistencetest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace std;

void PrintShard(const DequeOfShard& shards) {
  for (const auto& shard : shards) {
    LOG_GENERAL(INFO, "Shard:")
    for (const auto& node : shard) {
      LOG_GENERAL(INFO, "  Node: " << get<SHARD_NODE_PEER>(node) << " "
                                   << get<SHARD_NODE_PUBKEY>(node));
    }
  }
}

BOOST_AUTO_TEST_SUITE(persistencetest)

BOOST_AUTO_TEST_CASE(testShardStructure) {
  INIT_STDOUT_LOGGER();

  // Put key (blockNum) = 1->5->3, then check that latest entry is always what
  // we retrieve (i.e., not sorted by key)

  DequeOfShard shards = TestUtils::GenerateDequeueOfShard(3);
  DequeOfShard shardsDeserialized;

  BOOST_CHECK(BlockStorage::GetBlockStorage().PutShardStructure(
      1, shards, TestUtils::DistUint32()));
  BOOST_CHECK(
      BlockStorage::GetBlockStorage().GetShardStructure(shardsDeserialized));
  BOOST_CHECK(shards == shardsDeserialized);

  PrintShard(shardsDeserialized);

  shards = TestUtils::GenerateDequeueOfShard(4);

  BOOST_CHECK(BlockStorage::GetBlockStorage().PutShardStructure(
      5, shards, TestUtils::DistUint32()));
  BOOST_CHECK(
      BlockStorage::GetBlockStorage().GetShardStructure(shardsDeserialized));
  BOOST_CHECK(shards == shardsDeserialized);

  PrintShard(shardsDeserialized);

  shards = TestUtils::GenerateDequeueOfShard(2);

  BOOST_CHECK(BlockStorage::GetBlockStorage().PutShardStructure(
      3, shards, TestUtils::DistUint32()));
  BOOST_CHECK(
      BlockStorage::GetBlockStorage().GetShardStructure(shardsDeserialized));
  BOOST_CHECK(shards == shardsDeserialized);

  PrintShard(shardsDeserialized);
}

BOOST_AUTO_TEST_SUITE_END()