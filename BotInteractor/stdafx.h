// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#define TINS_HAVE_TCPIP
#include "tins\tins.h"
#include "tins\network_interface.h"
#include "tins/tcp_ip/stream_follower.h"
#include "tins/sniffer.h"
#include "tins/packet.h"
#include "tins/ip_address.h"
#include "tins/ipv6_address.h"

#include "tins/network_interface.h"
#include <tins/exceptions.h>
#include "Tools\Tools.h"
#include "WowInteraction\WowInteraction.h"
#include "WowWorldsData\WowWorldsData.h"
            // »сключите редко используемые компоненты из заголовков Windows


// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
