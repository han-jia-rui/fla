#include <fla/pda.h>
#include <fla/simulator.h>
#include <fla/tm.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SymbolSeq test", "[SymbolSeq]") {
    REQUIRE(fla::SymbolSeq("_ab") == fla::SymbolSeq("_**"));
}
