#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"
#include "../config-parser.hpp"

SCENARIO("config file is present","[fileparser]"){
    GIVEN("config file has valid syntax"){
        WHEN("rule is present"){
            THEN("a valid ruleset should be built and can be retrieved"){
                using namespace mods::acl;
                FileParser parser;
                parser.setFile("/etc/foo/bar/non/existent/file");
                int ret = parser.parse();
                REQUIRE(ret == FileParser::FILE_CANNOT_OPEN);
                std::cout << ret << "\n";
            }
            THEN("modifying rules should work"){
                using namespace mods::acl;
                FileParser parser;
                parser.setFile("foobar.conf");
                std::cout << "Read " << parser.read() << " bytes\n";
                int ret = parser.parse();
                std::cout << ret << "\n";
            }
        }
    }
    GIVEN("constructor is supplied a file name"){
        WHEN("file can be opened"){
            THEN("This should succeed but that doesnt guarantee a valid syntax"){

            }
        }
        WHEN("File cannot be opened"){
            THEN("client code should not be able to parse the file"){

            }
        }
    }
};
