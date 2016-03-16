#!/usr/bin/env python
# -*- coding: utf-8 -*-
from BotInteractor.BotInteractor import BotInteractor, Unit, ObjectManager,Player, Guid128, QuestManager


class PassiveBot(object):
    BotInteractor = BotInteractor
    ObjectManager = ObjectManager
    QuestManager = QuestManager
    def __init__(self):
        pass

    def start(self):
        BotInteractor.Init()
        BotInteractor.StartGame('arttambel@gmail.com', 'archi911', "Testintaurenr".decode('utf-8'))





