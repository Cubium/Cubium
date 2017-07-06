#include <cstdint>
#include <map>
#include <vector>

#include <routing_table.hpp>

/**
 *  Sub class with added testing functionality
 */
class _RoutingTable : public RoutingTable
{
public:
  std::map<LogicalAddress, uint32_t, LogicalAddressCompare> getTable() { return routingTable; }
};

TEST(RoutingTable, insert)
{
  _RoutingTable rt;
  LogicalAddress l1(1, 1);
  LogicalAddress l2(2, 2);
  LogicalAddress l3(1, 2);
  LogicalAddress l4(0, 0);

  rt.insert(l1, 1);
  rt.insert(l2, 2);
  rt.insert(l3, 3);

  std::map<LogicalAddress, uint32_t, LogicalAddressCompare> table = rt.getTable();

  EXPECT_EQ(table[l1], 1);
  EXPECT_EQ(table[l2], 2);
  EXPECT_EQ(table[l3], 3);

  // Should return iterator to end if key does not exist in map
  EXPECT_TRUE(table.find(l4) == table.end());
}

TEST(RoutingTable, exists)
{
  _RoutingTable rt;
  LogicalAddress l1(1, 1);
  LogicalAddress l2(2, 1);
  rt.insert(l1, 1);

  EXPECT_TRUE(rt.exists(l1));
  EXPECT_FALSE(rt.exists(l2));
}

TEST(RoutingTable, getPhysicalAddress)
{
  _RoutingTable rt;
  LogicalAddress l1(1, 1);
  LogicalAddress l2(1, 2);
  rt.insert(l1, 9);

  EXPECT_EQ(rt.getPhysicalAddress(l1), 9);
  EXPECT_EQ(rt.getPhysicalAddress(l2), -1);
}
