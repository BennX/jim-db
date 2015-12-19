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

#pragma once
#include <string>
#include <rapidjson/document.h>

namespace jimdb
{
    namespace common
    {
        /**
        All values that can be obtained.

        SIZE_OF_ENUM is not obtainable!
        */
        enum ConfigValues
        {
            LOG_LEVEL = 0,
            LOG_FILE,
            THREADS,
			IP,
            PORT,
            MAX_TASKS,
            PAGE_HEADER,
            PAGE_BODY,
			PAGE_FRAGMENTATION_CLEAN,
			PAGE_BUCKETS,
            SIZE_OF_ENUM,
        };

        /**
        small mapping function struct
        */
        struct ConfigValuesMap
        {
            //need the strings for the size
            static const char* EnumString[];

            /**
            @return the configValue as const char*
            */
            static const char* get(ConfigValues e);
        };

        /**
        * \brief The Configuration class to obtain config values
        *
        * \author Benjamin Meyer
        */
        class Configuration
        {
        public:
            static std::string m_logFileName;
            static Configuration& getInstance();


            /**
            \brief Parse config file into the config

            Parses a config file into the configuration object.
            if it has missing filds this function will add the
            default values to the file and to the config.

            @throws runtime_error if there are missing values
            \author Benjamin Meyer
            \date 09.07.2015 13:02
            */
            bool init(const std::string& filename);

            /**\brief Returns the value to the key
            *
            * If the key does not exist in the config
            \param[in] key
            \return the rapidjson value to check what it is an get the right value of it
            *@author Benjamin Meyer
            *@date Dienstag, 28. Juli 2015
            */
            rapidjson::Value& operator[](const ConfigValues& key);

            /**
            \brief generates a default configuration

            @return a default json configuration as string
            @author Benjamin Meyer
            @date 07.10.2015 11:33
            */
            std::string generate() const;

            friend std::ostream& operator<<(std::ostream& os, const Configuration& obj);

        private:
            Configuration() { };
            ~Configuration() { };

            //no copy no move no swap
            Configuration(const Configuration&) = delete;
            Configuration& operator=(const Configuration& other) = delete;

            /**
            Check if the document contains the config value
            @author Benjamin Meyer
            @date 05.10.2015 11:50
            */
            void check(const ConfigValues& c);

            rapidjson::Document m_values;

            //member
            static Configuration m_instance;
            //std::map<std::string, std::string> m_values;
        };
    }
}