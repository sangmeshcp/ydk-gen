/*  ----------------------------------------------------------------
 Copyright 2016 Cisco Systems

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ------------------------------------------------------------------*/
#define BOOST_TEST_MODULE RestconfProviderTest
#include <boost/test/unit_test.hpp>
#include <string.h>
#include "../core/src/restconf_provider.hpp"
#include "../core/src/errors.hpp"
#include <iostream>
#include "config.hpp"

using namespace ydk;
using namespace std;


BOOST_AUTO_TEST_CASE(CreateDelRead)
{
	ydk::path::Repository repo{TEST_HOME};
	RestconfServiceProvider provider{repo, "localhost", "admin", "admin", 12306, EncodingFormat::JSON};

	auto root_schema = provider.get_root_schema();
	BOOST_REQUIRE(root_schema != nullptr);

	ydk::path::RootSchemaNode* schema = provider.get_root_schema();

	BOOST_REQUIRE(schema != nullptr);

	auto s = ydk::path::CodecService{};

	auto runner = schema->create("ydktest-sanity:runner", "");

	BOOST_REQUIRE( runner != nullptr );

	//get the root
	std::unique_ptr<const ydk::path::DataNode> data_root{runner->root()};

	BOOST_REQUIRE( data_root != nullptr );

	//first delete
	std::unique_ptr<ydk::path::Rpc> delete_rpc { schema->rpc("ydk:delete") };
	auto json = s.encode(runner, EncodingFormat::JSON, false);
	delete_rpc->input()->create("entity", json);
	//call delete
	(*delete_rpc)(provider);

	auto number8 = runner->create("ytypes/built-in-t/number8", "3");
	BOOST_REQUIRE( number8 != nullptr );
    json = s.encode(runner, EncodingFormat::JSON, false);
    BOOST_CHECK_MESSAGE( !json.empty(),
                        "JSON output :" << json);
    //call create
    std::unique_ptr<ydk::path::Rpc> create_rpc { schema->rpc("ydk:create") };
    create_rpc->input()->create("entity", json);
    (*create_rpc)(provider);

    //read
    std::unique_ptr<ydk::path::Rpc> read_rpc { schema->rpc("ydk:read") };
	auto runner_read = schema->create("ydktest-sanity:runner", "");
	BOOST_REQUIRE( runner_read != nullptr );

	json = s.encode(runner_read, EncodingFormat::JSON, false);
	BOOST_REQUIRE( !json.empty() );
	read_rpc->input()->create("filter", json);

	auto read_result = (*read_rpc)(provider);

	runner = schema->create("ydktest-sanity:runner", "");
    number8 = runner->create("ytypes/built-in-t/number8", "5");
    BOOST_REQUIRE( number8 != nullptr );
	json = s.encode(runner, EncodingFormat::JSON, false);
	BOOST_CHECK_MESSAGE( !json.empty(),
						"JSON output :" << json);
	//call update
	std::unique_ptr<ydk::path::Rpc> update_rpc { schema->rpc("ydk:update") };
	update_rpc->input()->create("entity", json);
	(*update_rpc)(provider);


}

