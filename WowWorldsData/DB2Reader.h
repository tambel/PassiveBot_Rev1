#pragma once
#include "Tools\BinaryReader.h"
#include "wdb_headers.h"

class DB2Reader
{
	unsigned string_table_start;
	bool cache_strings;
	char * strings = nullptr;
	string filename;
public:
	DB2Reader(string & filename, bool cache_strings=false);
	~DB2Reader();
	template <typename T>
	vector<T> ReadWhole();
	string ReadString(unsigned ofs);
	inline char * GetStrings() { return strings; };


};

template<typename T>
inline vector<T> DB2Reader::ReadWhole()
{
	BinaryReader reader = BinaryReader(filename);
	unsigned magic = reader.Read<unsigned>();
	reader.SetPosition(reader.GetPosition() - 4);
	wdb5_db2_header header = reader.Read<wdb5_db2_header>();
	struct field_structure
	{
		int16_t size;                                               // size in bits as calculated by: byteSize = (32 - size) / 8; this value can be negative to indicate field sizes larger than 32-bits
		uint16_t position;                                          // position of the field within the record, relative to the start of the record
	};
	field_structure fields[1000];// = new field_structure[header.field_count];
	reader.ReadArray<field_structure>(fields, header.field_count);
	
	T * records = new T[header.record_count];
	reader.ReadArray<T>(records, header.record_count);
	string_table_start = reader.GetPosition();
	if (cache_strings)
	{
		strings = new char[header.string_table_size];
		reader.ReadArray<char>(strings, header.string_table_size);
	}
	vector<T> result= vector<T>(records, records+header.record_count);
	delete[] records;
	return result;
}

