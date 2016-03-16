#!/usr/bin/env python
# -*- coding: utf-8 -*-
from PassiveBot.PassiveBot import PassiveBot

from PassiveBot import Data

bot = PassiveBot()

bot.start()
while 1:
    for unit in bot.ObjectManager.GetUnitsList():
        print unit.GetName()


