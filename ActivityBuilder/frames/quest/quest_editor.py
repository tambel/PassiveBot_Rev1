import wx
from frames.base_frame import BaseFrame
from frames.quest.quest_frames import SelectQuestGiverFrame


class QuestFrame(BaseFrame):
    def __init__(self,parent_frame):
        """
        :param parent_frame:
        :type parent_frame: Builder
        :return:
        """
        BaseFrame.__init__(self, parent_frame, name='Quest Editor',pos=(500, 200), size=(600, 400))
        self.add_quest_button = wx.Button(self.panel, id=-1, label='Add Quest',pos=(300, 300), size=(100, 50))
        self.add_quest_button.Bind(wx.EVT_BUTTON, self.add_quest_button_click)
        self.com=self.parent.com
        self.current_target=None
        self.start_bg_communication()

    def add_quest_button_click(self,event):
        '''
        packet =RequestPacket(2)
        self.com.send(packet)
        target=TargerObjevtInfoRequest(self.com.recieve())
        self.target_label.SetLabelText(u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format(*(target.fields[f] for f in "name,guid,type,position".split(','))))
        '''

    def select_current_target_button_click(self,event):
        select_qg_frame=SelectQuestGiverFrame(self)


    def on_close(self):
        self.parent.open_quest_button.Enable()

    def add_quest(self):


    def get_quest_giver_info(self):
        if self.current_target is None:
            return
        else:
            self.select_current_target_button.Disable()
            return self.select_current_target_button;






