from PyQt4.QtGui import QApplication,QWidget,QMainWindow, QListView,QStandardItem,QStandardItemModel,QTextEdit, \
    QTextEdit
from PyQt4.QtCore import QModelIndex
from PyQt4.QtCore import SIGNAL,SLOT
from PyQt4 import uic,QtCore
from PassiveBot.PassiveBot import PassiveBot,ObjectManager,Guid128
from threading import Thread,Event
import time

class GameInfoWidget(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        pass


class MyWidget(QMainWindow):
    class ObjectStates(object):
        def __init__(self):
            self.player_guid = Guid128()
            self.target_guid = Guid128()

    def __init__(self):
        QWidget.__init__(self)
        uic.loadUi("UI/MainWindow.ui", self)
        self.bot = PassiveBot()
        self.bot_thread=Thread(target=self.bot_thread_worker)
        self.bot_thread.start()
        time.sleep(0.5)
        self.updated = Event()
        self.object_states = MyWidget.ObjectStates()
        self.init_quest_list()

    def init_quest_list(self):


        self.bot.QuestManager.EnumActiveQuests()
        self.model=QStandardItemModel(self.quest_list)
        for quest in [self.bot.QuestManager.GetQuest(quest_id)for quest_id in self.bot.QuestManager.GetQuestIds()]:
            item=QStandardItem(quest.GetTitle())
            item.setEditable(False)
            self.model.appendRow(item)
        self.quest_list.setModel(self.model)
        self.quest_list.connect(self.quest_list, SIGNAL('itemActivated(QModelIndex)'), self.quest_id_edit, SLOT('setText'))


    @QtCore.pyqtSlot("QModelIndex")
    def on_quest_list_clicked(self, model_index):

        '''
        :type model_index: QModelIndex
        :param model_index:
        :return:
        '''
        for quest_id in self.bot.QuestManager.GetQuestIds():
            if self.bot.QuestManager.GetQuest(quest_id).GetTitle()==self.model.item(model_index.row()).text():
                self.quest_id_edit.setText(str(quest_id))
                break

    def bot_thread_worker(self):
        self.bot.start()
        while 1:
            obj = ObjectManager.GetTargetObject()
            player = ObjectManager.GetPlayer()
            self.player_name_edit.setText(player.GetName(True))
            self.player_guid_edit.setText("{}{}".format(player.GetGuid().high, player.GetGuid().low))
            if obj:

                self.target_name_edit.setText(obj.GetName(True))
                self.target_guid_edit.setText("{}{}".format(obj.GetGuid().high, obj.GetGuid().low))
                self.target_type_edit.setText(str(ord(obj.GetType(True))))
            else:
                self.target_name_edit.setText("No Target.")

            time.sleep(0.5)








app=QApplication([])
l=MyWidget()
l.show()
app.exec_()