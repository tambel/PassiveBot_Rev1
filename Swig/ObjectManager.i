/* File : ObjectManager.i */
%module ObjectManager
%{
#include "../WowInteraction/ObjectManager.h"
%}
%include <std_string.i>
%include <std_wstring.i>
%include "pyabc.i"
%include "std_vector.i"
%include "WowInteractionUtils.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/ObjectManager.h"
%template (UnitVector) std::vector<Unit*>;
