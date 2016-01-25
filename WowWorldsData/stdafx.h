// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <algorithm>

#include "Tools\Configuration.h"
#include "Tools\BinaryReader.h"
#include "Adt.h"
#include "ADTCache.h"
#include "ADTStructs.h"
#include "ADTWorker.h"
#include "Navigation.h"
#include "Chunk.h"
#include "ChunkCache.h"
#include "ChunkedData.h"
#include "SquareArea.h"
#include "NavArea.h"
#include "Utils.h"
#include "Model.h"
#include "WMORoot.h"
#include "WMOGroup.h"
#include "WMO.h"
#include "WMOPart.h"

#include "WMOStructs.h"
#include "WowObjectAvatar.h"


#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows



// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
