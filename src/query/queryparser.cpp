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
#include "queryparser.h"
#include <string>
#include "../log/logger.h"
#include <algorithm>

namespace jimdb
{
    namespace query
    {
        const char QueryParser::m_delimiter = ' ';
        const char* QueryParser::m_queryTypes[] =
        {
            "select",
            "delete",
            "insert",
        };

        const char* QueryParser::m_condTypes[] =
        {
            "where",
            "or",
            "and",
            "!=",
            "="
        };
        
        Condition::Condition(const bool& eq, const std::string& name, const std::string& value): m_equal(eq), m_name(name),
            m_value(value) {}

        void QueryParser::parse(std::string& s)
        {
            std::stringstream ss(s);
            std::string item;
            //get the first item
            std::getline(ss, item, m_delimiter);

            //set the type
            if(item == m_queryTypes[SELECT])
            {
                m_type = SELECT;

                //at select second item must be where
                //get the first item
                std::getline(ss, item, m_delimiter);
                erase(item);

                if (item != m_condTypes[WHERE])
                {
                    LOG_WARN << "invalid select query";
                    return;
                }
                parseConditions(ss);
            }
            else if (item == m_queryTypes[DELETE_R])
            {
                std::getline(ss, item, m_delimiter);
                erase(item);

                if (item != m_condTypes[DELETE_R])
                {
                    LOG_WARN << "invalid select query";
                    return;
                }
                m_type = DELETE_R;
                parseConditions(ss);
            }
            else if (item == m_queryTypes[INSERT])
            {
                std::getline(ss, item, m_delimiter);
                erase(item);

                if (item != m_condTypes[INSERT])
                {
                    LOG_WARN << "invalid select query";
                    return;
                }
                m_type = INSERT;
            }
        }

        void QueryParser::parseConditions(std::stringstream& ss)
        {
            std::string item;
            while (std::getline(ss, item, m_delimiter))
            {
                //now it must follow touple of "name"="value" or !=
                //so we need to split the item at = or !=
                std::size_t pos;
                if ((pos = item.find(m_condTypes[EQUAL])) != std::string::npos && item.find(m_condTypes[NOTEQUAL]) == std::string::npos)
                {
                    m_conditions.push_back(Condition(true, erase(item.substr(0, pos)), erase(item.substr(pos + 1))));
                }
                else if ((pos = item.find(m_condTypes[NOTEQUAL])) != std::string::npos)
                {
                    m_conditions.push_back(Condition(false, erase(item.substr(0, pos)), erase(item.substr(pos + 2))));
                }
                else
                {
                    LOG_WARN << "invalid condition";
                }
                //now last but not least if there is one more item it's
                //a "and" or a "or"
                if (std::getline(ss, item, m_delimiter))
                {
                    //item is not and or or
                    if (item == m_condTypes[AND])
                    {
                        m_andOr.push_back(AND);
                    }
                    else if (item == m_condTypes[OR])
                    {
                        m_andOr.push_back(OR);
                    }
                }
                else
                {
                    // nothing to find here so we are done
                    return;
                }
            }
        }

        std::string& QueryParser::erase(std::string&& s) const
        {
            s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
            return s;
        }

        std::string& QueryParser::erase(std::string& s) const
        {
            s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
            return s;
        }
    }
}