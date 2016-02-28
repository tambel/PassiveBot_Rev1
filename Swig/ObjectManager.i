/* File : ObjectManager.i */
%module ObjectManager
%{
#include "../WowInteraction/ObjectManager.h"
%}
%include "pyabc.i"
%include "std_vector.i"
%include "WowInteractionUtils.i"
/* Let's just grab the original header file here */
%include "../WowInteraction/ObjectManager.h"
%template (FooVector) std::vector<Unit*>;
