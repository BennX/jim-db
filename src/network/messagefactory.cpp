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
        MessageFactory::MessageFactory() {}

        std::shared_ptr<std::string> MessageFactory::generate(const MessageTypes& t) const
        {
            rapidjson::Document doc;
            doc.SetObject();
            switch (t)
            {
                case HANDSHAKE:
                    {
                        //todo maybe a better handshake?!
                        doc.AddMember("type", "handshake", doc.GetAllocator());
                        doc.AddMember("data", "hi", doc.GetAllocator());
                    }
                    break;
            }
            // Convert JSON document to string
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            doc.Accept(writer);
            return std::make_shared<std::string>(strbuf.GetString());
        }

        std::shared_ptr<std::string> MessageFactory::generate(const rapidjson::GenericValue<rapidjson::UTF8<>>& result)
        {
            return nullptr;
        }

        std::shared_ptr<std::string> MessageFactory::generateResultInsert(const size_t& oid)
        {
            rapidjson::Document doc;
            doc.SetObject();
            //todo maybe a better handshake?!
            doc.AddMember("type", "result", doc.GetAllocator());

            rapidjson::GenericValue<rapidjson::UTF8<>> data;
            data.SetObject();
			rapidjson::Value l_oid(oid);
            data.AddMember("oid__", l_oid, doc.GetAllocator());
            doc.AddMember("data", data, doc.GetAllocator());

            // Convert JSON document to string
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            doc.Accept(writer);
            return std::make_shared<std::string>(strbuf.GetString());
        }
    }
}