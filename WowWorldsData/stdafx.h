// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <algorithm>

#include "Tools\Configuration.h"
#include "Tools\BinaryReader.h"
#include "Tools\Exceptions.h"
#include "Adt.h"
#include "ADTCache.h"
#include "ADTStructs.h"
#include "ADTWorker.h"
#include "Navigation.h"
#include "Chunk.h"
#include "ChunkCache.h"
#include "ChunkedData.h"
#include "Area.h"
#include "NavArea.h"
#include "Utils.h"
#include "Model.h"
#include "Doodad.h"
#include "WMORoot.h"
#include "WMOGroup.h"
#include "WMO.h"
#include "WMOPart.h"
#include "WMOStructs.h"



#define WIN32_LEAN_AND_MEAN             // »сключите редко используемые компоненты из заголовков Windows



// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
