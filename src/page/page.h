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
#include <memory>
#include "../datatype/freetype.h"
#include <rapidjson/document.h>
#include "../thread/rwlock.h"
#include "../thread/spinlock.h"

namespace jimdb
{
    namespace memorymanagement
    {
        class HeaderMetaData;

        /**
        \brief The page Class

        It holds data and header information.

        @author Benjamin Meyer
        @date 23.10.2015 16:34
        */
        class Page
        {
        public:
            Page(long long header, long long body);
            ~Page();

            long long getID();
            void setNext(const long long& id);
            long long getNext() const;

            long long free();

            bool isLocked() const;

            /**
            \brief check if there is a chunk min size

            @return true if there is one chunk big enough
            	AND if there is one more header slot!
            @author Benjamin Meyer
            @date 31.10.2015 13:29
            */
            bool free(const size_t& size);
            /**
            \brief Insert a json obj to the page and checks for the meta data

            it also creates the metadata for the objects if needed
            @return returns the object ID
            @author Benjamin Meyer
            @date 23.10.2015 17:26
            */
            size_t insert(const rapidjson::GenericValue<rapidjson::UTF8<>>& value);

            /**
            \brief set it after sys crashed

            @author Benjamin Meyer
            @date 31.10.2015 14:54
            */
            void setObjCounter(const long long& value) const;


            /**
            \brief Generate the object from headerpos

            @author Benjamin Meyer
            @date 02.11.2015 11:44
            */
            std::shared_ptr<std::string> getJSONObject(const long long& headerpos);

        private:
            static long long m_objCount;
            //const voidptr to memory to static cast as we like
            char* const m_header;
            char* const m_body;

            /**############################################
            * private methods for body
            * ############################################*/

            //pointer to the free typ chain start
            FreeType* m_free;
            //holds the information of free space
            long long m_freeSpace;
            //position of the free typ object start info
            //it doesnt change so its const
            long long* const m_freepos;

            /**############################################
            * private methods for header
            * ############################################*/

            //offset to the next free header position
            //it doesnt change so its const too
            long long* const m_headerFreePos;
            long long m_headerSpace;
            //Freetype of the header
            FreeType* m_headerFree;


            //holds the ID of the next page
            long long m_next;
            long long m_id;

            //id generation with static counter
            static long long m_s_idCounter;

            //lock for getFree and so on
            tasking::RWLock m_rwLock;

            //lock for the free to be sure there is noone else checking right now
            tasking::SpinLock m_spin;

            /**############################################
             * private methods for object
             * ############################################*/

            /**
            \brief Insert a Object

            @param[in] last the previous type of "inner" objects,
            	nullptr if there was no previous since its the frist object to be
            	inserted.
            @param[in] value the rapidjson vlaue to be inserted
            @return the ptr to the first element and the last element
            @author Benjamin Meyer
            @date 29.10.2015 12:12
            */
            std::pair<void*, void*> insertObject(const rapidjson::GenericValue<rapidjson::UTF8<>>& value,
                                                 BaseType<size_t>* const l_last);

            /**
            \brief returns a ptr to the slot where it can fit
            	also can be used to find a valid spot!

            @param[in] size size of the object to fit
            @param[in] aloc bool to indicate we should aloc the space. Default is TRUE!
            	Set false for checking if there is a slot for the size without alocing.
            @return void* is also a FreeType* or nullptr if there is no slot!
            @author Benjamin Meyer
            @date 04.11.2015 08:28
            */
            void* find(const size_t& size, bool aloc = true);

            /**
            \brief Caluclate the distance between two pointers

            @author Benjamin Meyer
            @date 04.11.2015 08:35
            */
            std::ptrdiff_t dist(const void* const p1, const void* const p2) const;

            /**
            \brief insert a whole array

            @param[in] prev the prev element to set the next
            @return the ptr to the last element
            @author Benjamin Meyer
            @date 31.10.2015 15:22
            */
            void* insertArray(const rapidjson::GenericValue<rapidjson::UTF8<>>& arr, BaseType<size_t>* prev);


            /**############################################
            * private methods for header insert
            * ############################################*/

            HeaderMetaData* insertHeader(const size_t& id, const size_t& hash, const size_t& type, const size_t& pos);
            /**
            \brief returns a header position, also used to check if we have a valid slot
            	just set aloc to false which is default true!

            @param[in] aloc do also aloc the space. If false we can check for a header position
            @author Benjamin Meyer
            @date 04.11.2015 08:51
            */
            void* findHeaderPosition(bool aloc = true);

            /**############################################
            * private methods for building the Object out of memory
            * ############################################*/


            /**
            \brief Add Member to a rapidjson::Value
            	It iterates the MetaInformation of the Hash value
            	to generate the members.

            @param[in] hash the name hash of the object to be inserted to get the Meta for it
            @param[in] start the first element ob the object
            @param[in] toAdd the Value where we add the members to
            @param[in] aloc the allocator for new values
            @return the last element ptr
            @author Benjamin Meyer
            @date 02.11.2015 14:59
            */
            void* buildObject(const size_t& hash, void* start, rapidjson::Value& toAdd, rapidjson::MemoryPoolAllocator<>& aloc);

            void* buildArray(const long long& elemCount, void* start, rapidjson::Value& toAdd,
                             rapidjson::MemoryPoolAllocator<>& aloc);
        };
    }
}
#include "page.hpp"