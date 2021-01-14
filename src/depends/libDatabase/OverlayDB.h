/*
	This file is part of cpp-ethereum.
	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file OverlayDB.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#ifndef __OVERLAYDB_H__
#define __OVERLAYDB_H__

#include <memory>

#include "common/Constants.h"
#include "depends/common/Common.h"
#include "depends/common/RLP.h"
#include "LevelDB.h"
#include "MemoryDB.h"

namespace dev
{
	static std::vector<h256> list_placeholder;

	// TODO: change inheritance from MemoryDB to composition
	class OverlayDB: public MemoryDB
	{
	public:
		explicit OverlayDB(const std::string & dbName): m_levelDB(dbName) {}
		~OverlayDB() = default;

		void ResetDB();
		bool RefreshDB();

		bool commit(bool keepHistory = false, std::vector<h256>& toPurge = list_placeholder);
		void rollback();

		std::string lookup(h256 const& _h) const;
		bool exists(h256 const& _h) const;
		void kill(h256 const& _h);

		bytes lookupAux(h256 const& _h) const;

		void printDB() {
		  LOG_MARKER();
		  std::unique_ptr<leveldb::Iterator> it(
		      m_levelDB.GetDB()->NewIterator(leveldb::ReadOptions()));
		  it->SeekToFirst();
		  for (; it->Valid(); it->Next()) {
		  	LOG_GENERAL(INFO, "key: " << it->key().ToString() << " value: " << it->value().ToString());
		  }
		}

	protected:
		// using MemoryDB::clear;

		LevelDB m_levelDB;
	};
}

#endif // __OVERLAYDB_H__