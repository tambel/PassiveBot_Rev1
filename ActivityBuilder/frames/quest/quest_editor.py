import wx
from frames.base_frame import BaseFrame
from frames.quest.quest_frames import SelectQuestGiverFrame, SelectQuestDialog
from game_types import GUID


class QuestFrame(BaseFrame):
    def __init__(self, parent_frame):
        """
        :param parent_frame:
        :type parent_frame: Builder
        :return:
        """
        BaseFrame.__init__(self, parent_frame, name='Quest Editor', pos=(400, 200), size=(600, 400))

        self.quest_giver_box = wx.StaticBox(self.panel, label="Quest Giver", pos=(10, 10), size=(200, 100))
        self.quest_select_box = wx.StaticBox(self.panel, label="Quest Giver", pos=(10, 110), size=(200, 100))
        self.quest_giver_label = wx.StaticText(self.quest_giver_box, label="None", pos=(20, 20), size=(100, 40))
        self.add_quest_button = wx.Button(self.quest_giver_box, id=-1, label='Select quest giver', pos=(50, 60),
                                          size=(100, 30))
        self.add_quest_button.Bind(wx.EVT_BUTTON, self.add_quest_button_click)
        self.select_quest_button = wx.Button(self.quest_select_box, id=-1, label='Select quest', pos=(50, 60),
                                             size=(100, 30))
        self.selected_quest_label = wx.StaticText(self.quest_select_box, label="None", pos=(20, 20), size=(100, 40))
        self.select_quest_button.Bind(wx.EVT_BUTTON, self.select_quest_button_click)
        self.select_quest_button.Disable()
        self.com = self.parent.com
        self.current_target = None
        self.quest_info = None

    def add_quest_button_click(self, event):
        qg_dialog = SelectQuestGiverFrame(self)
        if qg_dialog.ShowModal() == wx.ID_OK:
            self.current_target = qg_dialog.current_target
            self.quest_giver_label.SetLabelText(
                u"Name: {}\nType: {}".format(self.current_target.fields["name"], self.current_target.fields["type"]))
            self.select_quest_button.Enable()

    def select_quest_button_click(self, event):
        sq_dialog = SelectQuestDialog(self)
        if sq_dialog.ShowModal() == wx.ID_OK:
            self.quest_info = sq_dialog.quest_info
            self.selected_quest_label.SetLabelText(
                u"Quest ID: {}\nQuest title: {}".format(self.quest_info.fields['id'].value,
                                                        self.quest_info.fields['title'].text))

    def select_current_target_button_click(self, event):
        select_qg_frame = SelectQuestGiverFrame(self)

    def OnListBox1Listbox(self, event):
        pass

    def on_close(self):
        self.parent.open_quest_button.Enable()

    def get_quest_giver_info(self):
        if self.current_target is None:
            return
        else:
            self.select_current_target_button.Disable()
            return self.select_current_target_button;
