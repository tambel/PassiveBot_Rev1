/* File : Player.i */
%module Player
%{
#include "../WowInteraction/Player.h"
%}
%include <std_string.i>
%include <std_wstring.i>
%include "Unit.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/Player.h"

