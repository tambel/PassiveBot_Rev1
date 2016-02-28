/* File : BotInteractor.i */
%module BotInteractor
%{
#include "../BotInteractor/BotInteractor.h"
%}
%include "NavArea.i"
%include "Player.i"
/* Let's just grab the original header file here */
%include "../BotInteractor/BotInteractor.h"


