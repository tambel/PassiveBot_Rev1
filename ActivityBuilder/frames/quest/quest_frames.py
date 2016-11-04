import wx
from frames.base_frame import BaseFrame, BaseDialog
from packets import TargerObjevtInfoReply, RequestPacket, Requests, TargetQuestGiverQuestListReply, \
    SelectFromQuestListReply


class SelectQuestGiverFrame(BaseDialog):
    def __init__(self, parent_frame):
        """

        :param parent_frame:
        :type parent_frame: QuestFrame
        :return:
        """
        BaseDialog.__init__(self, parent_frame, name="Select Quest Giver")
        self.okButton = wx.Button(self.panel, wx.ID_OK, "OK", pos=(140, 200), size=(100, 40))
        self.okButton.Bind(wx.EVT_BUTTON, self.select_current_target_button_click)
        self.com = self.parent.com
        self.current_target = None
        self.target_label = wx.StaticText(self.panel,
                                          label=u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format("None", 0, 0, 0),
                                          pos=(10, 0), size=(100, 50))

    def background_communication(self):
        packet = RequestPacket(2)
        self.com.send(packet)
        self.current_target = TargerObjevtInfoReply(self.com.receive())
        self.target_label.SetLabelText(u"Name: {}\nGUID: {}\nType: {}\nPosition: {}".format(
            *(self.current_target.fields[f] for f in "name,guid,type,position".split(','))))

    def select_current_target_button_click(self, event):
        self.stop_bg_communication()
        if self.current_target is not None:

            self.EndModal(wx.ID_OK)
        else:
            self.EndModal(wx.ID_CANCEL)


class SelectQuestDialog(BaseDialog):
    def __init__(self, parent):
        BaseDialog.__init__(self, parent, "Select Quest")
        self.okButton = wx.Button(self.panel, wx.ID_OK, "OK", pos=(140, 200), size=(100, 40))
        self.okButton.Bind(wx.EVT_BUTTON, self.select_current_target_button_click)
        self.com = self.parent.com
        self.quest_list = None
        self.quest_info = None
        self.select_quest_label = wx.StaticText(self.panel, label="Select available quest", pos=(10, 10),
                                                size=(380, 20))
        self.quest_list_box = wx.ListBox(choices=[], name='listBox1', parent=self.panel, pos=wx.Point(10, 30),
                                         size=wx.Size(380, 100), style=0)
        self.getting_list = True;
        self.selected_quest_label = wx.StaticText(self.panel, label="", pos=(10, 150), size=(380, 200))
        self.selected_quest_label.Hide()

    def select_current_target_button_click(self, event):
        if self.quest_info is not None:
            self.EndModal(wx.ID_OK)
        else:
            self.EndModal(wx.ID_CANCEL)

    def get_available_quest_list(self):
        self.quest_list_box.Clear()
        packet = RequestPacket(Requests.TargetQuestGiverQuestList)
        self.com.send(packet)
        self.quest_list = TargetQuestGiverQuestListReply(self.com.receive())
        for i in range(0, self.quest_list.fields["count"].value):
            self.quest_list_box.Append(self.quest_list.fields["name{}".format(i)].text)

    def select_quest(self):
        packet = RequestPacket(Requests.SelectQuestFromList)
        self.com.send(packet)
        self.quest_info = SelectFromQuestListReply(self.com.receive())
        self.quest_list_box.Hide()

        self.select_quest_label.SetLabelText(
            u"Quest ID: {}\nQuest title: {}".format(self.quest_info.fields['id'].value, self.quest_info.fields['title'].text))
        self.selected_quest_label.Show()

    def background_communication(self):
        if self.getting_list:
            self.get_available_quest_list()
            self.select_quest()
            self.stop_bg_communication()

