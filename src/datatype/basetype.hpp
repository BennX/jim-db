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

template <typename T>
BaseType<T>::DataUnion::DataUnion(T t)
{
    this->data = t;
}

template <typename T>
BaseType<T>::BaseType() : m_data(), m_next(0) {}

template <typename T>
BaseType<T>::BaseType(T t) : m_data(t), m_next(0) {}

template <typename T>
BaseType<T>::~BaseType()
{
    //used to clear this, set the size to 0 so
    //we know it is a simple type without something behind
    m_data.size = 0;
}

template <typename T>
void BaseType<T>::setNext(ptrdiff_t next)
{
    m_next = next;
}

template <typename T>
std::ptrdiff_t BaseType<T>::getNext()
{
    return m_next;
}

template <typename T>
void BaseType<T>::setData(T t)
{
    m_data.data = t;
}

template <typename T>
T BaseType<T>::getData() const
{
    return m_data.data;
}
