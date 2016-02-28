/* File : Player.i */
%module Player
%{
#include "../WowInteraction/Player.h"
%}
%include "Unit.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/Player.h"

