/// YANG Development Kit
// Copyright 2016 Cisco Systems. All rights reserved
//
////////////////////////////////////////////////////////////////
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>

#include "errors.hpp"
#include "netconf_client.hpp"
#include "netconf_model_provider.hpp"
#include "types.hpp"
#include "ydk_yang.hpp"
#include <memory>
#include <boost/log/trivial.hpp>

using namespace std;
using namespace ydk;

namespace ydk
{
NetconfModelProvider::NetconfModelProvider(NetconfClient & client)
	: client(client)
{
}

NetconfModelProvider::~NetconfModelProvider()
{
}

std::string NetconfModelProvider::get_hostname_port()
{
	return client.get_hostname_port();
}

string NetconfModelProvider::get_model(const string& name, const string& version, Format format)
{
    string model{};

    if(name == ydk::path::YDK_MODULE_NAME && version == ydk::path::YDK_MODULE_REVISION) {
       return ydk::path::YDK_MODULE;
    }

    if(name == ydk::IETF_NETCONF_MODULE_NAME && version == ydk::IETF_NETCONF_MODULE_REVISION) {
       return ydk::IETF_NETCONF_MODULE;
    }

    //have to craft and send the raw payload since the schema might
    //not be available

    string file_format = "yang";
    if(format == Format::YIN) {
        file_format = "yin";
    }

    path::CodecService codec_service{};

    string payload{"<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"};
    payload+= R"(<get-schema xmlns="urn:ietf:params:xml:ns:yang:ietf-netconf-monitoring">)";
    payload+="<identifier>";
    payload+=name;
    payload+="</identifier>";
    if(!version.empty()){
        payload+="<version>";
        payload+=version;
        payload+="</version>";
    }
    payload+="<format>";
    payload+=file_format;
    payload+="</format>";
    payload+="</get-schema>";
    payload+="</rpc>";

    BOOST_LOG_TRIVIAL(trace) << "Get schema request " << payload;
    string reply = client.execute_payload(payload);
    BOOST_LOG_TRIVIAL(trace) << "Get schema reply " << reply;


    auto data_start = reply.find("<data ");
    if(data_start == string::npos) {
        return model;
    }

    auto data_end = reply.find("</data>", data_start);
    if(data_end == string::npos) {
        //indicates that data was probably empty
        return model;
    }

    //need to find the end of the "<data start tag
    auto data_start_end = reply.find(">", data_start);
    data_start = data_start_end + 1;

    data_end -= 1;
    model = reply.substr(data_start, data_end-data_start+1);

    //Remove <!CDATA[ if any
    auto cdata_start = model.find("<![CDATA[");
    if(cdata_start != string::npos) {
        auto cdata_end = model.find("]]>", cdata_start);
        if(cdata_end != string::npos) {
            data_start = cdata_start + sizeof("<![CDATA[") - 1;
            data_end = cdata_end;
            model = model.substr(data_start, data_end - data_start);
        }
    }

    BOOST_LOG_TRIVIAL(trace) << "Model " << model;

    return model;
}
}
