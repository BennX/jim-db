/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM-DB.                                         #
# Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

#include "messagefactory.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace jimdb
{
    namespace network
    {
        const char* MessageTypeMap::EnumString[] =
        {
            "handshake",
            "result",
            "error",
            "what"
        };

        static_assert(sizeof(MessageTypeMap::EnumString) / sizeof(char*) == ENUM_SIZE, "size dont match!");

        const char* MessageTypeMap::get(const MessageTypes& e)
        {
            return EnumString[e];
        }

        MessageFactory::MessageFactory() {}

        std::shared_ptr<std::string> MessageFactory::handshake()
        {
            rapidjson::Document doc;
            doc.SetObject();
            doc.AddMember(rapidjson::Value(MessageTypeMap::get(HANDSHAKE), doc.GetAllocator()), rapidjson::Value("hi"),
                          doc.GetAllocator());
            return generate(HANDSHAKE, doc);
        }

        std::shared_ptr<std::string> MessageFactory::error(const std::string& what)
        {
            rapidjson::Document doc;
            doc.SetObject();
            doc.AddMember(rapidjson::Value(MessageTypeMap::get(WHAT), doc.GetAllocator()), rapidjson::Value(what.c_str(),
                          doc.GetAllocator()), doc.GetAllocator());
            return generate(ERROR, doc);
        }

        std::shared_ptr<std::string> MessageFactory::generateResultInsert(const uint64_t& oid)
        {
            rapidjson::Document doc;
            doc.SetObject();
            doc.AddMember("oid__", rapidjson::Value(oid), doc.GetAllocator());
            return generate(RESULT, doc);
        }

        std::shared_ptr<std::string> MessageFactory::generate(const MessageTypes& t,
                rapidjson::Value& data)
        {
            rapidjson::Document doc;
            doc.SetObject();
            doc.AddMember("type", rapidjson::Value(MessageTypeMap::get(t), doc.GetAllocator()), doc.GetAllocator());
            doc.AddMember("data", data, doc.GetAllocator());
            return toString(doc);
        }

        std::shared_ptr<std::string> MessageFactory::toString(rapidjson::Value& data) const
        {
            // Convert JSON document to string
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            data.Accept(writer);
            return std::make_shared<std::string>(strbuf.GetString());
        }
    }
}