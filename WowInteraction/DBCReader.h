#pragma once
struct WoWClientDB
{
	void *funcTable;
	int isLoaded;
	int numRows;
	int maxIndex;
	int minIndex;
	int stringTable;
	void *funcTable2;
	int FirstRow;
	DWORD **Rows;
};
struct Test
{
	void *funcTable;
	wchar_t * f1;
	wchar_t * f2;
	wchar_t * f3;
	wchar_t * f4;
	wchar_t * f5;
	wchar_t * f6;
	wchar_t * f7;
	wchar_t * f8;
	wchar_t * f9;
	wchar_t * f10;
	wchar_t * f11;
	wchar_t * f12;
	wchar_t * f13;
	wchar_t * f14;
	wchar_t * f15;
	wchar_t * f16;
	wchar_t * f17;
	wchar_t * f18;
	wchar_t * f19;
	wchar_t * f20;
	wchar_t * f21;
	wchar_t * f22;
	wchar_t * f23;
	wchar_t * f24;
	wchar_t * f25;
	wchar_t * f26;
	wchar_t * f27;
	wchar_t * f28;
	wchar_t * f29;
	wchar_t * f30;
	wchar_t * f31;
	wchar_t * f32;
	wchar_t * f33;
	wchar_t * f34;
	wchar_t * f35;
	DWORD **Rows;
};

class DBCReader
{

public:
	DBCReader();
	~DBCReader();
};
//class DBC<T> : IEnumerable<T> where T : struct
//{
//	private readonly WoWClientDB m_dbInfo;
//	private readonly DBCFile m_fileHdr;
//
//	public int MinIndex{ get{ return m_dbInfo.MinIndex; } }
//	public int MaxIndex{ get{ return m_dbInfo.MaxIndex; } }
//	public int NumRows{ get{ return m_dbInfo.NumRows; } }
//
//	public T this[int index]{ get{ return WowMemory.Read<T>(GetRowPtr(index)); } }
//
//	public bool HasRow(int index) { return GetRowPtr(index) != IntPtr.Zero; }
//
//	/// <summary>
//	/// Initializes a new instance of DBC class using specified memory address
//	/// </summary>
//	/// <param name="dbcBase">DBC memory address</param>
//	public DBC(IntPtr dbcBase)
//	{
//		IntPtr addr = WowMemory.Rebase(dbcBase);
//		m_dbInfo = WowMemory.Read<WoWClientDB>(addr);
//		m_fileHdr = WowMemory.Read<DBCFile>(m_dbInfo.Data);
//	}
//
//	private IntPtr GetRowPtr(int index)
//	{
//		if (index < MinIndex || index > MaxIndex)
//			return IntPtr.Zero;
//
//		int actualIndex = index - MinIndex;
//
//		int v5 = WowMemory.Read<int>(m_dbInfo.Unk1 + (4 * (actualIndex >> 5)));
//
//		int a2a = (int)(actualIndex & 0x1Fu);
//
//		if (((1 << a2a) & v5) != 0)
//		{
//			int bitsSet = CountBitsSet(v5 << (31 - a2a));
//			int entry = bitsSet + GetArrayEntryBySizeType(m_dbInfo.Unk3, actualIndex >> 5) - 1;
//
//			if (m_dbInfo.Unk2 == 0)
//			{
//				entry = GetArrayEntryBySizeType(m_dbInfo.Rows, entry);
//			}
//			return m_dbInfo.FirstRow + m_fileHdr.RecordSize * entry;
//		}
//
//		return IntPtr.Zero;
//	}
//
//	private int CountBitsSet(int a1)
//	{
//		return 0x1010101 *
//			((((a1 - ((a1 >> 1) & 0x55555555)) & 0x33333333) +
//				(((a1 - ((a1 >> 1) & 0x55555555)) >> 2) & 0x33333333) +
//				((((a1 - ((a1 >> 1) & 0x55555555)) & 0x33333333) +
//					(((a1 - ((a1 >> 1) & 0x55555555)) >> 2) & 0x33333333)) >> 4)) & 0x0F0F0F0F) >> 24;
//	}
//
//	private int GetArrayEntryBySizeType(IntPtr arrayPtr, int index)
//	{
//		if (m_dbInfo.RowEntrySize == 2)
//			return WowMemory.Read<short>(arrayPtr + (2 * index));
//		else
//			return WowMemory.Read<int>(arrayPtr + (4 * index));
//	}
//
//	public IEnumerator<T> GetEnumerator()
//	{
//		for (uint i = 0; i < NumRows; ++i)
//			yield return WowMemory.Read<T>(m_dbInfo.FirstRow + (int)(i * m_fileHdr.RecordSize));
//	}
//
//	System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
//	{
//		return GetEnumerator();
//	}
//}
