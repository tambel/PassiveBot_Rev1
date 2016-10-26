// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
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
            // ��������� ����� ������������ ���������� �� ���������� Windows


// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
