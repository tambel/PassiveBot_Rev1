/* File : WowObject.i */
%module WowObject
%{
#include "../WowInteraction/WowObject.h"
%}
%include <std_string.i>
%include <std_wstring.i>
%include "WowInteractionUtils.i"
/* Let's just grab the original header file here */

%include "../WowInteraction/WowObject.h"


