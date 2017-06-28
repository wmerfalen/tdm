#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"
#include "../config-parser.hpp"
#include "../../util-conf.hpp"

SCENARIO("config file is present","[fileparser]"){
    GIVEN("config file has valid syntax"){
        WHEN("rule is present"){
            THEN("a valid ruleset should be built and can be retrieved"){
                mods::util::conf::stfu = false;
                mods::util::conf::genstr("/home/wmerfalen/foobar.conf","lol.conf");
                using namespace mods::acl;
                FileParser parser;
                parser.setFile("/etc/foo/bar/non/existent/file");
                int ret = parser.parse();
                REQUIRE(ret == FileParser::FILE_CANNOT_OPEN);
            }
            THEN("modifying rules should work"){
                using namespace mods::acl;
                FileParser parser;
                parser.setFile("foobar.conf");
                int ret = parser.parse();
                parser.dump_tree();
                REQUIRE(ret > 0);
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
