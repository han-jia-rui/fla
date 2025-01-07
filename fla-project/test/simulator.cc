#include <catch2/catch_test_macros.hpp>

#include <fla/pda.h>
#include <fla/simulator.h>

namespace fla {

class SimulatorTest {
  public:
    static void test_verbose() {
        PDASimulator pda{};
        pda.set_verbose(true);
        REQUIRE(pda._verbose == true);
    }
};

} // namespace fla

TEST_CASE("simulator test", "[simulator]") { fla::SimulatorTest::test_verbose(); }
