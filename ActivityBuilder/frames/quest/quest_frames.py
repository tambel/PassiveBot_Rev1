import wx
from frames.base_frame import BaseFrame, BaseDialog
from packets import TargerObjevtInfoReply, RequestPacket, Requests, TargetQuestGiverQuestListReply, \
    SelectFromQuestListReply, PlayerPositionReply, TargetEntityIdReply
import time
import math


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
        self.selected_quest_label = wx.StaticText(self.panel, label="", pos=(10, 150), size=(380, 50))
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
            u"Quest ID: {}\nQuest title: {}".format(self.quest_info.fields['id'].value,
                                                    self.quest_info.fields['title'].text))
        self.selected_quest_label.Show()

    def background_communication(self):
        if self.getting_list:
            self.get_available_quest_list()
            self.select_quest()
            self.stop_bg_communication()


def calc_distance(v1, v2):
    return math.sqrt((v1.x - v2.x) ** 2 + (v1.y - v2.y) ** 2)

class CompleteQuestObjectiveDialog(BaseDialog):
    pass




class QuestObjectiveDialog(BaseDialog):
    def __init__(self, parent):
        BaseDialog.__init__(self, parent, "Objective dialog")
        self.path_to_objective_recording = False
        self.objective_blob_recording = False
        self.path = None
        self.blob=None
        self.monster_to_kill_id=None
        self.start_record_path_button = wx.Button(self.panel, label="Start record path", pos=(10, 10), size=(100, 30))
        self.start_record_path_button.Bind(wx.EVT_BUTTON, self.start_record_path_button_click)
        self.path_to_objective_label = wx.StaticText(self.panel, label="None", pos=(130, 10), size=(100, 40))

        self.start_record_blob_button = wx.Button(self.panel, label="Start record blob", pos=(10, 50), size=(100, 30))
        self.start_record_blob_button.Bind(wx.EVT_BUTTON, self.start_record_blob_button_click)
        self.start_record_blob_label = wx.StaticText(self.panel, label="None", pos=(130, 50), size=(100, 40))

        self.select_monster_to_kill_button = wx.Button(self.panel, label="Select Monster", pos=(10, 90), size=(100, 30))
        self.select_monster_to_kill_button.Bind(wx.EVT_BUTTON, self.select_monster_to_kill_button_click)
        self.select_monster_to_kill_label = wx.StaticText(self.panel, label="None", pos=(130, 90), size=(100, 40))

    def start_record_path_button_click(self, event):
        self.path_to_objective_recording = not self.path_to_objective_recording

    def start_record_blob_button_click(self, event):
        self.objective_blob_recording = not self.objective_blob_recording

    def select_monster_to_kill_button_click(self,event):
        packet = RequestPacket(Requests.TargetEntityID)
        self.com.send(packet)
        monster_to_kill = TargetEntityIdReply(self.com.receive())
        self.monster_to_kill_id=monster_to_kill.fields['id'].value
        self.select_monster_to_kill_label.SetLabelText("Monster ID: {}".format(self.monster_to_kill_id))

    def background_communication(self):
        if self.path_to_objective_recording:
            self.path = []
            packet = RequestPacket(Requests.PlayerPosition)
            self.com.send(packet)
            player_position = PlayerPositionReply(self.com.receive())
            old_pos = player_position.fields["position"]
            old_rot = player_position.fields["rotation"]
            last_rotation = []
            while self.path_to_objective_recording:
                packet = RequestPacket(Requests.PlayerPosition)
                self.com.send(packet)
                player_position = PlayerPositionReply(self.com.receive())
                pos = player_position.fields["position"]
                rot = player_position.fields["rotation"]
                if old_rot != rot:
                    last_rotation.append(pos)
                else:
                    if len(last_rotation):
                        self.path.append(last_rotation[len(last_rotation) - 1])
                        last_rotation = []
                old_rot = rot
                self.path_to_objective_label.SetLabelText("Path points count: {}".format(len(self.path)))
                time.sleep(0.09)

        if self.objective_blob_recording:
            self.path = []
            packet = RequestPacket(Requests.PlayerPosition)
            self.com.send(packet)
            player_position = PlayerPositionReply(self.com.receive())
            old_pos = player_position.fields["position"]
            old_rot = player_position.fields["rotation"]
            last_rotation = []
            while self.objective_blob_recording:
                packet = RequestPacket(Requests.PlayerPosition)
                self.com.send(packet)
                player_position = PlayerPositionReply(self.com.receive())
                pos = player_position.fields["position"]
                rot = player_position.fields["rotation"]
                if old_rot != rot:
                    last_rotation.append(pos)
                else:
                    if len(last_rotation):
                        self.path.append(last_rotation[len(last_rotation) - 1])
                        last_rotation = []
                old_rot = rot
                self.start_record_blob_label.SetLabelText("Path points count: {}".format(len(self.path)))
                time.sleep(0.09)
